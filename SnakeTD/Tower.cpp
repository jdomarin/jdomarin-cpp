#include "Player.h"
#include "Sprite.h"
#include "Explosion.h"
#include "Game.h"

#include <limits.h>
vector<TowerType> towerTypes;
ExplosionType *deathExpl, *wallExpl;
Mix_Chunk *deathChunk, *newLevelChunk;
vector<SDL_Surface *> *regenAnim;

void loadTowerTypes()
{
	TowerType t;
	t.splash = false;
	t.blind = false;
	t.fluid = false;
	t.ray = false;
	t.alterHeading = false;
	t.drain = false;
	t.damageShields = false;
	t.multipleTargets = false;
	t.preferredAngle = 0;
	t.xoff = 0;
	t.yoff = 0;
	t.cat = HEAD;
	t.life = 1000;
	t.speed = 50;
	t.power = 20;
	t.proj = true;
	t.slow = 0;
	t.slowWpn = 0;
	t.baseLevel = 1;
	t.range = 128;
	t.proj_speed = 20;
	t.towerImage = loadImages("chaton", true);
	t.projImage = loadImages("proj_head", true);
	t.explType = new ExplosionType(255, 0, 0, 255, 255, 0);
	t.fireChunk = Mix_LoadWAV("fire_head.wav");
	t.impactChunk = Mix_LoadWAV("take_generic.wav");
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	t.nextForm = -1;
	towerTypes.push_back(t);

	t.cat = OFFENSIVE;
	t.life = 500;
	t.preferredAngle = -90;
	t.speed = 30; // 30
	t.power = 70; // 100
	t.range = 128;
	t.proj = true;
	t.proj_speed = 20;
	t.towerImage = loadImages("tower_rhinoceros", true);
	t.projImage = loadImages("proj_rhinoceros", true);
	//t.explType = new ExplosionType(255, 0, 0, 255, 255, 0);
	t.explType = new ExplosionType(100, 100, 100, 200, 200, 200);
	t.fireChunk = Mix_LoadWAV("fire_rhinoceros.wav");
	t.impactChunk = Mix_LoadWAV("take_rhinoceros.wav");
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	towerTypes.push_back(t);

	t.cat = OFFENSIVE;
	t.preferredAngle = 0;
	t.life = 300;
	t.speed = 150;
	t.power = 10;
	t.range = 196;
	t.proj_speed = 30;
	t.towerImage = loadImages("tower_std2", true);
	t.projImage = loadImages("proj_std2", true);
	t.explType = new ExplosionType(255, 0, 0, 255, 128, 0);
	t.fireChunk = Mix_LoadWAV("fire_off2.wav");
	t.impactChunk = Mix_LoadWAV("take_generic.wav");
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	towerTypes.push_back(t);

	t.cat = SLOW;
	t.life = 600;
	t.speed = 10;
	t.power = 2;
	t.slow = 80;
	t.slowFactor = 2;
	t.proj_speed = 10;
	t.range = 96;
	t.towerImage = loadImages("tower_slow1", true);
	t.projImage = loadImages("proj_slow1", true);
	t.explType = new ExplosionType(0, 0, 255, 150, 150, 255);
	t.fireChunk = Mix_LoadWAV("fire_slow1.wav");
	t.impactChunk = Mix_LoadWAV("take_slow1.wav");
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	towerTypes.push_back(t);

	t.cat = SHIELD;
	t.slow = 0;
	t.slowFactor = 0;
	t.life = 1000;
	t.speed = 10;
	t.power = 400;
	t.range = 0;
	t.preferredAngle = -90;
	t.proj = false;
	t.towerImage = loadImages("tower_shield", true);
	t.specialAttackTypeId = -1;
	towerTypes.push_back(t);

	t.cat = SLOW_WPN;
	t.life = 400;
	t.speed = 10;
	t.power = 4;
	t.slowWpn = 80;
	t.slowWpnFactor = 2;
	t.preferredAngle = 0;
	t.proj = true;
	t.proj_speed = 40;
	t.range = 96;
	t.preferredAngle = -90;
	t.towerImage = loadImages("tower_slowwpn", true);
	t.projImage = loadImages("proj_slowwpn", true);
	t.explType = new ExplosionType(139, 69, 19, 222, 184, 135);
	t.fireChunk = Mix_LoadWAV("fire_slowwpn1.wav");
	t.impactChunk = Mix_LoadWAV("take_slowwpn1.wav");
	t.followMode = ACCEL;
	t.specialAttackTypeId = -1;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.life = 1000;
	t.slowWpn = 0;
	t.speed = 10;
	t.preferredAngle = 0;
	t.power = 300;
	t.proj = true;
	t.proj_speed = 20;
	t.range = 256;
	t.towerImage = loadImages("tower_lion1", true);
	t.projImage = loadImages("proj_lion", true);
	t.explType = new ExplosionType(255, 0, 0, 255, 100, 0);
	t.fireChunk = Mix_LoadWAV("fire_abomination1.wav");
	t.impactChunk = Mix_LoadWAV("take_generic.wav");
	t.specialAttackTypeId = -1;
	t.followMode = SPEED;
	t.nextForm = 7;
	towerTypes.push_back(t);

	t.life = 5000;
	t.speed = 15;
	t.power = 350;
	t.slowWpnFactor = 4;
	t.slowWpn = 100;
	t.proj_speed = 50;
	t.towerImage = loadImages("tower_lion2", true);
	t.nextForm = 8;
	towerTypes.push_back(t);

	t.life = 10000;
	t.speed = 20;
	t.power = 400;
	t.slow = 100;
	t.slowFactor = 4;
	t.range = 128;
	t.specialAttackTypeId = 1;
	t.towerImage = loadImages("tower_lion3", true);
	t.nextForm = -1;
	towerTypes.push_back(t);

	t.cat = OFFENSIVE;
	t.life = 1000;
	t.power = 300;
	t.baseLevel = 7;
	t.speed = 30;
	t.proj_speed = 10;
	t.slowFactor = 4;
	t.slowWpnFactor = 0;
	t.slowWpn = 0;
	t.slow = 150;
	t.range = 96;
	t.towerImage = loadImages("tower_lombric", true);
	t.projImage = loadImages("proj_lombric", true);
	t.fireChunk = Mix_LoadWAV("fire_lombric.wav");
	t.impactChunk = Mix_LoadWAV("take_lombric.wav");
	t.explType = new ExplosionType(160, 82, 45, 100, 100, 255);
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	towerTypes.push_back(t);

	t.cat = OFFENSIVE;
	t.slow = 0;
	t.life = 2000;
	t.power = 200;
	t.speed = 200;
	t.proj_speed = 40;
	t.slow = 0;
	t.towerImage = loadImages("tower_taupe", true);
	t.projImage = loadImages("proj_taupe", true);
	t.fireChunk = Mix_LoadWAV("fire_taupe.wav");
	t.impactChunk = Mix_LoadWAV("impact_taupe.wav");
	t.preferredAngle = -90;
	t.explType = new ExplosionType(160, 82, 45, 255, 50, 50);
	t.blind = true;
	towerTypes.push_back(t);

	t.towerImage = loadImages("tower_fourmilier", true);
	t.ray = true;
	t.preferredAngle = -90;
	t.life = 1500;
	t.power = 4;
	t.speed = 8000;
	t.rayThickness = 3;
	t.range = 128;
	t.blind = false;
	t.drain = true;
	t.fireChunk = Mix_LoadWAV("firing_fourmilier.wav");
	t.explType = new ExplosionType(100, 100, 200, 100, 100, 255);
	towerTypes.push_back(t);

	t.towerImage = loadImages("tower_scarabe", true);
	t.projImage = loadImages("proj_scarabe", true);
	t.life = 3000;
	t.power = 70;
	t.speed = 50;
	t.proj_speed = 40;
	t.preferredAngle = -90;
	t.range = 196;
	t.ray = false;
	t.fireChunk = Mix_LoadWAV("fire_rhinoceros.wav");
	t.impactChunk = Mix_LoadWAV("take_scarabe.wav");
	t.drain = false;
	t.damageShields = true;
	t.explType = new ExplosionType(160, 82, 45, 100, 100, 255);
	towerTypes.push_back(t);

	t.towerImage = loadImages("tower_lapin", true);
	t.projImage = loadImages("proj_lapin", true);
	t.life = 2000;
	t.power = 50;
	t.preferredAngle = -90;
	t.speed = 20;
	t.range = 128;
	t.proj_speed = 15;
	t.fireChunk = Mix_LoadWAV("fire_lapin.wav");
	t.impactChunk = Mix_LoadWAV("take_lapin.wav");
	t.damageShields = false;
	t.alterHeading = true;
	t.explType = new ExplosionType(255, 165, 0, 255, 140, 0);
	towerTypes.push_back(t);

	t.life = 400;
	t.power = 30;
	t.speed = 50;
	t.proj_speed = 10;
	t.alterHeading = false;
	t.preferredAngle = 0;
	t.range = 96;
	t.towerImage = loadImages("tower_fourmi", true);
	t.explType = new ExplosionType(255, 0, 0, 255, 128, 0);
	t.projImage = loadImages("proj_std2", true);
	t.fireChunk = Mix_LoadWAV("fire_off2.wav");
	t.impactChunk = Mix_LoadWAV("take_generic.wav");
	t.specialAttackTypeId = -1;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.life = 10000;
	t.splash = false;
	t.fluid = true;
	t.fluidDensity = 20;
	t.slow = 0;
	t.fluidType = new Fluid(50, 100, 50, 200, 255, 200);
	t.explType = new ExplosionType(0, 0, 200, 100, 255, 100);
	t.power = 1;
	t.speed = 8000;
	t.range = 256;
	t.slowWpn = 0;
	t.proj = true;
	t.proj_speed = 30;
	t.fireChunk = Mix_LoadWAV("firing_reine2.wav");
	t.impactChunk = Mix_LoadWAV("take_bvvv.wav");
	t.towerImage = loadImages("tower_reine1", true);
	t.specialAttackTypeId = -1;
	t.xoff = 77-48;
	t.nextForm = towerTypes.size()+1;
	t.yoff = 73-48;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.life = 15000;
	t.splash = false;
	t.fluid = true;
	t.fluidDensity = 30;
	t.slow = 0;
	t.fluidType = new Fluid(50, 100, 50, 200, 255, 200);
	t.power = 1;
	t.speed = 8000;
	t.range = 256;
	t.slowWpn = 0;
	t.proj = true;
	t.proj_speed = 30;
	t.towerImage = loadImages("tower_reine2", true);
	t.nextForm = towerTypes.size()+1;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.life = 20000;
	t.splash = false;
	t.fluid = true;
	t.fluidDensity = 50;
	t.slow = 0;
	t.fluidType = new Fluid(100, 50, 50, 255, 255, 200);
	t.power = 1;
	t.speed = 8000;
	t.range = 256;
	t.slowWpn = 0;
	t.proj = true;
	t.proj_speed = 30;
	t.towerImage = loadImages("tower_reine3", true);
	t.specialAttackTypeId = 3;
	t.nextForm = -1;
	towerTypes.push_back(t);

	t.life = 2000;
	t.cat = OFFENSIVE;
	t.range = 128;
	t.specialAttackTypeId = -1;
	t.baseLevel = 14;
	t.proj_speed = 30;
	t.towerImage = loadImages("tower_plancton", true);
	t.fluidType = new Fluid(100, 100, 200, 200, 200, 255);
	t.explType = new ExplosionType(200, 20, 20, 100, 100, 255);
	t.power = 6;
	t.fireChunk = Mix_LoadWAV("firing_plancton.wav");
	t.fluidDensity = 5;
	t.drain = true;
	t.xoff = 0;
	t.power = 1;
	t.followMode = SPEED;
	t.yoff = 0;
	towerTypes.push_back(t);

	t.life = 6000;
	t.towerImage = loadImages("tower_homard", true);
	t.projImage = loadImages("proj_homard", true);
	t.fireChunk = Mix_LoadWAV("fire_homard.wav");
	t.impactChunk = Mix_LoadWAV("take_homard.wav");
	t.fluid = false;
	t.drain = false;
	t.explType = new ExplosionType(200, 200, 20, 100, 100, 255);
	t.power = 130;
	t.speed = 100;
	t.range = 96;
	t.followMode = NONE;
	t.proj_speed = 40;
	t.preferredAngle = -90;
	towerTypes.push_back(t);

	t.towerImage = loadImages("tower_poisson", true);
	t.projImage = loadImages("proj_poisson", true);
	t.life = 3000;
	t.power = 650;
	t.speed = 20;
	t.proj_speed = 30;
	t.range = 128;
	t.preferredAngle = 90;
	t.fireChunk = Mix_LoadWAV("fire_off1.wav");
	t.impactChunk = Mix_LoadWAV("take_off1.wav");
	t.explType = new ExplosionType(255, 0, 0, 255, 128, 0);
	t.followMode = ACCEL;
	towerTypes.push_back(t);

	t.towerImage = loadImages("tower_anguille", true);
	t.life = 3000;
	t.power = 10;
	t.followMode = NONE;
	t.ray = true;
	t.rayThickness = 3;
	t.range = 128;
	t.fireChunk = Mix_LoadWAV("firing_anguille.wav");
	t.speed = 8000;
	t.explType = new ExplosionType(255, 255, 0, 100, 100, 0);
	t.ray_r = 200; t.ray_g = 200; t.ray_b = 50;
	t.multipleTargets = true;
	t.preferredAngle = 0;
	towerTypes.push_back(t);

	t.ray = false;
	t.speed = 10;
	t.multipleTargets = false;
	t.proj = false;
	t.towerImage = loadImages("tower_bernard", true);
	t.preferredAngle = -90;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.preferredAngle = 0;
	t.life = 30000;
	t.splash = true;
	t.proj = true;
	t.fluid = false;
	t.power = 400;
	t.speed =  30;
	t.proj = true;
	t.proj_speed = 50;
	t.range = 256;
	t.preferredAngle = 0;
	t.towerImage = loadImages("tower_poulpe1", true);
	t.projImage = loadImages("proj_poulpe", true);
	t.explType = new ExplosionType(255, 255, 255, 155, 155, 155);
	t.fireChunk = Mix_LoadWAV("fire_poulpe.wav");
	t.impactChunk = Mix_LoadWAV("take_poulpe.wav");
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	t.nextForm = towerTypes.size()+1;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.life = 25000;
	t.power = 400;
	t.speed =  50;
	t.proj = true;
	t.proj_speed = 50;
	t.range = 256;
	t.towerImage = loadImages("tower_poulpe2", true);
	t.projImage = loadImages("proj_poulpe", true);
	t.specialAttackTypeId = -1;
	t.followMode = NONE;
	t.nextForm = towerTypes.size()+1;
	towerTypes.push_back(t);

	t.cat = ABOMINATION;
	t.life = 50000;
	t.splash = true;
	t.proj = true;
	t.fluid = false;
	t.power = 400;
	t.speed = 70;
	t.proj = true;
	t.proj_speed = 50;
	t.range = 256;
	t.towerImage = loadImages("tower_poulpe3", true);
	t.projImage = loadImages("proj_poulpe", true);
	t.specialAttackTypeId = 2;
	t.followMode = NONE;
	t.nextForm = -1;
	towerTypes.push_back(t);

	deathExpl = new ExplosionType(0, 0, 0, 255, 255, 255);
	wallExpl = new ExplosionType(255, 255, 0, 150, 150, 150);

	deathChunk = Mix_LoadWAV("death.wav");
	newLevelChunk = Mix_LoadWAV("newlevel.wav");
	regenAnim = loadAnimation("anim_regen");
}


Tower::Tower(Game *_game, int _id, int ix, int iy, char _deltax, char _deltay, Tower *_next, TowerType *_type, int _ownerId, bool _appeared)
{
	game = _game;
	id = _id;
	x = ix*P; y = iy*P;
	ownerId = _ownerId;
	type = _type;
	next = _next;
	towerSprite = new Sprite(type->towerImage, 0, 1, 1, 20, true);
	ox = (ix-16)/32; oy = (iy-16)/32;
	dx = ox + _deltax; dy = oy + _deltay;
	deltax = _deltax; deltay = _deltay;

	if(ownerId == -1)
	{
		theta = type->preferredAngle;
		otheta = theta;
		dtheta = theta;
	}
	else
	{
		if(deltax == 0 && deltay == 1) theta = 180;
		if(deltax == 0 && deltay == -1) theta = 0;
		if(deltax == 1 && deltay == 0) theta = -90;
		if(deltax == -0 && deltay == 0) theta = 90;
		dtheta = theta;
		otheta = theta;
	}
	towerSprite->setAngle(theta);
	towerSprite->x = x/P;
	towerSprite->y = y/P;
	life = type->life;
	speed = type->speed;
	power = type->power;
	slow = type->slow;
	slowWpn = type->slowWpn;
	level = type->baseLevel;
	reload = 0;
	maxLife = life;
	toAttach = false;
	nextRegen = 0;
	exp = 0;
	expNextLevel = BASE_EXP*pow(1.35, level-1);
	lvlLife = 0; lvlPower = 0; lvlSpeed = 0;
	shieldsEnabled = false;
	slowedWpn = 0;
	toDelete = false;
	if(_appeared) appearedCounter = 50;
	else appearedCounter = 0;
	specialAttack = NULL;
	specialAttackAvailableId = type->specialAttackTypeId;
	if(specialAttackAvailableId != -1) engageSpecialAttack();
	isFiring = false;
}

Tower * Tower::onBlock(vector<Tower *> *towers, int x, int y)
{
	for(int i=0; i<(*towers).size(); i++)
	{
		if((*towers)[i]->id == id) continue;
		if((*towers)[i]->next != NULL) if((*towers)[i]->next->id == id) continue;
		if(next != NULL) if(next->id == (*towers)[i]->id) continue;
		//if((*towers)[i]->getOx() == x && (*towers)[i]->getOy() == y) return (*towers)[i];
		if((*towers)[i]->getType()->cat == ABOMINATION && abs((*towers)[i]->getDx()-x) <= 1 && abs((*towers)[i]->getDy()-y) <= 1) return (*towers)[i];
		if((*towers)[i]->getDx() == x && (*towers)[i]->getDy() == y) return (*towers)[i];
	}
	return NULL;
}

void Tower::updateTheta()
{
	theta += (dtheta-otheta)*0.2;
	if(otheta != dtheta)
	{
		if((theta-dtheta)*(dtheta-otheta) >= 0)
		{
			theta = dtheta;
			otheta = dtheta;
		}
		towerSprite->setAngle(theta);
	}
}

bool Tower::updatePos(bool move, bool *newTower)
{
	int aox, aoy;
	Map *map = game->map;
	vector<Tower *> *towers = &(game->towers);
	if((map->blockType(ox, oy)->regen))
	{
		if(life < maxLife) life += maxLife/1000+20;
		if(owner->shields != 0 && owner->shieldsLoad > 0)
		{
			owner->shieldsLoad -= 10+owner->shieldsLoad/1000;
			if(owner->shieldsLoad < 0) owner->shieldsLoad = 0;
		}
		if(life >= maxLife) fullLife();
		if(owner->speed() < 16) owner->regenSpeed();
		if(slowWpn > 0)
		{
			slowWpn -= 20+slowWpn/1000;
			if(slowWpn < 0) slowWpn = 0;
		}
	}

	if ( move && ownerId != -1 && (((dx == ox) && (dy == oy)) || (x == P*(dx*32+16) && y == P*(dy*32+16))) )
	{
		if(owner->bonusRegen != 0 && life < maxLife && life > 0)
			regen(maxLife*(float)owner->bonusRegen/BONUS_REGEN_FACTOR);

		aox = ox; aoy = oy;
		if(next != NULL) next->heading(dx-ox, dy-oy);

		if(toAttach)
		{
			toAttach = false;
			next->setAttached(ownerId, owner, ox*32+16, oy*32+16, dx-ox, dy-oy);
		}
		ox = dx; oy = dy;
		//x = dx+0.5;
		//y = dy+0.5;
		dx = ox+deltax; dy = oy+deltay;
		if(map->blockType(dx, dy)->regen)
		{
			towerSprite->playAnimation(new Animation(regenAnim, 0, 0, 2));
		}

		if(type->cat == HEAD)
		{
			if(! map->blockType(dx, dy)->pass) move = false;
			else if(life > 0)
			{
				Tower *t = onBlock(towers, dx, dy);
				if(t != NULL)
				{
					if(t->ownerId == -1 && t->life == 0)
					{
						TowerType *ty = t->getType();
						Tower *eq = owner->getHead()->getEq(ty);
						if(eq != NULL)
						{
	/*						if(eq->level < t->level-1)
							{
								eq->gainLevels(t->level-eq->level/2);
							}
							else */
							owner->getHead()->globalGainExp(400*powf(1.35, t->level)/owner->getHead()->nChilds());
							t->toDelete = true;
						}
						else
						{
							if(t->getType()->cat == ABOMINATION)
							{
								specialAttackAvailableId = t->getType()->specialAttackTypeId;
								game->launchNextLevel();
							}
							else
								attachTower(t);
						}
						(*newTower) = true;
					}
					else if(t->ownerId == ownerId)
					{
						globalDamage(maxLife/5);
					}
					else move = false;
				}

				int bonusId;
				bonusId = game->getBonus(dx, dy);
				if(bonusId != -1)
				{
					switch(game->bonuses[bonusId]->type)
					{
						case BONUS_ATK: owner->bonusAtk++; break;
						case BONUS_EXP: owner->bonusExp++; break;
						case BONUS_REGEN: owner->bonusRegen++; break;
						case BONUS_SPECATK: specialAttackAvailableId = game->bonuses[bonusId]->specAtkId; break;
					}
					delete game->bonuses[bonusId];
					game->bonuses.erase(game->bonuses.begin()+bonusId);
				}
			}
			if(! move)
			{
				takeDamage(maxLife/50);//, (dx*32+16)*P, (dy*32+16)*P);
				playContinuousDamageAnimation();
				dx = ox; dy = oy;
				ox = aox; oy = aoy;
			}
			else
				game->notOccupied(aox, aoy);

		}
		updateDTheta();

		if(type->cat != HEAD)
		{
			//heading(nextDx, nextDy);
		}

	}
	if(move)
	{
		x += (dx-ox)*owner->speed();
		y += (dy-oy)*owner->speed();
		if(owner->speed() == 32)
		{
			if(rand() % 20 == 0)
				takeDamage(life/10);//, x+P*(-16+(rand() % 32)), y+P*(-16+(rand() % 32)));
			playContinuousDamageAnimation();
		}
	}

	return move;
}

void Tower::update(bool move, bool *newTower)
{
	bool nmove;
	updateTheta();

	if(ownerId != -1) nmove = updatePos(move, newTower);

	game->occupied(this, dx, dy);
	game->occupied(this, ox, oy);

	towerSprite->x = x/P;
	towerSprite->y = y/P;
	updateFire();
	updateProjectiles();
	if(next != NULL) next->update(nmove, newTower);

	if(ownerId != -1 && life < maxLife)
	{
		if(nextRegen <= 0)
		{
			life += maxLife/300;
			if(life >= maxLife) fullLife();
			nextRegen += 10;
		}
		else nextRegen--;
	}
	if(appearedCounter > 0) appearedCounter--;

	if(specialAttack != NULL)
	{
		if(specialAttack->update())
		{
			delete specialAttack;
			specialAttack = NULL;
		}
	}
}

void Tower::updateDTheta()
{
	if(deltax == 1)
		dtheta = (otheta != 180 ? (otheta != 270 ? -90 : 270) : 270);
	else if(deltax == -1)
	{
		dtheta = 90;
		if(otheta == 270)
		{
			otheta -= 360;
			theta -= 360;
		}
	}

	if(deltay == 1)
	{
		dtheta = 180;
		if(otheta == -90)
		{
			otheta += 360;
			theta += 360;
		}
	}

		else if(deltay == -1)
	{
		dtheta = 0;
		if(otheta == 270)
		{
			otheta -= 360;
			theta -= 360;
		}
	}
}

void Tower::draw(SDL_Surface *gameLayer)
{
	towerSprite->blit(gameLayer);
	if(type->cat == HEAD) rectangleRGBA(gameLayer, 32*dx, 32*dy, 32*(dx+1)-1, 32*(dy+1)-1, owner->c[0], owner->c[1], owner->c[2], 0xff);
	if(ownerId != -1) if(owner->shields > 0)
	{
		float s = owner->shieldsLoadR();
		rectangleRGBA(gameLayer, x/P-16, y/P-16, x/P+31-16, y/P+31-16, 255*s, 0, 255*(1-s), 255);
	}

	if(life <= 0 && ownerId != -1) rectangleRGBA(gameLayer, x/P-16, y/P-16, x/P+15, y/P+15, 0x00, 0x00, 0x00, 0xff);
	if(life <= 0 && ownerId == -1)
	{
		if(brokenAnimationCounter <= 5)
		{
			if(type->cat == ABOMINATION)
			{
				rectangleRGBA(gameLayer, x/P-48, y/P-48, x/P+47, y/P+47, 0xff, 0x44, 0x44, 0xff);
			}
			else
			{
				rectangleRGBA(gameLayer, x/P-16, y/P-16, x/P+15, y/P+15, 0xff, 0x44, 0x44, 0xff);
			}
		}
		else
		{
			if(type->cat == ABOMINATION)
			{
				rectangleRGBA(gameLayer, x/P-48, y/P-48, x/P+47, y/P+47, 0xff, 0xff, 0xff, 0xff);
			}
			else
			{
				rectangleRGBA(gameLayer, x/P-16, y/P-16, x/P+15, y/P+15, 0xff, 0xff, 0xff, 0xff);
			}
		}
		brokenAnimationCounter++;
		if(brokenAnimationCounter >= 10) brokenAnimationCounter = 0;
	}
	//for(int i=0; i<projectiles.size(); i++) projectiles[i]->draw(gameLayer);
	for(int i=0; i<explosions.size(); i++)
	{
		if(explosions[i]->draw(gameLayer))
		{
			delete explosions[i];
			explosions.erase(explosions.begin()+i);
			i--;
		}
	}
	if(slowedWpn > 0) filledRectangleRGB(gameLayer, x/P+12, y/P-12, x/P+14, y/P-10, 200, 10, 10);
}

void Tower::drawInfo(SDL_Surface *infoLayer)
{
	char message[3];
	if(type->cat == ABOMINATION)
	{
		// On affiche juste la vie
		filledRectangleRGB(infoLayer, x/P+32+10, y/P+32+15-30*life/maxLife, x/P+32+14, y/P+32+15, 255*(1-((float)life)/maxLife), 255*((float)life)/maxLife, 127*((float)life)/maxLife);
		filledRectangleRGB(infoLayer, x/P+32+10, y/P+32-15-2, x/P+32+14, y/P+32+15-30*life/maxLife, 255, 255, 255);
		rectangleRGBA(infoLayer, x/P+32+9, y/P+32-15-1, x/P+32+14, y/P+32+15, 0x00, 0x00, 0x00, 0xff);
		return;
	}
	sprintf(message, "%d", level);
	stringRGBA(infoLayer, x/P-16, y/P-16, message, 0xff, 0xff, 0xff, 0xff);
	// Affichage de la vie
	filledRectangleRGB(infoLayer, x/P+12, y/P+15-15*life/maxLife, x/P+14, y/P+15, 255*(1-((float)life)/maxLife), 255*((float)life)/maxLife, 127*((float)life)/maxLife);
	filledRectangleRGB(infoLayer, x/P+12, y/P-2, x/P+14, y/P+15-15*life/maxLife, 255, 255, 255);
	rectangleRGBA(infoLayer, x/P+11, y/P-1, x/P+14, y/P+15, 0x00, 0x00, 0x00, 0xff);

	// Répartition des bonus
	if(level > 1)
	{
		int tot = lvlLife+lvlPower+lvlSpeed;
		int xPower, xSpeed;
		xPower = 20*((float)lvlLife)/(tot);
		xSpeed = 20*((float)lvlLife+lvlPower)/(tot);
		filledRectangleRGB(infoLayer, x/P-10, y/P+7, x/P-10+xPower, y/P+9, 100, 255, 100);
		filledRectangleRGB(infoLayer, x/P-10+xPower, y/P+7, x/P-10+xSpeed, y/P+9, 255, 150, 100);
		filledRectangleRGB(infoLayer, x/P-10+xSpeed, y/P+7, x/P-10+20, y/P+9, 100, 150, 255);
		rectangleRGBA(infoLayer, x/P-11, y/P+6, x/P+10, y/P+9, 0, 0, 0, 255);
	}

	// Affichage de la barre d'expérience
	filledRectangleRGB(infoLayer, x/P-10, y/P+10, x/P-10+20*((float)exp/expNextLevel), y/P+12, 100, 100, 255);
	filledRectangleRGB(infoLayer, x/P-10+20*((float)exp/expNextLevel), y/P+10, x/P+10, y/P+12, 255, 255, 255);
	rectangleRGBA(infoLayer, x/P-11, y/P+9, x/P+10, y/P+12, 0, 0, 0, 255);
}

void Tower::drawProjectiles(SDL_Surface *gameLayer)
{
	for(int i=0; i<projectiles.size(); i++)
		projectiles[i]->draw(gameLayer);
}

void Tower::drawSpecialAttack(SDL_Surface *gameLayer)
{
	if(specialAttack != NULL) specialAttack->draw(gameLayer);
}

void Tower::playContinuousDamageAnimation()
{
	if(rand() % 4 == 0) explosions.push_back(new Explosion(wallExpl, (Thing *) this, 4, 8, - 16 + rand() % 32, -16 + rand() % 32));
}


void Tower::setAttached(int _ownerId, Player *_owner, int _x, int _y, char _deltax, char _deltay)
{
	ownerId = _ownerId; x = P*_x; y = P*_y; deltax = _deltax; deltay = _deltay;
	owner = _owner;
	ox = (_x-16)/32; oy = (_y-16)/32;
	dx = ox + deltax; dy = oy + deltay;
	fullLife();
	updateDTheta();
}

void Tower::attachTower(Tower *t)
{
	if(next != NULL)
	{
		next->attachTower(t);
	}
	else
	{
		next = t;
		toAttach = true;
	}
}

void Tower::printPos(SDL_Surface *screen, int pos)
{
	char message[100];
	sprintf(message, "%d - %d | %d - %d | %d - %d | %d/%d | %d |R : %d - P : %d", x, y, ox, oy, dx, dy, deltax, deltay, toAttach, reload, projectiles.size());
	stringRGBA(screen, 10, pos, message, 0xff, 0xff, 0xff, 0xff);
	if(next != NULL) next->printPos(screen, pos+20);
}

int Tower::distance(Tower *t)
{
	return (t->x/P-x/P)*(t->x/P-x/P)+(t->y/P-y/P)*(t->y/P-y/P);
}

void Tower::updateFire()
{
	int min_distance = INT_MAX;
	int dist;
	vector<Tower *> targets;
	Tower *curTarget = NULL;
	if(type->cat == SHIELD || !type->proj) return;
	if(reload <= 0 && life > 0)
	{
		for(int i=0; i<game->towers.size(); i++)
		{
			if(game->towers[i]->ownerId == ownerId) continue;
			if(ownerId != -1) if(owner->friendly && game->towers[i]->ownerId != -1) continue;
			dist = distance(game->towers[i]);
			if(game->towers[i]->life > 0 &&  dist < type->range*type->range && (type->cat == OFFENSIVE || type->cat == HEAD || type->cat == SLOW_WPN || (game->towers[i]->ownerId != -1)))
			{
				if(type->multipleTargets) targets.push_back(game->towers[i]);
				if(dist < min_distance)
				{
					curTarget = game->towers[i];
					min_distance = dist;
				}
			}
		}
		if(curTarget != NULL && ! type->multipleTargets) targets.push_back(curTarget);

		for(int i=0; i<targets.size(); i++)
		{
			fire(targets[i]);
			if(slowedWpn <= 0)
			{
				reload = 1000/speed;
			} else reload = slowedWpnFactor*1000/speed;
		}

		if((type->fluid || type->ray) && targets.size() == 0)
			haltSound();
	}
	else
	{
		reload--;
	}
	if(slowedWpn > 0) slowedWpn--;
	if(slowedWpn == 0) slowedWpnFactor = 0;
}

void Tower::updateProjectiles()
{
	for(int i=0; i<projectiles.size(); i++)
	{
		if(projectiles[i]->update(game->map, &(game->towers)))
		{
			delete projectiles[i];
			projectiles.erase(projectiles.begin()+i);
		}
	}
}

void Tower::fire(Tower *target)
{
	if(!type->fluid)
		projectiles.push_back(new Projectile(game->map, this, target));
	else for(int j=0; j<type->fluidDensity; j++) projectiles.push_back(new Projectile(game->map, this, target));

	if(type->ray || type->fluid)
	{
		if(! isFiring)
		{
			isFiring = true;
			soundChannel = Mix_PlayChannel(-1, type->fireChunk, -1);
		}
	}
	else
		playChunk(type->fireChunk);
}

void Tower::takeDamage(int damage)
{
	//if(ownerId == -1) damage /= sqrt(level);
	if(life == 0) return;
	if(type->cat == SHIELD)
	{
		gainExp(damage);
		//if(ownerId != -1) printf("Bouclier : %d - %d - %d\n", (shieldsEnabled == true ? 1 : 0), owner->shields, owner->shieldsLoad);
	}
	if(ownerId != -1) if(owner->shields > 0)
	{
		owner->takeDamageOnShields(1+damage/4);
		damage = 1+damage/SHIELD_FACTOR(owner->shields);
	}

	/*if(type->cat == ABOMINATION)
	{
		if(life-damage <= maxLife && life > maxLife/2 && life > 0)
		{
			if(specialAttackAvailableId != -1) engageSpecialAttack();
			towerSprite->setMode(2);
		}
		if(life-damage <= 2*maxLife/3 && life > 2*maxLife/3)
		{
			speed *= 10;
			towerSprite->setMode(1);
		}
		if(life-damage <= maxLife/3 && life > maxLife/3)
		{
			speed *= 4;
		}
	}*/
	life -= damage;
	nextRegen+=2;
	if(life <= 0)
	{
		if(! (game->map->blockType(ox, oy)->regen || game->map->blockType(dx, dy)->regen))
		{
			playChunk(deathChunk);
			die();
		}
		else
			life = 1;
	}
	//towerSprite->playAnimation(new Animation(expl, (ix-x)/2/P, (iy-y)/2/P, 30*(1-expf(-(float)damage/100))));
}

void Tower::doSlowWpn(int factor, int dmg)
{
	if(factor >= slowedWpnFactor)
	{
		slowedWpnFactor = factor;
		slowedWpn += dmg;
		if(slowedWpn > 500)
			slowedWpn = 500;
	}
}

void Tower::die()
{
	if(game->map->blockType(ox, oy)->regen)
	{
		life = 1;
		return;
	}
	life = 0;
	haltSound();
	explosions.push_back(new Explosion(deathExpl, (Thing *) this, 32, 25));
	if(type->cat == HEAD)
	{
		//owner->reduceSpeed(2, 100);
		owner->bonusAtk = 0;
		owner->bonusExp = 0;
		owner->bonusRegen = 0;
		if(next != NULL) next->globalDie();
		game->newBonus(x/P/32, y/P/32);
	}

	brokenAnimationCounter = 0;

	if(type->cat == HEAD) nextRegen = 500;
	 else nextRegen = 1000;

	if(type->cat == SHIELD && ownerId != -1 && shieldsEnabled)
	{
		owner->shields -= power;
		shieldsEnabled = false;
	}

	endSpecialAttack();
	if((type->cat == ABOMINATION && type->nextForm != -1) || type == &(towerTypes[TOWER_FOURMI]))
	{
		toDelete = true;
	}
}

void Tower::fullLife()
{
	life = maxLife;
	if(ownerId != -1 && type->cat == SHIELD && !shieldsEnabled)
	{
		owner->shields += power;
		shieldsEnabled = true;
	}
}

void Tower::globalDamage(int damage)
{
	takeDamage(damage);//, x, y);
	if(next != NULL) next->globalDamage(damage);
}

void Tower::globalDie()
{
	game->newBonus(x/P/32, y/P/32);
	die();
	if(next != NULL) next->globalDie();
}

void Tower::gainExp(int n, bool bonusExp)
{
	if(level >= game->maxLevel) return;
	if(type->drain) n /= 3;
	if(bonusExp) n = n*game->bonusExp/100;
	if(ownerId == -1) return;
	exp += (ownerId == -1 ? n/3 : n*(1.0+(float)owner->bonusExp/BONUS_EXP_FACTOR));
	if(exp >= expNextLevel)
	{
		exp = exp % expNextLevel;
		towerSprite->playAnimation(new Animation(newLevelAnim, 0, 0, 2));
		playChunk(newLevelChunk);
		nextLevel();
	}
}

void Tower::nextLevel()
{
	if(level >= game->maxLevel) return;
	if(type->cat == SHIELD && ownerId != -1 && shieldsEnabled)
	{
		owner->shields -= power;
		shieldsEnabled = false;
	}

	level++;
	expNextLevel = BASE_EXP*pow(1.35, level-1);

	if(type->cat == HEAD)
	{
		maxLife *= 1.2;
		power *= 1.1;
		lvlLife+=2;
		lvlPower+=1;
		fullLife();
		return;
	}

	int n = rand() % 4;
	if(n == 3 && rand() % 2 == 0) n = 2; // Si on augmente trop souvent la vitesse, ça devient vite n'importe quoi
	if(speed >= 1000 && n == 3) n = 2;
	if(type->cat == SHIELD && n == 3) n = 2;
	if(type->cat == SLOW)
	{
		n = rand() % 4;
		if(n == 2) n = 4;
	}
	if(type->cat == SLOW_WPN)
	{
		n = rand() % 4;
		if(n == 2) n = 5;
	}
	switch(n)
	{
		case 0:
		case 1: maxLife *= 1.3; lvlLife++; break;
		case 2: power *= 1.3; lvlPower++; break;
		case 3: speed *= 1.3; if(speed >= 1000) speed = 1000; lvlSpeed++; break;
		case 4: slow *= 1.3; lvlPower++; break;
		case 5: slowWpn *= 1.3; lvlPower++; break;
	}
	fullLife();
	//if(level > 20) expNextLevel = INT_MAX;
}

void Tower::gainLevels(int n)
{
	for(int i=0; i<n; i++) nextLevel();
}

bool Tower::shieldsReloading()
{
	if(type->cat == SHIELD)
	{
		if(game->map->blockType(ox, oy)->regen)
			return true;
	}
	if(next != NULL) return next->shieldsReloading();
	else return false;
}
void Tower::destroyShieldGenerators()
{
	if(type->cat == SHIELD && life > 0) die();
	if(next != NULL)
	{
		next->destroyShieldGenerators();
	}
}

void Tower::engageSpecialAttack()
{
	if(specialAttackAvailableId == -1 || specialAttack != NULL)
	{
		// Play(pas content)
	}
	switch(specialAttackAvailableId)
	{
		case 0: specialAttack = (SpecialAttack *) new Fury(this); break;
		case 1: specialAttack = (SpecialAttack *) new Slurp(this); break;
		case 2: specialAttack = (SpecialAttack *) new Poulpofolie(this); break;
		case 3: specialAttack = (SpecialAttack *) new Appel(this); break;
		default: specialAttack = NULL; return;
	}
	game->newSpecialAttack(specialAttack);
	specialAttackAvailableId = -1;
}

void Tower::endSpecialAttack()
{
	if(specialAttack != NULL)
	{
		if(game->drawSpecialAttack == specialAttack) game->drawSpecialAttack = NULL;
		delete specialAttack;
		specialAttack = NULL;
	}
}

void Tower::haltSound()
{
	if(isFiring)
	{
		isFiring = false;
		if(soundChannel != -1) Mix_HaltChannel(soundChannel);
	}
}

Tower::~Tower()
{
	game->notOccupied(ox, oy);
	delete towerSprite;
	haltSound();
	endSpecialAttack();
	for(int i=0; i<game->towers.size(); i++) game->towers[i]->removeTarget(this);
	for(int i=0; i<projectiles.size(); i++) delete projectiles[i];
}

void Tower::removeTarget(Tower *target)
{
	for(int i=0; i<projectiles.size(); i++)
	{
		if(projectiles[i]->target == target)
		{
			if(type->ray)
			{
				delete projectiles[i];
				projectiles.erase(projectiles.begin()+i);
				i--;
			}
			else
			{
				projectiles[i]->target = NULL;
			}
		}
	}
}
