#include "boss.h"
//=============================================================================
// default constructor
//=============================================================================
Boss::Boss() : Entity()
{
    spriteData.width = bossNS::WIDTH;           
    spriteData.height = bossNS::HEIGHT;
    spriteData.x = bossNS::X;                   // location on screen
    spriteData.y = bossNS::Y;
    spriteData.rect.bottom = bossNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = bossNS::WIDTH;
    
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = bossNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = bossNS::HEIGHT/2;
	edge.top = -bossNS::HEIGHT/2;
	edge.right = bossNS::WIDTH/2;
	edge.left = -bossNS::WIDTH/2;
	spriteData.scale = 1;
	active = true;
	speed = 100;
	hits = 0;
	setInvisible();
	emp.resetScale();
	emp.setMaxEMPTime(0.3f);
	//maxHits = 3;
}
void Boss::wasHit()
{
	hits += 1;
	if(hits>=maxHits)
	{
		setInvisible();
	}
	char msgbu[2048];
	sprintf(msgbu, "Hits: %d\n", hits);
	OutputDebugStringA(msgbu);
}
void Boss::setMaxHits(int max)
{
	maxHits = max;
}
bool Boss::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Boss::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Boss::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Boss::update(float frameTime)
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
	emp.update(frameTime);
	incPosition(foo);
	Image::setX(getPositionX());
	Image::setY(getPositionY());
    Entity::update(frameTime);
	emp.setPos(spriteData.x-emp.getWidth()*emp.getScale()/2,spriteData.y-emp.getHeight()*emp.getScale()/2);
}

void Boss::evade()
{
	//add code here
	
	return;
}

void Boss::deltaTrack()
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
void Boss::vectorTrack()//We're going to want to change this so that it takes an argument that is what we want to track
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

void Boss::empHit()
{
	hits += 5;
	if(hits>=maxHits)
	{
		setInvisible();
	}
}


void Boss::ai(float time, Entity &t)
{ 
	targetEntity = t;
	/*vectorTrack();
	deltaTrack();
	evade();*/
	return;
}