#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// PI values defn.
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2

const int x = 0;
const int y = 1;

#define D2R 0.0174533 // 1° = 0.0174533 rad

int baseline = 20;

float posX = 400.0 - 10;
float posY = 460.0;
float ang = 90.0;

int windowWidth = 1600;
int windowHeight = 800;

float scaleDownMovement = 2;

int rayCount = 1;
int unit_ = 1;
int map_len = 20;

int FOV = 90;

float posVec[] = {300.0, 200.0};
float dirVec[] = {0.0 , -1.0  };
float plaVec[] = {-dirVec[y], dirVec[x]};
float steVec[] = {-dirVec[y], dirVec[x]};

const int TEX_SIZE = 64;
Uint32 texture[TEX_SIZE * TEX_SIZE];


Uint32 frameStart;
Uint32 frameTime;
Uint32 lastTime = 0;
int frameCount = 0;

float fps = 0.0f;

int map[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,0,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,1,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,
    1,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

};

int main(int argc, char **argv)
{
    for (int y = 0; y<TEX_SIZE; y++) {
        for (int x = 0; x<TEX_SIZE; x++) {
            int checker = ((x / 16) % 2) ^ ((y / 16) % 2);
            Uint8 c = checker ? 255 : 0;
            texture[y*TEX_SIZE + x] = (c << 24) | (c << 16) | (c << 8) | 255;
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not be initiated: " << SDL_GetError();
    }
    else
    {
        std::cout << "SDL video system is ready to go\n";
    }

    SDL_Window *window = SDL_CreateWindow(
        "My SDL2 Window",       // Title
        SDL_WINDOWPOS_CENTERED, // X position
        SDL_WINDOWPOS_CENTERED, // Y position
        windowWidth,            // Width
        windowHeight,           // Height
        SDL_WINDOW_SHOWN        // Flags
    );

    if (window == NULL)
    {
        std::cout << "Window could not be created! Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == NULL)
    {
        std::cout << "Renderer Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_J]) {
            ang += 0.8;
            if (ang > 360) ang -= 360;
        }

        if (keystate[SDL_SCANCODE_L]) {
            ang -= 0.8;
            if(ang < 0) ang += 360;
        }

        if (keystate[SDL_SCANCODE_Q]) {
            FOV -= 1;
            if(FOV < 30) FOV = 30;
        }

        if (keystate[SDL_SCANCODE_E]) {
            FOV += 1;
            if(FOV >140) FOV = 140;
        }


        float cos_component = cos(D2R*ang) * scaleDownMovement;
        float sin_component = sin(D2R*ang) * scaleDownMovement;

        // player movements and collision detection
    
        if (keystate[SDL_SCANCODE_W]) {
            if (map[int(posY / 40.0) * map_len + int((posX + cos_component) / 40.0)] == 0)
            {
                posX += cos_component;
            }
            if (map[int((posY - sin_component) / 40.0) * map_len + int(posX / 40.0)] == 0)
            {
                posY -= sin_component;
            }
        }

        if (keystate[SDL_SCANCODE_S]) {
            if (map[int(posY / 40.0) * map_len + int((posX - cos_component) / 40.0)] == 0)
            {
                posX -= cos_component;
            }
            if (map[int((posY + sin_component) / 40.0) * map_len + int(posX / 40.0)] == 0)
            {
                posY += sin_component;
            }
        }
        

        if (keystate[SDL_SCANCODE_A]) {
            if (map[int(posY / 40.0) * map_len + int((posX - sin_component) / 40.0)] == 0)
            {
                posX -= sin_component;
            }
            if (map[int((posY - cos_component) / 40.0) * map_len + int(posX / 40.0)] == 0)
            {
                posY -= cos_component;
            }
        }

        if (keystate[SDL_SCANCODE_D]) {
            if (map[int(posY / 40.0) * map_len + int((posX + sin_component) / 40.0)] == 0)
            {
                posX += sin_component;
            }
            if (map[int((posY + cos_component) / 40.0) * map_len + int(posX / 40.0)] == 0)
            {
                posY += cos_component;
            }
        }

        // applying player position after all 2D transformations
        posVec[x] = posX;
        posVec[y] = posY;

        // hyp here is always 1
        // hyp^2 = 1 = sin(ang)^2 + cos(ang)^2
        dirVec[x] = cos(ang * D2R) * 1;                     // direction vector of the player (X-component) from pythagoras theorem
        dirVec[y] = -sin(ang * D2R) * 1;                    // direction vector of the player (Y-component) from pythagoras theorem

        float planeLen = tan( (FOV*D2R)/2.0f );             // length of plane calculated from the FOV

        plaVec[x] = -dirVec[y] * planeLen;                  // this is jus the plane vector calculations
        plaVec[y] = dirVec[x]  * planeLen;                  // this is the plane vector calculations, in the y component

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);     // black
        SDL_RenderClear(renderer);
        
        // Set draw color (background)
        SDL_SetRenderDrawColor(renderer, 0, 105, 105, 255); // sky blue

        // Clear screen, i.e., fill with skyblue
        SDL_Rect fillRect = { 0+800, 0, 800, 400 };
        SDL_RenderFillRect(renderer, &fillRect);

        SDL_SetRenderDrawColor(renderer, 0, 105, 0, 205);   // grass green

        fillRect = { 0+800, 400, 800, 400 };
        SDL_RenderFillRect(renderer, &fillRect);

        // Map visual in squares
        SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);

        for (int i = 0; i<map_len; i++) {
            for (int j = 0; j<map_len; j++) {
                if (map[j* map_len + i] == 1) {
                    SDL_Rect rect = {i*40, j*40, 40, 40};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        
        // Direction Line ,i.e., the black line

        //                                           black dot x       black dot y  
        //                                         ______|_______    ______|_______ 
        //                            CamX  CamY  |              |  |              |
        SDL_RenderDrawLineF(renderer, posX, posY, posX + dirVec[x], posY + dirVec[y]);


        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        // camera Plane, i,e., the blue line
        SDL_RenderDrawLineF(renderer, posX + dirVec[x],             posY + dirVec[y],
                                      posX + dirVec[x] + plaVec[x], posY + dirVec[y] + plaVec[y]
        );

        SDL_RenderDrawLineF(renderer, posX + dirVec[x],             posY + dirVec[y],
                                      posX + dirVec[x] - plaVec[x], posY + dirVec[y] - plaVec[y]
        );

        for (int a = 0; a<800; a++) {
            double CamX = 2 * a / double(800) - 1;
            // CamX = 0;
            double rayDirX = dirVec[x] + plaVec[x] * CamX;
            double rayDirY = dirVec[y] + plaVec[y] * CamX;

            // normalized coordinates based on the number of map-blocks
            float coorX = float(posX / 40.0);
            float coorY = float(posY / 40.0);

            // map box number - discrete position
            int mapX = int(coorX);
            int mapY = int(coorY);

            //length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            double deltaDistX = sqrt(1 + ((rayDirY / rayDirX)*(rayDirY / rayDirX)));
            double deltaDistY = sqrt(1 + ((rayDirX / rayDirY)*(rayDirX / rayDirY)));

            // declaration of variables to store the direction increment data of the DDA ray calculation
            int stepX, stepY;

            // first ray update from starting position in X-direction
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (coorX - mapX) * deltaDistX;
            }else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - coorX) * deltaDistX;
            }

            // first ray update from starting position in Y-direction
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (coorY - mapY) * deltaDistY;
            }else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - coorY) * deltaDistY;
            }

            int hit = 0;
            int side;

            // DDA algorithm for x and y after first ray update
            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                if (map[mapY * map_len + mapX] == 1) {
                    hit = 1;
                }
            }

            double perpWallDist;
            double wallX;

            if (side == 0) {
                perpWallDist = (mapX - coorX + (1 - stepX) / 2) / rayDirX;
                wallX = coorY + perpWallDist * rayDirY;
            } else {
                perpWallDist = (mapY - coorY + (1 - stepY) / 2) / rayDirY;
                wallX = coorX + perpWallDist * rayDirX;
            }


            wallX -= floor(wallX); // keep only decimal part

            // coloring the 2D rays based on wall hit...
            SDL_SetRenderDrawColor(renderer, 200+ (wallX*55), (side==0)?230:255, 0, 255);

            // rednering the rays on the 2D display
            SDL_RenderDrawLineF(renderer,
                posX,
                posY,
                posX + rayDirX * perpWallDist * 40,
                posY + rayDirY * perpWallDist * 40
            );

            float projPlaneDist = (800 / 2.0f) / tan((FOV * D2R) / 2.0f);
            int lineHeight = (int)(projPlaneDist / perpWallDist);

            // calculate start and end of line
            int drawStart = -lineHeight / 2 + windowHeight / 2;
            int drawEnd   =  lineHeight / 2 + windowHeight / 2;

            // clamp (important)
            if (drawStart < 0) drawStart = 0;
            if (drawEnd >= windowHeight) drawEnd = windowHeight - 1;

            int texX = int(wallX * TEX_SIZE);

            if (side == 0 && rayDirX > 0) texX = TEX_SIZE - texX - 1;
            if (side == 1 && rayDirY < 0) texX = TEX_SIZE - texX - 1;

            for (int y = drawStart; y < drawEnd; y++) {

                int d = y * 256 - windowHeight * 128 + lineHeight * 128;
                int texY = ((d * TEX_SIZE) / lineHeight) / 256;

                Uint32 color = texture[TEX_SIZE * texY + texX];     // retrieving the color value according to the scaled texture

                // encoding the pixel color
                Uint8 r = (side == 0) ? ((color >> 24) & 0xFF) : ((color >> 24) & 0b10111111);
                Uint8 g = (side == 0) ? ((color >> 16) & 0xFF) : ((color >> 16) & 0b10111111);
                Uint8 b = (side == 0) ? ((color >> 8)  & 0xFF) : ((color >> 8)  & 0b10111111);
                
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                SDL_RenderDrawPoint(renderer, a + 800, y);
            }
        }

        // Drawing player position
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // red
        SDL_RenderDrawPointF(renderer, posX, posY);

        // Show on screen
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime > 0)
            fps = 1000.0f / frameTime;

        std::cout << "FPS: " << (int)fps << "\n";
    }

    // Destroy window and quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
