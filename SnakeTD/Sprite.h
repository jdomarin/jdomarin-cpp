#ifndef SPRITE_H
#define SPRITE_H
#include <iostream>
#include <SDL.h>
#include "SDL_rotozoom.h"
#include "SDL_gfxPrimitives.h"
#include <math.h>
#include <vector>
using namespace std;

#define SPRITE_CENTER

#define DT 10
typedef vector<vector<SDL_Surface *> > ImageList;

vector<SDL_Surface *> *loadAnimation(char *filename);
ImageList *loadImages(char * filename, bool enableRotate=false);

void getPixel(SDL_Surface *surface, int x, int y, unsigned char &r, unsigned char &g, unsigned char &b);

void filledRectangleRGBA(SDL_Surface *screen, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void filledRectangleRGB(SDL_Surface *screen, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b);
SDL_Surface *createSurface(int w, int h, bool colorkey, bool hw);
void getPixel(SDL_Surface *surface, int x, int y, unsigned char &r, unsigned char &g, unsigned char &b);
void thickLineRGB(SDL_Surface *screen, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 thickness, unsigned char r, unsigned char g, unsigned char b);
extern SDL_PixelFormat *fmt;

class Animation // Pour dessiner une animation par dessus un sprite (explosion, ...)
{
	private:
		int x, y; // Relativement au centre du sprite
		vector<SDL_Surface *> *images;
		int wait;
		int waitCounter;
		int index;
	public:
		Animation(vector<SDL_Surface *> *images, int x, int y, int wait);
		bool blit(SDL_Surface *surface, int ix, int iy);
};

class Sprite
{
	private:
		int m_wait;
		unsigned int m_Counter;
		int m_waitCounter;
		ImageList *m_Images;
		ImageList *m_animation;
		int m_mode;
		bool enableRotate;
		vector<Animation *> animations;
		bool loop;
	protected:
		SDL_Rect m_pos;
	public:
		Sprite(ImageList *images, int mode, int x, int y, int wait, bool enableRotate=false);
		Sprite(SDL_Surface * im, int x, int y);
		virtual ~Sprite();

		SDL_Surface *image();
		virtual void blit(SDL_Surface *screen);
		int x, y; // Oui, mais bon, sprite.setX(sprite.getX()+3), c'est moche

		inline int width() const { return (*m_Images)[m_mode][0]->w;};
		inline int height() const { return (*m_Images)[m_mode][0]->h;};
		void setImages (ImageList *imageList) { m_Images = imageList; };
		SDL_Rect getPos() const;
		void setPos(int x, int y);
		void setX(int x);
		void setY(int y);
		void setWait(int wait) {m_wait = wait;}
		void translate(int x, int y);
		void translateB(int x, int y);
		void setMode(int m, bool loop=true);
		void setAngle(float t);
		void playAnimation(Animation *animation);
//		int distancec(int x, int y) { return dsc(m_pos.x+width()/2, m_pos.y+height()/2, x, y); };
		bool in(int x, int y) const {return (x > m_pos.x && y > m_pos.y && x < m_pos.x+width() && y < m_pos.y+height());};
	protected:
};

#endif // SPRITE_H
