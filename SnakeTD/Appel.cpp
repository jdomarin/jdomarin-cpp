#include "Game.h"

Appel::Appel(Tower *_owner) : SpecialAttack(_owner, &(specialAttackTypes[0]))
{
	//ctor
	preparation = true;
	appelCounter = 0;
	endCounter = 0;
}

Appel::~Appel()
{
	//dtor
	for(int i=0; i<owner->game->towers.size(); i++)
	{
		if(owner->game->towers[i]->getType() == &(towerTypes[TOWER_FOURMI]) && owner->game->towers[i]->ownerId == owner->ownerId)
		{
			owner->game->removeTower(i);
			i--;
		}
	}
}


bool Appel::update()
{
	appelCounter++;
	if(owner->getType()->cat == ABOMINATION) if((preparation && appelCounter >= 10) || (appelCounter >= 200))
	{
		appelCounter = 0;
		int x = -2, y = -2;
		int xdest = +2, ydest = -2;
		bool xincr = true;
		bool start = true;
		while(ydest != y || ydest > -4)
		{
			if(! owner->game->blockTaken(owner->getOx()+x, owner->getOy()+y, false))
			{
				owner->game->towers.push_back(new Tower(owner->game, owner->game->towers.size(), 32*(owner->getOx()+x)+16, 32*(owner->getOy()+y)+16, 0, 0, NULL, &(towerTypes[TOWER_FOURMI]), -1, true));
				break;
			}

			if(xdest > x) x++;
			if(xdest < x) x--;
			if(ydest > y) y++;
			if(ydest < y) y--;
			if(xdest == x && ydest == y)
			{
				if(xincr)
				{
					ydest = (ydest > 0 ? -ydest-1 : -ydest);
					xincr = false;
				}
				else
				{
					if(start)
					{
						xdest = -xdest;
						start = false;
					}
					else xdest = (xdest > 0 ? -xdest : -xdest+1);
					xincr = true;
				}
			}

		}
		if(preparation && (ydest <= -4 && ydest == y)) preparation = false;
	}
	if(owner->getType()->cat == HEAD)
	{
		endCounter++;
		if(endCounter >= 1000) return true;
		int x = owner->getOx()+(owner->getDy()-owner->getOy());
		int y = owner->getOy()+(owner->getDx()-owner->getOx());
		if(! owner->game->blockTaken(x, y, false))
		{
			owner->game->towers.push_back(new Tower(owner->game, owner->game->towers.size(), 32*x+16, 32*y+16, 0, 0, NULL, &(towerTypes[TOWER_FOURMI]), owner->ownerId, true));
			owner->game->towers.back()->owner = owner->owner;
		}
		x = owner->getOx()-(owner->getDy()-owner->getOy());
		y = owner->getOy()-(owner->getDx()-owner->getOx());
		if(! owner->game->blockTaken(x, y, false))
		{
			owner->game->towers.push_back(new Tower(owner->game, owner->game->towers.size(), 32*x+16, 32*y+16, 0, 0, NULL, &(towerTypes[TOWER_FOURMI]), owner->ownerId, true));
			owner->game->towers.back()->owner = owner->owner;
		}
	}

	return false;
}

void Appel::draw(SDL_Surface *gameLayer)
{
}
