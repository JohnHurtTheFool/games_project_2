#include "EMP.h"
EMP::EMP(void)
{
	spriteData.width = EMPNS::WIDTH;           // size of Ship1
    spriteData.height = EMPNS::HEIGHT;
    spriteData.rect.bottom = EMPNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = EMPNS::WIDTH;
    startFrame = 0;              // first frame of ship animation
    endFrame   = 0;              // last frame of ship animation
    currentFrame = startFrame;
    //radius = playerNS::WIDTH/2.0;                 // collision radius
	collisionType =entityNS::BOX;
	//radius = (shieldNS::WIDTH)/2.0;
	resetScale();
	edge.bottom = EMPNS::HEIGHT/2;
	edge.top = -EMPNS::HEIGHT/2;
	edge.right = EMPNS::WIDTH/2;
	edge.left = -EMPNS::WIDTH/2;
	radius = EMPNS::WIDTH/2;
	currentTime = 0.0f;
	counterActive = false;
	maxEMPTime = 0.5f;
}



EMP::~EMP(void)
{
}
void EMP::update(float frameTime)
{
	if(counterActive)
	{
		setScale(getScale()*EMPNS::growthRatePerFrame);
		currentTime += frameTime;
	}
	if(currentTime>=maxEMPTime)
	{
		setInvisible();
		resetScale();
	}
	Entity::update(frameTime);
}
bool EMP::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}
void EMP::draw()
{
    Image::draw();
}
void EMP::setPos(double x, double y)
{
	setPosition(VECTOR2(x,y));
	spriteData.x = getPositionX();
	spriteData.y = getPositionY();
}