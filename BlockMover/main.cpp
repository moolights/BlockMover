// Block Mover project

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <random>

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

SDL_Texture* gTextureSet[TEXTURE_TOTAL];

bool init();

bool loadMedia(); // Loads media

void close();

SDL_Rect generateRect(int width, int height);

SDL_Rect generateRect(int width, int height, int randFlag);

SDL_Texture* loadTexture(std::string path);

bool collisionCheck(SDL_Rect& srcRect, SDL_Rect& destRect);

int main(int argv, char* argc[])
{
	if (!init())
	{
		printf("Failed to initialize systems\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool isRunning = true;
			SDL_Event e;

			SDL_Rect dRect = generateRect(150, 150); // Square that moves
			SDL_Rect sRect = generateRect(100, 100, 0); // Square that is stationary

			while (isRunning)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						isRunning = false;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							dRect.y -= 10;
							break;
						case SDLK_DOWN:
							dRect.y += 10;
							break;
						case SDLK_RIGHT:
							dRect.x += 10;
							break;
						case SDLK_LEFT:
							dRect.x -= 10;
							break;
						default:
							break;
						}
					}

					SDL_RenderClear(gRenderer);
					SDL_RenderCopy(gRenderer, gTextureSet[DYNAMIC_SQUARE_TEXTURE], NULL, &dRect);
					SDL_RenderCopy(gRenderer, gTextureSet[STATIC_SQUARE_TEXTURE], NULL, &sRect);
					SDL_RenderPresent(gRenderer);

					collisionCheck(dRect, sRect);
				}
			}
		}
	}

	close();
	return 0;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("FAILED TO INITIALIZE! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("Block Mover", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Failed to create window! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Failed to create renderer! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 255, 209, 220, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("Failed to initialize IMG Systems! SDL_IMG Error: %s\n", IMG_GetError());
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
		printf("Failed to load image %s. SDL Error: %s\n", "grey.png", SDL_GetError());
		success = false;
	}

	gTextureSet[DYNAMIC_SQUARE_TEXTURE] = loadTexture("LaCroixKitten.png");
	if (gTextureSet[DYNAMIC_SQUARE_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s\n", "LaCroixKitten.png", SDL_GetError());
		success = false;
	}

	gTextureSet[WINDOW_ERROR_TEXTURE] = loadTexture("red.png");
	if (gTextureSet[WINDOW_ERROR_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s\n", "red.png", SDL_GetError());
		success = false;
	}

	gTextureSet[WINDOW_SUCCESS_TEXTURE] = loadTexture("green.png");
	if (gTextureSet[WINDOW_SUCCESS_TEXTURE] == NULL)
	{
		printf("Failed to load image %s. SDL Error: %s\n", "green.png", SDL_GetError());
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

	IMG_Quit();
	SDL_Quit();

}

SDL_Texture* loadTexture(std::string path)
{

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Failed to load image %s. SDL_Image Error: %s\n", path.c_str(), IMG_GetError());
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

SDL_Rect generateRect(int width, int height)
{
	SDL_Rect rect;

	rect.w = width;
	rect.h = height;
	rect.x = (SCREEN_WIDTH - rect.w) / 2;
	rect.y = (SCREEN_HEIGHT - rect.h) / 2;

	return rect;
}

// Fix where square spawns
SDL_Rect generateRect(int width, int height, int randFlag)
{
	SDL_Rect rect;

	rect.w = width;
	rect.h = height;

	// Default location
	if (randFlag == -1)
	{
		rect.x = 1601;
		rect.y = 373;
	}
	else
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_int_distribution<> distribX(1, SCREEN_WIDTH - rect.w);
		int xNum = distribX(gen);

		std::uniform_int_distribution<> distribY(1, SCREEN_HEIGHT - rect.h);
		int yNum = distribY(gen);

		/*printf("X num: %d\n", xNum);
		printf("Y num: %d\n", yNum);*/
		
		// Case if square is in the left or right section of window
		if (xNum <= SCREEN_WIDTH / 3 || xNum >= ((SCREEN_WIDTH / 3) * 2) && yNum <= SCREEN_HEIGHT / 3 || yNum >= (SCREEN_HEIGHT / 3) * 2)
		{
			rect.x = xNum;
			rect.y = yNum;
		}
		else if (xNum > SCREEN_WIDTH / 3 && xNum < ((SCREEN_WIDTH / 3) * 2) && yNum > SCREEN_HEIGHT / 3 || yNum < (SCREEN_HEIGHT / 3) * 2)
		{
			rect.x = xNum;
			rect.y = yNum;
		}
		// Case if square is in the middle section of window
		else if (xNum < ((SCREEN_WIDTH / 3) * 2) && yNum <= SCREEN_HEIGHT / 3 || yNum >= (SCREEN_HEIGHT / 3) * 2)
		{
			rect.x = xNum;
			rect.y = yNum;
		}
	}

	return rect;
}

// Fix collision
bool collisionCheck(SDL_Rect& srcRect, SDL_Rect& destRect)
{
	bool collide = false;


	return collide;
}