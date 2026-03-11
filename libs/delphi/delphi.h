#pragma once

#ifdef _WIN32
  #ifdef DELPHI_DLL_EXPORTS
    #define DELPHI_API __declspec(dllexport)
  #else
    #define DELPHI_API __declspec(dllimport)
  #endif
  #define DELPHI_CALL __stdcall
#else
  #define DELPHI_API
  #define DELPHI_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

// simple math example (kept for compatibility)
DELPHI_API int DELPHI_CALL Add(int a, int b);

// init/shutdown of the library
DELPHI_API void DELPHI_CALL Initialize();
DELPHI_API void DELPHI_CALL Shutdown();

// audio playback controls
// mixrate: samples per second (eg 44100)
DELPHI_API bool DELPHI_CALL AudioInit(int mixrate);
DELPHI_API bool DELPHI_CALL AudioPlay(const char* filename);
DELPHI_API void DELPHI_CALL AudioStop();

#ifdef __cplusplus
}
#endif
