// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"

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


    if (!playerTM.initialize(graphics,PADDLE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));

   if (!enemyTM.initialize(graphics,PUCK_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));

	if (!player.initialize(this, playerTM.getWidth(), playerTM.getHeight(), 0,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "player not initialized"));

	if (!backgroundTM.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "My texture initialization failed"));
	if (!background.initialize(graphics, 0,0,0, &backgroundTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init my background"));

	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
    player.setCollisionType(entityNS::BOX);
    player.setEdge(COLLISION_BOX_player);
    player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(.5);

	if (!enemy.initialize(this, 0, 0, 0,&enemyTM))
		throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
	enemy.setPosition(VECTOR2(100, 100));
	enemy.setCollision(entityNS::BOX);
	enemy.setEdge(COLLISION_BOX_PUCK);
	enemy.setX(enemy.getPositionX());
	enemy.setY(enemy.getPositionY());
	enemy.setScale(.5);

	background.setX(0);
	background.setY(0);
	background.setScale(BACKGROUND_SCALE);

	//patternsteps
	patternStepIndex = 0;
	for (int i = 0; i< maxPatternSteps; i++)
	{
		patternSteps[i].initialize(&enemy);
		patternSteps[i].setActive();
	}
	patternSteps[0].setAction(RIGHT);
	patternSteps[0].setTimeForStep(3);
	patternSteps[1].setAction(DOWN);
	patternSteps[1].setTimeForStep(2);
	patternSteps[2].setAction(TRACK);
	patternSteps[2].setTimeForStep(4);
	/*patternSteps[3].setAction(NONE);
	patternSteps[3].setTimeForStep(2);*/
	patternSteps[3].setAction(EVADE);
	patternSteps[3].setTimeForStep(2);

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

	enemy.update(frameTime);
	
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	enemy.ai(frameTime, player);
	if (patternStepIndex == maxPatternSteps)
		return;
	if (patternSteps[patternStepIndex].isFinished())
		patternStepIndex++;
	patternSteps[patternStepIndex].update(frameTime);
}

//=============================================================================
// Handle collisions
//=============================================================================
void CollisionTypes::collisions()
{
    collisionVector = D3DXVECTOR2(0,0);
	collision = false;
	if (player.collidesWith(enemy, collisionVector))
	{
		collision = true;
		puck.changeDirectionY();
		audio->playCue(BEEP1);
	}
}

//=============================================================================
// Render game items
//=============================================================================
void CollisionTypes::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	background.draw();
	player.draw();

	enemy.draw();
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
