#include "Game.h"

Mob::Mob(Game *_game, int _x, int _y, int _life, int _speed, bool _flying, ImageList *image)
{
	game = _game;
	x = _x;
	y = _y;
	speed = _speed;
	flying = _flying;
	sprite = new Sprite(image, 0, _x/P, _y/P, 10, true);
	theta = (rand() % 360);
}

bool Mob::update()
{
	theta += (rand() % 2 == 0 ? -0.1 : 0.1);
	if(theta < 0) theta += 360;
	if(theta > 360) theta -= 360;
	vx = speed*cos(M_PI*theta/180);
	vy = speed*sin(M_PI*theta/180);
	x += vx;
	y += vy;
	if((!flying) && ! game->map->blockType(x/P/32, y/P/32)->pass)
	{
		x -= vx;
		y -= vy;
		theta = 360-theta;
	}

	if(life < 0) return true;
	return false;
}

void Mob::takeDamage(int dmg)
{
	life -= dmg;
}

void Mob::draw(SDL_Surface *gameLayer)
{
	sprite->blit(gameLayer);
}

Mob::~Mob()
{
	delete sprite;
}


