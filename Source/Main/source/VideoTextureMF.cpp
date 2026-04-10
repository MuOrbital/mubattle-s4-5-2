
#define NOMINMAX
#include "stdafx.h"
#include "VideoTextureMF.h"

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

CVideoTextureMF::CVideoTextureMF() {}
CVideoTextureMF::~CVideoTextureMF() { Close(); }

static bool HasGLContext_()
{
    return (wglGetCurrentContext() != nullptr) && (wglGetCurrentDC() != nullptr);
}

LONGLONG CVideoTextureMF::NowQpc()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return li.QuadPart;
}

LONGLONG CVideoTextureMF::QpcFreq()
{
    static LONGLONG freq = 0;
    if (!freq)
    {
        LARGE_INTEGER li;
        QueryPerformanceFrequency(&li);
        freq = li.QuadPart;
    }
    return freq;
}

bool CVideoTextureMF::EnsureCOMAndMF()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    if (SUCCEEDED(hr))
        m_comInitedHere = true;
    else if (hr == RPC_E_CHANGED_MODE)
        m_comInitedHere = false;
    else
        return false;

    hr = MFStartup(MF_VERSION);
    if (FAILED(hr))
        return false;

    m_mfInitedHere = true;
    return true;
}

void CVideoTextureMF::ReleaseAll()
{
    if (m_nextSample) { m_nextSample->Release(); m_nextSample = nullptr; }
    m_hasNext = false;

    if (m_reader) { m_reader->Release(); m_reader = nullptr; }

    if (m_tex)
    {
        if (HasGLContext_())
            glDeleteTextures(1, &m_tex);
        m_tex = 0;
    }

    m_frame.clear();
    m_width = 0;
    m_height = 0;

    if (m_mfInitedHere) { MFShutdown(); m_mfInitedHere = false; }
    if (m_comInitedHere) { CoUninitialize(); m_comInitedHere = false; }

    m_open = false;
    m_ended = false;
    m_firstTs100ns = -1;
    m_startQpc = 0;
}

bool CVideoTextureMF::Open(const wchar_t* filePath, bool loop)
{
    __try
    {
        Close();

        if (!filePath || !*filePath)
            return false;

        if (!HasGLContext_())
            return false;

        if (!EnsureCOMAndMF())
            return false;

        m_loop = loop;
        m_ended = false;

        IMFAttributes* attrs = nullptr;
        HRESULT hr = MFCreateAttributes(&attrs, 2);
        if (FAILED(hr)) { ReleaseAll(); return false; }

        hr = attrs->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
        if (FAILED(hr)) { attrs->Release(); ReleaseAll(); return false; }

        hr = MFCreateSourceReaderFromURL(filePath, attrs, &m_reader);
        attrs->Release();
        if (FAILED(hr) || !m_reader) { ReleaseAll(); return false; }


        IMFMediaType* type = nullptr;
        hr = MFCreateMediaType(&type);
        if (FAILED(hr)) { ReleaseAll(); return false; }

        hr = type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
        if (FAILED(hr)) { type->Release(); ReleaseAll(); return false; }

        hr = type->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
        if (FAILED(hr)) { type->Release(); ReleaseAll(); return false; }

        hr = type->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
        if (FAILED(hr)) { type->Release(); ReleaseAll(); return false; }

        hr = m_reader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, type);
        type->Release();
        if (FAILED(hr)) { ReleaseAll(); return false; }

        IMFMediaType* current = nullptr;
        hr = m_reader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM, &current);
        if (FAILED(hr) || !current) { ReleaseAll(); return false; }

        UINT32 w = 0, h = 0;
        hr = MFGetAttributeSize(current, MF_MT_FRAME_SIZE, &w, &h);
        current->Release();
        if (FAILED(hr) || w == 0 || h == 0) { ReleaseAll(); return false; }

        m_width = w;
        m_height = h;
        m_frame.resize((size_t)m_width * (size_t)m_height * 4);


        while (glGetError() != GL_NO_ERROR) {}

        glGenTextures(1, &m_tex);
        if (!m_tex) { ReleaseAll(); return false; }

        glBindTexture(GL_TEXTURE_2D, m_tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_width, (GLsizei)m_height,
            0, GL_BGRA, GL_UNSIGNED_BYTE, m_frame.data());

        if (glGetError() != GL_NO_ERROR)
        {
            ReleaseAll();
            return false;
        }

        m_open = true;
        m_firstTs100ns = -1;
        m_startQpc = NowQpc();

        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        ReleaseAll();
        return false;
    }
}

void CVideoTextureMF::Restart()
{
    if (!m_open || !m_reader) return;

    PROPVARIANT var;
    PropVariantInit(&var);
    var.vt = VT_I8;
    var.hVal.QuadPart = 0;

    HRESULT hr = m_reader->SetCurrentPosition(GUID_NULL, var);
    PropVariantClear(&var);

    if (SUCCEEDED(hr))
    {
        m_ended = false;
        m_firstTs100ns = -1;
        m_startQpc = NowQpc();

        if (m_nextSample) { m_nextSample->Release(); m_nextSample = nullptr; }
        m_hasNext = false;
    }
}

bool CVideoTextureMF::ReadNextSample()
{
    if (!m_reader) return false;

    DWORD streamIndex = 0;
    DWORD flags = 0;
    LONGLONG ts100ns = 0;
    IMFSample* sample = nullptr;

    HRESULT hr = m_reader->ReadSample(
        (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,
        &streamIndex,
        &flags,
        &ts100ns,
        &sample
    );

    if (FAILED(hr))
    {
        if (sample) sample->Release();
        m_ended = true;
        return false;
    }

    if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
    {
        if (sample) sample->Release();

        if (m_loop) { Restart(); return true; }
        m_ended = true;
        return true;
    }

    if (!sample)
        return true;

    if (m_nextSample) m_nextSample->Release();
    m_nextSample = sample;
    m_nextTs100ns = ts100ns;
    m_hasNext = true;
    return true;
}

bool CVideoTextureMF::UploadSample(IMFSample* sample)
{
    if (!sample || !m_tex) return false;

    IMFMediaBuffer* buffer = nullptr;
    HRESULT hr = sample->ConvertToContiguousBuffer(&buffer);
    if (FAILED(hr) || !buffer) return false;

    IMF2DBuffer* buffer2D = nullptr;
    hr = buffer->QueryInterface(IID_PPV_ARGS(&buffer2D));

    if (SUCCEEDED(hr) && buffer2D)
    {
        BYTE* pScan0 = nullptr;
        LONG pitch = 0;

        hr = buffer2D->Lock2D(&pScan0, &pitch);
        if (SUCCEEDED(hr) && pScan0 && pitch != 0)
        {
            const UINT32 dstPitch = m_width * 4;
            for (UINT32 y = 0; y < m_height; ++y)
                memcpy(&m_frame[(size_t)y * dstPitch], pScan0 + (size_t)y * pitch, dstPitch);

            buffer2D->Unlock2D();
        }
        buffer2D->Release();
    }
    else
    {
        // Fallback
        BYTE* data = nullptr;
        DWORD maxLen = 0, curLen = 0;
        hr = buffer->Lock(&data, &maxLen, &curLen);
        if (SUCCEEDED(hr) && data && curLen >= (DWORD)m_frame.size())
            memcpy(m_frame.data(), data, m_frame.size());
        if (SUCCEEDED(hr)) buffer->Unlock();
    }

    buffer->Release();

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei)m_width, (GLsizei)m_height,
        GL_BGRA, GL_UNSIGNED_BYTE, m_frame.data());

    return true;
}

bool CVideoTextureMF::Update()
{
    if (!m_open || !m_reader || m_ended) return false;

    const double freq = (double)QpcFreq();
    const double elapsedSec = (double)(NowQpc() - m_startQpc) / freq;
    const LONGLONG elapsed100ns = (LONGLONG)(elapsedSec * 10000000.0);

    if (!m_hasNext)
        ReadNextSample();

    if (!m_hasNext || !m_nextSample)
        return true;

    if (m_firstTs100ns < 0)
        m_firstTs100ns = m_nextTs100ns;

    const LONGLONG targetTs = (m_nextTs100ns - m_firstTs100ns);


    if (elapsed100ns < targetTs)
        return true;


    UploadSample(m_nextSample);
    m_nextSample->Release();
    m_nextSample = nullptr;
    m_hasNext = false;


    for (int i = 0; i < 2; ++i)
    {
        ReadNextSample();
        if (!m_hasNext || !m_nextSample) break;

        const LONGLONG t2 = (m_nextTs100ns - m_firstTs100ns);
        if (t2 <= elapsed100ns)
        {
            m_nextSample->Release();
            m_nextSample = nullptr;
            m_hasNext = false;
        }
        else break;
    }

    return true;
}

typedef void (APIENTRY* PFNGLACTIVETEXTUREPROC_)(unsigned int);
typedef void (APIENTRY* PFNGLUSEPROGRAMPROC_)(unsigned int);

static PFNGLACTIVETEXTUREPROC_ Get_glActiveTexture()
{
    static PFNGLACTIVETEXTUREPROC_ fn = nullptr;
    if (!fn) fn = (PFNGLACTIVETEXTUREPROC_)wglGetProcAddress("glActiveTexture");
    if (!fn) fn = (PFNGLACTIVETEXTUREPROC_)wglGetProcAddress("glActiveTextureARB");
    return fn;
}

static PFNGLUSEPROGRAMPROC_ Get_glUseProgram()
{
    static PFNGLUSEPROGRAMPROC_ fn = nullptr;
    if (!fn) fn = (PFNGLUSEPROGRAMPROC_)wglGetProcAddress("glUseProgram");
    if (!fn) fn = (PFNGLUSEPROGRAMPROC_)wglGetProcAddress("glUseProgramObjectARB");
    return fn;
}

void CVideoTextureMF::Render(float x, float y, float w, float h)
{
    if (!m_open || !m_tex) return;

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    float windowWidth = (float)vp[2];
    float windowHeight = (float)vp[3];

    if (windowWidth <= 0 || windowHeight <= 0) return;

    float videoAspect = (float)m_width / (float)m_height;
    float windowAspect = windowWidth / windowHeight;

    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (videoAspect > windowAspect) {
        scaleY = windowAspect / videoAspect;
    }
    else {
        scaleX = videoAspect / windowAspect;
    }

    float drawWidth = windowWidth * scaleX;
    float drawHeight = windowHeight * scaleY;

    float offsetX = (windowWidth - drawWidth) * 0.5f;
    float offsetY = (windowHeight - drawHeight) * 0.5f;

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, windowWidth, windowHeight, 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (auto glUseProgram_ = Get_glUseProgram()) glUseProgram_(0);
    if (auto glActiveTexture_ = Get_glActiveTexture()) glActiveTexture_(0x84C0);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glColor4f(1.f, 1.f, 1.f, 1.f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex2f(offsetX, offsetY);
    glTexCoord2f(1.f, 0.f); glVertex2f(offsetX + drawWidth, offsetY);
    glTexCoord2f(1.f, 1.f); glVertex2f(offsetX + drawWidth, offsetY + drawHeight);
    glTexCoord2f(0.f, 1.f); glVertex2f(offsetX, offsetY + drawHeight);
    glEnd();

    glDepthMask(GL_TRUE);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}


void CVideoTextureMF::Close()
{
    ReleaseAll();
}


