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
#include "player.h"
#include "puck.h"
#include "enemy.h"
#include "patternStep.h"


#define maxPatternSteps 6

//=============================================================================
// This class is the core of the game
//=============================================================================
class CollisionTypes : public Game
{
private:
    // game items
    TextureManager playerTM, puckTM, enemyTM, backgroundTM;   // game texture
	Image background;
    VECTOR2 collisionVector;    // collision vector
	Player player;
	Puck puck;
	int score;
	bool collision;
	Enemy enemy[NUM_ENEMIES_INITIAL];	
	PatternStep allPatterns[NUM_ENEMIES_INITIAL][maxPatternSteps];
	int patternStepIndex[NUM_ENEMIES_INITIAL];


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
