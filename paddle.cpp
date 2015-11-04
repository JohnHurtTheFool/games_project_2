
#include "paddle.h"

//=============================================================================
// default constructor
//=============================================================================
Paddle::Paddle() : Entity()
{
    spriteData.width = paddleNS::WIDTH;           // size of Ship1
    spriteData.height = paddleNS::HEIGHT;
    spriteData.x = paddleNS::X;                   // location on screen
    spriteData.y = paddleNS::Y;
    spriteData.rect.bottom = paddleNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = paddleNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    //radius = paddleNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;
    target = false;
	edge.bottom = -paddleNS::HEIGHT/2;
	edge.top = paddleNS::HEIGHT/2;
	edge.right = -paddleNS::WIDTH/2;
	edge.left = paddleNS::WIDTH/2;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Paddle::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Paddle::update(float frameTime)
{
	
	Entity::update(frameTime);

	incPosition(D3DXVECTOR2(velocity*frameTime));
	velocity = D3DXVECTOR2(0,0);

    if (getPositionX() > GAME_WIDTH-getWidth()*getScale()) 
		setPositionX(GAME_WIDTH-getWidth()*getScale()); 
    if (getPositionX() < 0) 
        setPositionX(0);
	if (getPositionY() > GAME_HEIGHT-getHeight()*getScale())
		setPositionY(GAME_HEIGHT-getHeight()*getScale());
	if (getPositionY() < 0)
		setPositionY(0);
	spriteData.x = getPositionX();
	spriteData.y = getPositionY();
}