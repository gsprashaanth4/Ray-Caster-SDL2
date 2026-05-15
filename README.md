# Ray-Caster-SDL2
Ray Caster Engine implemented in C++ and SDL2 using vector-based architecture

## Vector-Basis Camera Raycaster (SDL2)
A lightweight 2.5D raycasting engine written in C++ using SDL2, implementing a vector-basis camera system and real-time DDA ray traversal for scene rendering. (concepts based from https://lodev.org/cgtutor/raycasting.html)

The program is built upon a vector-basis camera system, where the camera's yaw acts as the primary basis upon which all further translations and directional computations are performed.
Scene rendering is achieved entirely through mathematical raycasting and manual projection calculations, while SDL2 is used strictly for rendering and window management.

The program also features a custom texture mapping feature, that can be manually programmed to project any mathematically describable texture.


### Features
- Vector-basis camera implementation<br>
- Real-time DDA raycasting<br>
- Textured wall rendering<br>
- Adjustable Field of View (FOV)<br>
- Collision detection<br>
- Real-time first-person movement<br>
- 2D overhead debug map<br>
- Direction and camera plane visualization<br>
- Dynamic perspective projection<br>
- Side-based wall shading<br>
- Real-time FPS calculation

## Camera System
The engine uses a vector-basis camera representation:

```dirVec``` - Forward direction vector<br>
```plaVec``` - Camera plane vector<br>
```posVec``` - Camera position vector

Instead of rotating individual rays manually through angular stepping (My prev repo: Ray caster OpenGL), the renderer constructs ray directions using:<br>
```cpp 
double rayDirX = dirVec[x] + plaVec[x] * CamX;
double rayDirY = dirVec[y] + plaVec[y] * CamX;
```

This creates a mathematically correct perspective projection similar to classic engines such as:<br>
- Wolfenstein 3D<br>
- DOOM

## How to run

Required libraries:

SDL2<br>
SDL2_ttf

Compile:
```shell
g++ ray_caster_sdl.cpp -o ray -lSDL2 -lSDL2_ttf

```
run the executable
```shell
./ray
```

### Controls

| Key | Action |
| :--- | :--- |
| **W** | Move Forward |
| **S** | Move Backward |
| **A** | Strafe Left |
| **D** | Strafe Right |
| **J** | Rotate Camera Left |
| **L** | Rotate Camera Right |
| **Q** | Decrease FOV |
| **E** | Increase FOV |

## Renderings:

<img width="1628" height="866" alt="Screenshot from 2026-05-15 18-38-59" src="https://github.com/user-attachments/assets/88e4aad2-2ab4-46e8-9d1e-bcd18532ea67" /><br>
<img width="1628" height="866" alt="Screenshot from 2026-05-15 18-38-34" src="https://github.com/user-attachments/assets/68036a86-0164-4f2e-a189-920c90e32726" /><br>
<img width="1628" height="866" alt="Screenshot from 2026-05-15 18-38-08" src="https://github.com/user-attachments/assets/02e431c5-2bc6-4f06-be90-eebfb86a5338" />
