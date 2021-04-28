#include "LargeAsteroid.h"

#include <cstdlib>
using namespace std;

LargeAsteroid::LargeAsteroid(SDL_Renderer* renderer, float x, float y){
	active = false;

	speed = 200.0;

	SDL_Surface* surface = IMG_Load("./Assets/Large_Asteroid.png");

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	posRect.x = x;
	posRect.y = y;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	rockAngle = 0.0f;

	// center of the rock's sprite
	rockCenter.x = posRect.w / 2;
	rockCenter.y = posRect.h / 2;
}

void LargeAsteroid::Reposition(){
	active = true;

	// location on screen
	int location = rand() % 4;

	// movement of asteroid
	int direction = rand() % 2;

	switch (location) {
	case LEFT:
	{
		// off screen on the left
		posRect.x = -posRect.w;
		pos_X = posRect.x;

		// random from top to bottom
		int bottomofScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomofScreen + posRect.h;
		pos_Y = posRect.y;
		break;

		// determine movement direction
		if (direction == UP)
		{
			xDir = 1;
			yDir = -1;
		}
		else
		{
			xDir = 1;
			yDir = 1;
		}
	}
	case RIGHT:
	{
		// off screen on the right
		posRect.x = 1024;
		pos_X = posRect.x;

		// random from top to bottom
		int bottomofScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomofScreen + posRect.h;
		pos_Y = posRect.y;

		// determine movement direction
		if (direction == UP)
		{
			xDir = -1;
			yDir = -1;
		}
		else
		{
			xDir = -1;
			yDir = 1;
		}
		break;
	}
	case TOP:
	{
		// off screen on the top
		posRect.y = -posRect.h;
		pos_Y = posRect.y;

		// random from left to right
		int sideOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % sideOfScreen + posRect.w;
		pos_X = posRect.x;

		if (direction == UP)
		{
			xDir = -1;
			yDir = 1;
		}
		else
		{
			xDir = 1;
			yDir = 1;
		}
		break;
	}
	case BOTTOM:
	{
		// off screen on the bottom
		posRect.y = 768;
		pos_Y = posRect.y;

		// random from left to right
		int sideOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % sideOfScreen + posRect.w;
		pos_X = posRect.x;

		if (direction == UP)
		{
			xDir = -1;
			yDir = -1;
		}
		else
		{
			xDir = 1;
			yDir = -1;
		}
		break;
	}
	}

}

void LargeAsteroid::Update(float deltaTime){
	if (active) {
		// get large rock's new position
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;

		// adjust for precision loss
		posRect.x = (int)pos_X + 0.5f;
		posRect.y = (int)pos_Y + 0.5f;

		rockAngle += .1;

		// check to see if the large rock is off the left of the screen
		if (posRect.x < (0 - posRect.w)) {
			posRect.x = 1024;
			pos_X = posRect.x;
		}

		// off right of screen
		if (posRect.x > 1024) {
			posRect.x = (0 - posRect.w);
			pos_X = posRect.x;
		}

		// off top of screen
		if (posRect.y < (0 - posRect.w)) {
			posRect.y = 768;
			pos_Y = posRect.y;
		}

		// off bottom of screen
		if (posRect.y > 768) {
			posRect.y = (0 - posRect.w);
			pos_Y = posRect.y;
		}
	}
}

void LargeAsteroid::Deactivate() {
	active = false;

	posRect.x = -2000;
	posRect.y = -2000;

	pos_X = posRect.x;
	pos_Y = posRect.y;
}

void LargeAsteroid::Draw(SDL_Renderer* renderer){
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);
}

LargeAsteroid::~LargeAsteroid(){
	//SDL_DestroyTexture(texture);
}