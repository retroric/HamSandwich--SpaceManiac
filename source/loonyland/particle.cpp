#include "particle.h"
#include <memory>
#include "bullet.h"
#include "monster.h"
#include "options.h"

static std::unique_ptr<Particle[]> particleList;
static int maxParticles;
static int snowCount=0;

Particle::Particle(void)
{
	this->life=0;
}

Particle::~Particle(void)
{
}

void Particle::Go(byte type,int x,int y,int z,byte angle,byte force)
{
	byte fforce;

	if(force==0)
		return;

	this->type=type;
	size=2;
	fforce=force/4;

	if(fforce==0)
		fforce=1;
	this->x=x+Random(4<<FIXSHIFT)-(2<<FIXSHIFT);
	this->y=y+Random(4<<FIXSHIFT)-(2<<FIXSHIFT);
	this->z=z;
	this->dx=Cosine(angle)*Random(fforce);
	this->dy=Sine(angle)*Random(fforce);
	this->dz=Random(fforce*2)<<FIXSHIFT;
	this->life=Random(force)+10;
}

void Particle::GoLightning(int x,int y,int x2,int y2)
{
	this->type=PART_LIGHTNING;
	this->x=x;
	this->y=y;
	this->dx=x2;
	this->dy=y2;
	this->life=4;
	this->color=64;
	this->size=(abs((x2-x)>>FIXSHIFT)+abs((y2-y)>>FIXSHIFT))/16;
}

void Particle::GoRandom(byte type,int x,int y,int z,byte force)
{
	this->type=type;
	size=2;
	if(force==0)
		return;

	this->x=x+Random(32<<FIXSHIFT)-(16<<FIXSHIFT);
	this->y=y+Random(32<<FIXSHIFT)-(16<<FIXSHIFT);
	this->z=z;
	this->dx=(Random(force)-force/2)<<FIXSHIFT;
	this->dy=(Random(force)-force/2)<<FIXSHIFT;
	this->dz=Random(force*2)<<FIXSHIFT;
	this->life=Random(force)+20;
}

void Particle::Update(Map *map)
{
	byte mapx,mapy;
	byte v;
	byte c1;
	char brt;
	int camx,camy;

	// kill ones that go too far afield
	GetCamera(&camx,&camy);
	if(((camx-x/FIXAMT)>330 ||
		(camy-y/FIXAMT)>250) && type!=PART_LIGHTNING)
	{
		life=0;
		return;
	}

	if(life>0)
	{
		if(type!=PART_LIGHTNING)
		{
			dz-=FIXAMT;
			x+=dx;
			y+=dy;
			z+=dz;
			if(z<0)
			{
				z=0;
				dz=-dz/2;
			}
		}

		life--;
		switch(type)
		{
			case PART_HAMMER:
				v=life;
				if(v>31-8)
					v=31-8;
				color=128+v;
				if(life>20)
					size=2;
				else if(life<10)
					size=0;
				else
					size=1;
				break;
			case PART_GLASS:
				if(life>20)
					size=2;
				else if(life<10)
					size=0;
				else
					size=1;
				break;
			case PART_DIRT:
				v=life;
				if(v>31-16)
					v=31-16;
				color=64+v;
				if(life>20)
					size=2;
				else if(life<10)
					size=0;
				else
					size=1;
				break;
			case PART_SNOW2:
				v=life*2;
				if(v>31)
					v=31;
				if(v<31-16)
					v=31-16;
				color=v;
				if(life>20)
					size=2;
				else if(life<10)
					size=0;
				else
					size=1;
				break;
			case PART_WATER:
				v=life;
				if(v>31)
					v=31;
				if(v<8)
					v=8;
				color=96+v;
				if(life>20)
					size=2;
				else if(life<10)
					size=0;
				else
					size=1;
				break;
			case PART_SLIME:
				v=life;
				if(v>31-8)
					v=31-8;
				color=32+4+v;
				if(life>10)
					size=2;
				else if(life<5)
					size=0;
				else
					size=1;
				break;
			case PART_YELLOW:
				v=life;
				if(v>31-8)
					v=31-8;
				color=32*5+4+v;
				if(life>10)
					size=2;
				else if(life<5)
					size=0;
				else
					size=1;
				break;
			case PART_SNOW:
				dx+=Random(FIXAMT)-FIXAMT/2;
				dy+=Random(FIXAMT)-FIXAMT/2;
				Dampen(&dx,FIXAMT/8);
				Dampen(&dy,FIXAMT/8);
				dz+=FIXAMT*15/16;	// not as much gravity as other things
				color=31;
				if(z==0)
				{
					dx=0;
					dy=0;
					dz=0;
					if(life<50)
						size=1;
					if(life<25)
						size=0;
					color=life/2;
					if(color<26)
						color=26;
					if(color>31)
						color=31;
					snowCount++;
				}
				else
					life++;	// can't die while airborne
				break;
			case PART_LIGHTNING:
				// get dimmer with each frame
				color/=2;
				break;
			case PART_CIRCLE:
				dz+=FIXAMT;
				if(size>1)
				{
					size-=2;
					if(size<0)
					{
						size=0;
						life=0;
					}
				}
				if(color>228)
					color-=2;
				break;
			case PART_RING:
				size+=8;
				life-=7;
				if(color>228)
					color-=2;
				break;
			case PART_FIRE:
				dz+=FIXAMT+Random(FIXAMT/4);
				Dampen(&dx,FIXAMT/8);
				Dampen(&dy,FIXAMT/8);
				dx=dx-FIXAMT/8+Random(FIXAMT/4);
				dy=dy-FIXAMT/8+Random(FIXAMT/4);
				if(size>0)
					size--;
				else
					life=0;
				if(life>40)
					color=191;
				else if(life>20)
					color=160+(life-20);
				else
					color=128+life;
				break;
			case PART_COLDFIRE:
				dz+=FIXAMT+Random(FIXAMT/4);
				Dampen(&dx,FIXAMT/8);
				Dampen(&dy,FIXAMT/8);
				dx=dx-FIXAMT/8+Random(FIXAMT/4);
				dy=dy-FIXAMT/8+Random(FIXAMT/4);
				if(size>0)
					size--;
				else
					life=0;
				if(color>0)
					color--;
				else
					life=0;
				break;
		}


		if(x<0 || y<0 || x>=((map->width*TILE_WIDTH)<<FIXSHIFT) || y>=((map->height*TILE_HEIGHT)<<FIXSHIFT))
		{
			life=0;
			return;
		}

		if(type==PART_LIGHTNING || type==PART_GLASS || type==PART_FIRE || type==PART_COLDFIRE)
		{
			// nothing to do
		}
		else
		{
			mapx=(x/TILE_WIDTH)>>FIXSHIFT;
			mapy=(y/TILE_HEIGHT)>>FIXSHIFT;

			// brighten it appropriately
			brt=map->map[mapx+mapy*map->width].templight;
			c1=(color&(~31));	// c1 is the color range
			color+=brt;
			if(color>c1+31 || color<c1)
			{
				if(brt>0)
					color=c1+31;
				else
					color=c1;
			}
		}
	}
}

bool Particle::Alive(void)
{
	return(life>0);
}

//--------------------------------------------------------------------------

void InitParticles(int max)
{
	particleList = std::make_unique<Particle[]>(max);
	maxParticles = max;
}

void ExitParticles(void)
{
	maxParticles = 0;
	particleList.reset();
}

void UpdateParticles(Map *map)
{
	int i;

	snowCount=0;
	for(i=0;i<maxParticles;i++)
		particleList[i].Update(map);
}

void RenderParticle(int x,int y,byte *scrn,byte color,byte size)
{
	byte c1,c2;

	if(x<0 || x>639 || y<0 || y>479)
		return;

	switch(size)
	{
		case 2:	// big particle
			if(x<2 || x>637 || y<2 || y>477)
				return;

			if((color&31)>1)
				c1=color-2;	// only do this if subtracting 2 keeps it in the same color group
			else
				c1=color;
			if((c1&31)>1)
				c2=c1-2;	// only do this if subtracting 4 keeps it in the same color group
			else
				c2=c1;

			scrn+=(x+(y-2)*640);
			*scrn=c2;
			scrn+=639;
			*scrn++=c1;
			*scrn++=color;
			*scrn=c1;
			scrn+=637;
			*scrn++=c2;
			*scrn++=c1;
			*scrn++=color;
			*scrn++=c1;
			*scrn=c2;
			scrn+=637;
			*scrn++=c1;
			*scrn++=color;
			*scrn=c1;
			*(scrn+639)=c2;
			break;
		case 1:	// normal particle
			if(x<1 || x>638 || y<1 || y>478)
				return;
			if(color&31)
				c1=color-1;	// only do this if subtracting 1 keeps it in the same color group
			else
				c1=color;
			scrn+=(x+(y-1)*640);
			*scrn=c1;
			scrn+=639;
			*scrn++=c1;
			*scrn++=color;
			*scrn=c1;
			scrn+=639;
			*scrn=c1;
			break;
		case 0:	// tiny particle (1 pixel)
			scrn[x+y*640]=color;
			break;
	}
}

void HLine(int x,int x2,int y,byte c,byte *scrn)
{
	if(x<0)
		x=0;
	if(x2>639)
		x2=639;
	if(x2<0)
		return;
	if(x>639)
		return;
	if(y<0)
		return;
	if(y>479)
		return;

	scrn+=(x+y*640);
	memset(scrn,c,x2-x+1);
}

void HLine2(int x,int x2,int y,byte c,byte *scrn)
{
	int i;
	byte b,b2,b3;
	byte c1,c2;

	if(x<0)
		x=0;
	if(x2>639)
		x2=639;
	if(x2<0)
		return;
	if(x>639)
		return;
	if(y<0)
		return;
	if(y>479)
		return;

	c1=(c&31);
	c2=(c&(~31));
	scrn+=(x+y*640);
	for(i=x;i<=x2;i++)
	{
		b=*scrn;
		b2=b&(~31);
		b3=(b&31)+c1;
		if(b3>31)
			b3=31;
		*scrn=(b3+c2);
		scrn++;
	}
}

void RenderCircleParticle(int x,int y,int radius,byte c,byte *scrn)
{
	int d,curX,curY;

	d=3-(2*radius);
	curX=0;
	curY=radius;

	while(curX<=curY)
	{
		HLine(x-curY,x+curY,y+curX,c,scrn);
		if(curX!=0)
			HLine(x-curY,x+curY,y-curX,c,scrn);
		if(d<=0)
			d=d+(curX*4)+6;
		else
		{
			d=d+4*(curX-curY)+10;
			if(curY!=curX)
			{
				HLine(x-curX,x+curX,y+curY,c,scrn);
				HLine(x-curX,x+curX,y-curY,c,scrn);
			}
			curY--;
		}
		curX++;
	}
}

void RenderRingParticle(int x,int y,int radius,byte c,byte *scrn)
{
	int d,curX,curY,pitch;

	d=3-(2*radius);
	curX=0;
	curY=radius;

	pitch=GetDisplayMGL()->GetWidth();

	while(curX<=curY)
	{
		PlotPixel(x-curX,y+curX,c,scrn,pitch);
		PlotPixel(x+curX,y+curX,c,scrn,pitch);

		if(curX!=0)
		{
			PlotPixel(x-curY,y-curX,c,scrn,pitch);
			PlotPixel(x+curY,y-curX,c,scrn,pitch);
		}
		if(d<=0)
			d=d+(curX*4)+6;
		else
		{
			d=d+4*(curX-curY)+10;
			if(curY!=curX)
			{
				PlotPixel(x-curX,y+curY,c,scrn,pitch);
				PlotPixel(x+curX,y+curY,c,scrn,pitch);
				PlotPixel(x-curX,y-curY,c,scrn,pitch);
				PlotPixel(x+curX,y-curY,c,scrn,pitch);
			}
			curY--;
		}
		curX++;
	}
}

void RenderGlowCircleParticle(int x,int y,int radius,byte c,byte *scrn)
{
	int i;
	int y1,y2,y3;
	int wid;

	wid=0;
	y1=y-(radius*3/4);
	y2=y-(radius/2);
	y3=y-(radius/4);
	for(i=y-radius+1;i<=y;i++)
	{
		HLine2(x-wid,x+wid,i,c,scrn);
		if(i>=y3)
			wid--;
		else if(i>=y2)
			wid++;
		else if(i>=y1)
			wid=1;
	}
	/*
	int d,curX,curY;

	d=3-(2*radius);
	curX=0;
	curY=radius;

	while(curX<=curY)
	{
		HLine2(x-curY,x+curY,y+curX,c,scrn);
		if(curX!=0)
			HLine2(x-curY,x+curY,y-curX,c,scrn);
		if(d<=0)
			d=d+(curX*4)+6;
		else
		{
			d=d+4*(curX-curY)+10;
			if(curY!=curX)
			{
				HLine2(x-curX,x+curX,y+curY,c,scrn);
				HLine2(x-curX,x+curX,y-curY,c,scrn);
			}
			curY--;
		}
		curX++;
		if(c&31)
			c--;
	}
	*/
}

// this was going to be local to renderlightningparticle, but that would've seriously
// chomped up the stack, since that function's recursive.
byte ctab[]={8,3,2,3,8,
		     3,2,1,2,3,
			 2,1,0,1,2,
			 3,2,1,2,3,
			 8,3,2,3,8};

void RenderLightningParticle(int x1,int y1,int x2,int y2,int range,byte bright,byte *scrn)
{
	int midx,midy;
	byte *ctptr;
	byte b,brt;

	if((x1<0 && x2<0) || (y1<0 && y2<0) || (x1>639 && x2>639) ||
		(y1>479 && y2>479))
		return;	// quit if both ends are off-screen

	// base case: draw the (x1,y1) pixel
	if((x1-x2<2 && x1-x2>-2) && (y1-y2<2 && y1-y2>-2))
	{
		if(x1>=0 && x1<635 && y1>=0 && y1<475)
		{
			scrn+=(x1+y1*640);
			ctptr=&ctab[0];
			for(midy=y1;midy<y1+5;midy++)
			{
				for(midx=x1;midx<x1+5;midx++)
				{
					b=*scrn;
					brt=bright>>(*ctptr);
					if(((b+brt)&(~31))!=(b&(~31)))
						*scrn=(b&(~31))+31;
					else
						*scrn=b+brt;
					ctptr++;
					scrn++;
				}
				scrn+=640-5;
			}
		}
	}
	else	// recursive case, find a (midx,midy) between the other two points
	{
		if(range<1)
			range=1;
		if(x1<x2)
			midx=x1+(x2-x1)/2;
		else
			midx=x2+(x1-x2)/2;
		midx+=Random(range)-range/2;
		if(y1<y2)
			midy=y1+(y2-y1)/2;
		else
			midy=y2+(y1-y2)/2;
		midy+=Random(range)-range/2;
		RenderLightningParticle(x1,y1,midx,midy,range*3/4,bright,scrn);
		RenderLightningParticle(midx,midy,x2,y2,range*3/4,bright,scrn);
	}
}

void RenderParticles(void)
{
	int i;

	for(i=0;i<maxParticles;i++)
	{
		if(particleList[i].Alive())
		{
			if(particleList[i].type==PART_LIGHTNING)
				LightningDraw(particleList[i].x>>FIXSHIFT,particleList[i].y>>FIXSHIFT,
								particleList[i].dx>>FIXSHIFT,particleList[i].dy>>FIXSHIFT,
								particleList[i].color,(char)particleList[i].size);
			else if(particleList[i].type==PART_CIRCLE)
				ParticleDraw(particleList[i].x>>FIXSHIFT,particleList[i].y>>FIXSHIFT,
							 particleList[i].z>>FIXSHIFT,particleList[i].color,(byte)Random(particleList[i].size+1),
							 DISPLAY_DRAWME|DISPLAY_CIRCLEPART);
			else if(particleList[i].type==PART_RING)
				ParticleDraw(particleList[i].x>>FIXSHIFT,particleList[i].y>>FIXSHIFT,
							 particleList[i].z>>FIXSHIFT,particleList[i].color,particleList[i].size,
							 DISPLAY_DRAWME|DISPLAY_RINGPART);
			else if(particleList[i].type==PART_FIRE || particleList[i].type==PART_COLDFIRE)
			{
				if(opt.cheats[CH_FROGWPN])
				{
					RenderItem(particleList[i].x>>FIXSHIFT,particleList[i].y>>FIXSHIFT,
						particleList[i].z>>FIXSHIFT,ITM_FROGDOLL,0,0);
				}
				else
					ParticleDraw(particleList[i].x>>FIXSHIFT,particleList[i].y>>FIXSHIFT,
							 particleList[i].z>>FIXSHIFT,particleList[i].color,particleList[i].size/4,
							 DISPLAY_DRAWME|DISPLAY_CIRCLEPART|DISPLAY_GLOW);
			}
			else
				ParticleDraw(particleList[i].x>>FIXSHIFT,particleList[i].y>>FIXSHIFT,
							 particleList[i].z>>FIXSHIFT,particleList[i].color,particleList[i].size,
							 DISPLAY_DRAWME|DISPLAY_PARTICLE);
		}
	}
}

void MakeCircleParticle(int x,int y, int z,byte size)
{
	int i;
	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].x=x;
			particleList[i].y=y;
			particleList[i].z=z;
			particleList[i].dx=0;
			particleList[i].dy=0;
			particleList[i].dz=0;
			particleList[i].life=size/2;
			particleList[i].size=size;
			particleList[i].color=255;
			particleList[i].type=PART_CIRCLE;
			break;
		}
	}
}

void MakeCircleParticle2(int x,int y, int z,byte size)
{
	int i;
	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].x=x;
			particleList[i].y=y;
			particleList[i].z=z;
			particleList[i].dx=0;
			particleList[i].dy=0;
			particleList[i].dz=0;
			particleList[i].life=size/4+2;
			particleList[i].size=size;
			particleList[i].color=255;
			particleList[i].type=PART_CIRCLE;
			break;
		}
	}
}

void MakeRingParticle(int x,int y, int z,byte size,byte cnt)
{
	int i;
	byte ang;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			ang=(byte)Random(256);
			particleList[i].x=x+Cosine(ang)*size;
			particleList[i].y=y+Sine(ang)*size;
			particleList[i].z=z;
			particleList[i].dx=0;
			particleList[i].dy=0;
			particleList[i].dz=Random(FIXAMT*2);
			particleList[i].color=191;
			particleList[i].life=10+(byte)Random(30);
			particleList[i].size=8*4+(byte)Random(4*4);
			particleList[i].type=PART_FIRE;
			cnt--;
			if(!cnt)
				break;
		}
	}
}

void MakeColdRingParticle(int x,int y, int z,byte size)
{
	int i;
	byte ang;

	ang=0;
	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].x=x+Cosine(ang)*size;
			particleList[i].y=y+Sine(ang)*size;
			particleList[i].z=z;
			particleList[i].dx=0;
			particleList[i].dy=0;
			particleList[i].dz=Random(FIXAMT*2);
			particleList[i].color=31;
			particleList[i].life=10+Random(30);
			particleList[i].size=8*4+(byte)Random(4*4);
			particleList[i].type=PART_COLDFIRE;
			ang++;
			if(!ang)
				break;
		}
	}
}

void GlassShatter(int x,int y,int x2,int y2,int z,byte amt)
{
	int i;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].x=(x+Random(x2-x));
			particleList[i].y=(y+Random(y2-y));
			particleList[i].z=z;
			particleList[i].GoRandom(PART_GLASS,(x+Random(x2-x)),(y+Random(y2-y)),
					Random(10*FIXAMT),20);
			particleList[i].color=(byte)Random(8)*32+16;
			if(!(--amt))
				break;
		}
	}
}

void SpurtParticles(byte type,bool left,int x,int y,int z,byte angle,byte force)
{
	int i,amt;
	byte ang2;

	amt=force;

	x+=Cosine(angle)*10;
	y+=Sine(angle)*10;
	if(left)
		ang2=angle+64;
	else
		ang2=angle+128+64;

	x+=Cosine(ang2)*5;
	y+=Sine(ang2)*5;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].Go(type,x,y,z,angle,force);
			if(!--amt)
				break;
		}
	}
}

void ExplodeParticles(byte type,int x,int y,int z,byte force)
{
	int i,amt;

	amt=force;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].GoRandom(type,x,y,z,force);
			if(!--amt)
				break;
		}
	}
}

void ExplodeParticles2(byte type,int x,int y,int z,byte num,byte force)
{
	int i;

	if(num==0)
		return;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].GoRandom(type,x,y,z,force);
			if(!--num)
				break;
		}
	}
}

void MakeItSnow(Map *map)
{
	int i;
	int cx,cy;

	// only 25% of particles may be snowflakes
	if(Random(100)>30 || snowCount>maxParticles/4)
		return;

	GetCamera(&cx,&cy);
	cx-=320;
	cy-=240;
	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{

			particleList[i].x=(Random(640)+cx)<<FIXSHIFT;
			particleList[i].y=(Random(480)+cy)<<FIXSHIFT;
			particleList[i].z=(300+Random(300))<<FIXSHIFT;
			particleList[i].dx=0;
			particleList[i].dy=0;
			particleList[i].dz=0;
			particleList[i].size=2;
			particleList[i].life=50+Random(50);
			particleList[i].type=PART_SNOW;
			particleList[i].color=31;
			break;
		}
	}
}

void SpecialSnow(int x,int y)
{
	int i;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{

			particleList[i].x=x-FIXAMT*10+Random(FIXAMT*20);
			particleList[i].y=y-FIXAMT*10+Random(FIXAMT*20);
			particleList[i].z=(50+Random(20))<<FIXSHIFT;
			particleList[i].dx=0;
			particleList[i].dy=0;
			particleList[i].dz=0;
			particleList[i].size=2;
			particleList[i].life=20+Random(30);
			particleList[i].type=PART_SNOW;
			break;
		}
	}
}

void LightningBolt(int x,int y,int x2,int y2)
{
	int i;

	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].GoLightning(x,y,x2,y2);
			break;
		}
	}
}

void Burn(int x,int y,int z)
{
	int i;
	byte num;

	num=(byte)Random(8)+1;
	for(i=0;i<maxParticles;i++)
	{
		if(!particleList[i].Alive())
		{
			particleList[i].x=x-FIXAMT*2+Random(FIXAMT*4);
			particleList[i].y=y;
			particleList[i].z=z-FIXAMT*2+Random(FIXAMT*4);
			particleList[i].dx=-FIXAMT*2+Random(FIXAMT*4);
			particleList[i].dy=0;
			particleList[i].dz=Random(FIXAMT*2);
			particleList[i].color=191;
			particleList[i].life=10+Random(30);
			particleList[i].size=8*4+(byte)Random(4*4);
			particleList[i].type=PART_FIRE;
			if(--num==0)
				break;
		}
	}
}
