#include "stdafx.h"
#include "miniz.h"
#include "Update/InGameUpdater.h"
#include "ZzzOpenglUtil.h"
#include <windows.h>
#include <wininet.h>
#include <shlobj.h>
#include <string>
#include <regex>
#include <vector>
#include <wincrypt.h>
#include <cmath>
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shell32.lib")
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

#define UI_BAR_HEIGHT           28
#define UI_BAR_WIDTH_RATIO      0.62f
#define UI_GLOW_LAYERS          6
#define UI_PARTICLE_COUNT       80
#define UI_CHECKING_MSGS_COUNT  6
#define UI_ANIM_FPS_TARGET      60

#define BG_RIBBON_COUNT         18
#define BG_ORBS_COUNT           6
#define BG_STARS_COUNT          160
#define BG_WAVE_RINGS           12

enum eUpdateColorId
{
    UPDATE_COLOR_BACKGROUND = 1,
    UPDATE_COLOR_BACKGROUND_GLOW,
    UPDATE_COLOR_STARS,
    UPDATE_COLOR_ORB_1,
    UPDATE_COLOR_ORB_2,
    UPDATE_COLOR_ORB_3,
    UPDATE_COLOR_ORB_4,
    UPDATE_COLOR_ORB_5,
    UPDATE_COLOR_ORB_6,
    UPDATE_COLOR_RIBBON_1,
    UPDATE_COLOR_RIBBON_2,
    UPDATE_COLOR_RIBBON_3,
    UPDATE_COLOR_SPHERE_GLOW,
    UPDATE_COLOR_SPHERE_BODY,
    UPDATE_COLOR_SPHERE_RINGS,
    UPDATE_COLOR_SPHERE_HIGHLIGHT,
    UPDATE_COLOR_BAR_OUTER,
    UPDATE_COLOR_BAR_BACKGROUND,
    UPDATE_COLOR_BAR_START,
    UPDATE_COLOR_BAR_END,
    UPDATE_COLOR_PARTICLES,
    UPDATE_COLOR_DETAILS,
    UPDATE_COLOR_TEXT_TOP,
    UPDATE_COLOR_TEXT_MIDDLE,
    UPDATE_COLOR_TEXT_BOTTOM,
    UPDATE_COLOR_HIGHLIGHT,
};

static const UPDATE_COLOR_INFO& GetUpdateColor(int id)
{
    if (id < 1 || id > UPDATE_COLOR_COUNT)
    {
        id = UPDATE_COLOR_HIGHLIGHT;
    }

    return gProtect->m_MainInfo.m_UpdateColors[id - 1];
}

static float GetUpdateColorComponent(BYTE value, float scale = 1.0f)
{
    float result = ((float)value / 255.0f) * scale;

    if (result < 0.0f) return 0.0f;
    if (result > 1.0f) return 1.0f;
    return result;
}

static float GetUpdateColorRed(int id, float scale = 1.0f)
{
    return GetUpdateColorComponent(GetUpdateColor(id).Red, scale);
}

static float GetUpdateColorGreen(int id, float scale = 1.0f)
{
    return GetUpdateColorComponent(GetUpdateColor(id).Green, scale);
}

static float GetUpdateColorBlue(int id, float scale = 1.0f)
{
    return GetUpdateColorComponent(GetUpdateColor(id).Blue, scale);
}

static COLORREF GetUpdateTextColor(int id)
{
    const UPDATE_COLOR_INFO& color = GetUpdateColor(id);
    return RGB(color.Red, color.Green, color.Blue);
}

static const char* g_checkingMsgs[UI_CHECKING_MSGS_COUNT] =
{
    "Consultando os servidores de atualizações...",
    "Verificando integridade do cliente...",
    "Obtendo resposta de atualização...",
    "Calculando atualização...",
    "Verificando se o cliente precisa de atualização...",
    "Comparando atualizações..."
};

static InGameUpdateStatus g_UpdateStatus = {};
static HANDLE             g_hUpdateThread = NULL;
static HWND               g_hGameWnd = NULL;
static CRITICAL_SECTION   g_csUpdate;
static bool               g_csInit = false;
static volatile bool      g_bShutdown = false;

static GLuint g_texMain = 0;
static GLuint g_texSub = 0;
static GLuint g_texTip = 0;
static GLuint g_texTitle = 0;
static int    g_texMainW = 0, g_texMainH = 0;
static int    g_texSubW = 0, g_texSubH = 0;
static int    g_texTipW = 0, g_texTipH = 0;
static int    g_texTitleW = 0, g_texTitleH = 0;

static GLuint g_texMainTitle = 0;
static GLuint g_texSubTitle = 0;
static int    g_texMainTitleW = 0, g_texMainTitleH = 0;
static int    g_texSubTitleW = 0, g_texSubTitleH = 0;
static int    mainTitleY = 0;

static char   g_cachedMain[512] = {};
static char   g_cachedSub[512] = {};
static char   g_cachedTip[512] = {};

struct Particle {
    float x, y, vx, vy, life, maxLife, size, alpha;
};
static Particle g_particles[UI_PARTICLE_COUNT];
static bool     g_particlesInit = false;

static DWORD    g_renderStartTick = 0;
static int      g_currentTip = 0;
static DWORD    g_lastTipChange = 0;
static float    g_tipAlpha = 1.0f;
static bool     g_tipFading = false;

struct BgRibbon {
    float baseAngle;
    float speed;
    float radius;
    float width;
    float phase;
    float r, g, b;
    float alphaMult;
};
static BgRibbon  g_ribbons[BG_RIBBON_COUNT];
static bool      g_ribbonsInit = false;

struct BgOrb {
    float cx, cy;
    float radius;
    float pulseSpeed;
    float pulsePhase;
    float r, g, b;
    float intensity;
};
static BgOrb g_orbs[BG_ORBS_COUNT];
static bool  g_orbsInit = false;

struct BgStar {
    float x, y;
    float size;
    float twinkleSpeed;
    float twinklePhase;
    float alpha;
};
static BgStar g_stars[BG_STARS_COUNT];
static bool   g_starsInit = false;

static DWORD WINAPI UpdateThreadProc(LPVOID);
static int   ReadLocalVersion(const char* key);
static void  WriteLocalVersion(const char* key, int ver);
static int   QueryServerVersion(const char* baseUrl, const char* pattern);
static bool  DownloadFile(const char* url, const char* destPath);
static bool  ExtractZip(const char* zipPath, const char* destDir);
static void  SetStatus(INGAME_UPDATE_STATE state, float pct, const char* text, const char* sub = nullptr);
static void  LaunchLauncherAndExit();
static void  GetAppDir(char* outDir, DWORD size);
static void  GetIniPath(char* outPath, DWORD size);
static void  GetIniExePath(char* outPath, DWORD size);
static void  WriteErrorLog(const char* fmt, ...);
static void  RestartGameAndExit();

static void DrawBgBackground(int W, int H, float t);
static void DrawBgStars(int W, int H, float t);
static void DrawBgOrbs(int W, int H, float t);
static void DrawBgRibbons(int W, int H, float t);
static void DrawBgScanlines(int W, int H, float t);
static void DrawBgGrid(int W, int H, float t);
static void DrawBgCentralSphere(int W, int H, float t);
static void InitBgElements(int W, int H);

extern unsigned int WindowWidth;
extern unsigned int WindowHeight;
extern HGLRC g_hRC;

static void GetAppDir(char* outDir, DWORD size)
{
    GetModuleFileNameA(NULL, outDir, size);
    char* sl = strrchr(outDir, '\\');
    if (sl) *(sl + 1) = '\0';
}

static void GetIniPath(char* outPath, DWORD size)
{
    char appDir[MAX_PATH];
    GetAppDir(appDir, MAX_PATH);
    sprintf_s(outPath, size, "%s%s", appDir, INGAME_UPDATE_INI_PATH);
}

static void WriteErrorLog(const char* fmt, ...)
{
    char appDir[MAX_PATH];
    GetAppDir(appDir, MAX_PATH);

    char logDir[MAX_PATH];
    sprintf_s(logDir, "%sData\\GameLoad", appDir);
    SHCreateDirectoryExA(NULL, logDir, NULL);

    char logPath[MAX_PATH];
    sprintf_s(logPath, "%s\\update_log.txt", logDir);

    HANDLE hLog = CreateFileA(logPath, GENERIC_WRITE, FILE_SHARE_READ,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLog == INVALID_HANDLE_VALUE) return;
    SetFilePointer(hLog, 0, NULL, FILE_END);

    SYSTEMTIME st;
    GetLocalTime(&st);
    char header[64];
    int hlen = sprintf_s(header, "[%02d:%02d:%02d] ERRO: ", st.wHour, st.wMinute, st.wSecond);

    char body[2048];
    va_list args;
    va_start(args, fmt);
    int blen = vsprintf_s(body, fmt, args);
    va_end(args);

    DWORD written = 0;
    WriteFile(hLog, header, hlen, &written, NULL);
    WriteFile(hLog, body, blen, &written, NULL);
    WriteFile(hLog, "\r\n", 2, &written, NULL);
    CloseHandle(hLog);
}

static void InitBgElements(int W, int H)
{
    if (g_ribbonsInit && g_orbsInit && g_starsInit) return;

    if (!g_ribbonsInit)
    {
        g_ribbonsInit = true;
        srand(12345);
        for (int i = 0; i < BG_RIBBON_COUNT; i++)
        {
            BgRibbon& rb = g_ribbons[i];
            rb.baseAngle = (float)(rand() % 360) * 3.14159f / 180.f;
            rb.speed = 0.08f + (rand() % 100) * 0.0008f;
            if (rand() % 2) rb.speed = -rb.speed;
            rb.radius = (float)(W / 2) * (0.22f + (rand() % 100) * 0.004f);
            rb.width = 1.5f + (rand() % 60) * 0.05f;
            rb.phase = (float)(rand() % 628) * 0.01f;
            rb.alphaMult = 0.05f + (rand() % 100) * 0.002f;
            int colorChoice = rand() % 3;
            int colorId = UPDATE_COLOR_RIBBON_1 + colorChoice;
            rb.r = GetUpdateColorRed(colorId);
            rb.g = GetUpdateColorGreen(colorId);
            rb.b = GetUpdateColorBlue(colorId);
        }
    }

    if (!g_orbsInit)
    {
        g_orbsInit = true;
        g_orbs[0] = { 0.5f, 0.42f, 0.38f, 0.6f, 0.0f, GetUpdateColorRed(UPDATE_COLOR_ORB_1), GetUpdateColorGreen(UPDATE_COLOR_ORB_1), GetUpdateColorBlue(UPDATE_COLOR_ORB_1), 1.0f };
        g_orbs[1] = { 0.18f, 0.25f, 0.14f, 0.9f, 1.1f, GetUpdateColorRed(UPDATE_COLOR_ORB_2), GetUpdateColorGreen(UPDATE_COLOR_ORB_2), GetUpdateColorBlue(UPDATE_COLOR_ORB_2), 0.7f };
        g_orbs[2] = { 0.82f, 0.30f, 0.12f, 0.7f, 2.3f, GetUpdateColorRed(UPDATE_COLOR_ORB_3), GetUpdateColorGreen(UPDATE_COLOR_ORB_3), GetUpdateColorBlue(UPDATE_COLOR_ORB_3), 0.6f };
        g_orbs[3] = { 0.10f, 0.70f, 0.10f, 1.1f, 0.7f, GetUpdateColorRed(UPDATE_COLOR_ORB_4), GetUpdateColorGreen(UPDATE_COLOR_ORB_4), GetUpdateColorBlue(UPDATE_COLOR_ORB_4), 0.5f };
        g_orbs[4] = { 0.88f, 0.65f, 0.11f, 0.8f, 3.5f, GetUpdateColorRed(UPDATE_COLOR_ORB_5), GetUpdateColorGreen(UPDATE_COLOR_ORB_5), GetUpdateColorBlue(UPDATE_COLOR_ORB_5), 0.5f };
        g_orbs[5] = { 0.50f, 0.85f, 0.09f, 1.3f, 1.8f, GetUpdateColorRed(UPDATE_COLOR_ORB_6), GetUpdateColorGreen(UPDATE_COLOR_ORB_6), GetUpdateColorBlue(UPDATE_COLOR_ORB_6), 0.4f };
    }

    if (!g_starsInit)
    {
        g_starsInit = true;
        srand(54321);
        for (int i = 0; i < BG_STARS_COUNT; i++)
        {
            g_stars[i].x = (float)(rand() % W);
            g_stars[i].y = (float)(rand() % H);
            g_stars[i].size = 0.5f + (rand() % 30) * 0.05f;
            g_stars[i].twinkleSpeed = 0.5f + (rand() % 100) * 0.03f;
            g_stars[i].twinklePhase = (float)(rand() % 628) * 0.01f;
            g_stars[i].alpha = 0.1f + (rand() % 100) * 0.005f;
        }
    }
}

static void DrawBgBackground(int W, int H, float t)
{
    float pulse = 0.5f + 0.5f * sinf(t * 0.3f);
    float r0 = GetUpdateColorRed(UPDATE_COLOR_BACKGROUND);
    float g0 = GetUpdateColorGreen(UPDATE_COLOR_BACKGROUND, 0.85f + pulse * 0.15f);
    float b0 = GetUpdateColorBlue(UPDATE_COLOR_BACKGROUND, 0.80f + pulse * 0.20f);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    float cx = W * 0.5f, cy = H * 0.42f;
    glBegin(GL_QUADS);
    glColor3f(r0 * 0.4f, g0 * 0.4f, b0 * 0.4f); glVertex2i(0, 0);
    glColor3f(r0 * 0.4f, g0 * 0.4f, b0 * 0.4f); glVertex2i(W, 0);
    glColor3f(r0 * 0.4f, g0 * 0.4f, b0 * 0.4f); glVertex2i(W, H);
    glColor3f(r0 * 0.4f, g0 * 0.4f, b0 * 0.4f); glVertex2i(0, H);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    int steps = 40;
    float maxR = (float)(W > H ? W : H) * 0.72f;
    for (int s = steps; s >= 1; s--)
    {
        float frac = (float)s / steps;
        float rr = maxR * frac;
        float alpha = (1.f - frac) * 0.07f;
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(
            GetUpdateColorRed(UPDATE_COLOR_BACKGROUND_GLOW),
            GetUpdateColorGreen(UPDATE_COLOR_BACKGROUND_GLOW),
            GetUpdateColorBlue(UPDATE_COLOR_BACKGROUND_GLOW),
            alpha);
        glVertex2f(cx, cy);
        int seg = 32;
        for (int k = 0; k <= seg; k++)
        {
            float ang = k * 2.f * 3.14159f / seg;
            glColor4f(0.f, 0.f, 0.f, 0.f);
            glVertex2f(cx + cosf(ang) * rr, cy + sinf(ang) * rr);
        }
        glEnd();
    }
    glDisable(GL_BLEND);
}

static void DrawBgStars(int W, int H, float t)
{
    (void)W; (void)H;
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < BG_STARS_COUNT; i++)
    {
        BgStar& s = g_stars[i];
        float tw = s.alpha * (0.5f + 0.5f * sinf(t * s.twinkleSpeed + s.twinklePhase));
        glColor4f(
            GetUpdateColorRed(UPDATE_COLOR_STARS),
            GetUpdateColorGreen(UPDATE_COLOR_STARS),
            GetUpdateColorBlue(UPDATE_COLOR_STARS),
            tw);
        glVertex2f(s.x, s.y);
    }
    glEnd();
    glDisable(GL_BLEND);
}

static void DrawBgOrbs(int W, int H, float t)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    for (int i = 0; i < BG_ORBS_COUNT; i++)
    {
        BgOrb& o = g_orbs[i];
        float pulse = 0.75f + 0.25f * sinf(t * o.pulseSpeed + o.pulsePhase);
        float cx = o.cx * W;
        float cy = o.cy * H;
        float maxR = o.radius * (W < H ? W : H) * pulse;

        int layers = 28;
        for (int k = layers; k >= 1; k--)
        {
            float frac = (float)k / layers;
            float rr = maxR * frac;
            float alpha = (1.f - frac) * (1.f - frac) * 0.12f * o.intensity * pulse;
            glBegin(GL_TRIANGLE_FAN);
            glColor4f(o.r, o.g, o.b, alpha);
            glVertex2f(cx, cy);
            int seg = 24;
            for (int s = 0; s <= seg; s++)
            {
                float ang = s * 2.f * 3.14159f / seg;
                glColor4f(o.r * 0.2f, o.g * 0.2f, o.b * 0.2f, 0.f);
                glVertex2f(cx + cosf(ang) * rr, cy + sinf(ang) * rr);
            }
            glEnd();
        }
    }
    glDisable(GL_BLEND);
}

static void DrawBgRibbons(int W, int H, float t)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glLineWidth(1.0f);

    float cx = W * 0.5f;
    float cy = H * 0.42f;

    for (int i = 0; i < BG_RIBBON_COUNT; i++)
    {
        BgRibbon& rb = g_ribbons[i];
        float angle = rb.baseAngle + t * rb.speed;
        int   segs = 120;
        float rx = rb.radius * 1.6f;
        float ry = rb.radius * 0.55f;
        float waveAmp = rb.radius * 0.07f;

        glBegin(GL_TRIANGLE_STRIP);
        for (int s = 0; s <= segs; s++)
        {
            float frac = (float)s / segs;
            float a = angle + frac * 2.f * 3.14159f + rb.phase;
            float wave = waveAmp * sinf(frac * 8.f * 3.14159f + t * 1.5f);

            float px = cx + cosf(a) * rx;
            float py = cy + sinf(a) * ry + wave;

            float nx = -sinf(a) * rb.width * 0.5f;
            float ny = cosf(a) * rb.width * 0.18f;

            float depthAlpha = 0.5f + 0.5f * sinf(a);
            float edgeAlpha = rb.alphaMult * depthAlpha;

            glColor4f(rb.r, rb.g, rb.b, edgeAlpha);
            glVertex2f(px + nx, py + ny);
            glColor4f(rb.r * 0.5f, rb.g * 0.5f, rb.b * 0.5f, 0.f);
            glVertex2f(px - nx, py - ny);
        }
        glEnd();
    }

    glDisable(GL_BLEND);
    glLineWidth(1.f);
}

static void DrawBgGrid(int W, int H, float t)
{
    //glDisable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    //float scroll = fmodf(t * 18.f, 60.f);  // scroll em pixels
    //float alpha = 0.04f + 0.015f * sinf(t * 0.7f);
    //glColor4f(0.05f, 0.30f, 0.70f, alpha);
    //glLineWidth(1.0f);
    //
    //// Linhas horizontais com perspectiva falsa
    //int gridBottom = H;
    //int gridTop = (int)(H * 0.62f);
    //int horizY = (int)(H * 0.58f);
    //int cols = 24;
    //int rows = 16;
    //
    //glBegin(GL_LINES);
    //// Linhas horizontais
    //for (int r = 0; r <= rows; r++)
    //{
    //    float frac = (float)r / rows;
    //    float y = gridTop + (gridBottom - gridTop) * frac * frac + scroll * frac;
    //    if (y > gridBottom) continue;
    //    float lineAlpha = alpha * (frac * frac * 2.f + 0.1f);
    //    glColor4f(0.05f, 0.30f, 0.70f, lineAlpha);
    //    glVertex2f(0.f, y);
    //    glVertex2f((float)W, y);
    //}
    //// Linhas verticais convergindo para ponto de fuga (centro)
    //float vpX = W * 0.5f;
    //for (int c = 0; c <= cols; c++)
    //{
    //    float frac = (float)c / cols;
    //    float xBot = frac * W;
    //    float xTop = vpX + (xBot - vpX) * 0.15f;  // converge
    //    glColor4f(0.05f, 0.30f, 0.70f, alpha * 0.5f);
    //    glVertex2f(xTop, (float)gridTop);
    //    glVertex2f(xBot, (float)gridBottom);
    //}
    //glEnd();
    //
    //glDisable(GL_BLEND);
}

static void DrawBgScanlines(int W, int H, float t)
{
    //glDisable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    //int step = 4;
    //float alpha = 0.025f + 0.008f * sinf(t * 0.5f);
    //glColor4f(0.f, 0.f, 0.f, alpha);
    //glBegin(GL_QUADS);
    //for (int y = 0; y < H; y += step * 2)
    //{
    //    glVertex2i(0, y);
    //    glVertex2i(W, y);
    //    glVertex2i(W, y + step);
    //    glVertex2i(0, y + step);
    //}
    //glEnd();
    //
    //// Linha de scan luminosa descendo
    //float scanY = fmodf(t * 80.f, (float)H);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBegin(GL_QUADS);
    //glColor4f(0.10f, 0.40f, 0.80f, 0.0f);
    //glVertex2i(0, (int)scanY - 20);
    //glVertex2i(W, (int)scanY - 20);
    //glColor4f(0.10f, 0.40f, 0.80f, 0.06f);
    //glVertex2i(W, (int)scanY);
    //glVertex2i(0, (int)scanY);
    //glColor4f(0.10f, 0.40f, 0.80f, 0.0f);
    //glVertex2i(0, (int)scanY + 8);
    //glVertex2i(W, (int)scanY + 8);
    //glVertex2i(W, (int)scanY);
    //glVertex2i(0, (int)scanY);
    //glEnd();
    //
    //glDisable(GL_BLEND);
}

static void DrawBgCentralSphere(int W, int H, float t)
{
    float cx = W * 0.5f;
    float cy = H * 0.42f;
    float baseR = (float)(W < H ? W : H) * 0.13f;
    float pulse = 1.0f + 0.04f * sinf(t * 1.8f);
    float sphereR = baseR * pulse;

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    int glowLayers = 20;
    for (int k = glowLayers; k >= 1; k--)
    {
        float frac = (float)k / glowLayers;
        float rr = sphereR * (1.f + frac * 1.8f);
        float alpha = (1.f - frac) * (1.f - frac) * 0.04f;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(
            GetUpdateColorRed(UPDATE_COLOR_SPHERE_GLOW),
            GetUpdateColorGreen(UPDATE_COLOR_SPHERE_GLOW),
            GetUpdateColorBlue(UPDATE_COLOR_SPHERE_GLOW),
            alpha);
        glVertex2f(cx, cy);
        int seg = 48;
        for (int s = 0; s <= seg; s++)
        {
            float ang = s * 2.f * 3.14159f / seg;
            glColor4f(0.f, 0.f, 0.f, 0.f);
            glVertex2f(cx + cosf(ang) * rr, cy + sinf(ang) * rr);
        }
        glEnd();
    }

    int bodyLayers = 32;
    for (int k = bodyLayers; k >= 1; k--)
    {
        float frac = (float)k / bodyLayers;
        float rr = sphereR * frac;
        float colorIntensity = 0.3f + (1.f - frac) * 0.7f;
        float cr = GetUpdateColorRed(UPDATE_COLOR_SPHERE_BODY, colorIntensity);
        float cg = GetUpdateColorGreen(UPDATE_COLOR_SPHERE_BODY, colorIntensity);
        float cb = GetUpdateColorBlue(UPDATE_COLOR_SPHERE_BODY, colorIntensity);
        float alpha = 0.08f;
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_TRIANGLE_FAN);
        glColor4f(cr, cg, cb, alpha);
        glVertex2f(cx, cy);
        int seg = 48;
        for (int s = 0; s <= seg; s++)
        {
            float ang = s * 2.f * 3.14159f / seg;
            glColor4f(cr * 0.2f, cg * 0.2f, cb * 0.2f, 0.f);
            glVertex2f(cx + cosf(ang) * rr, cy + sinf(ang) * rr);
        }
        glEnd();
    }

    int ringCount = BG_WAVE_RINGS;
    for (int r = 0; r < ringCount; r++)
    {
        float ringPhase = (float)r * 3.14159f / ringCount;
        float ringAngleX = t * (0.4f + r * 0.05f) + ringPhase;
        float ringAngleZ = t * (0.25f + r * 0.03f) + ringPhase * 1.3f;
        float ringR = sphereR * (0.85f + (float)r * 0.02f);
        float cosX = cosf(ringAngleX), sinX = sinf(ringAngleX);
        float cosZ = cosf(ringAngleZ), sinZ = sinf(ringAngleZ);
        float alpha = 0.08f + 0.04f * sinf(t * 0.9f + ringPhase);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glLineWidth(0.8f);
        glBegin(GL_LINE_LOOP);
        int seg = 64;
        for (int s = 0; s < seg; s++)
        {
            float ang = s * 2.f * 3.14159f / seg;
            float lx = cosf(ang) * ringR;
            float ly = sinf(ang) * ringR;
            float rx = lx * cosZ - ly * sinX * sinZ;
            float ry = ly * cosX;
            glColor4f(
                GetUpdateColorRed(UPDATE_COLOR_SPHERE_RINGS),
                GetUpdateColorGreen(UPDATE_COLOR_SPHERE_RINGS),
                GetUpdateColorBlue(UPDATE_COLOR_SPHERE_RINGS),
                alpha * (0.5f + 0.5f * cosf(ang)));
            glVertex2f(cx + rx, cy + ry);
        }
        glEnd();
    }

    float hx = cx - sphereR * 0.28f;
    float hy = cy - sphereR * 0.32f;
    float hr = sphereR * 0.22f;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(
        GetUpdateColorRed(UPDATE_COLOR_SPHERE_HIGHLIGHT),
        GetUpdateColorGreen(UPDATE_COLOR_SPHERE_HIGHLIGHT),
        GetUpdateColorBlue(UPDATE_COLOR_SPHERE_HIGHLIGHT),
        0.25f);
    glVertex2f(hx, hy);
    int seg = 24;
    for (int s = 0; s <= seg; s++)
    {
        float ang = s * 2.f * 3.14159f / seg;
        glColor4f(
            GetUpdateColorRed(UPDATE_COLOR_SPHERE_HIGHLIGHT, 0.85f),
            GetUpdateColorGreen(UPDATE_COLOR_SPHERE_HIGHLIGHT, 0.85f),
            GetUpdateColorBlue(UPDATE_COLOR_SPHERE_HIGHLIGHT, 0.85f),
            0.f);
        glVertex2f(hx + cosf(ang) * hr, hy + sinf(ang) * hr);
    }
    glEnd();

    glDisable(GL_BLEND);
    glLineWidth(1.f);
}

void InGameUpdate_RenderBackground(int W, int H, float t)
{
    InitBgElements(W, H);
    DrawBgBackground(W, H, t);
    DrawBgStars(W, H, t);
    DrawBgOrbs(W, H, t);
    DrawBgRibbons(W, H, t);
    DrawBgCentralSphere(W, H, t);
    DrawBgGrid(W, H, t);
    DrawBgScanlines(W, H, t);
}

void InGameUpdate_Start(HWND hWnd)
{
    if (!g_csInit) { InitializeCriticalSection(&g_csUpdate); g_csInit = true; }
    g_hGameWnd = hWnd;
    ZeroMemory(&g_UpdateStatus, sizeof(g_UpdateStatus));
    g_UpdateStatus.state = IUS_CHECKING;
    strcpy_s(g_UpdateStatus.statusText, "Verificando atualizacoes...");
    g_renderStartTick = GetTickCount();
    g_lastTipChange = GetTickCount();
    int tipCount = gProtect->m_MainInfo.m_UpdateTipsCount;
    g_currentTip = (tipCount > 0) ? (rand() % tipCount) : 0;

    g_hUpdateThread = CreateThread(NULL, 0, UpdateThreadProc, NULL, 0, NULL);
}

INGAME_UPDATE_STATE InGameUpdate_GetState()
{
    if (g_csInit) EnterCriticalSection(&g_csUpdate);
    INGAME_UPDATE_STATE s = g_UpdateStatus.state;
    if (g_csInit) LeaveCriticalSection(&g_csUpdate);
    return s;
}

void InGameUpdate_GetStatus(InGameUpdateStatus& out)
{
    if (g_csInit) EnterCriticalSection(&g_csUpdate);
    out = g_UpdateStatus;
    if (g_csInit) LeaveCriticalSection(&g_csUpdate);
}

void InGameUpdate_Shutdown()
{
    g_bShutdown = true;

    if (g_hUpdateThread)
    {
        WaitForSingleObject(g_hUpdateThread, 3000);
        CloseHandle(g_hUpdateThread);
        g_hUpdateThread = NULL;
    }

    ExitProcess(0);
}

static GLuint BuildTextTexture(HDC hDC, const char* text,
    int texW, int texH, int fontSize,
    COLORREF color, DWORD dtFlags, const char* fontName,
    int fontWeight = FW_BOLD, GLuint existingTex = 0)
{
    HDC     hMemDC = CreateCompatibleDC(hDC);
    HBITMAP hBmp = CreateCompatibleBitmap(hDC, texW, texH);
    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

    RECT rcFill = { 0, 0, texW, texH };
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(hMemDC, &rcFill, hBrush);
    DeleteObject(hBrush);

    HFONT hFont = CreateFontA(fontSize, 0, 0, 0, fontWeight, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName);
    HFONT hOldFont = (HFONT)SelectObject(hMemDC, hFont);
    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, color);
    DrawTextA(hMemDC, text, -1, &rcFill, dtFlags);
    SelectObject(hMemDC, hOldFont);
    DeleteObject(hFont);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = texW;
    bmi.bmiHeader.biHeight = -texH;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    std::vector<DWORD> pixels((size_t)texW * texH);
    GetDIBits(hMemDC, hBmp, 0, texH, pixels.data(), &bmi, DIB_RGB_COLORS);
    SelectObject(hMemDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hMemDC);

    std::vector<BYTE> rgba((size_t)texW * texH * 4);
    for (int i = 0; i < texW * texH; i++)
    {
        BYTE b = (pixels[i] >> 0) & 0xFF;
        BYTE g = (pixels[i] >> 8) & 0xFF;
        BYTE r = (pixels[i] >> 16) & 0xFF;
        int  lum = (int)r + g + b;
        BYTE a = (lum < 12) ? 0 : 255;
        rgba[i * 4 + 0] = r;
        rgba[i * 4 + 1] = g;
        rgba[i * 4 + 2] = b;
        rgba[i * 4 + 3] = a;
    }

    GLuint tex = existingTex;
    if (tex == 0) glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

static void DrawTexture(GLuint tex, int x, int y, int w, int h, float alpha = 1.0f,
    float r = 1.f, float g = 1.f, float b = 1.f)
{
    if (!tex) return;
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, alpha);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex2i(x, y);
    glTexCoord2f(1.f, 0.f); glVertex2i(x + w, y);
    glTexCoord2f(1.f, 1.f); glVertex2i(x + w, y + h);
    glTexCoord2f(0.f, 1.f); glVertex2i(x, y + h);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

static void InitParticles(int barX, int barY, int barW, int barH)
{
    if (g_particlesInit) return;
    g_particlesInit = true;
    srand(GetTickCount());
    for (int i = 0; i < UI_PARTICLE_COUNT; i++)
    {
        Particle& p = g_particles[i];
        p.x = (float)(barX + rand() % barW);
        p.y = (float)(barY + rand() % barH);
        p.vx = ((float)(rand() % 100) - 50) * 0.006f;
        p.vy = ((float)(rand() % 100) - 50) * 0.006f - 0.3f;
        p.maxLife = 60.f + (rand() % 120);
        p.life = (float)(rand() % (int)p.maxLife);
        p.size = 1.5f + (rand() % 30) * 0.1f;
        p.alpha = 0.f;
    }
}

static void UpdateParticles(int barX, int barY, int barW, int barH, float fillFraction)
{
    for (int i = 0; i < UI_PARTICLE_COUNT; i++)
    {
        Particle& p = g_particles[i];
        p.life += 1.f;
        p.x += p.vx;
        p.y += p.vy;
        p.vy -= 0.008f;
        float t = p.life / p.maxLife;
        p.alpha = (t < 0.2f) ? t / 0.2f : 1.f - ((t - 0.2f) / 0.8f);
        p.alpha = max(0.f, min(1.f, p.alpha));
        if (p.life >= p.maxLife)
        {
            int spawnW = max(1, (int)(barW * fillFraction));
            p.x = (float)(barX + rand() % spawnW);
            p.y = (float)(barY + rand() % barH);
            p.vx = ((float)(rand() % 100) - 50) * 0.025f;
            p.vy = ((float)(rand() % 100) - 50) * 0.025f - 0.8f;
            p.maxLife = 60.f + (rand() % 120);
            p.life = 0.f;
            p.size = 1.5f + (rand() % 30) * 0.1f;
        }
    }
}

static void DrawParticles()
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(2.f);
    glBegin(GL_POINTS);
    for (int i = 0; i < UI_PARTICLE_COUNT; i++)
    {
        Particle& p = g_particles[i];
        if (p.alpha <= 0.01f) continue;
        float pulse = 0.7f + 0.3f * sinf(p.life * 0.15f);
        glColor4f(
            GetUpdateColorRed(UPDATE_COLOR_PARTICLES, pulse),
            GetUpdateColorGreen(UPDATE_COLOR_PARTICLES, pulse),
            GetUpdateColorBlue(UPDATE_COLOR_PARTICLES, pulse),
            p.alpha * 0.75f);
        glVertex2f(p.x, p.y);
        glColor4f(
            GetUpdateColorRed(UPDATE_COLOR_HIGHLIGHT),
            GetUpdateColorGreen(UPDATE_COLOR_HIGHLIGHT),
            GetUpdateColorBlue(UPDATE_COLOR_HIGHLIGHT),
            p.alpha * 0.3f);
        glVertex2f(p.x + p.size * 0.3f, p.y);
    }
    glEnd();
    glDisable(GL_BLEND);
}

static void DrawFilledRect(int x, int y, int w, int h,
    float r, float g, float b, float a = 1.f)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
    glDisable(GL_BLEND);
}

static void DrawRectOutline(int x, int y, int w, int h,
    float r, float g, float b, float a = 1.f, float lineW = 1.f)
{
    glLineWidth(lineW);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
    glDisable(GL_BLEND);
    glLineWidth(1.f);
}

static void DrawGradientBar(int x, int y, int w, int h,
    float r0, float g0, float b0,
    float r1, float g1, float b1,
    float alpha = 1.f)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(r0, g0, b0, alpha); glVertex2i(x, y);
    glColor4f(r1, g1, b1, alpha); glVertex2i(x + w, y);
    glColor4f(r1, g1, b1, alpha); glVertex2i(x + w, y + h);
    glColor4f(r0, g0, b0, alpha); glVertex2i(x, y + h);
    glEnd();
    glDisable(GL_BLEND);
}

static void DrawBarHighlight(int x, int y, int w, int h)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(GetUpdateColorRed(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorGreen(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorBlue(UPDATE_COLOR_HIGHLIGHT), 0.18f); glVertex2i(x, y);
    glColor4f(GetUpdateColorRed(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorGreen(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorBlue(UPDATE_COLOR_HIGHLIGHT), 0.18f); glVertex2i(x + w, y);
    glColor4f(GetUpdateColorRed(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorGreen(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorBlue(UPDATE_COLOR_HIGHLIGHT), 0.0f);  glVertex2i(x + w, y + h / 2);
    glColor4f(GetUpdateColorRed(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorGreen(UPDATE_COLOR_HIGHLIGHT), GetUpdateColorBlue(UPDATE_COLOR_HIGHLIGHT), 0.0f);  glVertex2i(x, y + h / 2);
    glEnd();
    glDisable(GL_BLEND);
}

static void DrawGlow(int x, int y, int w, int h, float r, float g, float b)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    for (int i = UI_GLOW_LAYERS; i >= 1; i--)
    {
        float pad = (float)i * 2.f;
        float a = 0.04f / i;
        glColor4f(r, g, b, a);
        glBegin(GL_QUADS);
        glVertex2f(x - pad, y - pad);
        glVertex2f(x + w + pad, y - pad);
        glVertex2f(x + w + pad, y + h + pad);
        glVertex2f(x - pad, y + h + pad);
        glEnd();
    }
    glDisable(GL_BLEND);
}

static void DrawHLine(int x, int y, int w, float r, float g, float b, float a = 1.f)
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, a);
    glBegin(GL_LINES);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glEnd();
    glDisable(GL_BLEND);
}

void InGameUpdate_SetLoadingProgress(float pct, const char* text)
{
    if (g_csInit) EnterCriticalSection(&g_csUpdate);
    g_UpdateStatus.state = IUS_LOADING_GAME;
    g_UpdateStatus.progress = pct;
    if (text)
        strcpy_s(g_UpdateStatus.statusText, text);
    g_UpdateStatus.subText[0] = '\0';
    if (g_csInit) LeaveCriticalSection(&g_csUpdate);
}

void InGameUpdate_RenderScreen(HDC hDC)
{
    InGameUpdateStatus st;
    InGameUpdate_GetStatus(st);

    wglMakeCurrent(hDC, g_hRC);

    int W = (int)WindowWidth;
    int H = (int)WindowHeight;
    float t = (GetTickCount() - g_renderStartTick) / 1000.0f;

    glClearColor(
        GetUpdateColorRed(UPDATE_COLOR_BACKGROUND),
        GetUpdateColorGreen(UPDATE_COLOR_BACKGROUND),
        GetUpdateColorBlue(UPDATE_COLOR_BACKGROUND),
        1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, W, H, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    InGameUpdate_RenderBackground(W, H, t);

    int barW = (int)(W * UI_BAR_WIDTH_RATIO);
    int barH = UI_BAR_HEIGHT;
    int barX = (W - barW) / 2;
    int barY = (int)(H * 0.60f);

    InitParticles(barX, barY - 20, barW, barH + 40);
    float fillFrac = st.progress / 100.0f;
    UpdateParticles(barX, barY - 10, (int)(barW * fillFrac) + 1, barH + 20, fillFrac);
    DrawParticles();

    DrawFilledRect(barX - 2, barY - 2, barW + 4, barH + 4,
        GetUpdateColorRed(UPDATE_COLOR_BAR_OUTER),
        GetUpdateColorGreen(UPDATE_COLOR_BAR_OUTER),
        GetUpdateColorBlue(UPDATE_COLOR_BAR_OUTER), 0.6f);
    DrawFilledRect(barX, barY, barW, barH,
        GetUpdateColorRed(UPDATE_COLOR_BAR_BACKGROUND),
        GetUpdateColorGreen(UPDATE_COLOR_BAR_BACKGROUND),
        GetUpdateColorBlue(UPDATE_COLOR_BAR_BACKGROUND));

    int fillW = (int)(barW * fillFrac);
    if (fillW > 0)
    {
        DrawGradientBar(barX, barY, fillW, barH,
            GetUpdateColorRed(UPDATE_COLOR_BAR_START),
            GetUpdateColorGreen(UPDATE_COLOR_BAR_START),
            GetUpdateColorBlue(UPDATE_COLOR_BAR_START),
            GetUpdateColorRed(UPDATE_COLOR_BAR_END),
            GetUpdateColorGreen(UPDATE_COLOR_BAR_END),
            GetUpdateColorBlue(UPDATE_COLOR_BAR_END));
        DrawBarHighlight(barX, barY, fillW, barH);

        float scanPulse = 0.4f + 0.35f * sinf(t * 3.5f);
        int   scanX = barX + (int)((fillW - 4) * (0.5f + 0.5f * sinf(t * 1.8f)));
        DrawFilledRect(scanX, barY, 3, barH,
            GetUpdateColorRed(UPDATE_COLOR_HIGHLIGHT),
            GetUpdateColorGreen(UPDATE_COLOR_HIGHLIGHT),
            GetUpdateColorBlue(UPDATE_COLOR_HIGHLIGHT), scanPulse * 0.5f);
        DrawGlow(barX, barY, fillW, barH,
            GetUpdateColorRed(UPDATE_COLOR_BAR_END),
            GetUpdateColorGreen(UPDATE_COLOR_BAR_END),
            GetUpdateColorBlue(UPDATE_COLOR_BAR_END));
    }

    DrawRectOutline(barX, barY, barW, barH,
        GetUpdateColorRed(UPDATE_COLOR_DETAILS),
        GetUpdateColorGreen(UPDATE_COLOR_DETAILS),
        GetUpdateColorBlue(UPDATE_COLOR_DETAILS), 0.7f, 1.5f);

    {
        float lineAlpha = 0.25f + 0.1f * sinf(t * 2.f);
        int   lx1 = barX - 18, lx2 = barX + barW + 18;
        DrawHLine(lx1 - 60, barY + barH / 2, 55, GetUpdateColorRed(UPDATE_COLOR_DETAILS), GetUpdateColorGreen(UPDATE_COLOR_DETAILS), GetUpdateColorBlue(UPDATE_COLOR_DETAILS), lineAlpha);
        DrawHLine(lx2 + 5, barY + barH / 2, 55, GetUpdateColorRed(UPDATE_COLOR_DETAILS), GetUpdateColorGreen(UPDATE_COLOR_DETAILS), GetUpdateColorBlue(UPDATE_COLOR_DETAILS), lineAlpha);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(GetUpdateColorRed(UPDATE_COLOR_DETAILS), GetUpdateColorGreen(UPDATE_COLOR_DETAILS), GetUpdateColorBlue(UPDATE_COLOR_DETAILS), lineAlpha * 1.5f);
        glBegin(GL_TRIANGLES);
        glVertex2i(lx1, barY + barH / 2 - 5);
        glVertex2i(lx1, barY + barH / 2 + 5);
        glVertex2i(lx1 - 12, barY + barH / 2);
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex2i(lx2, barY + barH / 2 - 5);
        glVertex2i(lx2, barY + barH / 2 + 5);
        glVertex2i(lx2 + 12, barY + barH / 2);
        glEnd();
        glDisable(GL_BLEND);
    }

    {
        float sepAlpha = 0.3f + 0.05f * sinf(t * 1.5f);
        DrawHLine(barX, barY - 40, barW, GetUpdateColorRed(UPDATE_COLOR_DETAILS), GetUpdateColorGreen(UPDATE_COLOR_DETAILS), GetUpdateColorBlue(UPDATE_COLOR_DETAILS), sepAlpha);
    }

    int mainTitleX = (W - g_texMainTitleW) / 2;
    int mainTitleYPos = barY - 380;

    {
        static bool mainTitleBuilt = false;
        int tw = 900, th = 100;
        if (!mainTitleBuilt)
        {
            mainTitleBuilt = true;
            g_texMainTitle = BuildTextTexture(hDC,
                gProtect->m_MainInfo.m_TitleName,
                tw, th, 66, GetUpdateTextColor(UPDATE_COLOR_TEXT_TOP),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Tahoma", FW_BOLD);
            g_texMainTitleW = tw; g_texMainTitleH = th;
        }
        float mainTitlePulse = 0.9f + 0.1f * sinf(t * 0.8f);
        DrawTexture(g_texMainTitle, mainTitleX, mainTitleYPos,
            g_texMainTitleW, g_texMainTitleH, mainTitlePulse, 1.f, 1.f, 1.f);
    }

    {
        static Particle titleParticles[40];
        static bool titleParticlesInit = false;
        if (!titleParticlesInit)
        {
            titleParticlesInit = true;
            for (int i = 0; i < 40; i++)
            {
                titleParticles[i].x = (float)(mainTitleX + rand() % g_texMainTitleW);
                titleParticles[i].y = (float)(mainTitleYPos + rand() % g_texMainTitleH);
                titleParticles[i].vx = ((float)(rand() % 100) - 50) * 0.008f;
                titleParticles[i].vy = ((float)(rand() % 100) - 50) * 0.008f - 0.35f;
                titleParticles[i].maxLife = 50.f + (rand() % 100);
                titleParticles[i].life = (float)(rand() % (int)titleParticles[i].maxLife);
                titleParticles[i].size = 1.0f + (rand() % 20) * 0.1f;
                titleParticles[i].alpha = 0.f;
            }
        }
        for (int i = 0; i < 40; i++)
        {
            Particle& p = titleParticles[i];
            p.life += 1.f; p.x += p.vx; p.y += p.vy; p.vy -= 0.01f;
            float tt = p.life / p.maxLife;
            p.alpha = (tt < 0.2f) ? tt / 0.2f : 1.f - ((tt - 0.2f) / 0.8f);
            p.alpha = max(0.f, min(1.f, p.alpha));
            if (p.life >= p.maxLife)
            {
                p.x = (float)(mainTitleX + rand() % g_texMainTitleW);
                p.y = (float)(mainTitleYPos + rand() % g_texMainTitleH);
                p.vx = ((float)(rand() % 100) - 50) * 0.008f;
                p.vy = ((float)(rand() % 80) - 40) * 0.008f - 0.3f;
                p.maxLife = 50.f + (rand() % 100); p.life = 0.f;
                p.size = 1.0f + (rand() % 20) * 0.1f;
            }
        }
        glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); glPointSize(2.5f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 40; i++)
        {
            Particle& p = titleParticles[i];
            if (p.alpha <= 0.01f) continue;
            float pulse = 0.7f + 0.3f * sinf(p.life * 0.2f);
            glColor4f(
                GetUpdateColorRed(UPDATE_COLOR_PARTICLES, pulse),
                GetUpdateColorGreen(UPDATE_COLOR_PARTICLES, pulse),
                GetUpdateColorBlue(UPDATE_COLOR_PARTICLES, pulse),
                p.alpha * 0.8f);
            glVertex2f(p.x, p.y);
        }
        glEnd(); glDisable(GL_BLEND);
    }

    int subTitleX = (W - g_texSubTitleW) / 2;
    int subTitleY = mainTitleYPos + 80;

    {
        static bool subTitleBuilt = false;
        int tw = 600, th = 40;
        if (!subTitleBuilt)
        {
            subTitleBuilt = true;
            g_texSubTitle = BuildTextTexture(hDC,
                gProtect->m_MainInfo.m_SubTitleName,
                tw, th, 28, GetUpdateTextColor(UPDATE_COLOR_TEXT_TOP),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Trebuchet MS", FW_SEMIBOLD);
            g_texSubTitleW = tw; g_texSubTitleH = th;
        }
        float subTitlePulse = 0.85f + 0.15f * sinf(t * 1.0f);
        DrawTexture(g_texSubTitle, subTitleX, subTitleY,
            g_texSubTitleW, g_texSubTitleH, subTitlePulse, 1.f, 1.f, 1.f);
    }

    {
        static Particle subParticles[30];
        static bool subParticlesInit = false;
        if (!subParticlesInit)
        {
            subParticlesInit = true;
            for (int i = 0; i < 30; i++)
            {
                subParticles[i].x = (float)(subTitleX + rand() % g_texSubTitleW);
                subParticles[i].y = (float)(subTitleY + rand() % g_texSubTitleH);
                subParticles[i].vx = ((float)(rand() % 100) - 50) * 0.007f;
                subParticles[i].vy = ((float)(rand() % 100) - 50) * 0.007f - 0.3f;
                subParticles[i].maxLife = 45.f + (rand() % 90);
                subParticles[i].life = (float)(rand() % (int)subParticles[i].maxLife);
                subParticles[i].size = 0.8f + (rand() % 15) * 0.1f;
                subParticles[i].alpha = 0.f;
            }
        }
        for (int i = 0; i < 30; i++)
        {
            Particle& p = subParticles[i];
            p.life += 1.f; p.x += p.vx; p.y += p.vy; p.vy -= 0.009f;
            float tt = p.life / p.maxLife;
            p.alpha = (tt < 0.2f) ? tt / 0.2f : 1.f - ((tt - 0.2f) / 0.8f);
            p.alpha = max(0.f, min(1.f, p.alpha));
            if (p.life >= p.maxLife)
            {
                p.x = (float)(subTitleX + rand() % g_texSubTitleW);
                p.y = (float)(subTitleY + rand() % g_texSubTitleH);
                p.vx = ((float)(rand() % 100) - 50) * 0.007f;
                p.vy = ((float)(rand() % 80) - 40) * 0.007f - 0.25f;
                p.maxLife = 45.f + (rand() % 90); p.life = 0.f;
                p.size = 0.8f + (rand() % 15) * 0.1f;
            }
        }
        glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); glPointSize(2.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 30; i++)
        {
            Particle& p = subParticles[i];
            if (p.alpha <= 0.01f) continue;
            float pulse = 0.7f + 0.3f * sinf(p.life * 0.22f);
            glColor4f(
                GetUpdateColorRed(UPDATE_COLOR_PARTICLES, pulse),
                GetUpdateColorGreen(UPDATE_COLOR_PARTICLES, pulse),
                GetUpdateColorBlue(UPDATE_COLOR_PARTICLES, pulse),
                p.alpha * 0.7f);
            glVertex2f(p.x, p.y);
        }
        glEnd(); glDisable(GL_BLEND);
    }

    {
        static bool titleBuilt = false;
        static bool wasLoading = false;
        bool isLoading = (st.state == IUS_LOADING_GAME);
        if (!titleBuilt || wasLoading != isLoading)
        {
            titleBuilt = true;
            wasLoading = isLoading;

            char titleStr[256];
            snprintf(titleStr, sizeof(titleStr),
                isLoading ? "%s  |  CARREGANDO" : "%s  |  ATUALIZANDO",
                gProtect->m_MainInfo.m_TitleName);

            int tw = barW, th = 38;
            g_texTitle = BuildTextTexture(hDC, titleStr,
                tw, th, 22, GetUpdateTextColor(UPDATE_COLOR_TEXT_MIDDLE),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Tahoma", FW_BOLD, g_texTitle);
            g_texTitleW = tw; g_texTitleH = th;
        }
        float titlePulse = 0.85f + 0.15f * sinf(t * 1.2f);
        DrawTexture(g_texTitle, barX, barY - 80,
            g_texTitleW, g_texTitleH, titlePulse, 1.f, 1.f, 1.f);
    }

    {
        int tw = barW, th = 28;
        if (strcmp(st.statusText, g_cachedMain) != 0)
        {
            strcpy_s(g_cachedMain, st.statusText);
            g_texMain = BuildTextTexture(hDC, st.statusText,
                tw, th, 15, GetUpdateTextColor(UPDATE_COLOR_TEXT_MIDDLE),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Tahoma", FW_SEMIBOLD, g_texMain);
            g_texMainW = tw; g_texMainH = th;
        }
        DrawTexture(g_texMain, barX, barY - 28, g_texMainW, g_texMainH);
    }

    {
        char subLine[512];
        if (st.subText[0] != '\0')
            sprintf_s(subLine, "%.1f%%   |   %s", st.progress, st.subText);
        else if (st.state == IUS_EXTRACTING_FILES && st.totalFiles > 0)
            sprintf_s(subLine, "%.1f%%   |   Arquivo %d de %d",
                st.progress, st.extractedFiles, st.totalFiles);
        else
            sprintf_s(subLine, "%.1f%%", st.progress);

        int tw = barW, th = 24;
        if (strcmp(subLine, g_cachedSub) != 0)
        {
            strcpy_s(g_cachedSub, subLine);
            g_texSub = BuildTextTexture(hDC, subLine,
                tw, th, 13, GetUpdateTextColor(UPDATE_COLOR_TEXT_MIDDLE),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Consolas", FW_NORMAL, g_texSub);
            g_texSubW = tw; g_texSubH = th;
        }
        DrawTexture(g_texSub, barX, barY + barH + 10, g_texSubW, g_texSubH);
    }

    if (st.state == IUS_EXTRACTING_FILES && st.currentFile[0] != '\0')
    {
        char fileInfo[512];
        if (st.currentDir[0] != '\0')
            sprintf_s(fileInfo, "%s%s", st.currentDir, st.currentFile);
        else
            sprintf_s(fileInfo, "%s", st.currentFile);

        static char   cachedFile[512] = {};
        static GLuint texFile = 0;
        static int    texFileW = 0, texFileH = 0;
        int tw = barW, th = 20;
        if (strcmp(fileInfo, cachedFile) != 0)
        {
            strcpy_s(cachedFile, fileInfo);
            texFile = BuildTextTexture(hDC, fileInfo,
                tw, th, 11, GetUpdateTextColor(UPDATE_COLOR_TEXT_MIDDLE),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Consolas", FW_NORMAL, texFile);
            texFileW = tw; texFileH = th;
        }
        float fileAlpha = 0.6f + 0.3f * sinf(t * 4.f);
        DrawTexture(texFile, barX, barY + barH + 36,
            texFileW, texFileH, fileAlpha, 1.f, 1.f, 1.f);
    }

    if (gProtect->m_MainInfo.m_UpdateTipsCount > 0)
    {
        int tipCount = gProtect->m_MainInfo.m_UpdateTipsCount;

        if (tipCount > UPDATE_TIP_MAX)
        {
            tipCount = UPDATE_TIP_MAX;
        }

        DWORD now = GetTickCount();
        if (now - g_lastTipChange > 15000 && tipCount > 1)
        {
            g_lastTipChange = now;
            int nextTip;
            do { nextTip = rand() % tipCount; } while (nextTip == g_currentTip);
            g_currentTip = nextTip;
        }
        float tipAlpha = 0.55f + 0.2f * sinf(t * 0.8f);
        const char* tip = gProtect->m_MainInfo.m_UpdateTips[g_currentTip];
        int tw = barW + 200, th = 52;
        if (strcmp(tip, g_cachedTip) != 0)
        {
            strcpy_s(g_cachedTip, tip);
            g_texTip = BuildTextTexture(hDC, tip,
                tw, th, 30, GetUpdateTextColor(UPDATE_COLOR_TEXT_BOTTOM),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Tahoma", FW_NORMAL, g_texTip);
            g_texTipW = tw; g_texTipH = th;
        }
        int tipX = (W - g_texTipW) / 2;
        int tipY = barY + barH + 76;
        DrawTexture(g_texTip, tipX, tipY, g_texTipW, g_texTipH, tipAlpha);
        DrawHLine(barX + 20, tipY - 6, barW - 40,
            GetUpdateColorRed(UPDATE_COLOR_DETAILS),
            GetUpdateColorGreen(UPDATE_COLOR_DETAILS),
            GetUpdateColorBlue(UPDATE_COLOR_DETAILS), 0.35f);
    }

    {
        int cx = barX - 6, cy = barY - 86;
        int cw = barW + 12, ch = barH + 100;
        int len = 18;
        float ca = 0.4f + 0.1f * sinf(t * 2.5f);
        glDisable(GL_TEXTURE_2D); glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1.5f); glColor4f(GetUpdateColorRed(UPDATE_COLOR_DETAILS), GetUpdateColorGreen(UPDATE_COLOR_DETAILS), GetUpdateColorBlue(UPDATE_COLOR_DETAILS), ca);
        glBegin(GL_LINES);
        glVertex2i(cx, cy);      glVertex2i(cx + len, cy);
        glVertex2i(cx, cy);      glVertex2i(cx, cy + len);
        glVertex2i(cx + cw, cy);      glVertex2i(cx + cw - len, cy);
        glVertex2i(cx + cw, cy);      glVertex2i(cx + cw, cy + len);
        glVertex2i(cx, cy + ch); glVertex2i(cx + len, cy + ch);
        glVertex2i(cx, cy + ch); glVertex2i(cx, cy + ch - len);
        glVertex2i(cx + cw, cy + ch); glVertex2i(cx + cw - len, cy + ch);
        glVertex2i(cx + cw, cy + ch); glVertex2i(cx + cw, cy + ch - len);
        glEnd(); glLineWidth(1.f); glDisable(GL_BLEND);
    }

    {
        static GLuint texVer = 0;
        static char   cachedVer[256] = {};
        char verStr[256];
        if (st.localVersion > 0 || st.serverFileVer > 0)
            sprintf_s(verStr, "v%d  ->  v%d", st.localVersion, st.serverFileVer);
        else
            sprintf_s(verStr, "%s  |  %s", gProtect->m_MainInfo.m_TitleName, gProtect->m_MainInfo.m_SubTitleName);
        if (strcmp(verStr, cachedVer) != 0)
        {
            strcpy_s(cachedVer, verStr);
            int tw = 400, th = 18;
            texVer = BuildTextTexture(hDC, verStr, tw, th, 11,
                GetUpdateTextColor(UPDATE_COLOR_TEXT_BOTTOM),
                DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                "Tahoma", FW_NORMAL, texVer);
        }
        DrawTexture(texVer, (W - 400) / 2, H - 30, 400, 18, 0.7f);
    }

    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}
static void LaunchCmdUpdaterAndExit(const char* zipPath,
    const char* appDir,
    int         newExeVer)
{
    char currentExe[MAX_PATH];
    GetModuleFileNameA(NULL, currentExe, MAX_PATH);

    char iniPath[MAX_PATH];
    GetIniExePath(iniPath, MAX_PATH);

    // Remove barra final ANTES de montar o cmdLine
    // Sem isso, appDir termina em \" que quebra o parse no processo filho
    char appDirClean[MAX_PATH];
    strcpy_s(appDirClean, appDir);
    size_t adLen = strlen(appDirClean);
    if (adLen > 0 && (appDirClean[adLen - 1] == '\\' || appDirClean[adLen - 1] == '/'))
        appDirClean[adLen - 1] = '\0';

    char cmdLine[MAX_PATH * 4];
    sprintf_s(cmdLine,
        "\"%s\" --apply-update %lu \"%s\" \"%s\" %d \"%s\"",
        currentExe,
        GetCurrentProcessId(),
        zipPath,
        appDirClean,
        newExeVer,
        iniPath);

    STARTUPINFOA si = {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {};

    WriteErrorLog("Lancando updater: %s", cmdLine);

    if (!CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        WriteErrorLog("Falha ao lancar instancia de update. err=%lu", GetLastError());
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    SetStatus(IUS_DONE, 100.f, "Aplicando atualizacao, aguarde...");
    Sleep(500);

    PostMessageA(g_hGameWnd, WM_CLOSE, 0, 0);
}

void InGameUpdate_RunAsUpdater(const char* cmdLine)
{
    if (!g_csInit)
    {
        InitializeCriticalSection(&g_csUpdate);
        g_csInit = true;
    }

    WriteErrorLog("RunAsUpdater chamado. cmdLine=%s", cmdLine ? cmdLine : "(null)");

    DWORD parentPid = 0;
    char  zipPath[MAX_PATH] = {};
    char  appDir[MAX_PATH] = {};
    int   newVer = 0;
    char  iniPath[MAX_PATH] = {};

    // Parse manual — lê token entre aspas (fecha na PRÓXIMA aspa, sem escape)
    // Isso funciona porque LaunchCmdUpdaterAndExit agora NÃO passa appDir com barra final
    auto readToken = [](const char*& src, char* dst, size_t dstSize) -> bool
        {
            while (*src == ' ') src++;
            if (*src == '\0') return false;

            if (*src == '"')
            {
                src++; // pula aspa de abertura
                size_t i = 0;
                while (*src && *src != '"') // fecha apenas na próxima aspa — sem lógica de escape
                {
                    if (i + 1 < dstSize)
                        dst[i++] = *src;
                    src++;
                }
                dst[i] = '\0';
                if (*src == '"') src++; // pula aspa de fechamento
            }
            else
            {
                size_t i = 0;
                while (*src && *src != ' ')
                {
                    if (i + 1 < dstSize)
                        dst[i++] = *src;
                    src++;
                }
                dst[i] = '\0';
            }
            return true;
        };

    // Pula "--apply-update"
    const char* p = cmdLine;
    while (*p == ' ') p++;
    if (_strnicmp(p, "--apply-update", 14) == 0) p += 14;
    while (*p == ' ') p++;

    // Lê PID
    parentPid = (DWORD)atol(p);
    while (*p && *p != ' ') p++;

    readToken(p, zipPath, MAX_PATH);
    readToken(p, appDir, MAX_PATH);

    // Lê versão
    while (*p == ' ') p++;
    newVer = atoi(p);
    while (*p && *p != ' ') p++;

    readToken(p, iniPath, MAX_PATH);

    WriteErrorLog("RunAsUpdater parsed: pid=%lu zip=%s appDir=%s ver=%d ini=%s",
        parentPid, zipPath, appDir, newVer, iniPath);

    // Valida parse — se ver=0 ou ini vazio, algo deu errado
    if (newVer <= 0 || iniPath[0] == '\0' || zipPath[0] == '\0')
    {
        WriteErrorLog("RunAsUpdater: parse falhou. Abortando para evitar loop infinito.");
        return; // retorna sem relancar — evita o loop
    }

    char currentExe[MAX_PATH];
    GetModuleFileNameA(NULL, currentExe, MAX_PATH);

    WriteErrorLog("RunAsUpdater: aguardando processo pai (pid=%lu) encerrar...", parentPid);

    HANDLE hParent = OpenProcess(SYNCHRONIZE, FALSE, parentPid);
    if (hParent)
    {
        DWORD waitResult = WaitForSingleObject(hParent, 20000);
        CloseHandle(hParent);
        WriteErrorLog("RunAsUpdater: WaitForSingleObject retornou %lu", waitResult);
        Sleep(1500);
    }
    else
    {
        WriteErrorLog("RunAsUpdater: nao abriu handle do pai (err=%lu), aguardando 3s", GetLastError());
        Sleep(3000);
    }

    // Confirma que o EXE foi liberado via rename em loop
    char exeRunning[MAX_PATH];
    sprintf_s(exeRunning, "%s.running", currentExe);
    DeleteFileA(exeRunning);

    bool exeFree = false;
    for (int attempt = 0; attempt < 120; attempt++)
    {
        if (MoveFileExA(currentExe, exeRunning, 0))
        {
            MoveFileExA(exeRunning, currentExe, MOVEFILE_REPLACE_EXISTING);
            exeFree = true;
            WriteErrorLog("RunAsUpdater: EXE liberado apos %d tentativas", attempt + 1);
            break;
        }
        Sleep(500);
    }

    if (!exeFree)
        WriteErrorLog("RunAsUpdater: timeout aguardando EXE. Continuando mesmo assim.");

    Sleep(500);

    // ── Extrai o ZIP ─────────────────────────────────────────────────────────
    WriteErrorLog("RunAsUpdater: iniciando extracao. zip=%s dest=%s", zipPath, appDir);

    mz_zip_archive zip = {};
    if (!mz_zip_reader_init_file(&zip, zipPath, 0))
    {
        WriteErrorLog("RunAsUpdater: falha ao abrir zip. path=%s err=%lu", zipPath, GetLastError());
        goto cleanup;
    }

    {
        int totalFiles = (int)mz_zip_reader_get_num_files(&zip);
        int extracted = 0;

        for (int i = 0; i < totalFiles; i++)
        {
            mz_zip_archive_file_stat stat = {};
            if (!mz_zip_reader_file_stat(&zip, i, &stat)) continue;
            if (mz_zip_reader_is_file_a_directory(&zip, i)) continue;

            char destPath[MAX_PATH];
            sprintf_s(destPath, "%s\\%s", appDir, stat.m_filename);
            for (char* c = destPath; *c; c++)
                if (*c == '/') *c = '\\';

            // Cria diretórios intermediários
            {
                char dirBuf[MAX_PATH];
                strcpy_s(dirBuf, destPath);
                char* last = strrchr(dirBuf, '\\');
                if (last)
                {
                    *last = '\0';
                    char* c = dirBuf + 3;
                    while (*c)
                    {
                        if (*c == '\\')
                        {
                            *c = '\0';
                            CreateDirectoryA(dirBuf, NULL);
                            *c = '\\';
                        }
                        c++;
                    }
                    CreateDirectoryA(dirBuf, NULL);
                }
            }

            // Para Main.exe: renomeia para .old antes de extrair
            bool isMainExe = (_stricmp(stat.m_filename, "Main.exe") == 0);
            if (isMainExe)
            {
                char oldExe[MAX_PATH];
                sprintf_s(oldExe, "%s.old", destPath);
                DeleteFileA(oldExe);

                bool renamed = false;
                for (int r = 0; r < 60; r++)
                {
                    if (MoveFileA(destPath, oldExe))
                    {
                        renamed = true;
                        WriteErrorLog("RunAsUpdater: Main.exe renomeado para .old na tentativa %d", r + 1);
                        break;
                    }
                    WriteErrorLog("RunAsUpdater: aguardando Main.exe ser liberado (tentativa %d, err=%lu)", r + 1, GetLastError());
                    Sleep(500);
                }

                if (!renamed)
                {
                    WriteErrorLog("RunAsUpdater: nao foi possivel renomear Main.exe. Abortando.");
                    mz_zip_reader_end(&zip);
                    goto cleanup;
                }
            }

            if (!mz_zip_reader_extract_to_file(&zip, i, destPath, 0))
            {
                WriteErrorLog("RunAsUpdater: falha ao extrair %s -> %s err=%lu",
                    stat.m_filename, destPath, GetLastError());

                if (isMainExe)
                {
                    char oldExe[MAX_PATH];
                    sprintf_s(oldExe, "%s.old", destPath);
                    MoveFileA(oldExe, destPath);
                }
            }
            else
            {
                extracted++;
                WriteErrorLog("RunAsUpdater: extraido %s", stat.m_filename);

                if (isMainExe)
                {
                    char oldExe[MAX_PATH];
                    sprintf_s(oldExe, "%s.old", destPath);
                    DeleteFileA(oldExe);
                }
            }
        }

        mz_zip_reader_end(&zip);
        WriteErrorLog("RunAsUpdater: extracao concluida. %d arquivo(s) extraidos.", extracted);
    }

    DeleteFileA(zipPath);

    // ── Atualiza versão no INI ────────────────────────────────────────────────
    {
        char dirBuf[MAX_PATH];
        strcpy_s(dirBuf, iniPath);
        char* last = strrchr(dirBuf, '\\');
        if (last) { *last = '\0'; SHCreateDirectoryExA(NULL, dirBuf, NULL); }

        char iniContent[4096] = {};
        HANDLE hRead = CreateFileA(iniPath, GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hRead != INVALID_HANDLE_VALUE)
        {
            DWORD bytesRead = 0;
            ReadFile(hRead, iniContent, sizeof(iniContent) - 1, &bytesRead, NULL);
            CloseHandle(hRead);
        }

        char newContent[4096] = {};
        char newLine[64];
        sprintf_s(newLine, "update_version = %d", newVer);

        bool replaced = false;
        char* pos = strstr(iniContent, "update_version");
        if (pos)
        {
            size_t beforeLen = pos - iniContent;
            memcpy(newContent, iniContent, beforeLen);
            char* lineEnd = strchr(pos, '\n');
            const char* after = lineEnd ? lineEnd + 1 : pos + strlen(pos);
            sprintf_s(newContent + beforeLen, sizeof(newContent) - beforeLen,
                "%s\r\n%s", newLine, after);
            replaced = true;
        }

        if (!replaced)
            sprintf_s(newContent, "[Update Config]\r\nupdate_version = %d\r\n", newVer);

        HANDLE hWrite = CreateFileA(iniPath, GENERIC_WRITE, 0,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hWrite != INVALID_HANDLE_VALUE)
        {
            DWORD written = 0;
            WriteFile(hWrite, newContent, (DWORD)strlen(newContent), &written, NULL);
            CloseHandle(hWrite);
            WriteErrorLog("RunAsUpdater: ini atualizado. version=%d", newVer);
        }
        else
        {
            WriteErrorLog("RunAsUpdater: falha ao escrever ini. err=%lu", GetLastError());
        }
    }

cleanup:
    WriteErrorLog("RunAsUpdater: relancando o jogo. exe=%s", currentExe);

    SHELLEXECUTEINFOA sei = {};
    sei.cbSize = sizeof(sei);
    sei.lpVerb = "open";
    sei.lpFile = currentExe;
    sei.nShow = SW_SHOWNORMAL;
    if (!ShellExecuteExA(&sei))
        WriteErrorLog("RunAsUpdater: falha ao relancar o jogo. err=%lu", GetLastError());
}

static DWORD WINAPI UpdateThreadProc(LPVOID)
{
    char appDir[MAX_PATH];
    GetAppDir(appDir, MAX_PATH);
    {
        char currentExe[MAX_PATH];
        GetModuleFileNameA(NULL, currentExe, MAX_PATH);
        char oldExe[MAX_PATH];
        sprintf_s(oldExe, "%s.old", currentExe);
        if (GetFileAttributesA(oldExe) != INVALID_FILE_ATTRIBUTES)
        {
            if (DeleteFileA(oldExe))
                WriteErrorLog("UpdateThreadProc: Main.exe.old residual removido.");
            else
                WriteErrorLog("UpdateThreadProc: falha ao remover Main.exe.old. err=%lu", GetLastError());
        }
    }
    if (g_bShutdown)
    {
        ExitThread(0);
        return 0;
    }

    DWORD checkStart = GetTickCount();

    SetStatus(IUS_CHECKING, 0.f, g_checkingMsgs[0]);
    Sleep(300);

    SetStatus(IUS_CHECKING, 5.f, g_checkingMsgs[1]);

    int localFileVer = ReadLocalVersion("update_version1");
    int localExeVer = ReadLocalVersion("update_version");

    SetStatus(IUS_CHECKING, 15.f, g_checkingMsgs[2]);
    Sleep(400);

    SetStatus(IUS_CHECKING, 30.f, g_checkingMsgs[3]);

    SetStatus(IUS_CHECKING, 45.f, g_checkingMsgs[4]);
    Sleep(300);

    SetStatus(IUS_CHECKING, 60.f, g_checkingMsgs[5]);
    int serverFileVer = QueryServerVersion(gProtect->m_MainInfo.m_AutoUpdateURL, INGAME_UPDATE_FILE_PATTERN);
    g_UpdateStatus.localVersion = localFileVer;
    g_UpdateStatus.serverFileVer = serverFileVer;
    if (serverFileVer < 0)
        WriteErrorLog("Falha ao consultar versao dos arquivos no servidor. url=%s pattern=%s",
            gProtect->m_MainInfo.m_AutoUpdateURL, INGAME_UPDATE_FILE_PATTERN);

    SetStatus(IUS_CHECKING, 80.f, "Processando resultado...");
    Sleep(300);

    DWORD elapsed = GetTickCount() - checkStart;
    if (elapsed < 3000)
    {
        DWORD remaining = 3000 - elapsed;
        DWORD steps = remaining / 50;
        if (steps < 1) steps = 1;
        float progStep = (100.f - 80.f) / steps;
        float prog = 80.f;
        for (DWORD s = 0; s < steps; s++)
        {
            prog += progStep;
            if (prog > 100.f) prog = 100.f;
            SetStatus(IUS_CHECKING, prog, "Finalizando verificacao...");
            Sleep(50);
        }
    }
    else
    {
        SetStatus(IUS_CHECKING, 100.f, "Finalizando verificacao...");
        Sleep(200);
    }

    // ── Atualização de arquivos (patches) ──────────────────────────────────
    bool didUpdateFiles = false;

    if (serverFileVer > localFileVer && serverFileVer > 0)
    {
        didUpdateFiles = true;
        int totalPatches = serverFileVer - localFileVer;
        int patchIdx = 0;

        for (int patchVer = localFileVer + 1; patchVer <= serverFileVer; patchVer++)
        {
            if (g_bShutdown) return 0;

            patchIdx++;

            char zipName[128], url[512], tempZip[MAX_PATH];
            sprintf_s(zipName, "%s%03d.zip", INGAME_UPDATE_FILE_PATTERN, patchVer);
            sprintf_s(url, "%s%s", gProtect->m_MainInfo.m_AutoUpdateURL, zipName);
            sprintf_s(tempZip, "%s%s", appDir, zipName);

            char statusBuf[256];
            sprintf_s(statusBuf, "Baixando patch %d de %d: %s", patchIdx, totalPatches, zipName);
            SetStatus(IUS_DOWNLOADING_FILES, 0.f, statusBuf);
            Sleep(200);

            if (!DownloadFile(url, tempZip))
            {
                WriteErrorLog("Patch nao encontrado, pulando para o proximo. url=%s dest=%s err=%lu",
                    url, tempZip, GetLastError());

                for (int p = 0; p <= 100; p += 5)
                {
                    char animBuf[256];
                    sprintf_s(animBuf, "Baixando patch %d de %d: %s",
                        patchIdx, totalPatches, zipName);
                    SetStatus(IUS_DOWNLOADING_FILES, (float)p, animBuf);
                    Sleep(15);
                }

                char skipBuf[256];
                sprintf_s(skipBuf, "Patch %d de %d concluido.", patchIdx, totalPatches);
                SetStatus(IUS_DOWNLOADING_FILES, 100.f, skipBuf);
                Sleep(250);
                continue;
            }

            {
                HANDLE hChk = CreateFileA(tempZip, GENERIC_READ, FILE_SHARE_READ,
                    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hChk == INVALID_HANDLE_VALUE)
                {
                    WriteErrorLog("Nao foi possivel abrir o ZIP apos download. path=%s err=%lu",
                        tempZip, GetLastError());
                    SetStatus(IUS_ERROR, 0.f, "Erro ao verificar arquivo baixado.");
                    return 0;
                }
                DWORD fsize = GetFileSize(hChk, NULL);
                CloseHandle(hChk);
                if (fsize < 22)
                {
                    WriteErrorLog("ZIP corrompido ou incompleto. path=%s tamanho=%lu bytes",
                        tempZip, fsize);
                    SetStatus(IUS_ERROR, 0.f, "Arquivo de atualizacao corrompido.");
                    DeleteFileA(tempZip);
                    return 0;
                }
            }

            char extractBuf[256];
            sprintf_s(extractBuf, "Extraindo patch %d de %d...", patchIdx, totalPatches);
            SetStatus(IUS_EXTRACTING_FILES, 0.f, extractBuf);
            Sleep(200);

            if (!ExtractZip(tempZip, appDir))
            {
                WriteErrorLog("Falha na extracao do ZIP, pulando para o proximo. zip=%s dest=%s",
                    tempZip, appDir);

                for (int p = 0; p <= 100; p += 5)
                {
                    char animBuf[256];
                    sprintf_s(animBuf, "Extraindo patch %d de %d...", patchIdx, totalPatches);
                    SetStatus(IUS_EXTRACTING_FILES, (float)p, animBuf);
                    Sleep(15);
                }

                DeleteFileA(tempZip);

                char skipBuf[256];
                sprintf_s(skipBuf, "Patch %d de %d concluido.", patchIdx, totalPatches);
                SetStatus(IUS_EXTRACTING_FILES, 100.f, skipBuf);
                Sleep(250);
                continue;
            }

            DeleteFileA(tempZip);
            WriteLocalVersion("update_version1", patchVer);
        }
    }

    // ── Após arquivos: verifica atualização do EXE ─────────────────────────
    SetStatus(IUS_CHECKING, 0.f, "Verificando atualizacao do executavel...");
    Sleep(300);

    int serverExeVer = QueryServerVersion(gProtect->m_MainInfo.m_AutoUpdateURL, INGAME_UPDATE_EXE_PATTERN);
    g_UpdateStatus.serverExeVer = serverExeVer;
    if (serverExeVer < 0)
        WriteErrorLog("Falha ao consultar versao do EXE no servidor. url=%s pattern=%s",
            gProtect->m_MainInfo.m_AutoUpdateURL, INGAME_UPDATE_EXE_PATTERN);

    if (serverExeVer > 0 && (localExeVer <= 0 || localExeVer < serverExeVer))
    {
        // Tem atualização no EXE: baixa e lança CMD updater
        SetStatus(IUS_NEED_EXE_UPDATE, 0.f, "Nova versao do executavel detectada! Baixando...");
        Sleep(500);

        char zipName[128], url[512], tempZip[MAX_PATH];
        sprintf_s(zipName, "%s%03d.zip", INGAME_UPDATE_EXE_PATTERN, serverExeVer);
        sprintf_s(url, "%s%s", gProtect->m_MainInfo.m_AutoUpdateURL, zipName);
        sprintf_s(tempZip, "%s%s", appDir, zipName);

        SetStatus(IUS_DOWNLOADING_FILES, 0.f, "Baixando atualizacao do executavel...");

        if (!DownloadFile(url, tempZip))
        {
            WriteErrorLog("Falha ao baixar EXE update. url=%s dest=%s", url, tempZip);
            SetStatus(IUS_ERROR, 0.f, "Erro ao baixar atualizacao do executavel.");
            Sleep(2000);
            return 0;
        }

        {
            HANDLE hChk = CreateFileA(tempZip, GENERIC_READ, FILE_SHARE_READ,
                NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hChk == INVALID_HANDLE_VALUE)
            {
                WriteErrorLog("Nao foi possivel abrir o ZIP do EXE apos download. path=%s err=%lu",
                    tempZip, GetLastError());
                SetStatus(IUS_ERROR, 0.f, "Erro ao verificar arquivo baixado.");
                return 0;
            }
            DWORD fsize = GetFileSize(hChk, NULL);
            CloseHandle(hChk);
            if (fsize < 22)
            {
                WriteErrorLog("ZIP do EXE corrompido. path=%s tamanho=%lu bytes", tempZip, fsize);
                SetStatus(IUS_ERROR, 0.f, "Arquivo de atualizacao corrompido.");
                DeleteFileA(tempZip);
                return 0;
            }
        }

        SetStatus(IUS_NEED_EXE_UPDATE, 100.f, "Download concluido! Aplicando atualizacao...");
        Sleep(800);

        LaunchCmdUpdaterAndExit(tempZip, appDir, serverExeVer);
        return 0;
    }

    // Sem atualização no EXE:
    // - Se atualizou arquivos: reinicia para aplicar
    // - Se não havia nada: entra no jogo normalmente
    if (didUpdateFiles)
    {
        SetStatus(IUS_DONE, 100.f, "Atualizacao concluida! Reiniciando...");
        Sleep(1200);
        RestartGameAndExit();
    }
    else
    {
        SetStatus(IUS_DONE, 100.f, "Tudo atualizado! Entrando no jogo...");
        Sleep(1200);
    }

    return 0;
}
static void SetStatus(INGAME_UPDATE_STATE state, float pct, const char* text, const char* sub)
{
    if (g_csInit) EnterCriticalSection(&g_csUpdate);
    g_UpdateStatus.state = state;
    g_UpdateStatus.progress = pct;
    strcpy_s(g_UpdateStatus.statusText, text ? text : "");
    strcpy_s(g_UpdateStatus.subText, sub ? sub : "");
    if (g_csInit) LeaveCriticalSection(&g_csUpdate);
}

static void GetIniExePath(char* outPath, DWORD size)
{
    char appDir[MAX_PATH];
    GetAppDir(appDir, MAX_PATH);
    sprintf_s(outPath, size, "%s%s", appDir, INGAME_UPDATE_INI_EXE_PATH);
}

static void TrimBuf(char* s)
{
    if (!s || !*s) return;
    char* end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
        *end-- = '\0';
    char* start = s;
    while (*start == ' ' || *start == '\t') start++;
    if (start != s)
        memmove(s, start, strlen(start) + 1);
}

static int ReadLocalVersion(const char* key)
{
    bool isExeKey = (strcmp(key, "update_version") == 0);

    char iniPath[MAX_PATH] = {};
    if (isExeKey)
        GetIniExePath(iniPath, MAX_PATH);
    else
        GetIniPath(iniPath, MAX_PATH);

    if (!isExeKey && GetFileAttributesA(iniPath) == INVALID_FILE_ATTRIBUTES)
    {
        char dirPath[MAX_PATH];
        strcpy_s(dirPath, iniPath);
        char* lastSlash = strrchr(dirPath, '\\');
        if (lastSlash)
        {
            *lastSlash = '\0';
            SHCreateDirectoryExA(NULL, dirPath, NULL);
        }
        HANDLE hFile = CreateFileA(iniPath, GENERIC_WRITE, FILE_SHARE_READ,
            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            const char* defaultContent = "[GameMain]\r\nupdate_version1=0\r\n";
            DWORD written = 0;
            WriteFile(hFile, defaultContent, (DWORD)strlen(defaultContent), &written, NULL);
            CloseHandle(hFile);
        }
    }

    if (isExeKey)
    {
        DWORD attr = GetFileAttributesA(iniPath);

        if (attr == INVALID_FILE_ATTRIBUTES)
        {
            WriteErrorLog("Arquivo de versao do EXE nao encontrado. path=%s", iniPath);
            return 0;
        }

        HANDLE hRaw = CreateFileA(iniPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hRaw == INVALID_HANDLE_VALUE)
        {
            WriteErrorLog("Falha ao abrir arquivo de versao do EXE. path=%s err=%lu",
                iniPath, GetLastError());
            return 0;
        }

        DWORD fileSize = GetFileSize(hRaw, NULL);
        char rawBuf[512] = {};
        DWORD bytesRead = 0;
        ReadFile(hRaw, rawBuf, min(fileSize, (DWORD)511), &bytesRead, NULL);
        CloseHandle(hRaw);

        // Pula BOM
        char* cursor = rawBuf;
        if (bytesRead >= 3 && (UCHAR)cursor[0] == 0xEF && (UCHAR)cursor[1] == 0xBB && (UCHAR)cursor[2] == 0xBF)
            cursor += 3;

        bool inSection = false;
        char* lineStart = cursor;
        char* bufEnd = rawBuf + bytesRead;

        while (lineStart < bufEnd)
        {
            char* lineEnd = lineStart;
            while (lineEnd < bufEnd && *lineEnd != '\n' && *lineEnd != '\r') lineEnd++;

            char line[256] = {};
            size_t len = lineEnd - lineStart;
            if (len >= sizeof(line)) len = sizeof(line) - 1;
            memcpy(line, lineStart, len);
            line[len] = '\0';

            lineStart = lineEnd;
            while (lineStart < bufEnd && (*lineStart == '\n' || *lineStart == '\r')) lineStart++;

            TrimBuf(line);
            if (line[0] == '\0' || line[0] == ';' || line[0] == '#') continue;

            if (line[0] == '[')
            {
                inSection = false;
                char* open = line + 1;
                char* close = strchr(open, ']');
                if (close)
                {
                    char sec[128] = {};
                    size_t slen = close - open;
                    if (slen >= sizeof(sec)) slen = sizeof(sec) - 1;
                    memcpy(sec, open, slen);
                    sec[slen] = '\0';
                    TrimBuf(sec);
                    inSection = (_stricmp(sec, "Update Config") == 0);
                }
                continue;
            }

            if (!inSection) continue;

            char* eq = strchr(line, '=');
            if (!eq) continue;

            char keyBuf[64] = {};
            size_t klen = eq - line;
            if (klen >= sizeof(keyBuf)) klen = sizeof(keyBuf) - 1;
            memcpy(keyBuf, line, klen);
            keyBuf[klen] = '\0';
            TrimBuf(keyBuf);

            if (strcmp(keyBuf, "update_version") == 0)
            {
                char valBuf[64] = {};
                strcpy_s(valBuf, eq + 1);
                TrimBuf(valBuf);
                return atoi(valBuf);
            }
        }

        WriteErrorLog("Chave 'update_version' nao encontrada em [Update Config]. path=%s", iniPath);
        return 0;
    }
    else
    {
        int result = GetPrivateProfileIntA("GameMain", key, 0, iniPath);
        return result;
    }
}

static void WriteLocalVersion(const char* key, int ver)
{
    char iniPath[MAX_PATH];
    GetIniPath(iniPath, MAX_PATH);

    char dirBuf[MAX_PATH]; strcpy_s(dirBuf, iniPath);
    char* p = dirBuf;
    while (*p)
    {
        if (*p == '\\' && p != dirBuf)
        {
            char saved = *p; *p = '\0';
            CreateDirectoryA(dirBuf, NULL);
            *p = saved;
        }
        p++;
    }
    char val[32]; sprintf_s(val, "%d", ver);
    BOOL ok = WritePrivateProfileStringA("GameMain", key, val, iniPath);
    if (!ok)
        WriteErrorLog("Falha ao salvar versao local. key=%s ver=%d path=%s err=%lu",
            key, ver, iniPath, GetLastError());
}

static int QueryServerVersion(const char* baseUrl, const char* pattern)
{
    HINTERNET hInet = InternetOpenA(
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (!hInet)
    {
        WriteErrorLog("InternetOpen falhou ao consultar servidor. err=%lu", GetLastError());
        return -1;
    }

    HINTERNET hUrl = InternetOpenUrlA(hInet, baseUrl, NULL, 0,
        INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (!hUrl)
    {
        WriteErrorLog("InternetOpenUrl falhou. url=%s err=%lu", baseUrl, GetLastError());
        InternetCloseHandle(hInet);
        return -1;
    }

    std::string html;
    char buf[4096]; DWORD read = 0;
    while (!g_bShutdown &&
        InternetReadFile(hUrl, buf, sizeof(buf) - 1, &read) &&
        read > 0)
    {
        buf[read] = '\0'; html += buf; read = 0;
    }
    InternetCloseHandle(hUrl); InternetCloseHandle(hInet);

    if (html.empty())
    {
        WriteErrorLog("Resposta vazia do servidor. url=%s pattern=%s", baseUrl, pattern);
        return -1;
    }

    std::string rxPat = std::string(pattern) + "(\\d+)\\.zip";
    std::regex  rx(rxPat, std::regex::icase);
    auto it = std::sregex_iterator(html.begin(), html.end(), rx);
    auto end = std::sregex_iterator();
    int highest = -1;
    for (; it != end; ++it)
    {
        int n = std::stoi((*it)[1].str()); if (n > highest) highest = n;
    }

    if (highest < 0)
        WriteErrorLog("Nenhuma versao encontrada no servidor. url=%s pattern=%s", baseUrl, pattern);

    return highest;
}

static bool DownloadFile(const char* url, const char* destPath)
{
    if (g_bShutdown) return false;

    HINTERNET hSess = WinHttpOpen(
        L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSess)
    {
        WriteErrorLog("WinHttpOpen falhou. err=%lu", GetLastError());
        return false;
    }

    DWORD dwTimeout120 = 120000;
    DWORD dwTimeout30 = 30000;
    WinHttpSetOption(hSess, WINHTTP_OPTION_RECEIVE_RESPONSE_TIMEOUT, &dwTimeout120, sizeof(DWORD));
    WinHttpSetOption(hSess, WINHTTP_OPTION_RECEIVE_TIMEOUT, &dwTimeout120, sizeof(DWORD));
    WinHttpSetOption(hSess, WINHTTP_OPTION_SEND_TIMEOUT, &dwTimeout120, sizeof(DWORD));
    WinHttpSetOption(hSess, WINHTTP_OPTION_CONNECT_TIMEOUT, &dwTimeout30, sizeof(DWORD));

    char urlCopy[1024];
    strcpy_s(urlCopy, url);

    bool isHttps = (strncmp(urlCopy, "https://", 8) == 0);
    char* hostStart = urlCopy + (isHttps ? 8 : 7);
    char* pathStart = strchr(hostStart, '/');
    char host[256] = {}, path[1024] = "/";
    if (pathStart)
    {
        size_t hostLen = pathStart - hostStart;
        strncpy_s(host, hostStart, hostLen);
        strcpy_s(path, pathStart);
    }
    else strcpy_s(host, hostStart);

    wchar_t wHost[256], wPath[1024];
    MultiByteToWideChar(CP_ACP, 0, host, -1, wHost, 256);
    MultiByteToWideChar(CP_ACP, 0, path, -1, wPath, 1024);

    INTERNET_PORT port = isHttps ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT;
    HINTERNET hConn = WinHttpConnect(hSess, wHost, port, 0);
    if (!hConn)
    {
        WriteErrorLog("WinHttpConnect falhou. host=%s err=%lu", host, GetLastError());
        WinHttpCloseHandle(hSess);
        return false;
    }

    DWORD flags = isHttps ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hReq = WinHttpOpenRequest(hConn, L"GET", wPath,
        NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    if (!hReq)
    {
        WriteErrorLog("WinHttpOpenRequest falhou. err=%lu", GetLastError());
        WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSess);
        return false;
    }

    WinHttpAddRequestHeaders(hReq,
        L"Accept: */*\r\nConnection: keep-alive",
        (DWORD)-1, WINHTTP_ADDREQ_FLAG_ADD);

    if (!WinHttpSendRequest(hReq, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
    {
        WriteErrorLog("WinHttpSendRequest falhou. url=%s err=%lu", url, GetLastError());
        WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSess);
        return false;
    }

    if (!WinHttpReceiveResponse(hReq, NULL))
    {
        WriteErrorLog("WinHttpReceiveResponse falhou. url=%s err=%lu", url, GetLastError());
        WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSess);
        return false;
    }

    LONGLONG contentLen = -1;
    {
        wchar_t clStr[64] = {}; DWORD clSize = sizeof(clStr);
        if (WinHttpQueryHeaders(hReq, WINHTTP_QUERY_CONTENT_LENGTH,
            WINHTTP_HEADER_NAME_BY_INDEX, clStr, &clSize, NULL))
            contentLen = _wtoi64(clStr);
    }

    HANDLE hFile = CreateFileA(destPath, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        WriteErrorLog("CreateFile falhou para destino do download. path=%s err=%lu",
            destPath, GetLastError());
        WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSess);
        return false;
    }

    if (contentLen > 0)
    {
        LARGE_INTEGER li; li.QuadPart = contentLen;
        SetFilePointerEx(hFile, li, NULL, FILE_BEGIN);
        SetEndOfFile(hFile);
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    }

    const DWORD READ_BUF = 512 * 1024;
    std::vector<char> buf(READ_BUF);
    LONGLONG totalRead = 0;
    DWORD    read = 0;
    DWORD    tickLast = GetTickCount();
    LONGLONG bytesLast = 0;

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    while (!g_bShutdown &&
        WinHttpReadData(hReq, buf.data(), READ_BUF, &read) &&
        read > 0)
    {
        DWORD written = 0;
        WriteFile(hFile, buf.data(), read, &written, NULL);
        totalRead += written;

        DWORD now = GetTickCount();
        DWORD diff = now - tickLast;
        if (diff >= 300)
        {
            float  pct = contentLen > 0 ? (float)((double)totalRead / contentLen * 100.0) : 0.f;
            double mbps = ((totalRead - bytesLast) / 1024.0 / 1024.0) / (diff / 1000.0);
            char   sub[256];
            sprintf_s(sub, "%.1f MB / %.1f MB  |  %.1f MB/s",
                totalRead / 1024.0 / 1024.0,
                contentLen / 1024.0 / 1024.0,
                mbps);

            if (g_csInit) EnterCriticalSection(&g_csUpdate);
            g_UpdateStatus.state = IUS_DOWNLOADING_FILES;
            g_UpdateStatus.progress = pct;
            strcpy_s(g_UpdateStatus.subText, sub);
            if (g_csInit) LeaveCriticalSection(&g_csUpdate);

            bytesLast = totalRead;
            tickLast = now;
        }
        read = 0;
    }

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    CloseHandle(hFile);
    WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSess);

    if (totalRead <= 0)
        WriteErrorLog("Download resultou em 0 bytes. url=%s", url);

    return totalRead > 0;
}

static bool ExtractZip(const char* zipPath, const char* destDir)
{
    if (g_bShutdown) return false;

    mz_zip_archive zip = {};
    if (!mz_zip_reader_init_file(&zip, zipPath, 0))
    {
        WriteErrorLog("miniz: falha ao abrir zip. path=%s", zipPath);
        return false;
    }

    int totalFiles = (int)mz_zip_reader_get_num_files(&zip);

    if (g_csInit) EnterCriticalSection(&g_csUpdate);
    g_UpdateStatus.totalFiles = totalFiles;
    g_UpdateStatus.extractedFiles = 0;
    if (g_csInit) LeaveCriticalSection(&g_csUpdate);

    int extracted = 0;
    for (int i = 0; i < totalFiles && !g_bShutdown; i++)
    {
        mz_zip_archive_file_stat stat = {};
        if (!mz_zip_reader_file_stat(&zip, i, &stat))
            continue;

        if (mz_zip_reader_is_file_a_directory(&zip, i))
            continue;

        char destPath[MAX_PATH];
        sprintf_s(destPath, "%s\\%s", destDir, stat.m_filename);

        for (char* p = destPath; *p; p++)
            if (*p == '/') *p = '\\';

        {
            char dirBuf[MAX_PATH];
            strcpy_s(dirBuf, destPath);
            char* last = strrchr(dirBuf, '\\');
            if (last)
            {
                *last = '\0';
                char* p = dirBuf + 3;
                while (*p)
                {
                    if (*p == '\\')
                    {
                        *p = '\0';
                        CreateDirectoryA(dirBuf, NULL);
                        *p = '\\';
                    }
                    p++;
                }
                CreateDirectoryA(dirBuf, NULL);
            }
        }

        if (!mz_zip_reader_extract_to_file(&zip, i, destPath, 0))
        {
            WriteErrorLog("miniz: falha ao extrair. entry=%s dest=%s",
                stat.m_filename, destPath);
            continue;
        }

        extracted++;

        {
            float pct = (float)extracted / (float)totalFiles * 100.f;
            char fileOnly[MAX_PATH] = {}, dirOnly[MAX_PATH] = {};
            const char* lastSlash = strrchr(stat.m_filename, '/');
            if (lastSlash)
            {
                size_t dlen = lastSlash - stat.m_filename + 1;
                strncpy_s(dirOnly, stat.m_filename,
                    min(dlen, (size_t)MAX_PATH - 1));
                strcpy_s(fileOnly, lastSlash + 1);
            }
            else
                strcpy_s(fileOnly, stat.m_filename);

            if (g_csInit) EnterCriticalSection(&g_csUpdate);
            g_UpdateStatus.state = IUS_EXTRACTING_FILES;
            g_UpdateStatus.progress = pct;
            g_UpdateStatus.extractedFiles = extracted;
            strcpy_s(g_UpdateStatus.currentFile, fileOnly);
            strcpy_s(g_UpdateStatus.currentDir, dirOnly);
            if (g_csInit) LeaveCriticalSection(&g_csUpdate);
        }
    }

    mz_zip_reader_end(&zip);
    return (extracted > 0);
}

static void RestartGameAndExit()
{
    char szGamePath[MAX_PATH];
    GetModuleFileNameA(NULL, szGamePath, MAX_PATH);

    // Limpa o .old após o processo morrer (o cmd espera 2s)
    char oldExe[MAX_PATH];
    sprintf_s(oldExe, "%s.old", szGamePath);
    if (GetFileAttributesA(oldExe) != INVALID_FILE_ATTRIBUTES)
    {
        char cmd[MAX_PATH * 2 + 64];
        sprintf_s(cmd, "cmd /c ping 127.0.0.1 -n 3 > nul & del /f /q \"%s\"", oldExe);
        STARTUPINFOA si = {}; si.cb = sizeof(si); si.dwFlags = STARTF_USESHOWWINDOW; si.wShowWindow = SW_HIDE;
        PROCESS_INFORMATION pi = {};
        CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
        if (pi.hProcess) CloseHandle(pi.hProcess);
        if (pi.hThread)  CloseHandle(pi.hThread);
    }

    SHELLEXECUTEINFOA sei = {};
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.lpVerb = "open";
    sei.lpFile = szGamePath;
    sei.nShow = SW_SHOWNORMAL;
    if (!ShellExecuteExA(&sei))
        WriteErrorLog("Falha ao reiniciar o processo do jogo. path=%s err=%lu",
            szGamePath, GetLastError());
    Sleep(300);
    PostMessageA(g_hGameWnd, WM_CLOSE, 0, 0);
}
