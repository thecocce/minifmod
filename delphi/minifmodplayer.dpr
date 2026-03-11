program minifmodplayer;

uses
  System.SysUtils, System.Classes, Winapi.Windows;

{$R *.res}
{$APPTYPE CONSOLE}

var
  AudioInit: function(rate: Integer): LongBool; stdcall;
  AudioPlay: function(filename: PAnsiChar): LongBool; stdcall;
  AudioStop: procedure; stdcall;
  Initialize: procedure; stdcall;
  Shutdown: procedure; stdcall;

const
  DLL_NAME = 'minifmod_delphi.dll';

begin

  var fe := FileExists(DLL_NAME);


  try
    if not LoadLibrary(DLL_NAME) <> 0 then
      raise Exception.Create('Failed to load ' + DLL_NAME);

    @AudioInit := GetProcAddress(GetModuleHandle(DLL_NAME), 'AudioInit');
    @AudioPlay := GetProcAddress(GetModuleHandle(DLL_NAME), 'AudioPlay');
    @AudioStop := GetProcAddress(GetModuleHandle(DLL_NAME), 'AudioStop');
    @Initialize := GetProcAddress(GetModuleHandle(DLL_NAME), 'Initialize');
    @Shutdown := GetProcAddress(GetModuleHandle(DLL_NAME), 'Shutdown');

    Initialize();

    if AudioInit(44100) then
    begin
      if AudioPlay('demo.xm') then
      begin
        Writeln('Playing demo.xm for 5 seconds...');
        Sleep(5000);
        AudioStop();
      end
      else
        Writeln('AudioPlay failed');
    end
    else
      Writeln('AudioInit failed');

  except on E:Exception do begin
    WriteLn('Error:'+ e.Message);
    end;
  end;
  ReadLn;
  Shutdown();
end.
