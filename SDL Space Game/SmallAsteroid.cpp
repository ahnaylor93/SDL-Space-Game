#include "SmallAsteroid.h"

#include <cstdlib>
using namespace std;

SmallAsteroid::SmallAsteroid(SDL_Renderer* renderer, float x, float y) {
	active = false;

	speed = 400.0;

	SDL_Surface* surface = IMG_Load("./Assets/Small_Asteroid.png");

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

void SmallAsteroid::Reposition(float x, float y) {
	active = true;

	posRect.x = x;
	posRect.y = y;

	pos_X = x;
	pos_Y = y;

	// movmenet; 1 = up, 2 = down
	int upOrDown = rand() % 2 + 1;

	// movement; 1 = left, 2 = right
	int leftOrRight = rand() % 2 + 1;

	if (upOrDown == 1) { // up
		if (leftOrRight == 1) {
			xDir = -1;
			yDir = -1;
		}
		else { // move right and up
			xDir = 1;
			yDir = -1;
		}
	}
	else {// down
		if (leftOrRight == 1) { // move left and down
			xDir = -1;
			yDir = 1;
		}
		else{ // move right and down
			xDir = 1;
			yDir = -1;
		}
	}
}

void SmallAsteroid::Update(float deltaTime) {
	if (active) {
		// get Small rock's new position
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;

		// adjust for precision loss
		posRect.x = (int)pos_X + 0.5f;
		posRect.y = (int)pos_Y + 0.5f;

		rockAngle += .1;

		// check to see if the small rock is off the left of the screen
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

void SmallAsteroid::Deactivate() {
	active = false;

	posRect.x = -2000;
	posRect.y = -2000;

	pos_X = posRect.x;
	pos_Y = posRect.y;
}

void SmallAsteroid::Draw(SDL_Renderer* renderer) {
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);
}

SmallAsteroid::~SmallAsteroid() {
	//SDL_DestroyTexture(texture);
}