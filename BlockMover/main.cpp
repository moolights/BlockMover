// Block Mover project

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

enum KeyPressTexture
{
	KEY_PRESS_DEFAULT,
	KEY_PRESS_UP,
	KEY_PRESS_DOWN,
	KEY_PRESS_LEFT,
	KEY_PRESS_RIGHT,
	KEY_PRESS_TOTAL
};

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture* gCurrentTexture = NULL;

SDL_Surface* gRectSurface = NULL; //Only used for the squares

// The two squares dont need to be global and will go in main

SDL_Texture* gTextureSet[KEY_PRESS_TOTAL];

bool init();

bool loadMedia();

void close();

SDL_Texture* loadTexture(std::string path);

int main(int argv, char* argc[])
{

}










bool init()
{
	bool success = true;

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		printf("FAILED TO INITIALIZE! SDL Error: %s", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Block Mover", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Failed to create window! SDL Error: %s", SDL_GetError());
			success = false;
		}
		else
		{
			gRectSurface = SDL_GetWindowSurface(gWindow);

			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Failed to create renderer! SDL Error: %s", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("Failed to initialize IMG Systems! SDL_IMG Error: %s", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{

}

void close()
{

}

SDL_Texture* loadTexture(std::string path)
{

}


