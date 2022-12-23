FROM mcr.microsoft.com/windows/servercore:ltsc2019

SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

RUN Invoke-WebRequest -OutFile visualcppbuildtools_full.exe https://aka.ms/vs/16/release/VC_BuildTools.exe; \
    Start-Process -Wait -FilePath .\visualcppbuildtools_full.exe -ArgumentList '--quiet', '--norestart', '--nocache', '--installPath', 'C:\BuildTools', '/InstallSelectableItems', 'Microsoft.VisualCppBuildTools.WindowsXP.x64', '/NoRestart'; \
    Remove-Item -Force visualcppbuildtools_full.exe

ENV PATH $PATH;C:\BuildTools\VC\Tools\MSVC\14.27.29110\bin\Hostx64\x64

COPY hangman.c .

RUN cl -O2 -openmp hangman.c

CMD ["./hangman.exe"]
