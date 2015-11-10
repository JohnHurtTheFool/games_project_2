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
	if (!shieldTM.initialize(graphics,SHIELD_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing shield texture"));
	if (!bonusTM.initialize(graphics,BONUS_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bonus texture"));


    if (!enemyTM.initialize(graphics,ENEMY_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy textures"));
    if (!enemyLaserTM.initialize(graphics,ENEMY_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy laser textures"));
    if (!playerLaserTM.initialize(graphics,PLAYER_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));

	//entity inits
	if (!player.initialize(this, SPACESHIP_SIZE,SPACESHIP_SIZE, 2,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "player not initialized"));

	if (!(*player.getShield()).initialize(this, shieldNS::WIDTH,shieldNS::HEIGHT, 0,&shieldTM))
		throw(GameError(gameErrorNS::WARNING, "shield not initialized"));

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

	float height;
	float width;

	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if (!enemy[i].initialize(this, SPACESHIP_SIZE, SPACESHIP_SIZE, 2,&enemyTM))
			throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
		if (!bonus[i].initialize(this, bonusNS::WIDTH, bonusNS::HEIGHT, 0,&bonusTM))
			throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
		height = rand()%GAME_HEIGHT;
		width = rand()%GAME_WIDTH;
		enemy[i].setPosition(VECTOR2(height, width));
		bonus[i].setPos(width, height);
		bonus[i].setInvisible();
		enemy[i].setCollision(entityNS::BOX);
		enemy[i].setEdge(COLLISION_BOX_PUCK);
		enemy[i].setX(enemy[i].getPositionX());
		enemy[i].setY(enemy[i].getPositionY());
		enemy[i].setScale(.5);
	}

	
	player.setFrames(2,3);
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setFrames(2,3);
	}

	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-150));
    player.setCollisionType(entityNS::BOX);
    player.setEdge(COLLISION_BOX_player);
    player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(playerNS::SCALE);
	
	


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
		allPatterns[j][0].setTimeForStep(3);
		allPatterns[j][1].setAction(RIGHT);
		allPatterns[j][1].setTimeForStep(3);
		allPatterns[j][2].setAction(DOWN);
		allPatterns[j][2].setTimeForStep(3);
		allPatterns[j][3].setAction(LEFT);
		allPatterns[j][3].setTimeForStep(3);
		allPatterns[j][4].setAction(UP);
		allPatterns[j][4].setTimeForStep(3);
		allPatterns[j][5].setAction(TRACK);
		allPatterns[j][5].setTimeForStep(3);
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
	player.update(frameTime);
	
	/*char msgbu[2048];
	sprintf(msgbu, "Posx: %f  posy: %f\n", player.getShield()->getPositionX(),player.getShield()->getPositionY());
	OutputDebugStringA(msgbu);*/
	VECTOR2 playerVel = player.getVelocity();
	double magSquared = playerVel.x * playerVel.x + playerVel.y * playerVel.y;
	if(input->isKeyDown(player_LEFT))
            player.left();
    if(input->isKeyDown(player_RIGHT))
			player.right();
	if(input->isKeyDown(player_UP) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y > 0))
            player.up();
    if(input->isKeyDown(player_DOWN) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y < 0))
			player.down();
	if(input->isKeyDown(PLAYER_SHOOT) && !shootKeyDownLastFrame && player.getVisible())
	{
		(playerLaser[playerNextLaserIndex]).setVisible();
		(playerLaser[playerNextLaserIndex]).setPositionX((player.getPositionX()+SPACESHIP_SIZE/4));//Center of the player's width
		(playerLaser[playerNextLaserIndex]).setPositionY((player.getPositionY()+SPACESHIP_SIZE/4));//top of player
		float rad = player.getRadians();
		VECTOR2 laserVelocity((player.getVelocity()).x+playerLaserNS::SPEED_X*sin(player.getRadians()),-player.getVelocity().y+playerLaserNS::SPEED_Y*cos(player.getRadians()));
		(playerLaser[playerNextLaserIndex]).setVelocity(laserVelocity);
		(playerLaser[playerNextLaserIndex]).setRadians(player.getRadians());
		shootKeyDownLastFrame = true;//Shoot key was down this frame.
		playerNextLaserIndex=(playerNextLaserIndex+1)%MAX_PLAYER_LASERS;
	}
	else if(!(input->isKeyDown(PLAYER_SHOOT)) && shootKeyDownLastFrame)
	{
		shootKeyDownLastFrame = false;//Player released shoot key.
	}
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if(!(rand()%1000)&&enemy[i].getVisible())
		{
			(enemyLaser[enemyNextLaserIndex]).setVisible();
			(enemyLaser[enemyNextLaserIndex]).setPositionX((enemy[i].getPositionX()+SPACESHIP_SIZE/4));//Center of the enemy's width
			(enemyLaser[enemyNextLaserIndex]).setPositionY((enemy[i].getPositionY()+SPACESHIP_SIZE/4));//top of enemy
			float rad = enemy[i].getRadians();
			VECTOR2 laserVelocityEnemy((enemy[i].getVelocity()).x+enemyLaserNS::SPEED_X*sin(enemy[i].getRadians()),-enemy[i].getVelocity().y+enemyLaserNS::SPEED_Y*cos(enemy[i].getRadians()));
			(enemyLaser[enemyNextLaserIndex]).setVelocity(laserVelocityEnemy);
			(enemyLaser[enemyNextLaserIndex]).setRadians(enemy[i].getRadians());
			enemyNextLaserIndex=(enemyNextLaserIndex+1)%MAX_ENEMY_LASERS;
		}
	}
	
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
		bonus[i].update(frameTime);
		bonus[i].setPos(enemy[i].getPositionX(),enemy[i].getPositionY());
	}
	
	if(player.getHealth() <= 60.00 && player.getHealth() > 30.00)
	{
		player.setFrames(10,11);
	}
	else if(player.getHealth() <= 30.00 && player.getHealth() >= 0.00)
	{
		player.setFrames(18,19);
	}
	else if(player.getHealth() <= 0.00)
	{
		player.setInvisible();
	}

	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if((enemy[i]).getHits() <= (enemy[i]).getMaxHits() && (enemy[i]).getHits() <= 0.3f *(enemy[i]).getMaxHits())
		{
			
		}
		else if((enemy[i]).getHits() > 0.3f *(enemy[i]).getMaxHits() && (enemy[i]).getHits() <= 0.6f *(enemy[i]).getMaxHits())
		{
			(enemy[i]).setFrames(10,11);
		}
		else if((enemy[i]).getHits() > 0.6f *(enemy[i]).getMaxHits() && (enemy[i]).getHits() <=(enemy[i]).getMaxHits())
		{
			(enemy[i]).setFrames(18,19);
		}
		else if((enemy[i]).getHits() > (enemy[i]).getMaxHits())
		{
			(enemy[i]).setInvisible();
		}
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
	if(!player.getShield()->getActive())//shield is not active
	{
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
		{
			//player with enemy collision
			if (player.collidesWith(enemy[i], collisionVector) && enemy[i].getVisible() && player.getVisible())
			{
				//collision = true;
				player.setHealth(player.getHealth() - kamikazeDamage);
				enemy[i].setInvisible();
				//puck.changeDirectionY();
				//audio->playCue(BEEP1);
			}
			if (player.collidesWith(bonus[i], collisionVector) && bonus[i].getVisible() && player.getVisible())
			{
				player.getShield()->setVisible();
				bonus[i].setInvisible();
				break;
			}
		}
		//laser with player collision
		for(int i = 0;i<MAX_ENEMY_LASERS;i++)
		{
			if (player.collidesWith(enemyLaser[i], collisionVector) && enemyLaser[i].getVisible())
			{
				//collision = true;
				player.setHealth(player.getHealth() - laserDamage);
				enemyLaser[i].setInvisible();
				//puck.changeDirectionY();
				//audio->playCue(BEEP1);
			}
		}
	}
	else//shield is active
	{
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
		{
			//player with enemy collision
			if (player.getShield()->collidesWith(enemy[i], collisionVector) /*&& enemy[i].getVisible()*/ /*&& player.getVisible()*/)
			{	
				enemy[i].setInvisible();
				//puck.changeDirectionY();
				audio->playCue(BEEP1);
				player.getShield()->setInvisible();
				break;
			}
			/*char msgbu[500];
			sprintf(msgbu, "enemy: %f - %f  player:%f - %f shield:%f - %f\n", enemy[i].getPositionX(), enemy[i].getPositionY(),player.getPositionX(), player.getPositionY(), player.getShield()->getPositionX(), player.getShield()->getPositionY());
		OutputDebugStringA(msgbu);*/
		}
		//laser with player collision
		for(int i = 0;i<MAX_ENEMY_LASERS;i++)
		{
			if (player.getShield()->collidesWith(enemyLaser[i], collisionVector) && enemyLaser[i].getVisible())
			{
				enemyLaser[i].setInvisible();
				//puck.changeDirectionY();
				//audio->playCue(BEEP1);
				enemyLaser[i].setInvisible();
			}
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
				if(!enemy[j].getActive() && !bonus[j].getVisible())
				{
					if(rand()%3==0)
					{
						bonus[j].setVisible();
					}
				}
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
		bonus[i].draw();
	}
	player.draw();
	(*player.getShield()).draw();
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
