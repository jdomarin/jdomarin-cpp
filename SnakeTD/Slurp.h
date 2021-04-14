#ifndef SLURP_H
#define SLURP_H

#include "SpecialAttack.h"
#include "Explosion.h"

typedef struct
{
	int x, y;
	int vx, vy;
	int r;
} Bvvv;



class Slurp : public SpecialAttack
{
	public:
		Slurp(Tower *owner);
		void draw(SDL_Surface *gameLayer);
		bool update();
		virtual ~Slurp();
		static ExplosionType *explSlurp;

		static void load();

	protected:
	private:
		vector<Bvvv> bvvvs;
		float theta;
		float dtheta;
		static Mix_Chunk *ploc;
		int length;
		int vl;
		int counter;
		int xoff, yoff;
};
#endif // SLURP_H
