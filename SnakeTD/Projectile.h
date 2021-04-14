#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Tower.h"
class Fluid
{
	private:
		unsigned char iR, iG, iB, oR, oG, oB;
	public:
		Fluid(unsigned char _iR, unsigned char _iG, unsigned char _iB, unsigned char _oR, unsigned char _oG, unsigned char _oB);
		void getColor(unsigned char *r, unsigned char *g, unsigned char *b);
};

class Projectile : public Thing
{
	private:
		Sprite *sprite;
		int vx, vy;
		int ax, ay;
		Tower *father;
		int distance(Tower *t);
		void updateVel();
		bool toExplode;
		bool toDestroy;
		Explosion *explosion;
		unsigned char r, g, b;
	public:
		//int x, y;
		Tower *target;
		~Projectile();
		bool update(Map *map, vector<Tower *> *towers);
		Projectile(Map *map, Tower *father, Tower *target);
		void draw(SDL_Surface *gameLayer);
		void explode(Tower *target=NULL); // Pas de cible : on centre l'explosion sur le projectile
		inline int getX() {return x;}
		inline int getY() {return y;}
};

#endif // PROJECTILE_H
