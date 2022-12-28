# Flappy Bird in C

A Almost Flappy Bird Clone made using Allegro 5.

## How to play on linux (Ubuntu 20.04)

1. Install Allegro 5 and build-essentials

   ```bash
   $ sudo add-apt-repository ppa:allegro/5.2
   $ sudo apt update
   $ sudo apt install liballegro5-dev
   $ sudo apt install liballegro-acodec5-dev liballegro-audio5-dev \
    liballegro-image5-dev liballegro-dialog5-dev liballegro-ttf5-dev \
    liballegro-physfs5-dev
   $ sudo apt install build-essential #For compile source code
   ```

1. Clone the repository:

   ```bash
   $ git clonehttps://github.com/ianitow/flappy-bird-game.git
   ```

   or download as zip and extract to a folder.

1. Enter in the directory

   ```bash
   $ gcc  headers/game.c main.c $(pkg-config --libs allegro-5 allegro_font-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5 allegro_primitives-5) -o game.out
   $ chmod +x game.out #permission for execute
   $ ./game.out
   ```

---

## How to play?

Use the mouse to position the bird and avoid obstacles

## Preview

![Flappy Bird](images/preview.gif)

## Next work

- [ ] Refactoring code
- [ ] Improve collisions
