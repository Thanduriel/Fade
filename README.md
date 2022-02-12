# Fade
![](https://github.com/Thanduriel/Fade/blob/8b9669f967bbdeef128f67d52751fdeac59ed547/imgs/Fade.png)
Fade is a local-multiplayer twin-stick-shooter with a twist: The battlefield is shrouded in darkness by default and only lights up partially through player actions. Primarly players light up when they shoot or when they are hit. Additionally a varity of power-ups can help in spotting and killing enemies.

## Build
Building requires C++ 14 compiler.
The only dependency is SFML. Uses a version found on the system with `find_package` if available. Otherwise a local version is expected at `dependencies/SFML`. The local version is integrated as submodule so just run 
```sh
$ git clone --recursive https://github.com/Thanduriel/Fade.git
$ cd Shoop
$ mkdir build
$ cd build
$ cmake ..
```

## Controls
The game can currently only be played with gamepads. The default controls are:
| Key         | Action    |  
|-------------|-----------|
| select      | join lobby|
| X           | change color (only in lobby)|
| left stick  | move      |
| right stick | aim       |
| RB          | fire      |
| RL          | use power-up|
| B           | switch light|

After running the game once with players entering the lobby a ```settings.ini``` will be generated in which key mappings for each player can be adjusted.