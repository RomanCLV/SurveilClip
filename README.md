# SurveilClip
A C++ (with Qt 6.8.2 and OpenCV 4.11.0) application to process video from a static camera. An MVC architecture has been setup and SOLID principles of OOP are applied.

## Required libraries
- OpenCV (We used OpenCV 4.11.0)

## Setup
- Install `OpenCV` for Windows (https://opencv.org/releases/)
- Add the `bin` and `lib` folder to the PATH. (e.g.: `C:\opencv\build\x64\vc16\bin` and `C:\opencv\build\x64\vc16\lib`).
- Install Qt.
- Install the `Qt Visual Studio Tools` extension for Visual Studio.

### Setup Visual Studio
Development was carried out using Visual Studio 2022.
Check the following parameters are correct / added:
- C/C++ > General > Additional Include Directories : `C:\opencv\build\include`
- Linker > General > Additional Library Directories : `C:\opencv\build\x64\vc16\lib`
- Linker > Input > Additional Dependencies : `opencv_world4110d.lib` (for Debug) and `opencv_world4110.lib` (for Release). Adapte the version according yours.
