#include "delphi.h"
#include <iostream>
#include <cstdio>

#include <minifmod/minifmod.h>
#include <minixm/system_file.h>  // for access to minifmod::file_access

// simple file callbacks that mimic example app
static void* df_fileopen(const char* name)
{
    return fopen(name, "rb");
}
static void df_fileclose(void* handle)
{
    fclose((FILE*)handle);
}
static size_t df_fileread(void* buffer, size_t size, void* handle)
{
    return fread(buffer, 1, size, (FILE*)handle);
}
static void df_fileseek(void* handle, long pos, int mode)
{
    fseek((FILE*)handle, pos, mode);
}
static long df_filetell(void* handle)
{
    return ftell((FILE*)handle);
}


static bool g_audio_inited = false;
static PlayerState* g_player = nullptr;

// Sample implementations
extern "C" {
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
        // ensure file access callbacks are configured
        if (!minifmod::file_access.open)
        {
            minifmod::file_access.open  = df_fileopen;
            minifmod::file_access.close = df_fileclose;
            minifmod::file_access.read  = df_fileread;
            minifmod::file_access.seek  = df_fileseek;
            minifmod::file_access.tell  = df_filetell;
        }

        bool ok = FSOUND_Init(mixrate);
        g_audio_inited = ok;
        std::cout << "AudioInit(" << mixrate << ") returned " << ok << "\n";
        return ok;
    }

    DELPHI_API bool DELPHI_CALL AudioPlay(const char* filename)
    {
        if (!g_audio_inited)
            return false;
        if (!filename)
        {
            std::cerr << "AudioPlay called with null filename\n";
            return false;
        }
        // copy filename to safeguard against invalid pointers from caller
        std::string fname(filename);
        std::cout << "AudioPlay: " << fname << "\n";
        std::cout << "-> calling FMUSIC_LoadSong\n";
        Module* mod = FMUSIC_LoadSong(fname.c_str(), nullptr);
        std::cout << "   FMUSIC_LoadSong returned " << mod << "\n";
        if (!mod)
        {
            std::cerr << "FMUSIC_LoadSong failed for " << fname << "\n";
            return false;
        }
        std::cout << "-> calling FMUSIC_PlaySong\n";
        g_player = FMUSIC_PlaySong(mod);
        std::cout << "   FMUSIC_PlaySong returned " << g_player << "\n";
        if (!g_player)
        {
            std::cerr << "FMUSIC_PlaySong returned null\n";
        }
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
