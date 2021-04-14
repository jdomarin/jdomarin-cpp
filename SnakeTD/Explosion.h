#include "Sprite.h"
#ifndef EXPLOSION_H
#define EXPLOSION_H

class Explosion;
class Thing;

class ExplosionType
{
	private:
		vector<SDL_Surface *> images;
		unsigned char insideColor[3], outsideColor[3];
	public:
		inline SDL_Surface *getImage(int i) {return images[i];}
		ExplosionType(unsigned char iR, unsigned char iG, unsigned char iB, unsigned char oR, unsigned char oG, unsigned char oB);
};

class Explosion
{
	private:
		int x, y;
		int time, totalTime;
		int radius;
		ExplosionType *type;
		Thing *owner;
	public:
		bool draw(SDL_Surface *gameLayer); // Renvoie true si fin de l'explosion
		Explosion(ExplosionType *expltype, Thing *owner, int radius, int _totalTime, int x = 0, int y = 0);
};

#endif
