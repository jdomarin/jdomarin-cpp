#include "Game.h"
vector<SpecialAttackType> specialAttackTypes;

void loadSpecialAttackTypes()
{
	SpecialAttackType t;
	t.anim = loadImages("cat", false);
	t.scream = Mix_LoadWAV("spcatk_1.wav");
	sprintf(t.name, "Furie");
	specialAttackTypes.push_back(t);

	t.anim = loadImages("lion", false);
	sprintf(t.name, "Slurp !");
	t.scream = Mix_LoadWAV("spe_slurp.wav");
	specialAttackTypes.push_back(t);
	Slurp::load();

	t.anim = loadImages("poulpe", false);
	sprintf(t.name, "Poulpofolie");
	specialAttackTypes.push_back(t);

	t.anim = loadImages("cat", false);
	sprintf(t.name, "Appel"); // Ouh, c'est pas une bonne idée ça...
	specialAttackTypes.push_back(t);

	Poulpofolie::load();
}

SpecialAttack::SpecialAttack(Tower *_owner, SpecialAttackType *_type)
{
	//ctor
	owner = _owner;
	type = _type;
	animCounter = 0;
	for(int i=0; i<30; i++)
	{
		Point p;
		p.x = rand() % (800*P);
		p.y = 200*P+rand() % (200*P);
		p.vx = rand() % (10*P);
		ellipses.push_back(p);
	}
	animSprite = new Sprite(type->anim, 0, 800, 300, 2, false);
}

SpecialAttack::~SpecialAttack()
{
	//dtor
	delete animSprite;
}

void SpecialAttack::drawAnim(SDL_Surface *screen)
{
	for(int i=0; i<ellipses.size(); i++)
	{
		Point p = ellipses[i];
		filledEllipseRGBA(screen, p.x/P, p.y/P, 20, 10, 255, 255, 255, 255);
	}
	if(animCounter < 30) animSprite->x += (200-animSprite->x)/10;
	if(animCounter == 30)
	{
		animSprite->setMode(1, false);
		playChunk(type->scream);
	}

	animSprite->blit(screen);
}

bool SpecialAttack::updateAnim()
{
	for(int i=0; i<ellipses.size(); i++)
	{
		ellipses[i].x -= ellipses[i].vx;
		if(ellipses[i].x < 0) ellipses[i].x = 820*P;
	}
	animCounter++;
	if(animCounter >= 100) return true;
	else return false;
}

void Fury::draw(SDL_Surface *gameLayer)
{
	Tower *t = owner;
	while(t != NULL)
	{
		for(int i=0; i<200; i++)
		{
			pixelRGBA(gameLayer, t->x/P-16+(rand()%32), t->y/P-16+(rand()%32), 200+(rand()%50), rand()%100, rand()%100, 255);
		}
		t = t->next;
	}
};

bool Fury::update()
{
	attackCounter--;
	Tower *t = owner->owner->getHead();
	while(t != NULL)
	{
		t->resetReloadCounter();
		t = t->next;
	}
	if(attackCounter == 0) return true;
	else return false;
}


