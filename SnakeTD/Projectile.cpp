#include "Game.h"


Projectile::Projectile(Map *map, Tower *_father, Tower *_target)
{
	float norm;
	float ivx, ivy, t, nx, ny;
	bool normal = true;
	toExplode = false; toDestroy = false;
	father = _father;
	target = _target;
	x = father->x+father->getType()->xoff*P; y = father->y+father->getType()->yoff*P;

	if(! father->getType()->ray)
	{
		if(father->getType()->blind)
		{
			float theta = (2*M_PI*rand())/RAND_MAX;
			vx = father->getType()->proj_speed*cos(theta);
			vy = father->getType()->proj_speed*sin(theta);
		}
		else
		{
			norm = sqrt((x-target->x)*(x-target->x)+(y-target->y)*(y-target->y));

			if(target->ownerId != -1 && map->blockType(target->getDx()+target->deltax, target->getDy()+target->deltay)->pass)
			{
				normal = false;
				/* Calcul exact ! */
				/* x0 = xt0-yp0 ; y0 = idem ; 0=(x0**2+y0**2)+2*(vx*v0+vy*y0)*t+(vx**2+vy**2-vp**2)*t**2 */
				float delta, x0, y0, tvx, tvy, vp;
				x0 = target->x-x;
				y0 = target->y-y;
				tvx = target->owner->speed()*(target->getDx()-target->getOx());
				tvy = target->owner->speed()*(target->getDy()-target->getOy());
				vp = father->getType()->proj_speed;
				delta = (tvx*x0+tvy*y0)*(tvx*x0+tvy*y0)-(x0*x0+y0*y0)*(tvx*tvx+tvy*tvy-vp*vp);
				if(delta < 0 || tvx*tvx+tvy*tvy == vp*vp)
				{
					normal = true;
				}
				else
				{
					float t1, t2;
					t1 = (-(tvx*x0+tvy*y0)+sqrt(delta))/(tvx*tvx+tvy*tvy-vp*vp);
					t2 = (-(tvx*x0+tvy*y0)-sqrt(delta))/(tvx*tvx+tvy*tvy-vp*vp);
					/*t = (tvx*tvx+tvy*tvy > vp*vp ?
						(-(tvx*x0+tvy*y0)+sqrt(delta))/(tvx*tvx+tvy*tvy-vp*vp)
						: (-(tvx*x0+tvy*y0)-sqrt(delta))/(tvx*tvx+tvy*tvy-vp*vp));*/
					if(t1 > 0 && t2 > 0) t = (t1 > t2 ? t2 : t1);
					else if(t1 < 0) t = t2;
					else t = t1;

					float dx = x0+tvx*t;
					float dy = y0+tvy*t;
					norm = sqrt(dx*dx+dy*dy);
					vx = vp*dx/norm;
					vy = vp*dy/norm;
					//sprite = new Sprite(towerTypes[1].projImage, 0, x/P, y/P, 20, true);
				}
			}
			if(normal)
			{
				vx = (target->x-x)*father->getType()->proj_speed/norm;
				vy = (target->y-y)*father->getType()->proj_speed/norm;
				//sprite = new Sprite(towerTypes[0].projImage, 0, x/P, y/P, 20, true);
			}
		}
		if(father->getType()->fluid)
		{
			x += (rand() % 49)-24;
			y += (rand() % 49)-24;
			vx += (rand() % 7)-3;
			vy += (rand() % 7)-3;
			//color = father->getType()->fluidType->getColor();
			father->getType()->fluidType->getColor(&r, &g, &b);
		}
		else
		{
			sprite = new Sprite(father->getType()->projImage, 0, x/P, y/P, 20, true);
			sprite->setAngle(180+180*atan2(vx, vy)/M_PI);
		}
	}
	explosion = NULL;
	ax = 0;
	ay = 0;
}

void Projectile::updateVel()
{
	float norm;
	norm = sqrt((x-target->x)*(x-target->x)+(y-target->y)*(y-target->y));

	if(father->getType()->followMode == SPEED)
	{
		vx = (target->x-x)*father->getType()->proj_speed/norm;
		vy = (target->y-y)*father->getType()->proj_speed/norm;

	}
	else if(father->getType()->followMode == ACCEL)
	{
		ax = (target->x-x)*father->getType()->proj_speed/20/norm;
		ay = (target->y-y)*father->getType()->proj_speed/20/norm;
		vx += ax;
		vy += ay;
		float v = vx*vx+vy*vy;
		if(v > father->getType()->proj_speed*father->getType()->proj_speed)
		{
			v = sqrt(v);
			vx *= father->getType()->proj_speed/v;
			vy *= father->getType()->proj_speed/v;
		}
	}

	if(!father->getType()->fluid) sprite->setAngle(180+180*atan2(vx, vy)/M_PI);
}

int Projectile::distance(Tower *t)
{
	return (t->x-x)*(t->x-x)+(t->y-y)*(t->y-y);
}

void Projectile::explode(Tower *target)
{
	if(explosion == NULL)
	{
		float factor = (father->ownerId == -1 ? 1 : (1.0+father->owner->bonusAtk/BONUS_ATK_FACTOR));
		int eqPower = (father->slow*father->getType()->slowFactor+father->power+father->slowWpn*father->getType()->slowWpnFactor)*factor;
		//int eqPower = (father->getType()->cat == SLOW || father->getType()->cat == SLOW_WPN ? father->power*5 : father->power);
		if(target != NULL)
		{
			if(! father->getType()->ray) target->newExplosion(new Explosion(father->getType()->explType, (Thing *) target, 2*log2(2+eqPower), 2*log2(2+eqPower), (x-target->x)/P, (y-target->y)/P));
			else if(rand() % 4 == 0) target->newExplosion(new Explosion(father->getType()->explType, (Thing *) target, 2*log2(2+4*eqPower), 2*log2(2+4*eqPower), (rand() %32)-16, (rand() %32)-16));
			toDestroy = true;
		}
		else
		{
			explosion = new Explosion(father->getType()->explType, (Thing *) this, 2*log2(2+eqPower), 2*log2(2+eqPower));
			toExplode = true;
		}
	}

}

bool Projectile::update(Map *map, vector<Tower *> *towers)
{
	if(toDestroy) return true;

	if((! toExplode) && (! map->blockType(x/P/32, y/P/32)->pass))
		explode();

	if(toExplode) // Pas joli : on ne va effacer le tir qu'après la fin de l'explosion (élément purement graphique qui ne devrait pas intervenir dans ces fonctions)
	{
		return false;
	}

	if(father->getType()->fluid && distance(father)/P/P >= father->getType()->range*father->getType()->range) return true;

	Tower *finalTarget = NULL;
	if(! father->getType()->ray)
	{
		if(father->getType()->followMode != NONE && target != NULL) updateVel();
		if(target != NULL)
			if(target->life <= 0) target = NULL;

		x += vx;
		y += vy;
		if(! father->getType()->fluid)
		{
			sprite->x = x/P;
			sprite->y = y/P;
		}

		if(x > 32*map->width()*P || y > 32*map->height()*P | x < 0 | y < 0)
		{
			return true;
		}



		for(int dx=-1; dx<=1; dx++) for(int dy=-1; dy<=1; dy++)
		{
			Tower *t = father->game->onBlockBC(x/P/32+dx, y/P/32+dy);
			if(t == NULL) continue;
			if(t->ownerId == father->ownerId) continue;
			if(father->ownerId != -1) if (father->owner->friendly && t->ownerId != -1) continue;
			if(t->life == 0) continue;
			if(distance(t) <= (24*P)*(24*P) || (t->getType()->cat == ABOMINATION && distance(t) <= (56*P)*(56*P)))
			{
				finalTarget = t;
				break;
			}
		}
	}
	else
		finalTarget = target;

	if(finalTarget != NULL)
	{
		float factor = (father->ownerId == -1 ? 1 : (1.0+father->owner->bonusAtk/BONUS_ATK_FACTOR));
		if(father->slowWpn > 0) finalTarget->doSlowWpn(father->getType()->slowWpnFactor, factor*father->slowWpn);
		if(father->slow > 0 && finalTarget->ownerId != -1) finalTarget->owner->reduceSpeed(father->getType()->slowFactor, factor*father->slow);
		if(father->power > 0) finalTarget->takeDamage(factor*father->power);
		if(father->getType()->splash) finalTarget->owner->newSplash(20);
		if(father->getType()->alterHeading && finalTarget->ownerId != -1)
		{
			int dx, dy;
			getDirection(rand() % 4, dx, dy);
			finalTarget->owner->getHead()->heading(dx, dy);
		}
		if(father->getType()->drain)
		{
			if(father->ownerId == -1) father->regen(father->power*factor/3);
			else father->owner->getHead()->globalRegen(1+father->power*factor/3/father->owner->getHead()->nChilds());
		}
		if(father->getType()->damageShields && finalTarget->ownerId != -1)
			finalTarget->owner->takeDamageOnShields(3*father->power*factor);

		int totalExp = (father->slow*father->getType()->slowFactor+father->power+father->slowWpn*father->getType()->slowWpnFactor/2)*factor;
		if(father->ownerId == -1)
		{
			switch(father->getType()->cat)
			{
				case OFFENSIVE : finalTarget->takeDamage(factor*father->power); break; // Deux fois plus de dommages
				case SLOW : finalTarget->takeDamage(2*factor*father->slow*father->getType()->slowFactor); break;
				case SLOW_WPN : finalTarget->takeDamage(2*factor*father->slowWpn*father->getType()->slowWpnFactor); break;
				default: break;
			}
		}
		//if(father->ownerId == -1)
		//	father->gainExp(totalExp);
		if(father->ownerId != -1)
		{
			father->gainExp(1+totalExp/3, true);
			father->owner->getHead()->globalGainExp(1+2*totalExp/3/father->owner->getHead()->nChilds(), true);
		}
		explode(finalTarget);
		if(!father->getType()->ray) playChunk(father->getType()->impactChunk);
	}
	return false;
}

void Projectile::draw(SDL_Surface *gameLayer)
{
	if(explosion != NULL && toExplode)
	{
		if(explosion->draw(gameLayer))
		{
			delete explosion;
			explosion = NULL;
			toDestroy = true;
		}
	}
	else
	{
		if(father->getType()->fluid) pixelRGBA(gameLayer, x/P, y/P, r, g, b, 255);
		else if(father->getType()->ray) thickLineRGB(gameLayer, x/P, y/P, target->x/P, target->y/P, father->getType()->rayThickness, father->getType()->ray_r, father->getType()->ray_g, father->getType()->ray_b);
		else
			sprite->blit(gameLayer);
	}
}

Projectile::~Projectile()
{
	if(! (father->getType()->ray || father->getType()->fluid)) delete sprite;
}

Fluid::Fluid(unsigned char _iR, unsigned char _iG, unsigned char _iB, unsigned char _oR, unsigned char _oG, unsigned char _oB)
{
	if(_iR > _oR)
	{
		iR = _oR;
		oR = _iR;
	}
	else
	{
		iR = _iR;
		oR = _oR;
	}
	if(_iG > _oG)
	{
		iG = _oG;
		oG = _oG;
	}
	else
	{
		iG = _iG;
		oG = _oG;
	}
	if(_iB > _oB)
	{
		iB = _oB;
		oB = _iB;
	}
	else
	{
		iB = _iB;
		oB = _oB;
	}
}
void Fluid::getColor(unsigned char *r, unsigned char *g, unsigned char *b)
{
	/*return SDL_MapRGB(fmt, iR+r*(iR-oR)/256,
					iG+r*(iG-oG)/256,
					iB+r*(iB-oB)/256);*/
	*r = (iR == oR ? iR : iR+(rand() % (oR-iR)));
	*g = (iG == oG ? iG : iG+(rand() % (oG-iG)));
	*b = (iB == oB ? iB : iB+(rand() % (oB-iB)));
}
