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
	dxFontSmall = new TextDX();
}

//=============================================================================
// Destructor
//=============================================================================
CollisionTypes::~CollisionTypes()
{
	delete dxFontSmall;
	delete output;
	delete levelOutput;
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void CollisionTypes::initialize(HWND hwnd)
{
	musicOn = true;
    Game::initialize(hwnd); // throws GameError
	audio->playCue(BACKGROUND);
	//audio->stopCue(BACKGROUND);*/
	//audio->playCue(ASOUND);
	timeInState = 0;
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
	 if (!splashTM.initialize(graphics,SPLASH_SCREEN))
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

	if (!splash.initialize(graphics, 2048,1024,0, &splashTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init my splash"));
	//background
	if (!backgroundTM.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "My texture initialization failed"));
	if (!background.initialize(graphics, 2048,1024,0, &backgroundTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init background"));
	if (!loseTM.initialize(graphics, GAME_OVER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "initialization failed"));
	if (!lose.initialize(graphics, 2048,1024,0, &loseTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init my background"));
	if(dxFontSmall->initialize(graphics, 18, true, false, "Calibri")== false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

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

	
	player.setFrames(0,1);
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setFrames(2,3);
	}

	player.setPosition(VECTOR2(rand()%GAME_WIDTH, rand()%GAME_HEIGHT));
    player.setCollisionType(entityNS::BOX);
    player.setEdge(COLLISION_BOX_player);
    player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(playerNS::SCALE);
	
	invEnem = 10;


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
		allPatterns[j][6].setAction(NONE);
		allPatterns[j][6].setTimeForStep(1);
	}
	playerNextLaserIndex = 0;
	enemyNextLaserIndex = 0;
	bool shootKeyDownLastFrame = false;
	scoreMsg = "";
	score = 0;
	playerFrames=2;
	bool backCheatKeyPressedLastFrame = false;
	gameState = SPLASH;
	mainMenu = new Menu();
	mainMenu->initialize(graphics, input);
	//outString = "Selected Item: ";
	output = new TextDX();
	if(output->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing output font"));
	levelOutput = new TextDX();
	if(levelOutput->initialize(graphics, 40, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing output font"));
	levelOutput->setFontColor(graphicsNS::BLUE);
	levelNumber=1;
	forcefield = false;
	LEVEL_UP_MSG = "LEVEL ";
	optionsScreenMSG = "MUSIC IS CURRENTLY";
	currentEnemyMaxHits = 0;
	toggleKeyPressedLastFrame = false;
	for(int i = 0;i<24;i++)
	{
		anyCheatKeyPressedLastFrame[i] = false;
	}
	cheatAttempt = "";
	cheatMSG = "ENTER CHEAT CODE: \nPRESS RETURN TO SUBMIT\nPRESS SPACE FOR MENU";
	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void CollisionTypes::update()
{
	VECTOR2 playerVel = player.getVelocity();
	double magSquared = playerVel.x * playerVel.x + playerVel.y * playerVel.y;
	bool backKeyPressedThisFrame = false;
	bool keyPressedThisFrame[24];
	for(int i = 0;i<24;i++)
	{
		keyPressedThisFrame[i] = false;
	}
	int x = 0;
	switch(gameState)
	{
	case CHEAT:
		for(int i = 0x41; i < 0x5A; i++)
		{
			if(input->isKeyDown(i)&&!anyCheatKeyPressedLastFrame[x])
			{
				cheatAttempt += i;
				keyPressedThisFrame[x] = true;
			}
<<<<<<< HEAD
			else if((input->isKeyDown(i)) && anyCheatKeyPressedLastFrame[x])
=======
			else if((input->isKeyDown(i)||input->isKeyDown(VK_BACK))&&anyCheatKeyPressedLastFrame)
>>>>>>> origin/master
			{
				keyPressedThisFrame[x] = true;
			}
<<<<<<< HEAD
			x++;
		}
		if(input->isKeyDown(VK_BACK)&&!backCheatKeyPressedLastFrame)
		{
			if(cheatAttempt.length()>0)cheatAttempt.pop_back();
			backKeyPressedThisFrame = true;
		}
		else if(input->isKeyDown(VK_BACK)&&backCheatKeyPressedLastFrame)
		{
			backKeyPressedThisFrame = true;
		}

		for(int j = 0;j<24;j++)
		{
			anyCheatKeyPressedLastFrame[j] = keyPressedThisFrame[j];
		}
		
		backCheatKeyPressedLastFrame = backKeyPressedThisFrame;
		
=======
			if(input->isKeyDown(VK_BACK)&&!anyCheatKeyPressedLastFrame)
			{
				if(cheatAttempt.length()>0)
					cheatAttempt.pop_back();

				keyPressedThisFrame = true;
			}
			anyCheatKeyPressedLastFrame = keyPressedThisFrame;
>>>>>>> origin/master
		break;
	case OPTIONS:
		if(input->isKeyDown(TOGGLE_MUSIC)&&!toggleKeyPressedLastFrame)
		{
			musicOn = !musicOn;
			if(!musicOn)
				audio->stopCue(BACKGROUND);
			if(musicOn)
				audio->playCue(BACKGROUND);
			toggleKeyPressedLastFrame = true;
		}
		else if(!input->isKeyDown(TOGGLE_MUSIC))
			toggleKeyPressedLastFrame = false;
		break;
	case MENU:
		mainMenu->update();
		break;
	case GAME_PLAY:
		player.update(frameTime);
		if(forcefield)
			player.getShield()->setVisible();
		if(input->isKeyDown(player_LEFT))
				player.left();
		if(input->isKeyDown(player_RIGHT))
				player.right();
		if(input->isKeyDown(player_UP) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y > 0))
				player.up();
		if(input->isKeyDown(player_DOWN) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y < 0))
				player.down();
		if(input->isKeyDown(player_UP) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y > 0))
		{
			if(magSquared < (playerNS::MAX_VELOCITY_SQUARED/3))
			{
				if(player.getHealth() >= 60.00 && playerFrames != 2)
				{
					player.setFrames(2,3);
					playerFrames = 2;
				}
				else if(player.getHealth() <= 60.00 && player.getHealth() > 30.00 && playerFrames != 10)
				{
					player.setFrames(10,11);
					playerFrames = 10;
				}
				else if(player.getHealth() <= 30.00 && player.getHealth() >= 0.00 && playerFrames != 18)
				{
					player.setFrames(18,19);
					playerFrames = 18;
				}
				else if(player.getHealth() <= 0.00)
				{
					player.setInvisible();
				}
			}
			else if(magSquared > (playerNS::MAX_VELOCITY_SQUARED/3) && magSquared < (2 * playerNS::MAX_VELOCITY_SQUARED/3))
			{
				if(player.getHealth() >= 60.00 && playerFrames != 4)
				{
					player.setFrames(4,5);
					playerFrames = 4;
				}
				else if(player.getHealth() <= 60.00 && player.getHealth() > 30.00 && playerFrames != 12)
				{
					player.setFrames(12,13);
					playerFrames = 12;
				}
				else if(player.getHealth() <= 30.00 && player.getHealth() >= 0.00 && playerFrames != 20)
				{
					player.setFrames(20,21);
					playerFrames = 20;
				}
				else if(player.getHealth() <= 0.00)
				{
					player.setInvisible();
				}
			}
			else if(magSquared > (2*playerNS::MAX_VELOCITY_SQUARED/3))
			{
				if(player.getHealth() >= 60.00 && playerFrames != 6)
				{
					player.setFrames(6,7);
					playerFrames = 6;
				}
				else if(player.getHealth() <= 60.00 && player.getHealth() > 30.00 && playerFrames != 14)
				{
					player.setFrames(14,15);
					playerFrames = 14;
				}
				else if(player.getHealth() <= 30.00 && player.getHealth() >= 0.00 && playerFrames != 22)
				{
					player.setFrames(22,23);
					playerFrames = 22;
				}
				else if(player.getHealth() <= 0.00)
				{
					player.setInvisible();
				}
			}
		}
		else
		{
			if(player.getHealth() >= 60.00 && playerFrames != 0)
			{
				player.setFrames(0,1);
				playerFrames = 0;
			}
			else if(player.getHealth() <= 60.00 && player.getHealth() > 30.00 && playerFrames != 8)
			{
				player.setFrames(8,9);
				playerFrames = 8;
			}
			else if(player.getHealth() <= 30.00 && player.getHealth() >= 0.00 && playerFrames != 16)
			{
				player.setFrames(16,17);
				playerFrames = 16;
			}
			else if(player.getHealth() <= 0.00)
			{
				player.setInvisible();
			}
		}
		if(input->isKeyDown(PLAYER_SHOOT) && !shootKeyDownLastFrame)
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
		/*
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
		*/
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
		{
			if((enemy[i]).getHits() <= currentEnemyMaxHits && (enemy[i]).getHits() <= 0.3f *currentEnemyMaxHits)
			{
				(enemy[i]).setFrames(2,3);
			}
			else if((enemy[i]).getHits() > 0.3f *currentEnemyMaxHits && (enemy[i]).getHits() <= 0.6f *currentEnemyMaxHits)
			{
				(enemy[i]).setFrames(10,11);
			}
			else if((enemy[i]).getHits() > 0.6f *currentEnemyMaxHits && (enemy[i]).getHits() <=currentEnemyMaxHits)
			{
				(enemy[i]).setFrames(18,19);
			}
			else if((enemy[i]).getHits() > currentEnemyMaxHits)
			{
				(enemy[i]).setInvisible();
			}
		}
		scoreMsg= "Score: "+std::to_string(score);
		break;
		case SPLASH:
			background.update(frameTime);
			break;
	}
	
}
void CollisionTypes::updateState()
{
	timeInState+=frameTime;
	bool enemiesRemain = false;
	for(int i = 0; i < NUM_ENEMIES_INITIAL;i++)
	{
		if((enemy[i]).getVisible())
		{
			enemiesRemain = true;
			break;
		}
	}
	if(gameState==SPLASH && timeInState >3)
	{
		gameState = MENU;
		timeInState = 0;
	}
	else if(gameState==MENU && !mainMenu->getSelectedItem())
	{
		mainMenu->setSelectedItem(-1);
		currentEnemyMaxHits = 0;
		levelNumber=1;
		player.setHealth(100);
		levelReset();
		gameState = NEW_LEVEL;
		timeInState = 0;
		score = 0;
	}
	else if(gameState==MENU && mainMenu->getSelectedItem()==1)
	{
		mainMenu->setSelectedItem(-1);
		gameState = CHEAT;
		cheatAttempt = "";
		timeInState = 0;
	}
	else if(gameState==MENU && mainMenu->getSelectedItem()==2)
	{
		mainMenu->setSelectedItem(-1);
		gameState = OPTIONS;
		timeInState = 0;
	}
	else if(gameState==GAME_PLAY && (!player.getVisible()))
	{
		gameState = LOSE_SCREEN;
		timeInState = 0;
	}
	else if(gameState==LOSE_SCREEN && timeInState > 3)
	{
		gameState = MENU;
		timeInState = 0;
	}
	else if(gameState==GAME_PLAY && !enemiesRemain)
	{
		levelNumber++;
		levelReset();
		gameState = NEW_LEVEL;
		timeInState = 0;
	}
	else if(gameState == NEW_LEVEL && timeInState > 4)
	{
		gameState = GAME_PLAY;
		timeInState = 0;
	}
	else if(gameState == OPTIONS && input->isKeyDown(VK_SPACE))
	{
		gameState = MENU;
		timeInState = 0;
	}
	else if(gameState == CHEAT && input->isKeyDown(VK_SPACE))
	{
		gameState = MENU;
		timeInState = 0;
	}
	else if(gameState == CHEAT && input->isKeyDown(VK_RETURN))
	{
		if(!strcmp(cheatAttempt.c_str(),INVINCIBILITY_CHEAT))
		{
			cheatAttempt = "";
			forcefield = true;
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
			if((enemy[i]).getVisible())
			{
				allPatterns[i][patternStepIndex[i]].setActive();
				patternStepIndex[i]=(rand())%maxPatternSteps;
			}
			else
			{
				allPatterns[i][patternStepIndex[i]].setActive();
				patternStepIndex[i]=6;
			}
				
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
				player.setHealth(player.getHealth() - kamikazeDamage);
				enemy[i].setInvisible();
				audio->playCue(CRASH);
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
				player.setHealth(player.getHealth() - laserDamage);
				enemyLaser[i].setInvisible();
				audio->playCue(LASER);
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
				audio->playCue(CRASH);
				player.getShield()->setInvisible();
				break;
			}
		}
		//player with laser collision
		for(int i = 0;i<MAX_ENEMY_LASERS;i++)
		{
			if (player.getShield()->collidesWith(enemyLaser[i], collisionVector) && enemyLaser[i].getVisible())
			{
				enemyLaser[i].setInvisible();
				audio->playCue(LASER);
				player.getShield()->incHits();
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
				score++;
				if(!enemy[j].getActive() && !bonus[j].getVisible())
				{
					if(rand()%3==0)
					{
						bonus[j].setVisible();
					}
					score+=4;
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
	std::stringstream ss,ss2;
	std::string levelUpOutput = LEVEL_UP_MSG + std::to_string(levelNumber);
	std::string on;
	if(musicOn)
		on = " on. Press tab to toggle.\n    Press space to return to main menu.";
	else
		on = " off. Press tab to toggle.\n    Press space to return to main menu.";
	std::string optionsOutput = optionsScreenMSG + on;
	switch(gameState)
	{
	case LOSE_SCREEN:
		graphics->spriteBegin();// begin drawing sprites
		lose.draw();
		dxFontSmall->print(scoreMsg,GAME_WIDTH*.95,GAME_HEIGHT*.01);
		graphics->spriteEnd();
		break;
	case CHEAT:
		ss << cheatMSG;
		ss2 << cheatAttempt;
		graphics->spriteBegin();// begin drawing sprites
		background.draw();
		levelOutput->print(ss.str(), GAME_WIDTH*.3,GAME_HEIGHT*.1);
		levelOutput->print(ss2.str(), GAME_WIDTH*.3,GAME_HEIGHT*.45);
		graphics->spriteEnd();
		break;
	case NEW_LEVEL:
		ss << levelUpOutput;
		graphics->spriteBegin();// begin drawing sprites
		background.draw();
		levelOutput->print(ss.str(), GAME_WIDTH*.45,GAME_HEIGHT*.45);
		graphics->spriteEnd();
		break;
	case MENU:
		

		graphics->spriteBegin();// begin drawing sprites
		background.draw();
		mainMenu->displayMenu();
		output->print(ss.str(), 0,0);
		graphics->spriteEnd();   
		break;
	case GAME_PLAY:
		graphics->spriteBegin();                // begin drawing sprites
		background.draw();
		dxFontSmall->print(scoreMsg,GAME_WIDTH*.95,GAME_HEIGHT*.01);//draw score message
	
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
		break;
	case SPLASH:
		graphics->spriteBegin();                // begin drawing sprites
		splash.draw();
		graphics->spriteEnd();                  // end drawing sprites
		break;
	case OPTIONS:
		ss << optionsOutput;
		graphics->spriteBegin();// begin drawing sprites
		background.draw();
		levelOutput->print(ss.str(), GAME_WIDTH*.24,GAME_HEIGHT*.45);
		graphics->spriteEnd();
	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CollisionTypes::releaseAll()
{
	playerTM.onLostDevice();
	puckTM.onLostDevice();

	playerTM.onLostDevice();
    puckTM.onLostDevice();
    enemyTM.onLostDevice();
    backgroundTM.onLostDevice();
    playerLaserTM.onLostDevice();
    enemyLaserTM.onLostDevice();
    shieldTM.onLostDevice();
    bonusTM.onLostDevice();
    splashTM.onLostDevice();
    loseTM.onLostDevice();

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

	playerTM.onLostDevice();
	puckTM.onLostDevice();

	playerTM.onResetDevice();
    puckTM.onResetDevice();
    enemyTM.onResetDevice();
    backgroundTM.onResetDevice();
    playerLaserTM.onResetDevice();
    enemyLaserTM.onResetDevice();
    shieldTM.onResetDevice();
    bonusTM.onResetDevice();
    splashTM.onResetDevice();
    loseTM.onResetDevice();
    Game::resetAll();
    return;
}
void CollisionTypes::levelReset()
{
	player.setHealth(100.00);
	audio->stopCue(BACKGROUND);
	if(musicOn)
		audio->playCue(BACKGROUND);
	currentEnemyMaxHits+=3;
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setMaxHits(currentEnemyMaxHits);
	}
	player.setVisible();
	player.getShield()->setInvisible();
	player.setPosition(VECTOR2(rand()%GAME_WIDTH, rand()%GAME_HEIGHT));
	VECTOR2 zeroVector(0,0);
	player.setVelocity(zeroVector);
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setVisible();
		int height =rand()%(GAME_HEIGHT-enemy[i].getHeight());
		int width =rand()%(GAME_WIDTH-enemy[i].getWidth());
		enemy[i].setPosition(VECTOR2(width,height));
		bonus[i].setPos(width, height);
		bonus[i].setInvisible();
		enemy[i].setX(enemy[i].getPositionX());
		enemy[i].setY(enemy[i].getPositionY());
		(enemy[i]).setHits(0);
	}
	for(int i = 0; i<MAX_PLAYER_LASERS;i++)
	{
		(playerLaser[i]).setInvisible();
	}
	for(int i = 0; i<MAX_ENEMY_LASERS;i++)
	{
		(enemyLaser[i]).setInvisible();
	}
}
