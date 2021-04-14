#ifndef SPECIALATTACK_H
#define SPECIALATTACK_H
#include "Tower.h"

typedef struct
{
	ImageList *anim;
	Mix_Chunk *scream;
	char name[20];
} SpecialAttackType;

typedef struct
{
	int x, y;
	int vx;
} Point;

extern vector<SpecialAttackType> specialAttackTypes;
void loadSpecialAttackTypes();

class SpecialAttack
{
	public:
		Tower *owner;
		SpecialAttackType *type;
		SpecialAttack(Tower *owner, SpecialAttackType *type);
		virtual ~SpecialAttack();

		virtual void draw(SDL_Surface *gameLayer) = 0;
		virtual bool update() = 0; // Renvoie true à la fin de l'attaque

		void drawAnim(SDL_Surface *screen);
		bool updateAnim();
	protected:
	private:
		int animCounter;
		vector<Point> ellipses;
		Sprite *animSprite;
};

class Fury : SpecialAttack
{
	public:
		void draw(SDL_Surface *screen);
		bool update();
		Fury(Tower *owner) : SpecialAttack(owner, &(specialAttackTypes[0])) {attackCounter = 500;}
	private:
		int attackCounter;
};

#endif // SPECIALATTACK_H
