# Pong64

Pong64 is a port of the classic Pong game that runs on a real Nintendo 64 console.
The project was created as an experiment in low-level programming, C development, and the open-source framework Libdragon.
The goal was to understand the fundamentals of hardware-level code while gaining practical experience with the N64 architecture.

The ROM has a resolution of 640x480 pixels and has been tested on a PAL N64 console with an Everdrive X7. An Expansion Pak is recommended.


<img width="2455" height="1265" alt="Title" src="https://github.com/user-attachments/assets/7db7c7f4-f4cd-4387-85c9-9ef5372e21e4" />

<img width="2278" height="1275" alt="Game" src="https://github.com/user-attachments/assets/f57a27f7-1fd9-4b26-95ad-54623a8a1476" />

## Build

Install the latest libdragon `preview` branch:
<https://github.com/DragonMinded/libdragon/tree/preview>

- Running this ROM requires real hardware or an accurate emulator
- Emulators like ares or gopher64 are accurate.

Then run:
```sh
make 
```
Or run for WSL 0.2.1:
```sh
make clean && make
```
This will create a `pong.z64` file.
However, I have already uploaded the finished ROM, so it can be played directly on an emulator or used with any N64 flash cartridge, such as the Everdrive 64 X7.


## Features
- Two-player mode using both controller ports of the N64
- COM mode: play against a computer opponent with selectable difficulty levels easy, normal, or hard
- The Libdragon logo appears when starting the ROM
- On the start screen, a ball bounces on the Pong sprites
- The ball speeds up after hitting the wall 5 times
- Uses the Libdragon font

## Control Settings
- Press Start to start the Two-player mode using both controller ports of the N64
- Use the N64 analog stick to move the Pong sprite up and down
- Press Z to get Multiplayer COM: with selectable difficulty levels easy, normal, or hard
- Press R for the rules
- **Still in development** Press A for endless Mode

## Future Features
- Endless mode: try to achieve the highest score, but the twist is that the opponent becomes faster and stronger over time
- Background music
- Rumble Pak support
