#include "Explosion.h"
#include "Tower.h"

ExplosionType::ExplosionType(unsigned char iR, unsigned char iG, unsigned char iB, unsigned char oR, unsigned char oG, unsigned char oB)
{
	for(int radius=1; radius<64; radius++)
	{
		SDL_Surface *temp;
		temp = createSurface(129, 129, true, true);
		for(int x=0; x<128; x++)
		{
			int ymax;
			if(abs(x-64) <= radius)
			{
				ymax = sqrt(radius*radius-(x-64)*(x-64));
				pixelRGBA(temp, x, 64+ymax+1, 0, 0, 0, 255);
				pixelRGBA(temp, x, 64-ymax-1, 0, 0, 0, 255);
				for(int y = 64-ymax; y<64+ymax; y++)
				{
					float r = 1-sqrt((x-64)*(x-64)+(y-64)*(y-64))/radius;
					pixelRGBA(temp, x, y, iR*r+oR*(1-r),
								iG*r+oG*(1-r),
								iB*r+oB*(1-r),
								255);
				}
			}
		}
		images.push_back(temp);
	}
}


Explosion::Explosion(ExplosionType *explType, Thing *_owner, int _radius, int _totalTime, int _x, int _y)
{
	radius = (_radius > 63 ? 63 : _radius);
	type = explType;
	time = 0;
	totalTime = _totalTime;
	x = _x; y = _y;
	owner = _owner;
}

bool Explosion::draw(SDL_Surface *gameLayer)
{
	int curRadius;
	if(time > 2*totalTime/3)
		curRadius = radius-radius*(time-2*totalTime/3)*3/totalTime;
	else
		curRadius = radius*time/totalTime;
	time++;
	if(curRadius > 63 || curRadius < 0 || time > totalTime) return true;

	SDL_Rect p;

	p.x = owner->getX()/P+x-type->getImage(0)->w/2;
	p.y = owner->getY()/P+y-type->getImage(0)->h/2;


	SDL_BlitSurface(type->getImage(curRadius), 0, gameLayer, &p);
	return false;
}

