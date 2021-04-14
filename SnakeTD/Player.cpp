#include "Game.h"

int cur(float x, char dx)
{
	return (int)(x-dx*0.5);
}

Player::Player(Game *_game, int _id, int _w, int _h, int _xs, int _ys, unsigned char r, unsigned char g, unsigned char b)
{
	//ctor
	game = _game;
	id = _id;
	head = new Tower(game, _id, 0, 0, 0, 1, NULL, &(towerTypes[0]), id, true);
	xs = _xs; ys = _ys;
	w = _w; h = _h;
	clipRect.w = w;
	clipRect.h = h;
	clipRect.x = 0;
	clipRect.y = 0;
	shield = 0;
	_speed = 16;
	requestSpeed = 16;
	head->owner = this;
	displayInfo = true;
	shields = 0;
	shieldsLoad = 0;
	c[0] = r; c[1] = g; c[2] = b;
	bonusAtk = 0;
	bonusExp = 0;
	bonusRegen = 0;
	friendly = true;
}

void Player::manageEvent(SDL_Event ev)
{
	if(ev.type == SDL_KEYDOWN)
	{
		if(id == 1)
		switch(ev.key.keysym.sym)
		{
			case SDLK_RSHIFT: increaseSpeed(); break;
			case SDLK_F10: head->power *= 100; break;
			default:;
		}
		if(id == 2)
		switch(ev.key.keysym.sym)
		{
			case SDLK_LSHIFT: increaseSpeed(); break;
			default:;
		}
		if(id == 3)
		switch(ev.key.keysym.sym)
		{
			case SDLK_i: increaseSpeed(); break;
			default:;
		}
	}

	if(ev.type == SDL_KEYUP)
	{
		if(id == 1)
		switch(ev.key.keysym.sym)
		{
			case SDLK_UP: head->heading(0, -1); break;
			case SDLK_DOWN: head->heading(0, 1); break;
			case SDLK_RIGHT: head->heading(1, 0); break;
			case SDLK_LEFT: head->heading(-1, 0); break;
			case SDLK_RSHIFT: normalSpeed(); break;
			case SDLK_RETURN: friendly = !friendly; break;
			case SDLK_F10: head->power /= 100; break;
			case SDLK_RCTRL: head->engageSpecialAttack();;
			default:;
		}

		if(id == 2)
		switch(ev.key.keysym.sym)
		{
			case SDLK_z: head->heading(0, -1); break;
			case SDLK_s: head->heading(0, 1); break;
			case SDLK_d: head->heading(1, 0); break;
			case SDLK_q: head->heading(-1, 0); break;
			case SDLK_LSHIFT: normalSpeed(); break;
			case SDLK_a: head->engageSpecialAttack(); break;
			case SDLK_w: friendly = !friendly; break;
			case SDLK_e: displayInfo = !displayInfo; break;
			default:;
		}
		if(id == 3)
		switch(ev.key.keysym.sym)
		{
			case SDLK_u: head->heading(0, -1); break;
			case SDLK_j: head->heading(0, 1); break;
			case SDLK_k: head->heading(1, 0); break;
			case SDLK_h: head->heading(-1, 0); break;
			case SDLK_i: normalSpeed(); break;
			case SDLK_b: friendly = !friendly; break;
			case SDLK_y: head->engageSpecialAttack(); break;
			default:;
		}
	}


}

SDL_Rect Player::pos()
{
	clipRect.x = (head->x)/P-w/2;
	clipRect.y = (head->y)/P-h/2;
	clipRect.w = w;
	clipRect.h = h;
	return clipRect;
}

SDL_Rect Player::screenPart()
{
	SDL_Rect screenpart;
	screenpart.w = w;
	screenpart.h = h;
	screenpart.x = xs;
	screenpart.y = ys;
	return screenpart;
}

void Player::update(bool *newTower)
{
	if(_speed != requestSpeed) if((head->x-16*P) % requestSpeed == 0 && (head->y-16*P) % requestSpeed == 0) _speed = requestSpeed;
	if(_speed < 16)
	{
		timeLowSpeedElapsed++;
		if(timeLowSpeedElapsed >= timeLowSpeed)
			requestSpeed = 16;
	}
	if(shields > 0 && shieldsLoad > 0)
	{
		shieldsLoad -= (rand() % 2)+shieldsLoad/1000;
		if(shieldsLoad < 0) shieldsLoad = 0;
	}
	head->update(true, newTower);

	for(int i=0; i<splashes.size(); i++)
	{
		if(splashes[i]->update())
		{
			delete splashes[i];
			splashes.erase(splashes.begin()+i);
			i--;
		}
	}
}

void Player::increaseSpeed()
{
	oldSpeed = (_speed <= 16 ? _speed : 16);
	if(head->life <= 0) return;
	requestSpeed = 32;
}

void Player::reduceSpeed(int factor, int time)
{
	int nspeed;
	if(_speed == 32) return;
	nspeed = 16/factor;
	if(nspeed <= _speed)
	{
		requestSpeed = nspeed;
		timeLowSpeed = (time > 0 ? 1+time/head->sumLevels() : -time);
		timeLowSpeedElapsed = 0;
	}
}

void Player::normalSpeed()
{
	if(_speed == 32)
		requestSpeed = oldSpeed;
}

void Player::regenSpeed()
{
	timeLowSpeedElapsed += 20;
}

void Player::takeDamageOnShields(int damage)
{
	shieldsLoad += damage;
	if(shieldsLoad >= shields)
	{
		if(! head->shieldsReloading())
		{
			head->destroyShieldGenerators();
			shieldsLoad = 0;
		}
		else
			shieldsLoad = shields-1;
	}
}
