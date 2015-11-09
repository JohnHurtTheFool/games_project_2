
#include "bonus.h"

//=============================================================================
// default constructor
//=============================================================================
Bonus::Bonus() : Entity()
{
    spriteData.width = bonusNS::WIDTH;           // size of Ship1
    spriteData.height = bonusNS::HEIGHT;
    spriteData.rect.bottom = bonusNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = bonusNS::WIDTH;
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    //radius = playerNS::WIDTH/2.0;                 // collision radius
	collisionType =entityNS::BOX;
	//radius = (shieldNS::WIDTH)/2.0;
	setScale(bonusNS::BONUS_SCALE);
	edge.bottom = bonusNS::HEIGHT/2;
	edge.top = -bonusNS::HEIGHT/2;
	edge.right = bonusNS::WIDTH/2;
	edge.left = -bonusNS::WIDTH/2;
	radius = bonusNS::WIDTH/2;
	setVisible();
}

void Bonus::draw()
{
    Image::draw();              //draw lineman
}

void Bonus::setPos(double x, double y)
{
	setPosition(VECTOR2(x,y));
	spriteData.x=getPositionX();
	spriteData.y=getPositionY();
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Bonus::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}
//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bonus::update(float frameTime)
{
	spriteData.x = getPositionX();
	spriteData.y = getPositionY();
	Entity::update(frameTime);

	//incPosition(D3DXVECTOR2(velocity*frameTime));
	//velocity = D3DXVECTOR2(0,0);

  /*if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
	{
		setPosition(D3DXVECTOR2(0,getPositionY()));
	}
	if (getPositionX() < 0)
	{
		setPosition(D3DXVECTOR2(GAME_WIDTH-Image::getWidth()*Image::getScale(),getPositionY()));
	}
	if (getPositionY() + Image::getHeight()*Image::getScale() > GAME_HEIGHT)
	{
		setPosition(D3DXVECTOR2(getPositionX(),0));
	}
	if (getPositionY() < 0)
	{
		setPosition(D3DXVECTOR2(getPositionX(),GAME_HEIGHT-Image::getHeight()*Image::getScale()));
	}*/
	
	/*char msgbu[2048];
	sprintf(msgbu, "Posx: %f  posy: %f\n", getPositionX(),getPositionY());
	OutputDebugStringA(msgb*/
}