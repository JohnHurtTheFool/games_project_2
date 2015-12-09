
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef ENEMY_H                 // Prevent multiple definitions if this 
#define ENEMY_H   
class Enemy;

#include "entity.h"
#include "constants.h"
#include "graphics.h"
#include "EMP.h"

namespace enemyNS
{
    const int WIDTH = 64;                   // image width
    const int HEIGHT = 64;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float SPEED_X = 0;                
	const float SPEED_Y = -0;
	const float ACCELERATION = .005;
}

// inherits from Entity class
class Enemy : public Entity
{
private: 
    bool collision;                 
    bool target;  
	int directionX;
	int directionY;
	VECTOR2 velocity;
	float speed;
	Entity targetEntity;
	EMP emp;
	VECTOR2 dir;
	int maxHits;
	int hits;
public:
    // constructor
    Enemy();
	void setMaxHits(int max);
	void setHits(int h) {hits = h;}
	//void setMaxHits(int max){maxHits = max;}
	//int getMaxHits(){return maxHits;}
	bool wasHit();
	EMP* getEMP(){return &emp;}

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

	void setInvisible();

	void setVisible();

	void setVelocity(VECTOR2 v) {velocity = v;}

	VECTOR2 getVelocity() {return velocity;}

	void ai(float time, Entity &t);

	void vectorTrack();
	void deltaTrack();
	void evade();
	int getHits() {return hits;}
};
#endif

