# TapGame OBS Plug-In

## General Prerequisites

* PowerShell 7
* AWS Amplify CLI 12+
* NodeJS 19+

## Backend Binding

To pull backend configuration and generated a necessary header, run:

```powershell
.\ci\scripts\Pull-Backend.ps1
```

## Building locally for Windows

### Prerequisites

* MS VS 2022 Community
  * Windows 10 SDK (minimum 10.0.20348.0)
  * C++ ATL for latest v143 build tools (x86 & x64)
* cmake

### Build

To fetch dependencies, generate solution files, and build, run:

```powershell
.\ci\scripts\Build-Windows.ps1
```

_Note: this script downloads all dependencies into the parent directory._

### Develop

Open a solution file `build_x64\obs-tapgame.sln` and build interactively.
