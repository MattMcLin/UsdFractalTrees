# UsdFractalTrees
Generate USD geometry for a fractal-based trees

### WARNING
This is a work in progress. 

## Dependencies
- Windows (for now, until cmake file is improved)
- cmake 3.0 or later
- Python 2.7 64-bit
- Pixar USD source
- a compiler such as those provided with Visual Studio 2015 or Xcode

## How to build
### Windows
```
git clone https://github.com/MattMcLin/UsdFractalTrees.git
mkdir build
cd build
cmake .. -G "Visual Studio 14 2015 Win64"
cmake --build .
```
