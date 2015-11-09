
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef _SHIELD_H                 // Prevent multiple definitions if this 
#define _SHIELD_H   
class shield;

#include "entity.h"
#include "constants.h"
#include <cmath>

namespace shieldNS
{
    const int WIDTH = 100;                   // image width
    const int HEIGHT = 100;                  // image height
	const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
	const float SHIELD_SCALE=0.5;
   const float maxShieldTime = 1000.00f;
   const int MAX_HITS = 2;
}

// inherits from Entity class
class Shield : public Entity
{
private:
	float currentTime;
	bool counterActive;
	int hits;//Hits from lasers.
public:
    // constructor
    Shield();

	virtual void draw();

    // inherited member functions
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);

	void Shield::setInvisible()
	{
		counterActive = false;
		currentTime = 0.0f;
		Image::setVisible(false);
		active = false;
	}

	void Shield::setVisible()
	{
		counterActive = true;
		Image::setVisible(true);
		active = true;
		hits=0;
	}
	void Shield::incHits()
	{
    	hits++;
		if(hits==shieldNS::MAX_HITS)
			setInvisible();
	}

    // Set collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual void setCollisionType(entityNS::COLLISION_TYPE ctype)
    {collisionType = ctype;}

    // Set RECT structure used for BOX and ROTATED_BOX collision detection.
    void setEdge(RECT e) {edge = e;}

	void setPos(double x, double y);

    // Get collision type
    entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

};
#endif

