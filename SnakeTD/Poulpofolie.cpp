#include "Game.h"

ExplosionType *Poulpofolie::explTentacule;
Mix_Chunk *Poulpofolie::tentaculeChunk;

void Poulpofolie::load()
{
	Poulpofolie::explTentacule = new ExplosionType(150, 150, 150, 150, 150, 250);
	Poulpofolie::tentaculeChunk = Mix_LoadWAV("take_tentacule.wav");
}

Poulpofolie::Poulpofolie(Tower *_owner) : SpecialAttack(_owner, &(specialAttackTypes[2]))
{
	if(owner->getType()->cat == ABOMINATION)
	{
		side = 96;
		setTentacule(0, 13, 79, 14, 73);
		setTentacule(1, 28, 61, 32, 56);
		setTentacule(2, 34, 46, 34, 41);
		setTentacule(3, 37, 38, 38, 35);
		setTentacule(4, 49, 31, 53, 29);
		setTentacule(5, 70, 43, 73, 45);
		setTentacule(6, 73, 45, 75, 48);
		setTentacule(7, 66, 59, 66, 56);
	}
	else
	{
		side = 0;
		for(int i=0; i<8; i++)
			setTentacule(i, 16*cos(2*M_PI*i/8), 16*sin(2*M_PI*i/8), 16*cos(2*M_PI*(i+1)/8), 16*sin(2*M_PI*(i+1)/8));
	}
	endCounter = 0;
}

void Poulpofolie::setTentacule(int id, int ix1, int iy1, int ix2, int iy2)
{
	tentacules[id].ix1 = ix1-side/2; tentacules[id].iy1 = iy1-side/2;
	tentacules[id].ix2 = ix2-side/2; tentacules[id].iy2 = iy2-side/2;
	tentacules[id].target = NULL;
	tentacules[id].phi = atan2(iy1+iy2, ix1+ix2);
	tentacules[id].hitStatus = 0;
	tentacules[id].length = 100;
	tentacules[id].hitIncrement = true;
}

#define R 70
#define F(hs) (((float)hs*hs)/200-1)
void Poulpofolie::draw(SDL_Surface *gameLayer)
{
	Sint16 x[5];
	Sint16 y[5];
	int fx, fy;
	for(int i=0; i<8; i++)
	{
		x[0] = owner->x/P+tentacules[i].ix1;
		y[0] = owner->y/P+tentacules[i].iy1;
		x[4] = owner->x/P+tentacules[i].ix2;
		y[4] = owner->y/P+tentacules[i].iy2;
		fx = xExtremite(i)/P;
		fy = yExtremite(i)/P;
		x[1] = x[0]+(fx-x[0])*R/100;
		y[1] = y[0]+(fy-y[0])*R/100;
		x[3] = x[4]+(fx-x[4])*R/100;
		y[3] = y[4]+(fy-y[4])*R/100;
		int ix = xRacine(i)/P;
		int iy = yRacine(i)/P;
		float d = sqrtf((fx-ix)*(fx-ix)+(fy-iy)*(fy-iy));
		x[2] = ix+((fx-ix)*R+d*(100-R)*cos(tentacules[i].phi+M_PI/3*(tentacules[i].hitIncrement ? 1 : -1)*F(tentacules[i].hitStatus)))/100;
		y[2] = iy+((fy-iy)*R+d*(100-R)*sin(tentacules[i].phi+M_PI/3*(tentacules[i].hitIncrement ? 1 : -1)*F(tentacules[i].hitStatus)))/100;
		filledPolygonRGBA(gameLayer, x, y, 5, 150, 150, 150, 255);
		polygonRGBA(gameLayer, x, y, 5, 0, 0, 0, 255);
		//filledTrigonRGBA(gameLayer, owner->x/P+tentacules[i].ix1, owner->y/P+tentacules[i].iy1, owner->x/P+tentacules[i].ix2, owner->y/P+tentacules[i].iy2, xExtremite(i)/P, yExtremite(i)/P, 150, 150, 150, 255);
		//trigonRGBA(gameLayer, owner->x/P+tentacules[i].ix1, owner->y/P+tentacules[i].iy1, owner->x/P+tentacules[i].ix2, owner->y/P+tentacules[i].iy2, xExtremite(i)/P, yExtremite(i)/P, 0, 0, 0, 255);
		//lineRGBA(gameLayer, owner->x/P+tentacules[i].ix1, owner->y/P+tentacules[i].iy1, owner->x/P+tentacules[i].ix2, owner->y/P+tentacules[i].iy2, 0, 255, 0, 255);
	}
}

int Poulpofolie::distance(int teId, Tower *to)
{
	int x = P*(tentacules[teId].ix1+tentacules[teId].ix2)/2+owner->x;
	int y = P*(tentacules[teId].iy1+tentacules[teId].iy2)/2+owner->y;
	return (to->x-x)*(to->x-x)+(to->y-y)*(to->y-y);
}

float Poulpofolie::angle(int teId, Tower *to)
{
	int x = P*(tentacules[teId].ix1+tentacules[teId].ix2)/2+owner->x;
	int y = P*(tentacules[teId].iy1+tentacules[teId].iy2)/2+owner->y;
	return atan2(to->y-y, to->x-x);
}

int Poulpofolie::xRacine(int teId)
{
	return P*(tentacules[teId].ix1+tentacules[teId].ix2)/2+owner->x;
}

int Poulpofolie::yRacine(int teId)
{
	return P*(tentacules[teId].iy1+tentacules[teId].iy2)/2+owner->y;
}

int Poulpofolie::xExtremite(int teId)
{
	return P*(tentacules[teId].ix1+tentacules[teId].ix2)/2+tentacules[teId].length*cos(tentacules[teId].phi)+owner->x;
}

int Poulpofolie::yExtremite(int teId)
{
	return P*(tentacules[teId].iy1+tentacules[teId].iy2)/2+tentacules[teId].length*sin(tentacules[teId].phi)+owner->y;
}

int Poulpofolie::distanceExtremite(int teId, Tower *to)
{
	return (to->x-xExtremite(teId))*(to->x-xExtremite(teId))+(to->y-yExtremite(teId))*(to->y-yExtremite(teId));
}

bool Poulpofolie::update()
{
	//vector<bool> taken;
	//for(int i=0; i<owner->game->towers.size(); i++) taken.push_back(false);
	for(int i=0; i<8; i++)
	{
		// Cible hors de vue ou pas de cible : nouvelle cible... ?
		//if(tentacules[i].target == NULL || distance(i, tentacules[i].target) > TENTACULE_LENGTH*TENTACULE_LENGTH)
		if(true) // On prend toujours la cible la plus proche
		{
			int min_dist = TENTACULE_LENGTH*TENTACULE_LENGTH;
			int dist;
			//int iTof;
			Tower *curTarget = NULL;
			for(int iTo=0; iTo<owner->game->towers.size(); iTo++)
			{
				if(owner->game->towers[iTo]->life <= 0 || owner->game->towers[iTo]->ownerId == owner->ownerId) continue;
				dist = distance(i, owner->game->towers[iTo]);
				//if(dist > TENTACULE_LENGTH*TENTACULE_LENGTH) continue;
				if(dist <= min_dist)
				{
					min_dist = dist;
					curTarget = owner->game->towers[iTo];
					//iTof = iTo;
				}
			}

			if(curTarget != NULL)
			{
				tentacules[i].target = curTarget;
				//taken[iTof] = true;
			}
			else
				tentacules[i].target = NULL; // C'est psychologique
		}

		int r;
		float phi;
		if(tentacules[i].target != NULL)
		{
			r = sqrt(distance(i, tentacules[i].target));
			phi = angle(i, tentacules[i].target);
		}
		else
		{
			r = 800;
			phi = atan2(tentacules[i].iy1+tentacules[i].iy2, tentacules[i].ix1+tentacules[i].ix2);
		}

		if(fabs(phi-tentacules[i].phi) > 0.2)
		{
			if((phi > tentacules[i].phi && phi-tentacules[i].phi < M_PI) || (phi < tentacules[i].phi && tentacules[i].phi-phi > M_PI)) tentacules[i].phi += 0.1;
				else
			tentacules[i].phi -= 0.1;
		}
		else
			tentacules[i].phi = phi;

		if(tentacules[i].phi > M_PI) tentacules[i].phi -= 2*M_PI;
		if(tentacules[i].phi < -M_PI) tentacules[i].phi += 2*M_PI;

		if(abs(tentacules[i].length-r) > 48)
		{
			if(tentacules[i].length < r)
				tentacules[i].length += 32;
			if(tentacules[i].length > r)
				tentacules[i].length -= 32;
		}
		else
			tentacules[i].length = r;

		if(tentacules[i].target != NULL) if(distanceExtremite(i, tentacules[i].target) <= (32*P)*(32*P))
		{
			tentacules[i].hitStatus++;
			if(tentacules[i].hitStatus == 14)
			{
				tentacules[i].target->takeDamage(200);
				tentacules[i].target->newExplosion(new Explosion(explTentacule, tentacules[i].target, 10, 10, (xExtremite(i)-tentacules[i].target->x)/P, (yExtremite(i)-tentacules[i].target->y)/P));
				playChunk(Poulpofolie::tentaculeChunk);
			}
			if(tentacules[i].hitStatus == 20)
			{
				tentacules[i].hitIncrement = !tentacules[i].hitIncrement;
				tentacules[i].hitStatus = 0;
			}

		}

		if(tentacules[i].length < 0) tentacules[i].length = 0;
		if(tentacules[i].length > TENTACULE_LENGTH) tentacules[i].length = TENTACULE_LENGTH;
	}

	if(owner->ownerId != -1)
	{
		endCounter++;
		if(endCounter >= 2000) return true;
	}

	return false;
}

Poulpofolie::~Poulpofolie()
{
	//dtor
}
