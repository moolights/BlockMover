// Block Mover project

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

enum TextureSetValues
{
	STATIC_SQUARE_TEXTURE,
	DYNAMIC_SQUARE_TEXTURE,
	WINDOW_ERROR_TEXTURE,
	WINDOW_SUCCESS_TEXTURE,
	TEXTURE_TOTAL
};

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture* gCurrentTexture = NULL;

SDL_Surface* gRectSurface = NULL; //Only used for the squares

// The two squares dont need to be global and will go in main

SDL_Texture* gTextureSet[TEXTURE_TOTAL];

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
				SDL_SetRenderDrawColor(gRenderer, 100, 82, 86, 0xFF);

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
	bool success = true;

	gTextureSet[STATIC_SQUARE_TEXTURE] = loadTexture("grey.png");
	if (gTextureSet[STATIC_SQUARE_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s", "grey.png", SDL_GetError());
		success = false;
	}

	gTextureSet[DYNAMIC_SQUARE_TEXTURE] = loadTexture("LaCroixKitten.png");
	if (gTextureSet[DYNAMIC_SQUARE_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s", "LaCroixKitten.png", SDL_GetError());
		success = false;
	}

	gTextureSet[WINDOW_ERROR_TEXTURE] = loadTexture("red.png");
	if (gTextureSet[WINDOW_ERROR_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s", "red.png", SDL_GetError());
		success = false;
	}

	gTextureSet[WINDOW_SUCCESS_TEXTURE] = loadTexture("green.png");
	if (gTextureSet[WINDOW_SUCCESS_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s", "green.png", SDL_GetError());
		success = false;
	}

	return success;
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	SDL_DestroyTexture(gCurrentTexture);
	gCurrentTexture = NULL;

	for (int i = 0; i < TEXTURE_TOTAL; i++)
	{
		SDL_DestroyTexture(gTextureSet[i]);
		gTextureSet[i] = NULL;
	}

	SDL_FreeSurface(gRectSurface);
	gRectSurface = NULL;

	IMG_Quit();
	SDL_Quit();

}

SDL_Texture* loadTexture(std::string path)
{

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Failed to load image %s. SDL_Image Error: %s", path.c_str(), IMG_GetError());
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Failed to create texture from surface!\n");
		}

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}


