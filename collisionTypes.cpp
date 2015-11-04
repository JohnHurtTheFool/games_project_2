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


    if (!paddleTM.initialize(graphics,PADDLE_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

   if (!brickTM.initialize(graphics,PUCK_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));

	if (!paddle.initialize(this, paddleTM.getWidth(), paddleTM.getHeight(), 0,&paddleTM))
		throw(GameError(gameErrorNS::WARNING, "Paddle not initialized"));
	paddle.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
    paddle.setCollisionType(entityNS::BOX);
    paddle.setEdge(COLLISION_BOX_PADDLE);
    paddle.setCollisionRadius(COLLISION_RADIUS);
	paddle.setScale(.5);

	if (!bricks.initialize(this, 0, 0, 0,&brickTM))
		throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
	bricks.setPosition(VECTOR2(100, 100));
	bricks.setCollision(entityNS::BOX);
	bricks.setEdge(COLLISION_BOX_PUCK);
	bricks.setX(bricks.getPositionX());
	bricks.setY(bricks.getPositionY());
	bricks.setScale(.5);

	//patternsteps
	patternStepIndex = 0;
	for (int i = 0; i< maxPatternSteps; i++)
	{
		patternSteps[i].initialize(&bricks);
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
	if(input->isKeyDown(PADDLE_LEFT))
            paddle.left();
    if(input->isKeyDown(PADDLE_RIGHT))
			paddle.right();
	if(input->isKeyDown(PADDLE_UP))
            paddle.up();
    if(input->isKeyDown(PADDLE_DOWN))
			paddle.down();
	paddle.update(frameTime);

	bricks.update(frameTime);
	
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	bricks.ai(frameTime, paddle);
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
	if (paddle.collidesWith(bricks, collisionVector))
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
	paddle.draw();

	bricks.draw();
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CollisionTypes::releaseAll()
{
	paddleTM.onLostDevice();
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
	paddleTM.onResetDevice();
	puckTM.onResetDevice();
    Game::resetAll();
    return;
}
