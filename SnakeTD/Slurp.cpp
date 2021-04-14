#include "Game.h"
ExplosionType *Slurp::explSlurp = NULL;
Mix_Chunk *Slurp::ploc;
void Slurp::load()
{
	explSlurp = new ExplosionType(20, 20, 255, 100, 100, 200);
	ploc = Mix_LoadWAV("take_bvvv.wav");
}
Slurp::Slurp(Tower *_owner) : SpecialAttack(_owner, &(specialAttackTypes[1]))
{
	//ctor
	length = 10;
	theta = M_PI;
	vl = 4;
	counter = 0;
	if(owner->getType()->cat == ABOMINATION)
	{
		xoff = -7;
		yoff = 30;
		dtheta = 0.02;
	}
	else
	{
		dtheta = 0.02;
		xoff = 0;
		yoff = 0;
	}

}

#define TONGUE_PRE 30
#define TONGUE_RX 16
void Slurp::draw(SDL_Surface *gameLayer)
{
	Sint16 x[TONGUE_PRE], y[TONGUE_PRE];
	float c = cos(theta);
	float s = sin(theta);

	for(int i=1; i<TONGUE_PRE-1; i++)
	{
		float X, Y;
		X = TONGUE_RX*cos(M_PI*(TONGUE_PRE-i-1)/(TONGUE_PRE-1));
		Y = length*sin(M_PI*(TONGUE_PRE-i-1)/(TONGUE_PRE-1));
		x[i] = xoff+owner->x/P+X*c+Y*s;
		y[i] = yoff+owner->y/P+X*s-Y*c;
	}
	char sign = (theta > M_PI/2 && theta < 3*M_PI/2 ? -1 : 1);
	x[0] = xoff+owner->x/P-sign*TONGUE_RX;
	x[TONGUE_PRE-1] = xoff+owner->x/P+sign*TONGUE_RX;
	y[0] = yoff+owner->y/P;
	y[TONGUE_PRE-1] = yoff+owner->y/P;
	filledPolygonRGBA(gameLayer, x, y, TONGUE_PRE, 255, 105, 80, 255);
	polygonRGBA(gameLayer, x, y, TONGUE_PRE, 0, 0, 0, 255);
	lineRGBA(gameLayer, xoff+owner->x/P, yoff+owner->y/P, xoff+owner->x/P+length*s, yoff+owner->y/P-length*c, 176, 48, 96, 255);

	for(int i=0; i<bvvvs.size(); i++)
	{
		filledCircleRGBA(gameLayer, bvvvs[i].x/P, bvvvs[i].y/P, bvvvs[i].r, 50, 50, 200, 255);
	}

}

bool Slurp::update()
{
	counter++;
	if(owner->getType()->cat != ABOMINATION && counter > 1000) return true;
	theta += dtheta;
	if(theta > 2*M_PI) theta -= 2*M_PI;
	if(counter % 4 == 0) vl += 2*(rand() % 2)-1;
	length += vl;
	if(length <= 10)
	{
		length = 10;
		vl++;
	}

	if(length >= 400)
	{
		length = 400;
		vl--;
	}

	vector<Tower *> &towers = (owner->game->towers);
	float ux = sin(theta);
	float uy = -cos(theta);
	for(int i=0; i<towers.size(); i++)
	{
		float h;
		float l;
		if(towers[i]->ownerId == owner->ownerId) continue;
		l = ux*(towers[i]->x/P-owner->x/P)+uy*(towers[i]->y/P-owner->y/P);
		if(l < 0) continue;
		h = sqrtf(owner->distance(towers[i])-l*l);
		if(h < TONGUE_RX*sqrtf(1-l*l/length/length))
		{
			towers[i]->takeDamage(3*dtheta/0.01);
			towers[i]->newExplosion(new Explosion(Slurp::explSlurp, (Thing *) towers[i], 16, 16, -16+rand() % 32, -16+rand() % 32));
		}
	}

	if(rand() % 4800/length == 0)
	{
		Bvvv bvvv;
		int l, v;
		l = rand() % length;
		bvvv.x = xoff*P+owner->x+ux*l*P;
		bvvv.y = yoff*P+owner->y+uy*l*P;
		v = 1+(rand() % (int)(1+l*dtheta));
		bvvv.vx = -uy*v*P;
		bvvv.vy = ux*v*P;
		bvvv.r = 8+rand() % 16;
		bvvvs.push_back(bvvv);
	}

	for(int i=0; i<bvvvs.size(); i++)
	{
		bool toDestroy = false;
		bvvvs[i].x += bvvvs[i].vx;
		bvvvs[i].y += bvvvs[i].vy;
		Tower *t = owner->game->inRange(bvvvs[i].x/P, bvvvs[i].y/P, bvvvs[i].r);
		if(t != NULL)
		{
			if(t->ownerId != owner->ownerId)
			{
				toDestroy = true;
				t->takeDamage(10*bvvvs[i].r);
				playChunk(Slurp::ploc);
				t->newExplosion(new Explosion(Slurp::explSlurp, (Thing *) t, 2*bvvvs[i].r, bvvvs[i].r/2, bvvvs[i].x/P-t->x/P, bvvvs[i].y/P-t->y/P));
			}
		}

		if(toDestroy || bvvvs[i].x/P < -64 || bvvvs[i].x/P > owner->game->map->width()*32+64 || bvvvs[i].y/P < -64 || bvvvs[i].y/P > owner->game->map->width()*32+64)
		{
			bvvvs.erase(bvvvs.begin()+i);
			i--;
		}
	}
	return false;
}
Slurp::~Slurp()
{
	//dtor
}
