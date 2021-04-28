#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>

#include "Bullet.h"
#include "LargeAsteroid.h"
#include "SmallAsteroid.h"

using namespace std;

// boolean to maintain program loop
bool quit = false;

// deltatime init() - for frame rate independence
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

// vars for player's angle
float x, y, playerAngle, oldAngle;

// for precision loss
float pos_X, pos_Y;

// floats for direction
float xDir, xDirOld;
float yDir, yDirOld;

// player speed
float playerSpeed = 400.0f;

// rectangle for player position
SDL_Rect playerPos;

// player's center point
SDL_Point center;

// bullets
vector<Bullet> bulletList;

// large asteroids
vector<LargeAsteroid> largeAstList;

// small asteroids
vector<SmallAsteroid> smallAstList;

// declare sounds
Mix_Chunk* laser, * explosion;

void CreateBullet() {
	for (int i = 0; i < bulletList.size(); i++) {
     		if (bulletList[i].active == false) {
  			Mix_PlayChannel(-1, laser, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = pos_X;
			bulletList[i].posRect.y = pos_Y;

			bulletList[i].pos_X = pos_X;
			bulletList[i].pos_Y = pos_Y;

			bulletList[i].xDir = xDirOld;
			bulletList[i].yDir = yDirOld;

			bulletList[i].Reposition();

			break;
		}
	}
}

int main(int argc, char* argv[]) {
	// seed random numbers
	srand(time(NULL));

	SDL_Window* window;		// declare a pointer
	SDL_Init(SDL_INIT_EVERYTHING);	// initialize SDL2
	SDL_Renderer* renderer = NULL;

	// create an application window with the following settings:
	window = SDL_CreateWindow(
		"Space Rocks",				//window title
		SDL_WINDOWPOS_UNDEFINED,	// initial x position
		SDL_WINDOWPOS_UNDEFINED,	// initial y position
		1024,						// width in pixels
		768,						// height in pixels
		SDL_WINDOW_OPENGL			// flags, see below
	);

	// check that the window was successfully created
	if (window == NULL) {
		// in the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// background image -- CREATE
	// create a SDL surface
	SDL_Surface* surface = IMG_Load("./ASsets/Background.png");

	// create bkgd texture
	SDL_Texture* bkgd;

	// place surface into the texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	// create rectangles for menu graphics
	SDL_Rect bkgdPos;

	// set bckgdPos x, y , width and height
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;

	// background image -- CREATE END

	// player image -- CREATE

	// create a SDL surface
	surface = IMG_Load("./Assets/player.png");

	// create player texture
	SDL_Texture* player;

	// place surface into the texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface
	SDL_FreeSurface(surface);

	// set playerPos x, y, width, and height
	playerPos.x = 1024 / 2;
	playerPos.y = 768 / 2;
	playerPos.w = 59;
	playerPos.h = 39;

	center.x = playerPos.w / 2;
	center.y = playerPos.h / 2;

	// pos_X and pos_Y for precision loss
	pos_X = playerPos.x;
	pos_Y = playerPos.y;

	// the player graphic is facing right, so the xDirOld is set to 1 so bullets go in correct direction
	xDirOld = 1;
	yDirOld = 0;

	// player image CREATE END

	// SDL event to handle input
	SDL_Event event;

	// create our bulletList
	for (int i = 0; i < 10; i++) {
		Bullet tempBullet(renderer, -1000.0f, -1000.0f);

		bulletList.push_back(tempBullet);
	}

	//// create our large asteroid list
	//for (int i = 0; i < 1; i++)
	//{
	//	LargeAsteroid tempAst(renderer, -1000.0f, -1000.0f);

	//	largeAstList.push_back(tempAst);
	//}

	//// create our Small asteroid list
	//for (int i = 0; i < 2; i++)
	//{
	//	SmallAsteroid tempAst(renderer, -1000.0f, -1000.0f);

	//	smallAstList.push_back(tempAst);
	//}

	// init audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	// load sounds
	laser = Mix_LoadWAV("./Assets/shoot.wav");
	explosion = Mix_LoadWAV("./Assets/explosion.wav");

	// LEVEL UP *******************************************************
	// rocks to start game
	int numberOfLargeRocks = 1;
	int numberOfSmallRocks = 2;

	// total rocks on the level
	int totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

	// total rocks currently destroyed
	int totalRocksDestroyed = 0;

	// create our large asteroid list
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		LargeAsteroid tempAst(renderer, -1000.0f, -1000.0f);

		largeAstList.push_back(tempAst);
	}

	// create our Small asteroid list
	for (int i = 0; i < numberOfSmallRocks; i++)
	{
		SmallAsteroid tempAst(renderer, -1000.0f, -1000.0f);

		smallAstList.push_back(tempAst);
	}

	// activate all large rocks for level 1
	for (int i = 0; i < numberOfLargeRocks; i++) {
		largeAstList[i].Reposition();
	}

	while (!quit)
	{
		// create deltatime
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		// check for input for closing window and firing
		if (SDL_PollEvent(&event)) {
			// close window by the window's x button
			if (event.type == SDL_QUIT) {
				quit = true;
			}

			switch (event.type) {
				// look for keypress and release
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_SPACE:
					CreateBullet();
					break;

				default:
					break;
				}
			}
		}

		// player movement
		// get the keyboard state
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		// check which arrows are pressed for xDIR
		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			// if left button pressed, set xDir to -1;
			xDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			// if right button pressed, set xDir to 1;
			xDir = 1.0f;
		}
		else
		{
			// if neither is pressed, set xDir to 0;
			xDir = 0.0f;
		}

		// check which arrows are pressed for yDir
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			// if up button pressed, set yDir to -1;
			yDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			// if down button pressed, set yDir to 1;
			yDir = 1.0f;
		}
		else
		{
			// if neither is pressed, set yDir to 0;
			yDir = 0.0f;
		}

		// START UPDATE **************************************************

		if (xDir != 0 || yDir != 0) {
			// get x and y
			x = playerPos.x - xDir;
			y = playerPos.y - yDir;

			//calculate angle
			playerAngle = atan2(yDir, xDir) * 180 / 3.14;

			// update old angle
			oldAngle = playerAngle;

			// update old direction
			xDirOld = xDir;
			yDirOld = yDir;
		}
		else {
			// update old angle
			oldAngle = playerAngle;
		}

		// get player's new position
		pos_X += (playerSpeed * xDir) * deltaTime;
		pos_Y += (playerSpeed * yDir) * deltaTime;

		// adjust for precision loss
		playerPos.x = (int)(pos_X + 0.5f);
		playerPos.y = (int)(pos_Y + 0.5f);

		// player wrapping
		// check if player is off the left of the screen
		if (playerPos.x < (0 - playerPos.w)) {
			playerPos.x = 1024;
			pos_X = playerPos.x;
		}

		// off right of screen
		if (playerPos.x > 1024) {
			playerPos.x = (0 - playerPos.w);
			pos_X = playerPos.x;
		}

		// off top of screen
		if (playerPos.y < (0 - playerPos.w)) {
			playerPos.y = 768;
			pos_Y = playerPos.y;
		}

		// off bottom of screen
		if (playerPos.y > 768) {
			playerPos.y = (0 - playerPos.w);
				pos_Y = playerPos.y;
		}

		// update bullets
		for (int i = 0; i < bulletList.size(); i++) {
			if (bulletList[i].active) {
				bulletList[i].Update(deltaTime);
			}
		}

		// update large asteroids
		for (int i = 0; i < largeAstList.size(); i++)
		{
			if (largeAstList[i].active)
			{
				largeAstList[i].Update(deltaTime);
			}
		}

		// update small asteroids
		for (int i = 0; i < smallAstList.size(); i++)
		{
			if (smallAstList[i].active)
			{
				smallAstList[i].Update(deltaTime);
			}
		}

		// wrapping (collision detection)

		// check for collision of bullets with large rocks
		// for loop to scroll through all the player's bullets
		for (int i = 0; i < bulletList.size(); i++) {
			// check to see if this bullet is active
			if (bulletList[i].active)
			{
				// check all large rocks against the active bullet
				for (int j = 0; j < largeAstList.size(); j++) {
					// see if there is a collision between this bullet and this enemy
					if (SDL_HasIntersection(&bulletList[i].posRect, &largeAstList[j].posRect)) {
						// play explosion sound
						Mix_PlayChannel(-1, explosion, 0);

						// need two small rocks thar are false as the number of small rocks grows
						int smallRockCounter = 0;

						// create 2 small rocks
						for (int k = 0; k < smallAstList.size(); k++) {
							if (!smallAstList[k].active) {
								smallAstList[k].Reposition(largeAstList[j].posRect.x, largeAstList[j].posRect.y);

								// increase smallRockCounter
								smallRockCounter++;
							}

							// once we find 2 incative rocks, exit loop
							if (smallRockCounter == 2) {
								break;
							}
						}

						// reset the enemy
						largeAstList[j].Deactivate();

						// reset the bullet
						bulletList[i].Deactivate();

						totalRocksDestroyed++;
					}
				}
			}
		}

		// check for collision of bullets with small rocks
		// for loop to scroll through all the player's bullets
		for (int i = 0; i < bulletList.size(); i++) {
			// check to see if this bullet is active
			if (bulletList[i].active)
			{
				// check all small rocks against the active bullet
				for (int j = 0; j < smallAstList.size(); j++) {
					// check to see if there is a collision between this bullet and this enemy
					if (SDL_HasIntersection(&bulletList[i].posRect, &smallAstList[j].posRect)) {
						// play explosion
						Mix_PlayChannel(-1, explosion, 0);

						// reset the enemy
						smallAstList[j].Deactivate();

						// reset the bullet
						bulletList[i].Deactivate();

						totalRocksDestroyed++;

						if (totalRocksDestroyed >= totalNumberOfRocks)
						{
							// zero out total rocks destroyed
							totalRocksDestroyed = 0;

							// add rocks to level
							numberOfLargeRocks++; // add one large rock
							numberOfSmallRocks += 2; // add 2 small rocks

							totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;

							// clear small rock list
							smallAstList.clear();

							// rebuild rock lists with new numbers

							// load large rocks
							for (int i = 0; i < numberOfLargeRocks; i++) {
								LargeAsteroid tempAst(renderer, -1000.0f, -1000.0f);

								largeAstList.push_back(tempAst);
							}

							// load small rocks
							for (int i = 0; i < numberOfSmallRocks; i++) {
								SmallAsteroid tempAst(renderer, -1000.0f, -1000.0f);

								smallAstList.push_back(tempAst);
							}

							// activate all large rocks for next level
							for (int i = 0; i < numberOfLargeRocks; i++) {
								largeAstList[i].Reposition();
							}

						}
					}
				}
			}
		}

		// END UPDATE ****************************************************

		// START DRAW ********************************************************

		// Draw section
		// clear the old buffer
		SDL_RenderClear(renderer);

		// draw bkgd
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

		// draw bullets
		for (int i = 0; i < bulletList.size(); i++) {
			if (bulletList[i].active == true) {
				bulletList[i].Draw(renderer);
			}
		}

		// draw large asteroids
		for (int i = 0; i < largeAstList.size(); i++)
		{
			if (largeAstList[i].active)
			{
				largeAstList[i].Draw(renderer);
			}
		}

		// draw small asteroids
		for (int i = 0; i < smallAstList.size(); i++)
		{
			if (smallAstList[i].active)
			{
				smallAstList[i].Draw(renderer);
			}
		}

		// draw player
		SDL_RenderCopyEx(renderer, player, NULL, &playerPos, playerAngle, &center, SDL_FLIP_NONE);

		// draw new info to the screen
		SDL_RenderPresent(renderer);

		// END DRAW **********************************************************
	} // ends game loop
	// close and destroy the window
	SDL_DestroyWindow(window);

	// clean up
	SDL_Quit();

	return 0;
}