# 1945 AIV Project

Gioco 2D sviluppato in C con raylib e automatizzato con CMake.

Il progetto è ispirato a 1945: il giocatore controlla un aereo, evita nemici e proiettili, spara ai nemici, accumula score e gestisce vite/energia. Il gioco include menu principale, schermata di game over, audio, animazioni, esplosioni, collisioni rettangolari, pooling di proiettili/nemici/effetti e uso di `aiv-collections`.

## Guida utente

Avvio:

1. Scaricare la release già pronta da GitHub.
2. Estrarre lo zip.
3. Avviare `1945.exe`.

Controlli:

- `WASD` oppure frecce direzionali: movimento player
- `SPACE`: sparo
- `ENTER`: conferma nel menu / ritorno al menu dal game over

Obiettivo:

- Distruggere i nemici per aumentare lo score.
- Evitare collisioni con nemici e proiettili.
- Sopravvivere il più a lungo possibile.

## Struttura progetto

- `src/Program/`: entry point dell'applicazione
- `src/Engine/`: sistemi riutilizzabili
  - `Animation`: spritesheet e animazioni
  - `Audio`: musica ed effetti
  - `Collision`: collisioni rettangolari
  - `Input`: gestione input centralizzata
  - `Resources`: caricamento/scaricamento asset
  - `Utils`: utility generiche
- `src/GameObjects/`: oggetti di gioco
  - `Actors`: actor base, player, nemici
  - `Projectiles`: proiettili e tipi di proiettile
  - `Effects`: esplosioni/effetti visuali
- `src/Scene/`: scene e sistemi di gameplay
  - `Game`: loop logico del gioco
  - `Level`: background, scrolling e isole
  - `UI`: HUD, menu e game over
- `Assets/`: asset runtime copiati nella cartella build
- `external/aiv-collections/`: libreria collections richiesta dalla consegna
- `tests/`: unit test con CLove

## Developer guidelines

- Linguaggio: C.
- Convenzione funzioni: PascalCase, ad esempio `GameUpdate`, `PlayerUpdate`, `EnemiesSpawn`.
- Le struct restano in PascalCase, ad esempio `Player`, `Enemy`, `BulletPool`.
- Le costanti globali e macro usano maiuscolo/snake case, ad esempio `BULLET_POOL_MAX`.
- La gestione memoria deve essere simmetrica:
  - ogni `LoadTexture` deve avere un `UnloadTexture`;
  - ogni `LoadSound`/`LoadMusicStream` deve avere il relativo `Unload`;
  - ogni `aiv_vector_new` deve avere un `aiv_vector_destroy`.
- Le cartelle `cmake-build-*`, `build/`, `out/`, `.idea/` e gli output generati non vanno committati.
- `raylib` non è versionata nel repository: viene scaricata da CMake tramite `FetchContent`.

## Dipendenze

- CMake 3.20+
- Compilatore C compatibile con C99
- Git
- Connessione internet al primo configure, per scaricare raylib tramite `FetchContent`

Nota: `external/aiv-collections` è incluso nel repository perché richiesto dal progetto.

## Build Debug

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

## Creare il pacchetto giocabile

Dopo la build release, creare uno zip contenente solo:

- `cmake-build-release/1945.exe`
- `cmake-build-release/assets/`
- `README.md`

Esempio:

```powershell
Compress-Archive -Path "cmake-build-release/1945.exe", "cmake-build-release/assets", "README.md" -DestinationPath "1945AIVProject-v1.0.0-win64.zip" -Force
```

Lo zip generato va caricato nella sezione GitHub Releases, non committato nel repository.

## Release

La release finale deve contenere:

- codice sorgente nel branch `main`;
- tag di versione, ad esempio `v1.0.0`;
- zip giocabile allegato alla GitHub Release;
- video dimostrativo del risultato finale, se richiesto dalla consegna.

## Test

I test coprono:

- collisioni rettangolari;
- animazioni;
- esplosioni;
- `aiv_vector` e `aiv_list`.

Comando:

```powershell
ctest --test-dir cmake-build-debug --output-on-failure
```
