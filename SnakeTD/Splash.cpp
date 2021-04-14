#include "Game.h"

Splash::Splash(int ampl, SDL_Rect screenPart)
{
	//ctor
	radius = rand() % (ampl*screenPart.w/100);
	counter = 0;
	x = screenPart.x+(rand() % screenPart.w);
	y = screenPart.y+(rand() % screenPart.h);
}

void Splash::draw(SDL_Surface *screen)
{
	filledCircleRGBA(screen, x, y, (counter <= 20 ? counter*radius/20 : radius-(counter-20)/3), 0, 0, 0, 255);
	//printf("%d\n", (counter <= radius/10 ? counter*10 : radius-(counter-radius/10))/10);
}

bool Splash::update()
{
	counter++;
	if(counter > 3*radius+20) return true;
	return false;
}

