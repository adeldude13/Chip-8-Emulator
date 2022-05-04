# CHIP-8-Emulator
A CHIP-8 emulator in C

![Air Plane](Pics/airPlane.png)

![ch8](Pics/ch8.png)


## Compiling and Running
* note: this is only tested on linux(Arch BTW) im not sure about other platforms.
* you need to have libsdl2 in order to compile the code
* to compile the project:
```bash
$ make main
```
* to run from the directory file you can:
```bash
$ make run scale={scale} rom={../ROMS/romfile or any rom file you want}
```
or you can just use the defaults:
```bash
$ make run
```
## References
Some helpful resources I used when writing this

- https://www.youtube.com/watch?v=yFLa3ln16w0
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Annn

