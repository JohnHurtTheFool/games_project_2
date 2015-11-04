#include "bricks.h"

//=============================================================================
// default constructor
//=============================================================================
Brick::Brick() : Entity()
{
    spriteData.width = brickNS::WIDTH;           
    spriteData.height = brickNS::HEIGHT;
    spriteData.x = brickNS::X;                   // location on screen
    spriteData.y = brickNS::Y;
    spriteData.rect.bottom = brickNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = brickNS::WIDTH;
    
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = brickNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -brickNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 50;
}

bool Brick::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Brick::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Brick::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Brick::update(float frameTime)
{
	VECTOR2 foo = velocity*frameTime*speed;
	if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
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
		setPosition(D3DXVECTOR2(getPositionX(),GAME_WIDTH-Image::getHeight()*Image::getScale()));
	}

	velocity = D3DXVECTOR2(0,0);
	incPosition(foo);
	Image::setX(getPositionX());
	Image::setY(getPositionY());
    Entity::update(frameTime);
}

void Brick::evade()
{
	//add code here
	
	return;
}

void Brick::deltaTrack()
{
	/*//add code here
	VECTOR2 vel = getCenterPoint()-targetEntity.getCenterPoint();
	if(abs(vel.x/vel.y) > 2 &&  vel.x>0)

	else if(abs(vel.x/vel.y) > 2 &&  vel.x<0)

	else if(abs(vel.x/vel.y) > 2 &&  vel.x>0)
	setVelocity(-vel);*/

	VECTOR2 vel = VECTOR2(1,1);
	VECTOR2 targetCenter = targetEntity.getCenterPoint();
	if(getCenterPoint().y <= targetCenter.y)
		vel.y = -1;
	if(getCenterPoint().x <= targetCenter.x)
		vel.y = -1;
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	setVelocity(-vel);
}
void Brick::vectorTrack()//We're going to want to change this so that it takes an argument that is what we want to track
{
	VECTOR2 vel = getCenterPoint()-targetEntity.getCenterPoint();
	if(vel.x == 0 && vel.y==0)
		return;
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	setVelocity(-vel);
}

void Brick::ai(float time, Entity &t)
{ 
	targetEntity = t;
	/*vectorTrack();
	deltaTrack();
	evade();*/
	return;
}