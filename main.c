#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_timer.h>


#define MAP_WIDTH 24
#define MAP_HEIGHT 24

#define WINDOW_FACTOR 75
int WINDOW_WIDTH = 640; 
int WINDOW_HEIGHT = 480;

int WorldMap[MAP_WIDTH][MAP_HEIGHT]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

//COLORS
int R, G, B;

//variables necessary for  camera:
double posX = 14, posY = 8;//Starting position
double dirX = -1, dirY = 0;//Initial direction vector
double planeX = 0, planeY = 0.66;//2d camera plane

double time = 0;//time of current frame
double OLDtime = 0;//time of previous frame
double FrameTime;
int fps;

double rayDirX, rayDirY;
double deltaDistX, deltaDistY;

//length of ray from current position to next x or y-side
double sideDistX;
double sideDistY;

int stepX; 
int stepY;

int hit = 0;//a je zadel zid?
int side;//Katera stran je bila zadeta (x ali y za za bolse shadanje)

double PerpWallDist;
int mapX, mapY;


//shit for drawing lines on the screen
//Zracunaj višino linije
float LineHeight;
//zracunaj najvišjo in najnižjo točko linije
float DrawStart, DrawEnd;

// how shadowy are the shadows
int WallBrightness = 255;

//move and rotate speed
double MoveSpeed, RotSpeed;
int main(int argc, char *argv[])
{
  //SDL INIT SHI;T
  //INIT window
  bool quit = false;
  SDL_Window* win = SDL_CreateWindow("Raycaster", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!win) 
  {

    printf("error creating window: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
      
  }
  //INIT renderer
  SDL_Renderer* rend = SDL_CreateRenderer(win, NULL);
  if (!rend)
  {
    printf("error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 1;
  }
  //Calculate ray position and direction
  

  while (!quit) 
  { 

    SDL_Event event;
    while (SDL_PollEvent(&event)) //event handling
    {
      switch (event.type)
      {
        case SDL_EVENT_QUIT:
          quit = true;
          break;

      }

      if (event.type == SDL_EVENT_KEY_DOWN)
      {
        if (event.key.scancode == SDL_SCANCODE_W)
        {
            if (WorldMap [(int)(posX + dirX * MoveSpeed)][(int)posY] == false)
              {
                posX +=dirX * MoveSpeed;
              }
            if (WorldMap [(int)posX][(int)(posY + dirY * MoveSpeed)] == false)
              {
                posY +=dirY * MoveSpeed;
              }
        }
        if (event.key.scancode == SDL_SCANCODE_S)
        {
            if (WorldMap [(int)(posX + dirX * MoveSpeed)][(int)posY] == false)
              {
                posX -=dirX * MoveSpeed;
              }
            if (WorldMap [(int)posX][(int)(posY + dirY * MoveSpeed)] == false)
              {
                posY -=dirY * MoveSpeed;
              }
        }
      }
    }

    for (int x = 0;x < WINDOW_WIDTH;x++)
    {

      hit = 0;
      int mapX = (int)posX;
      int mapY = (int)posY;
      /**/
      /*cameraX is the x-coordinate on the camera plane that the current x-coordinate of the screen represents, done*/
      /*this way so that the right side of the screen will get coordinate 1, the center of the screen gets coordinate 0,*/
      /*and the left side of the screen gets coordinate -1. Out of this, the direction of the ray can be calculated as was */
      /*explained earlier: as the sum of the direction vector, and a part of the plane vector. This has to be done both for */
      /*the x and y coordinate of the vector (since adding two vectors is adding their x-coordinates, and adding their y-coordinates).*/
      /**/
      //calculate rayposition vectors
      double cameraX = (2 * x)/ (double)WINDOW_WIDTH - 1.0; //x v koordinatnem sistemu od kamere
      double rayDirX = dirX + planeX * cameraX; // če je cameraX negative gre levo, če je pozitiven gre desno
      double rayDirY = dirY + planeY * cameraX;


      if (rayDirX == 0)
      {
        deltaDistX = 1e30;
      }else {
        deltaDistX = fabs(1 / rayDirX);
      }
      if (rayDirY == 0)
      {
        deltaDistY = 1e30;
      }else {
        deltaDistY = fabs(1 / rayDirY);
      }
      
      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a w;all
        if (WorldMap[mapX][mapY] > 0)
        {
          hit = 1;
        }

        if (side == 0)
        {
          PerpWallDist = sideDistX - deltaDistX;
        }
        else {
          PerpWallDist = sideDistY - deltaDistY;
        }
      } 

      LineHeight = (int)WINDOW_HEIGHT/PerpWallDist;

      DrawStart = -LineHeight / 2 + WINDOW_HEIGHT / 2;
      if(DrawStart < 0)DrawStart= 0;
      DrawEnd = LineHeight / 2 + WINDOW_HEIGHT / 2;
      if(DrawEnd >= WINDOW_HEIGHT)DrawEnd = WINDOW_HEIGHT - 1;
      
      if (side == 1)
      {
        WallBrightness = 150;
      }else {
        WallBrightness = 255;
      }
      //choose wall color
      switch(WorldMap[mapX][mapY])
      {
        case 1:  SDL_SetRenderDrawColor(rend, WallBrightness, 0, 0, 255);  break; //red
        case 2:  SDL_SetRenderDrawColor(rend, 0, WallBrightness,0, 255);  break; //green
        case 3:  SDL_SetRenderDrawColor(rend, 0, 0, WallBrightness, 255); break; //blue
        case 4:  SDL_SetRenderDrawColor(rend, WallBrightness, WallBrightness, WallBrightness, 255);  break; //white
        default: SDL_SetRenderDrawColor(rend, WallBrightness, WallBrightness, 0, 255); break; //yellow
      }
      
      SDL_RenderLine(rend, x, DrawStart, x, DrawEnd);


  }

    SDL_RenderPresent(rend);
    //time and shit
    OLDtime = time;
    time = SDL_GetPerformanceCounter();
    FrameTime = (double)(time - OLDtime) / SDL_GetPerformanceFrequency();

    SDL_SetRenderDrawColor(rend, 30, 30, 30, 255);
    SDL_RenderClear(rend);


    //speed modifications
    MoveSpeed = FrameTime * 100.0; //squares per second(is constant)
    RotSpeed = FrameTime * 60.0; //radian per second (is constant)
  }

  // zbrisi resurse preden exitas
  SDL_DestroyWindow(win);
  SDL_DestroyRenderer(rend);

  SDL_Quit();

  return EXIT_SUCCESS;
}
