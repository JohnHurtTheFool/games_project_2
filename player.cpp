
#include "player.h"

//=============================================================================
// default constructor
//=============================================================================
Player::Player() : Entity()
{
    spriteData.width = playerNS::WIDTH;           // size of Ship1
    spriteData.height = playerNS::HEIGHT;
    spriteData.x = playerNS::X;                   // location on screen
    spriteData.y = playerNS::Y;
    spriteData.rect.bottom = playerNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = playerNS::WIDTH;
    velocity.x = 1;                             // velocity X
    velocity.y = 0;                             // velocity Y
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    //radius = playerNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;
    target = false;
	edge.bottom = playerNS::HEIGHT/2;
	edge.top = -playerNS::HEIGHT/2;
	edge.right = playerNS::WIDTH/2;
	edge.left = -playerNS::WIDTH/2;
	health = 100.00;
	shield.setPos(spriteData.x-(playerNS::WIDTH/2*playerNS::SCALE)+12,spriteData.y-(playerNS::HEIGHT/2 *playerNS::SCALE)-5);
	//shield.setPos(getX()+playerNS::WIDTH/2-(emp.getCurrWidth()/2),getY()+playerNS::HEIGHT/2-(emp.getCurrHeight()/2));
	shield.setInvisible();
	emp.setPos(getX()+playerNS::WIDTH/2-(emp.getCurrWidth()/2),getY()+playerNS::HEIGHT/2-(emp.getCurrHeight()/2));
	emp.setInvisible();
	hasEmp = false;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Player::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime)
{
	
	Entity::update(frameTime);
	shield.update(frameTime);
	emp.update(frameTime);
	incPosition(D3DXVECTOR2(velocity*frameTime));
	//velocity = D3DXVECTOR2(0,0);

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
	spriteData.x = getPositionX();
	spriteData.y = getPositionY();
	shield.setPos(spriteData.x-(playerNS::WIDTH/2*playerNS::SCALE)+12,spriteData.y-(playerNS::HEIGHT/2 *playerNS::SCALE)-5);
	int w = emp.getWidth()/(2/emp.getScale());
	emp.setPos(spriteData.x-emp.getWidth()*emp.getScale()/2,spriteData.y-emp.getHeight()*emp.getScale()/2);
	
	/*char msgbu[2048];
	sprintf(msgbu, "Posx: %f  posy: %f\n", shield.getPositionX(),shield.getPositionY());
	OutputDebugStringA(msgbu);*/
}