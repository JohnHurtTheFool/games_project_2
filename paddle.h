
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef _PADDLE_H                 // Prevent multiple definitions if this 
#define _PADDLE_H   
class Paddle;

#include "entity.h"
#include "constants.h"

namespace paddleNS
{
    const int WIDTH = 90;                   // image width
    const int HEIGHT = 20;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float SPEED = 120;             
	
   
}

// inherits from Entity class
class Paddle : public Entity
{
private:
  
    bool collision;                 
    bool target;                    

public:
    // constructor
    Paddle();

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

    // Set RECT structure used for BOX and ROTATED_BOX collision detection.
    void setEdge(RECT e) {edge = e;}

    // Set target
    void setTarget(bool t) {target = t;}

    // Get collision
    bool getCollision() {return collision;}

    // Get collision type
    entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

    void right()
    {
        velocity.x = paddleNS::SPEED;
    }
    void left()
    {
        velocity.x = -paddleNS::SPEED;
    }
	void up()
    {
        velocity.y = -paddleNS::SPEED;
    }
	void down()
    {
        velocity.y = paddleNS::SPEED;
    }
};
#endif

