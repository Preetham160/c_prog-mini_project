# 2D Graphics Editor in C

A simple menu-driven 2D graphics editor that uses a character canvas filled with `_` and draws shapes with `*`.

## Features
- Add objects to the picture
- Delete objects from the picture
- Modify existing objects
- Display the picture
- Draw line, rectangle, circle, and triangle shapes

## Build

```bash
gcc -std=c11 -Wall -Wextra -O2 main.c -o graphics_editor
```

## Run

```bash
./graphics_editor
```

## Notes
- The canvas size is `25 x 60`.
- Shapes are stored as objects and redrawn on demand.
- Each object has an ID so it can be deleted or modified later.
