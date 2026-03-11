#include "delphi.h"
#include <iostream>

#include <minifmod/minifmod.h>

static bool g_audio_inited = false;
static PlayerState* g_player = nullptr;

// Sample implementations
extern "C" {
    DELPHI_API int DELPHI_CALL Add(int a, int b)
    {
        return a + b;
    }

    DELPHI_API void DELPHI_CALL Initialize()
    {
        std::cout << "Delphi DLL initialized\n";
    }

    DELPHI_API void DELPHI_CALL Shutdown()
    {
        std::cout << "Delphi DLL shutdown\n";
        if (g_player)
        {
            FMUSIC_StopSong(g_player);
            g_player = nullptr;
        }
        // there is no FSOUND_Close in this version, so just reset flag
        g_audio_inited = false;
    }

    DELPHI_API bool DELPHI_CALL AudioInit(int mixrate)
    {
        if (g_audio_inited)
            return true;
        g_audio_inited = FSOUND_Init(mixrate);
        return g_audio_inited;
    }

    DELPHI_API bool DELPHI_CALL AudioPlay(const char* filename)
    {
        if (!g_audio_inited)
            return false;
        Module* mod = FMUSIC_LoadSong(filename, nullptr);
        if (!mod)
            return false;
        g_player = FMUSIC_PlaySong(mod);
        return g_player != nullptr;
    }

    DELPHI_API void DELPHI_CALL AudioStop()
    {
        if (g_player)
        {
            FMUSIC_StopSong(g_player);
            g_player = nullptr;
        }
    }
}
