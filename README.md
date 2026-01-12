## Monster Shooter Game
Monster Shooter is a terminal-based shooting game built in C++ using the ncurses library.  
The game runs entirely in the terminal and uses keyboard controls for interaction.

## Game Video
[![Monster Shooter Gameplay Demo](https://img.youtube.com/vi/5Ffa1Yp_OcQ/0.jpg)](https://youtu.be/5Ffa1Yp_OcQ)

## Overview

- Monsters appear from the right side of the screen and move left
- You control a pistol that can move up and down
- Shoot bullets to destroy monsters before they reach you
- The game ends when you lose all your lives or quit manually

## Controls

- Up Arrow – Move pistol up
- Down Arrow – Move pistol down
- Spacebar – Shoot bullet
- S – Start the game
- Q – Quit the game

## Scoring & Lives

- +3 points for every monster hit
- You start with 10 lives
- Lose 1 life when a monster reaches the left side without being shot
- Game ends when lives reach 0

## Requirements

- Linux / WSL / macOS terminal
- g++ compiler
- ncurses library

### Install ncurses (if not already installed)

sudo apt update  
sudo apt install libncurses5-dev libncursesw5-dev

## Build & Run Instructions

From the project root directory:

- make clean (remove object files, executable, etc.)
- make main (compile the game)
- ./main (run the game)


