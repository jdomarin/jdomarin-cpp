#ifndef MOB_H
#define MOB_H
#include "Sprite.h"
class Game;

class Mob
{
	public:
		int x, y;
		int life;
		int speed;
		bool flying;
		bool update();
		void draw(SDL_Surface *gameLayer);
		Mob(Game *game, int x, int y, int life, int speed, bool flying, ImageList *image);
		void takeDamage(int damage);
		bool toDelete;
		~Mob();
	private:
		Game *game;
		Sprite *sprite;
		int vx, vy;
		float theta;
};

#endif // MOB_H
