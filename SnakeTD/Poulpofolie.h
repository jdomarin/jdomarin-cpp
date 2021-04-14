#ifndef POULPOFOLIE_H
#define POULPOFOLIE_H

#include "SpecialAttack.h"

#define TENTACULE_LENGTH 2400
#define TENTACULE_RX 8

typedef struct
{
	public:
		int length;
		float phi;
		Tower *target;
		int ix1, iy1, ix2, iy2;
		int hitStatus;
		bool hitIncrement;
} Tentacule;

class Poulpofolie : public SpecialAttack
{
	public:
		Poulpofolie(Tower *_owner);
		virtual ~Poulpofolie();
		void draw(SDL_Surface *surface);
		bool update();
		static void load();
		static ExplosionType *explTentacule;
	protected:
	private:
		int side;
		Tentacule tentacules[8];
		static Mix_Chunk *tentaculeChunk;
		void setTentacule(int id, int ix1, int iy1, int ix2, int iy2);
		int xExtremite(int teId);
		int yExtremite(int teId);
		int xRacine(int teId);
		int yRacine(int teId);
		int distance(int teId, Tower *to);
		float angle(int teId, Tower *to);
		int distanceExtremite(int teId, Tower *to);
		int endCounter;
};

#endif // POULPOFOLIE_H
