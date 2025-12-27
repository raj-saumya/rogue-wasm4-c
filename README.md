# The Rogue of Abyss

> ⚠️ **Disclaimer**: made just for fun! code is not subject to review lol. :p.

A simple roguelike game built in C for the [WASM-4](https://wasm4.org) fantasy console.

## 🎮 The Game

Navigate through the abyss, defeat enemies, and survive as long as you can.

**Controls:**

- **Arrow Keys** — Move the rogue
- **Z** — Special action / Start

## 📁 Project Structure

```
src/
├── main.c              # Entry point
├── wasm4.h             # WASM-4 API header
├── assets/             # Sprites & audio
│   ├── sprites.c/h
│   └── audio.c/h
├── core/
│   ├── game.c
│   ├── config.h
│   └── types.h
├── entities/
│   ├── rogue.c/h
│   └── enemy.c/h
└── ui/
    ├── screens.h
    ├── screen_start.c
    ├── screen_playing.c
    ├── screen_buff.c
    └── screen_gameover.c
```

## 🛠️ Build & Run

**Prerequisites:** [WASM-4 CLI](https://wasm4.org/docs/getting-started/setup?code-lang=c#quickstart) and a C compiler (clang/gcc with wasm target).

```bash
# Build
make

# Run locally
w4 run build/cart.wasm

# Bundle for web (generates index.html)
w4 bundle build/cart.wasm --title "The Rogue Of Abyss" --html index.html
```

## 📚 Resources

- [WASM-4 Docs](https://wasm4.org/docs)
