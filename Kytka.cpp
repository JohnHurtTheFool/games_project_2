
#include "shield.h"

//=============================================================================
// default constructor
//=============================================================================
Shield::Shield() : Entity()
{
    spriteData.width = shieldNS::WIDTH;           // size of Ship1
    spriteData.height = shieldNS::HEIGHT;
    spriteData.rect.bottom = shieldNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = shieldNS::WIDTH;
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    //radius = playerNS::WIDTH/2.0;                 // collision radius
	collisionType =entityNS::BOX;
	//radius = (shieldNS::WIDTH)/2.0;
	setScale(shieldNS::SHIELD_SCALE);
	edge.bottom = shieldNS::HEIGHT/2;
	edge.top = -shieldNS::HEIGHT/2;
	edge.right = shieldNS::WIDTH/2;
	edge.left = -shieldNS::WIDTH/2;
	radius = shieldNS::WIDTH/2;
	currentTime = 0.0f;
	counterActive = false;
}

void Shield::draw()
{
    Image::draw();              //draw lineman
}

void Shield::setPos(double x, double y)
{
	setPosition(VECTOR2(x,y));
	spriteData.x=getPositionX();
	spriteData.y=getPositionY();
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Shield::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}
//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Shield::update(float frameTime)
{
	if(counterActive)
	{
		currentTime += frameTime;
	}
	if(currentTime>=shieldNS::maxShieldTime)
	{
		setInvisible();
	}
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
}