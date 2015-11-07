// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"
#include <random>
#include "time.h"
//=============================================================================
// Constructor
//=============================================================================
CollisionTypes::CollisionTypes()
{
	//nothing here, move on
}

//=============================================================================
// Destructor
//=============================================================================
CollisionTypes::~CollisionTypes()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void CollisionTypes::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    if (!playerTM.initialize(graphics,PLAYER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));

    if (!enemyTM.initialize(graphics,ENEMY_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy textures"));
    if (!enemyLaserTM.initialize(graphics,ENEMY_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy laser textures"));
    if (!playerLaserTM.initialize(graphics,PLAYER_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));
	if (!player.initialize(this, SPACESHIP_SIZE,SPACESHIP_SIZE, 2,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "player not initialized"));
	for(int i = 0; i < MAX_PLAYER_LASERS; i++)
		if (!player.initialize(this, SPACESHIP_SIZE,SPACESHIP_SIZE, 2,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "player not initialized"));
	if (!backgroundTM.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "My texture initialization failed"));
	if (!background.initialize(graphics, 2048,1024,0, &backgroundTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init my background"));
	
	player.setFrames(2,3);
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setFrames(2,3);
	}
	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
    player.setCollisionType(entityNS::BOX);
    player.setEdge(COLLISION_BOX_player);
    player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(.5);
	
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if (!enemy[i].initialize(this, SPACESHIP_SIZE, SPACESHIP_SIZE, 2,&enemyTM))
			throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
		enemy[i].setPosition(VECTOR2(rand()%GAME_HEIGHT, rand()%GAME_WIDTH));
		enemy[i].setCollision(entityNS::BOX);
		enemy[i].setEdge(COLLISION_BOX_PUCK);
		enemy[i].setX(enemy[i].getPositionX());
		enemy[i].setY(enemy[i].getPositionY());
		enemy[i].setScale(.5);
	}


	background.setX(0);
	background.setY(0);
	background.setScale(BACKGROUND_SCALE);

	//patternsteps
	for(int j = 0; j < NUM_ENEMIES_INITIAL; j++)
	{
		patternStepIndex[j] = rand()%maxPatternSteps;
	}
	for(int j = 0; j < NUM_ENEMIES_INITIAL; j++)
	{
		for (int i = 0; i< maxPatternSteps; i++)
		{
			allPatterns[j][i].initialize(&enemy[j]);
			allPatterns[j][i].setActive();
		}
	}
	for(int j = 0; j < NUM_ENEMIES_INITIAL; j++)
	{
		allPatterns[j][0].setAction(TRACK);
		allPatterns[j][0].setTimeForStep(4);
		allPatterns[j][1].setAction(RIGHT);
		allPatterns[j][1].setTimeForStep(3);
		allPatterns[j][2].setAction(DOWN);
		allPatterns[j][2].setTimeForStep(2);
		allPatterns[j][3].setAction(LEFT);
		allPatterns[j][3].setTimeForStep(2);
		allPatterns[j][4].setAction(UP);
		allPatterns[j][4].setTimeForStep(2);
		allPatterns[j][5].setAction(TRACK);
		allPatterns[j][5].setTimeForStep(4);
	}
	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void CollisionTypes::update()
{
	if(input->isKeyDown(player_LEFT))
            player.left();
    if(input->isKeyDown(player_RIGHT))
			player.right();
	if(input->isKeyDown(player_UP))
            player.up();
    if(input->isKeyDown(player_DOWN))
			player.down();
	player.update(frameTime);
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		enemy[i].update(frameTime);
	}
	
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		enemy[i].ai(frameTime, player);
	}
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if (allPatterns[i][patternStepIndex[i]].isFinished())
		{
			allPatterns[i][patternStepIndex[i]].setActive();
			patternStepIndex[i]=(rand())%maxPatternSteps;
		}
	}
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		allPatterns[i][patternStepIndex[i]].update(frameTime);
	}
}

//=============================================================================
// Handle collisions
//=============================================================================
void CollisionTypes::collisions()
{
    collisionVector = D3DXVECTOR2(0,0);
	collision = false;
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if (player.collidesWith(enemy[i], collisionVector))
		{
			collision = true;
			puck.changeDirectionY();
			audio->playCue(BEEP1);
		}
	}
}

//=============================================================================
// Render game items
//=============================================================================
void CollisionTypes::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	background.draw();
	
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		enemy[i].draw();
	}
	player.draw();
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CollisionTypes::releaseAll()
{
	playerTM.onLostDevice();
	puckTM.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void CollisionTypes::resetAll()
{
	playerTM.onResetDevice();
	puckTM.onResetDevice();
    Game::resetAll();
    return;
}
