#include "bossLaser.h"

BossLaser::BossLaser() : Entity()
{
	spriteData.width = bossLaserNS::WIDTH;           
    spriteData.height = bossLaserNS::HEIGHT;
	Entity::setPositionX(rand()%GAME_WIDTH);
	Entity::setPositionY(rand()%GAME_HEIGHT);
	Entity::setVisible(false);
    spriteData.x = getPositionX();                // location on screen
    spriteData.y = getPositionY();
    spriteData.rect.bottom = bossLaserNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = bossLaserNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = bossLaserNS::WIDTH;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	setScale(bossLaserNS::SCALE);

	edge.bottom = bossLaserNS::HEIGHT/2;
	edge.top = -bossLaserNS::HEIGHT/2;
	edge.right = bossLaserNS::WIDTH/2;
	edge.left = -bossLaserNS::WIDTH/2;
	spriteData.scale = bossLaserNS::SCALE;

}


bool BossLaser::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
	directionY = -1;
	directionX = 1;
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));

}

void BossLaser::setMissedPosition() 
{
	setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT/5));
	setX(Entity::getPositionX());
	setY(Entity::getPositionY());
}
//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void BossLaser::update(float frameTime)
{
    Entity::update(frameTime);
	float foo = velocity.x * directionX * frameTime;
	Entity::incPositionX(foo);
	float bar = velocity.y * directionY * frameTime;
	Entity::incPositionY(bar);

	if (Entity::getPositionX()+spriteData.width > GAME_WIDTH)  
	{
        Entity::setPositionX(0); 
		Entity::setVisible(false);
	}
    else if (Entity::getPositionX() < 0)
	{
        Entity::setPositionX(GAME_WIDTH-spriteData.width); 
		Entity::setVisible(false);
	}
	else if (Entity::getPositionY() < 0) 
	{
		Entity::setPositionY(GAME_HEIGHT-spriteData.height);
		Entity::setVisible(false);
	}
	else if (Entity::getPositionY()+spriteData.height > GAME_HEIGHT)
	{
		Entity::setPositionY(0); 
		Entity::setVisible(false);
	}
	spriteData.x = Entity::getPositionX();
	spriteData.y = Entity::getPositionY();
}

