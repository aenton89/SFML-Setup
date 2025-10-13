# SFML 2.6.2 setup (on Windows)

## 1. required:
- CMake
- Conan
- Visual C++ 17 (2022)

## 2. run:
### 2.1 detect conan profile
```
conan profile detect --force
```

### 2.2 install dependencies (SFML, etc.)
```
conan install . --output-folder=build --build=missing
```
```
cd build
```
```
.\conanbuild.bat
```
> [!WARNING]
> till this day don't know why and what this does

### 2.3 cmake build
```
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
```
```
cmake --build . --config Release
```

### 2.4 run
```
.\Release\SFML-Base.exe
```