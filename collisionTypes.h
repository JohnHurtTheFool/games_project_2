// Programming 2D Games
// Copyright (c) 2011,2012 by: 
// Charles Kelly
// collisionTypes.h v1.0

#ifndef _COLLISION_TYPES_H      // Prevent multiple definitions if this 
#define _COLLISION_TYPES_H      // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class CollisionTypes;

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include <cmath>
#include "paddle.h"
#include "puck.h"
#include "bricks.h"
#include "patternStep.h"


#define maxPatternSteps 4

//=============================================================================
// This class is the core of the game
//=============================================================================
class CollisionTypes : public Game
{
private:
    // game items
    TextureManager paddleTM, puckTM, brickTM, backgroundTM;   // game texture
    VECTOR2 collisionVector;    // collision vector
	Paddle paddle;
	Puck puck;
	int score;
	bool collision;
	Brick bricks;	
	PatternStep patternSteps[maxPatternSteps];
	int patternStepIndex;


public:
    // Constructor
    CollisionTypes();

    // Destructor
    virtual ~CollisionTypes();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
