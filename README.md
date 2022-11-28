# Blasteroids

Blasteroids per Headfirst C.
Includes score keeping, asteroid health bars, shields, and alternate attack.

## Controls
ARROWS  - accelerate / deccelerate
SPACE   - blaster
ALT     - super blaster
LCTRL   - shields

## Build

Requires liballegro5 to be installed, refer to:
https://github.com/liballeg/allegro_wiki/wiki/Quickstart

Requires the C-Containers library to be built and installed, available at:
https://github.com/BetaPollux/C-Containers

The program is built using cmake.

    > git clone https://github.com/BetaPollux/Blasteroids.git
    > cd Blasteroids
    > mkdir build
    > cd build
    > cmake ..
    > cmake --build .

