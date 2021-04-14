#include "Sprite.h"

extern SDL_PixelFormat *fmt;

Uint32 getPixelColor(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void getPixel(SDL_Surface *surface, int x, int y, unsigned char &r, unsigned char &g, unsigned char &b)
{
	Uint32 c = getPixelColor(surface, x, y);
	SDL_GetRGB(c, fmt, &r, &g, &b);
}

void thickLineRGB(SDL_Surface *screen, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 thickness, unsigned char r, unsigned char g, unsigned char b)
{
	Sint16 x[4];
	Sint16 y[4];
	x[0] = x1-thickness/2; y[0] = y1;
	x[1] = x1+thickness/2; y[1] = y1;
	x[2] = x2+thickness/2; y[2] = y2;
	x[3] = x2-thickness/2; y[3] = y2;
	filledPolygonRGBA(screen, x, y, 4, r, g, b, 255);
	polygonRGBA(screen, x, y, 4, 0, 0, 0, 255);
}


SDL_Surface *createSurface(int w, int h, bool colorkey, bool hw)
{
	SDL_Surface *ret;
	ret = SDL_CreateRGBSurface((hw ? SDL_HWSURFACE : SDL_SWSURFACE), w, h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
	if(colorkey)
	 	SDL_SetColorKey(ret, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(ret->format, 1, 0, 254));
	SDL_FillRect(ret, 0, SDL_MapRGB(ret->format, 1, 0, 254));
	return ret;
}

void filledRectangleRGBA(SDL_Surface *screen, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Sint16 x[4], y[4];
	x[0] = x1; y[0]= y1;
	x[1] = x1; y[1] = y2;
	x[2] = x2; y[2] = y2;
	x[3] = x2; y[3] = y1;
	filledPolygonRGBA(screen, x, y, 4, r, g, b, a);
}

void filledRectangleRGB(SDL_Surface *screen, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 r, Uint8 g, Uint8 b)
{
	SDL_Rect rect;
	rect.x = x1; rect.y = y1; rect.w = x2-x1; rect.h = y2-y1;
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
}


vector<SDL_Surface *> *loadAnimation(char *filename)
{
	int i=0;
	char n[40];
	SDL_Surface *temp;
	vector<SDL_Surface *> *v;
	v = new vector<SDL_Surface *>;
	SDL_Surface *final;
	do
	{
		sprintf(n, "%s_%d.bmp", filename, i);
		temp = SDL_LoadBMP(n);
		if(temp != NULL)
		{
			SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 1, 0, 254));
			final = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			v->push_back(final);
		} else
			break;
		i++;
	} while(true);

	return v;
}



ImageList *loadImages(char * filename, bool enableRotate)
{
	int i = 0;
	int j = 0;
	SDL_Surface *temp = NULL, *final;
	char n[40];
	ImageList *ret = new vector<vector<SDL_Surface *> >;
	vector<SDL_Surface *> v;
	do
	{
		j = 0;
		//ret->push_back(vector<SDL_Surface *>()); // PAS BIEN
		do
		{
		 	sprintf(n, "%s_%d_%d.bmp", filename, i, j);
		 	cout << "Ouverture de " << n << endl;
		 	temp = SDL_LoadBMP(n);
		 	if(temp != NULL)
		 	{
		 		SDL_SetColorKey(temp, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(temp->format, 1, 0, 254));
		 		final = SDL_DisplayFormat(temp);
				SDL_FreeSurface(temp);
				v.push_back(final);
				//SDL_SetColorKey(v.back(), SDL_SRCCOLORKEY, SDL_MapRGB(v.back()->format, 1, 0, 254));

		 		cout << n << " ouvert\n";
		 		j++;
		 	} else break;
		} while(true);
		if(j != 0)
		{
			ret->push_back(v);
			v.clear();
		} else break;
		if(enableRotate)
		{
			int o = ret->size()-1;
			double theta;
			for(theta=DT; theta<360; theta += DT)
			{
				ret->push_back(vector<SDL_Surface *>());
				for(j=0; j<(*ret)[o].size(); j++)
				{
					if(theta == 90 || theta == 180 || theta == 270)
					{
						SDL_Surface *thing;
						SDL_PixelFormat t = *((*ret)[o][j]->format);
						t.BitsPerPixel = 32;
						t.BytesPerPixel = 4;
						thing = SDL_ConvertSurface((*ret)[o][j], &t, (*ret)[o][j]->flags);
						temp = rotateSurface90Degrees(thing, (int)4-(theta/90));
						SDL_FreeSurface(thing);
					}
					else
						temp = rotozoomSurface((*ret)[o][j], theta, 1.0, 0);
					final = SDL_DisplayFormat(temp);
					SDL_FreeSurface(temp);
					ret->back().push_back(final);
		 			SDL_SetColorKey(ret->back().back(), SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(ret->back().back()->format, 1, 0, 254));
				}
			}
		}
		i++;
	} while(j != 0);

	/*for(i=0; i<ret->size(); i++)
	for(j=0; j<(*ret)[i].size(); j++)
	{
		SDL_Surface *t;
		t = SDL_ConvertSurface((*ret)[i][j], (*ret)[i][j]->format, (*ret)[i][j]->flags | SDL_SRCCOLORKEY | SDL_RLEACCEL);
		SDL_FreeSurface((*ret)[i][j]);
		(*ret)[i][j] = t;
	}*/
	return ret;
}

Sprite::Sprite(ImageList *images, int mode, int _x, int _y, int wait, bool _enableRotate)
{
	m_Images = images;
	m_Counter = 0;
	m_waitCounter = 0;
	this->m_wait = wait;
	x = _x;
	y = _y;
	m_mode = mode;
	enableRotate = _enableRotate;
	loop = true;
	//ctor
}

Sprite::Sprite(SDL_Surface * im, int _x, int _y)
{
	m_Images = new vector<vector<SDL_Surface *> >;
	(*m_Images).push_back(vector<SDL_Surface *>());
	(*m_Images)[0].push_back(im);
	m_mode = 0;
	x = _x;
	y = _y;
	m_wait = -1;
	m_Counter = 0;
	m_waitCounter = 0;
	enableRotate = false;
	loop = true;
}

Sprite::~Sprite()
{
	for(m_Counter = 0; m_Counter < m_Images->size(); m_Counter++)
	{
		//SDL_FreeSurface((*m_Images)[m_Counter]);
	}
	//dtor
}

SDL_Surface *Sprite::image()
{
	SDL_Surface *ret;
	ret = (*m_Images)[m_mode][m_Counter];
	if(m_wait == -1) return ret;
	m_waitCounter++;
	if(m_waitCounter >= m_wait)
	{
		m_Counter++;
		m_waitCounter = 0;
		if(m_Counter >= (*m_Images)[m_mode].size())
		{
			if(loop) m_Counter = 0;
			else
			{
				m_waitCounter = -1;
				m_Counter--;
			}
		}
	}

	return ret;
}

void Sprite::blit(SDL_Surface *screen)
{
	#ifdef SPRITE_CENTER
	m_pos.x = x-width()/2;
	m_pos.y = y-height()/2;
	#else
	m_pos.x = x;
	m_pos.y = y;
	#endif
	SDL_BlitSurface(image(), 0, screen, &m_pos);
	for(int i=0; i<animations.size(); i++)
	{
		if(! animations[i]->blit(screen, x, y))
		{
			delete animations[i];
			animations.erase(animations.begin()+i);
		}
	}
}

void Sprite::setMode(int m, bool _loop)
{
	loop = _loop;
	m_Counter = 0;
	if(m_wait != -1) m_waitCounter = 0;
	if(enableRotate)
		m_mode = ((int)(360/DT))*m;
	else
		m_mode = m;
}

void Sprite::setAngle(float t)
{
	if(t >= 0) t = t-((int)(t/360))*360;
	else t = t+((int)(-t/360)+1)*360;
	if(t == 360) t = 0;
	m_mode = ((int)m_mode/(360/DT))*(360/DT)+t/DT;
}

SDL_Rect Sprite::getPos() const
{
	return m_pos;
}


void Sprite::setPos(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Sprite::translate(int dx, int dy)
{
	x += dx;
	y += dy;
}

void Sprite::translateB(int dx, int dy)
{
	x += dx;
	if(x+width() > 640) x = 640-width();
	if(x < 0) x = 0;
	y += dy;
	if(y+height() > 480) y = 480-height();
	if(y < 0) y = 0;
}

void Sprite::playAnimation(Animation *a)
{
	animations.push_back(a);
}

SDL_Rect intersection(SDL_Rect a, SDL_Rect b)
{
	SDL_Rect r;
	r.x = (a.x > b.x ? a.x : b.x);
	r.y = (a.y > b.y ? a.y : b.y);
	r.w = (a.x+a.w < b.x+b.w ? a.x+a.w-r.x : b.x+b.w-r.x);
	r.h = (a.y+a.h < b.y+b.h ? a.y+a.h-r.y : b.y+b.h-r.y);
	return r;
}

Animation::Animation(vector<SDL_Surface *> *_images, int _x, int _y, int _wait)
{
	images = _images;
	x = _x;
	y = _y;
	index = 0;
	wait = _wait;
	waitCounter = 0;
}

bool Animation::blit(SDL_Surface *screen, int ix, int iy)
{
	SDL_Rect r;
	waitCounter++;
	if(waitCounter >= wait)
	{
		waitCounter = 0;
		index++;
		if(index >= images->size()) return false;
	}
	#ifdef SPRITE_CENTER
	r.x = ix+x-(*images)[0]->w/2;
	r.y = iy+y-(*images)[0]->h/2;
	#else
	r.x = ix+x;
	r.y = iy+y;
	#endif
	SDL_BlitSurface((*images)[index], 0, screen, &r);
	return true;
}
