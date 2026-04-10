#pragma once
#define NOMINMAX

#include <windows.h>
#include <vector>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <gl/GL.h>

class CVideoTextureMF
{
public:
    CVideoTextureMF();
    ~CVideoTextureMF();

    bool Open(const wchar_t* filePath, bool loop);
    void Close();

    bool Update();
    void Render(float x, float y, float w, float h);

    bool IsOpen()  const { return m_open; }
    bool IsEnded() const { return m_ended; }

private:
    bool EnsureCOMAndMF();
    void ReleaseAll();
    void Restart();

    bool ReadNextSample();
    bool UploadSample(IMFSample* sample);

    static LONGLONG NowQpc();
    static LONGLONG QpcFreq();

private:
    IMFSourceReader* m_reader = nullptr;

    IMFSample* m_nextSample = nullptr;
    LONGLONG   m_nextTs100ns = 0;
    bool       m_hasNext = false;

    std::vector<unsigned char> m_frame;
    UINT32  m_width = 0;
    UINT32  m_height = 0;
    GLuint  m_tex = 0;

    bool    m_open = false;
    bool    m_loop = true;
    bool    m_ended = false;

    bool    m_comInitedHere = false;
    bool    m_mfInitedHere = false;

    LONGLONG m_firstTs100ns = -1;
    LONGLONG m_startQpc = 0;
};
