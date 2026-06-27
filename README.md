# 2026 AIVExam (1945)

Gioco 2D in C con raylib e CMake.

Questa versione mantiene gameplay e asset del progetto 1945, ma organizza il codice con una struttura ispirata al progetto TopDown:

- `src/Program/`: entry point
- `src/Engine/`: sistemi riusabili, animazione, audio, input, collisioni, risorse, utility
- `src/GameObjects/`: player, nemici, proiettili, effetti
- `src/Scene/`: logica di gioco, livello e UI
- `Assets/`: asset copiati dal progetto 1945
- `external/`: raylib e aiv-collections
- `tests/`: unit test CLove

## Build Debug

Al primo configure CMake scarica raylib tramite `FetchContent`, quindi serve una connessione internet.
`external/aiv-collections` è invece incluso nel repository perché richiesto dal progetto.

```powershell
cmake -S . -B cmake-build-debug -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --target 1945
ctest --test-dir cmake-build-debug --output-on-failure
```

## Build Release

```powershell
cmake -S . -B cmake-build-release -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build cmake-build-release --target 1945
```
