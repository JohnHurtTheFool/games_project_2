#pragma once
#include "entity.h"
#include "constants.h"

namespace playerLaserNS
{
    const int WIDTH = 2;                   // image width
    const int HEIGHT = 4;                  // image height
    const float SPEED_X = 0;                
	const float SPEED_Y = 100;
 
}
class PlayerLaser : public Entity
{
private:  
    bool collision;                 
    bool target;  
	int directionX;
	int directionY;

public:
    // constructor
    PlayerLaser();
    // inherited member functions
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);

    // Set collision Boolean
    void setCollision(bool c)
    {collision = c;}

    // Set collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual void setCollisionType(entityNS::COLLISION_TYPE ctype)
    {collisionType = ctype;}

	void PlayerLaser::setInvisible()
	{
		Image::setVisible(false);
		active = false;
	}

	void PlayerLaser::setVisible()
	{
		Image::setVisible(true);
		active = true;
	}

    // Set RECT structure used for BOX and ROTATED_BOX collision detection.
    void setEdge(RECT e) {edge = e;}

    // Set target
    void setTarget(bool t) {target = t;}

    // Get collision
    bool getCollision() {return collision;}

    // Get collision type
    entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

	void setMissedPosition();
};

