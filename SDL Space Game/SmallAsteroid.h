#pragma once
#include <SDL.h>
#include <SDL_image.h>

class SmallAsteroid {
public:
	enum LOCATION { LEFT, RIGHT, TOP, BOTTOM };
	enum DIRECTION { UP, DOWN };
	bool active;

	SDL_Texture* texture;
	SDL_Rect posRect;
	float xDir, yDir, speed, pos_X, pos_Y;

	SDL_Point rockCenter;
	float rockAngle;

	SmallAsteroid(SDL_Renderer* renderer, float x, float y);

	void Reposition(float x, float y);

	void Update(float deltaTime);

	void Deactivate();

	void Draw(SDL_Renderer* renderer);

	~SmallAsteroid();
};