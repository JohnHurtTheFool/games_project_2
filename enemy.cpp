#include "enemy.h"

//=============================================================================
// default constructor
//=============================================================================
Enemy::Enemy() : Entity()
{
    spriteData.width = enemyNS::WIDTH;           
    spriteData.height = enemyNS::HEIGHT;
    spriteData.x = enemyNS::X;                   // location on screen
    spriteData.y = enemyNS::Y;
    spriteData.rect.bottom = enemyNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = enemyNS::WIDTH;
    
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = enemyNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -enemyNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 50;
	hits = 0;
	maxHits = 3;
}

bool Enemy::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Enemy::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Enemy::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Enemy::update(float frameTime)
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
		setPosition(D3DXVECTOR2(getPositionX(),GAME_HEIGHT-Image::getHeight()*Image::getScale()));
	}

	//velocity = D3DXVECTOR2(0,0);
	incPosition(foo);
	Image::setX(getPositionX());
	Image::setY(getPositionY());
    Entity::update(frameTime);
}

void Enemy::evade()
{
	//add code here
	
	return;
}

void Enemy::deltaTrack()
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
void Enemy::vectorTrack()//We're going to want to change this so that it takes an argument that is what we want to track
{
	VECTOR2 vel = getCenterPoint()-targetEntity.getCenterPoint();
	if(vel.x == 0 && vel.y==0)
		return;
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	setVelocity(-vel);
	setRadians((atan((targetEntity.getCenterY()-getCenterY())/(targetEntity.getCenterX()-getCenterX())))-PI/2);
	if(targetEntity.getCenterX()>getCenterX())
		setRadians(getRadians()+PI);
}


void Enemy::ai(float time, Entity &t)
{ 
	targetEntity = t;
	/*vectorTrack();
	deltaTrack();
	evade();*/
	return;
}