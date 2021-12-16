// Block Mover

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <random>

// Window size
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

// Textures used in game
enum TextureSetValues
{
	STATIC_SQUARE_TEXTURE,
	DYNAMIC_SQUARE_TEXTURE,
	WINDOW_ERROR_TEXTURE,
	WINDOW_SUCCESS_TEXTURE,
	TEXTURE_TOTAL
};

// Window being renderered to
SDL_Window *gWindow = NULL;

// Renderer used in the window
SDL_Renderer *gRenderer = NULL;

// Set of textures used in game
SDL_Texture* gTextureSet[TEXTURE_TOTAL];

// Initializes systems
bool init();

// Loads media
bool loadMedia();

// Closes systems
void close(); 

// Generates a rectangle with a predetermined x & y
SDL_Rect generateRect(int width, int height);

// Generates a rectangle with a predetermined x & y if flag is set to 0 or random if -1
SDL_Rect generateRect(int width, int height, int randFlag);

// Loads textures into texture set
SDL_Texture* loadTexture(std::string path);

// Collision detection for two rects
bool blockCollision(SDL_Rect srcRect, SDL_Rect destRect);

// Collision detection for dynamic square and window "walls"
bool windowCollision(SDL_Rect dRect);

int main(int argv, char* argc[])
{
	if (!init()) // Initialize Systems
	{
		printf("Failed to initialize systems\n");
	}
	else
	{
		if (!loadMedia()) // Load Media
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool isRunning = true; // Main game loop value
			SDL_Event e; // Holds all SDL events

			SDL_Rect dRect = generateRect(150, 150); // Square that moves
			SDL_Rect sRect = generateRect(100, 100, 0); // Square that is stationary

			// Main game loop
			while (isRunning)
			{
				while (SDL_PollEvent(&e) != 0) // Poll each event during game time
				{
					if (e.type == SDL_QUIT) // End program if exit is pressed
					{
						isRunning = false;
					}
					else if (e.type == SDL_KEYDOWN) // Check if key is pressed
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							dRect.y -= 10; // Reduce y location of dynamic square
							break;
						case SDLK_DOWN:
							dRect.y += 10; // Increase y location of dynamic square
							break;
						case SDLK_RIGHT:
							dRect.x += 10; // Increase x location of dynamic square
							break;
						case SDLK_LEFT:
							dRect.x -= 10; // Decrease x location of dynamic square
							break;
						default:
							break;
						}
					}

					SDL_RenderClear(gRenderer); // Clear the renderer and window
					SDL_RenderCopy(gRenderer, gTextureSet[DYNAMIC_SQUARE_TEXTURE], NULL, &dRect); // Draw the dynamic rectangle with texture
					SDL_RenderCopy(gRenderer, gTextureSet[STATIC_SQUARE_TEXTURE], NULL, &sRect); // Draw stationary rectangle
					SDL_RenderPresent(gRenderer); // Render to the window

					// Check for collision
					if (blockCollision(dRect, sRect))
					{
						SDL_RenderCopy(gRenderer, gTextureSet[WINDOW_SUCCESS_TEXTURE], NULL, NULL); // Makes the screen green to confirm collision
						SDL_RenderPresent(gRenderer); // Render to window
						SDL_Delay(50); // Hold the green screen
						sRect = generateRect(100, 100, 0); // Move the rect to another place
					}

					// Check for collision
					if (windowCollision(dRect))
					{
						SDL_RenderCopy(gRenderer, gTextureSet[WINDOW_ERROR_TEXTURE], NULL, NULL); // Makes screen red
						SDL_RenderPresent(gRenderer); // Render to window
						SDL_Delay(50); // Hold the red screen
						dRect.x = (SCREEN_WIDTH - dRect.w) / 2; // Move the rect
						dRect.y = (SCREEN_HEIGHT - dRect.h) / 2;// back to the center
					}
				}
			}
		}
	}

	close(); // Close all systems
	return 0; // C needs this bleh
}

// Function to initialize systems
bool init()
{
	bool success = true; // Value to return for initialization status

	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize Video systems
	{
		printf("FAILED TO INITIALIZE! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create a window
		gWindow = SDL_CreateWindow("Block Mover", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Failed to create window! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create a renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Failed to create renderer! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				// Set the background color that the renderer will put in the window
				SDL_SetRenderDrawColor(gRenderer, 255, 209, 220, 0xFF);

				int imgFlags = IMG_INIT_PNG; // Flag for PNG
				if (!(IMG_Init(imgFlags) & imgFlags)) // Initialize IMG systems
				{
					printf("Failed to initialize IMG Systems! SDL_IMG Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success; // Status value
}

// Loads PNG images into texture set
bool loadMedia()
{
	bool success = true; // Status value

	// Loads respective textures into the pointer array of textures

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

// Closes systems
void close()
{
	SDL_DestroyRenderer(gRenderer); // Destroys renderer to free space
	SDL_DestroyWindow(gWindow); // Destroys window to free space

	// Points renderer and window to NULL 
	gRenderer = NULL;
	gWindow = NULL;

	// Frees space by deleting all textures
	// in the texture array. Then points
	// each location in array to NULL
	for (int i = 0; i < TEXTURE_TOTAL; i++)
	{
		SDL_DestroyTexture(gTextureSet[i]);
		gTextureSet[i] = NULL;
	}

	IMG_Quit(); // Closes IMG systems
	SDL_Quit(); // Closes SDL systems

}

// Loads texture from surface
SDL_Texture* loadTexture(std::string path)
{

	SDL_Texture* newTexture = NULL; // New texture to be loaded

	SDL_Surface* loadedSurface = IMG_Load(path.c_str()); // Gets surface from PNG 
	if (loadedSurface == NULL)
	{
		printf("Failed to load image %s. SDL_Image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Creates texture from create PNG surface
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Failed to create texture from surface!\n");
		}

		SDL_FreeSurface(loadedSurface); // Frees space used by temporary surface
	}

	return newTexture; // Return new texture
}

// Creates the dynamic block
SDL_Rect generateRect(int width, int height)
{
	SDL_Rect rect; // New rect to be made

	rect.w = width; // Sets width
	rect.h = height; // Sets height
	rect.x = (SCREEN_WIDTH - rect.w) / 2; // Places x at middle
	rect.y = (SCREEN_HEIGHT - rect.h) / 2; // Places y at middle

	return rect; // Return new rect
}

// Create the stationary block
SDL_Rect generateRect(int width, int height, int randFlag)
{
	SDL_Rect rect; // New rect

	rect.w = width; // Sets width
	rect.h = height; // Sets height

	// Default location
	if (randFlag == -1)
	{
		rect.x = 1601;
		rect.y = 373;
	}
	else
	{
		// Generates random number for x and y coordinates
		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_int_distribution<> distribX(1, SCREEN_WIDTH - rect.w);
		int xNum = distribX(gen); // Random x

		std::uniform_int_distribution<> distribY(1, SCREEN_HEIGHT - rect.h);
		int yNum = distribY(gen); // Random y
		
		// Case if square is in the left or right section of window
		if (xNum <= SCREEN_WIDTH / 3 || xNum >= ((SCREEN_WIDTH / 3) * 2) && yNum <= SCREEN_HEIGHT / 3 || yNum >= (SCREEN_HEIGHT / 3) * 2)
		{
			rect.x = xNum;
			rect.y = yNum;
		}
		// Case if square is in the middle section of the window
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

// Block to block collision
bool blockCollision(SDL_Rect srcRect, SDL_Rect destRect)
{
	bool collide = false;

	// Top-right corner collision 
	if (srcRect.y >= destRect.y && srcRect.y <= (destRect.y + destRect.h) && 
		(srcRect.x + srcRect.w) >= destRect.x && (srcRect.x + srcRect.w) <= (destRect.x + destRect.w))
	{
		collide = true;
	}
	// Top-left corner collision
	else if (srcRect.y >= destRect.y && srcRect.y <= (destRect.y + destRect.h) &&
		srcRect.x >= destRect.x && srcRect.x <= (destRect.x + destRect.w))
	{
		collide = true;
	}
	// Bottom-right corner collision
	else if ((srcRect.y + srcRect.h) >= destRect.y && (srcRect.y + srcRect.h) <= (destRect.y + destRect.h) &&
		srcRect.x >= destRect.x && srcRect.x <= (destRect.x + destRect.w))
	{
		collide = true;
	}
	// Bottom-left corner collision
	else if ((srcRect.y + srcRect.h) >= destRect.y && (srcRect.y + srcRect.h) <= (destRect.y + destRect.h) &&
		(srcRect.x + srcRect.w) >= destRect.x && (srcRect.x + srcRect.w) <= (destRect.x + destRect.w))
	{
		collide = true;
	}

	return collide;
}

// Block to window walls collision
bool windowCollision(SDL_Rect dRect)
{
	bool collide = false;

	// Block has collided with left or right wall
	if (dRect.x <= 0 || dRect.x + dRect.w >= SCREEN_WIDTH)
	{
		collide = true;
	}
	// Block has collided with top or bottom
	else if (dRect.y <= 0 || dRect.y + dRect.h >= SCREEN_HEIGHT)
	{
		collide = true;
	}
	
	return collide;
}