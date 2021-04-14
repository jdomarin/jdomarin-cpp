#include "Bonus.h"

vector<ImageList> bonusImages;
void loadBonusImages()
{
/*	bonusImages.push_back(loadImages("bonus_atk", false));
	bonusImages.push_back(loadImages("bonus_exp", false));
	bonusImages.push_back(loadImages("bonus_regen", false));*/
}

Bonus::Bonus(int _x, int _y)
{
	//ctor
	if(rand() % 50 == 0)
	{
		type = BONUS_SPECATK;
		specAtkId = 0;
	}
	else
	{
		int r = rand() % 3;
		switch(r)
		{
			case 0: type = BONUS_ATK; break;
			case 1: type = BONUS_EXP; break;
			case 2: type = BONUS_REGEN; break;
		}
	}
	x = _x; y = _y;
	c = 0; dc = 10;
	//sprite = new Sprite(bonusImags[r], 0, x*32, y*32, 10);
}

void Bonus::draw(SDL_Surface *gameLayer)
{

	//sprite->blit(gameLayer);
	rectangleRGBA(gameLayer, x*32, y*32, x*32+31, y*32+31, c, c, c, 255);
	switch(type)
	{
		case BONUS_ATK: stringRGBA(gameLayer, x*32+4, y*32+12, "ATK", c, 0, 0, 255); break;
		case BONUS_EXP: stringRGBA(gameLayer, x*32+4, y*32+12, "EXP", 0, 0, c, 255); break;
		case BONUS_REGEN: stringRGBA(gameLayer, x*32+4, y*32+12, "REG", 0, c, 0, 255); break;
		case BONUS_SPECATK: stringRGBA(gameLayer, x*32+4, y*32+12, "SPE", c, c, c, 255); break;
	}
	c += dc;
	if(c == 250 || c == 0) dc *= -1;
}

Bonus::~Bonus()
{
	//dtor
}
