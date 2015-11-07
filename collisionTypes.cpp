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

	//texture inits
    if (!playerTM.initialize(graphics,PLAYER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));

    if (!enemyTM.initialize(graphics,ENEMY_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy textures"));
    if (!enemyLaserTM.initialize(graphics,ENEMY_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy laser textures"));
    if (!playerLaserTM.initialize(graphics,PLAYER_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));

	//entity inits
	if (!player.initialize(this, SPACESHIP_SIZE,SPACESHIP_SIZE, 2,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "player not initialized"));
	for(int i = 0; i < MAX_PLAYER_LASERS; i++)
		if (!playerLaser[i].initialize(this, LASER_WIDTH,LASER_HEIGHT, 2,&playerLaserTM))
			throw(GameError(gameErrorNS::WARNING, "player's laser not initialized"));
	for(int i = 0; i < MAX_ENEMY_LASERS; i++)
		if (!enemyLaser[i].initialize(this, LASER_WIDTH,LASER_HEIGHT, 2,&enemyLaserTM))
			throw(GameError(gameErrorNS::WARNING, "enemy's laser not initialized"));

	//background
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
	playerNextLaserIndex = 0;
	enemyNextLaserIndex = 0;
	bool shootKeyDownLastFrame = false;
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
	if(input->isKeyDown(PLAYER_SHOOT) && !shootKeyDownLastFrame)
	{
		(playerLaser[playerNextLaserIndex]).setVisible();
		(playerLaser[playerNextLaserIndex]).setPositionX((player.getPositionX()+SPACESHIP_SIZE/4)-LASER_WIDTH/2);//Center of the player's width
		(playerLaser[playerNextLaserIndex]).setPositionY(player.getPositionY());//top of player
		playerNextLaserIndex=(playerNextLaserIndex+1)%MAX_PLAYER_LASERS;
		shootKeyDownLastFrame = true;//Shoot key was down this frame.
	}
	else if(!(input->isKeyDown(PLAYER_SHOOT)) && shootKeyDownLastFrame)
	{
		shootKeyDownLastFrame = false;//Player released shoot key.
	}
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if(!(rand()%1000))
		{
			(enemyLaser[enemyNextLaserIndex]).setVisible();
			(enemyLaser[enemyNextLaserIndex]).setPositionX((enemy[i].getPositionX()+SPACESHIP_SIZE/4)-LASER_WIDTH/2);//Center of the enemy's width
			(enemyLaser[enemyNextLaserIndex]).setPositionY(enemy[i].getPositionY());//top of enemy
			enemyNextLaserIndex=(enemyNextLaserIndex+1)%MAX_ENEMY_LASERS;
		}
	}
	player.update(frameTime);
	for(int i = 0; i < MAX_PLAYER_LASERS; i++)
	{
		playerLaser[i].update(frameTime);
	}
	for(int i = 0; i < MAX_ENEMY_LASERS; i++)
	{
		enemyLaser[i].update(frameTime);
	}
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		enemy[i].update(frameTime);
	}
	
	if(player.getHealth() <= 60.00 && player.getHealth() > 30.00)
	{
		player.setFrames(10,11);
	}
	else if(player.getHealth() <= 30.00)
	{
		player.setFrames(18,19);
	}
	else if(player.getHealth() <= 0.00)
	{
		player.setInvisible();
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
		//player with enemy collision
		if (player.collidesWith(enemy[i], collisionVector) && enemy[i].getVisible() && player.getVisible())
		{
			//collision = true;
			player.setHealth(player.getHealth() - kamikazeDamage);
			enemy[i].setInvisible();
			//puck.changeDirectionY();
			audio->playCue(BEEP1);
		}
	}
	//laser with player collision
	for(int i = 0;i<MAX_ENEMY_LASERS;i++)
	{
		if (player.collidesWith(enemyLaser[enemyNextLaserIndex], collisionVector) && enemyLaser[enemyNextLaserIndex].getVisible())
		{
			//collision = true;
			player.setHealth(player.getHealth() - laserDamage);
			enemyLaser[i].setInvisible();
			//puck.changeDirectionY();
			audio->playCue(BEEP1);
			enemyLaser[i].setInvisible();
		}
	}
	//player laser with enemy collision
	for(int j = 0; j < NUM_ENEMIES_INITIAL; j++)
	{
		for(int x = 0;x<MAX_PLAYER_LASERS;x++)
		{
			if(enemy[j].collidesWith(playerLaser[x], collisionVector) && playerLaser[x].getVisible() && enemy[j].getVisible())
			{
				playerLaser[x].setInvisible();
				enemy[j].wasHit();
				//update score
			}
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
	for(int i = 0; i < MAX_PLAYER_LASERS; i++)
	{
		playerLaser[i].draw();
	}
	for(int i = 0; i < MAX_ENEMY_LASERS; i++)
	{
		enemyLaser[i].draw();
	}
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
