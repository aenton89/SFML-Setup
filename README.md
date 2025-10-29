# SFML 2.6.2 setup (on Windows)

## 1. required:
- CMake
- Conan
- Visual C++ 17 (2022)

## 2. TODO's:
### PROJEKT 1 (strona 85. - chapter 3); TERMIN (chyba) 06.12; TODO:
- podstawowa mapa: przeszkody to pare rozmieszczonych okręgów
- implementacja kolizji: gracz/przeciwnicy/przeszkody/krawędź ściany
- collider gracza to okrąg (mimo, że jest reprezentowany przez trójkąt)
- exit pod ESC; sterowanie pod WSAD + myszke (LPM do strzelania); debug pod LeftAlt
- strzelanie raycast'em
- przeciwników jest dużo: prawie losowo rozmieszczani (żeby nie było że na przeszkodzie/graczu)
- raycast zabija przeciwników
- przeciwnicy atakują melee

*LEFT TO DO:*
- sterowani przez steering behaviours

## 3. run:
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