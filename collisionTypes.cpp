// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"
#include <random>
#include "time.h"
#include <windows.h>
#include <fstream>
#include <string>
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
	ShowCursor(false);
	
	//audio->stopCue(BACKGROUND);*/
	//audio->playCue(ASOUND);
	timeInState = 0;
	//texture inits
    if (!playerTM.initialize(graphics,PLAYER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));
	if (!shieldTM.initialize(graphics,SHIELD_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing shield texture"));
	if (!EMPTM.initialize(graphics,EMP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing emp texture"));
	if (!bonusTM.initialize(graphics,BONUS_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bonus texture"));
	if (!empPowerupTM.initialize(graphics,EMP_POWERUP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bonus texture"));

    if (!enemyTM.initialize(graphics,ENEMY_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy textures"));
    if (!enemyLaserTM.initialize(graphics,ENEMY_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing enemy laser textures"));
    if (!playerLaserTM.initialize(graphics,PLAYER_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));
	if (!bossLaserTM.initialize(graphics,ENEMY_LASER))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));
	 if (!splashTM.initialize(graphics,SPLASH_SCREEN))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));

	 if (!bossTM.initialize(graphics,BOSS_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player laser textures"));

	 if (!player.initialize(this, SPACESHIP_SIZE,SPACESHIP_SIZE, 2,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "player not initialized"));

	//entity inits
	 if (!boss.initialize(this, bossNS::WIDTH,bossNS::HEIGHT, 2,&bossTM))
		throw(GameError(gameErrorNS::WARNING, "boss not initialized"));

	if (!(*player.getShield()).initialize(this, shieldNS::WIDTH,shieldNS::HEIGHT, 0,&shieldTM))
		throw(GameError(gameErrorNS::WARNING, "shield not initialized"));
	if (!(*player.getEMP()).initialize(this, EMPNS::WIDTH,EMPNS::HEIGHT, 0,&EMPTM))
		throw(GameError(gameErrorNS::WARNING, "emp not initialized"));
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
		if (!(*enemy[i].getEMP()).initialize(this, EMPNS::WIDTH,EMPNS::HEIGHT, 0,&EMPTM))
			throw(GameError(gameErrorNS::WARNING, "emp not initialized"));
	for(int i = 0; i < MAX_PLAYER_LASERS; i++)
		if (!playerLaser[i].initialize(this, LASER_WIDTH,LASER_HEIGHT, 2,&playerLaserTM))
			throw(GameError(gameErrorNS::WARNING, "player's laser not initialized"));
	for(int i = 0; i < MAX_ENEMY_LASERS; i++)
		if (!enemyLaser[i].initialize(this, LASER_WIDTH,LASER_HEIGHT, 2,&enemyLaserTM))
			throw(GameError(gameErrorNS::WARNING, "enemy's laser not initialized"));
	for(int i = 0; i < MAX_BOSS_LASERS; i++)
		if (!bossLaser[i].initialize(this, LASER_WIDTH,LASER_HEIGHT, 2,&bossLaserTM))
			throw(GameError(gameErrorNS::WARNING, "enemy's laser not initialized"));

	if (!splash.initialize(graphics, 2048,1024,0, &splashTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init my splash"));
	//background
	if (!backgroundTM.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "My texture initialization failed"));
	if (!background.initialize(graphics, 2048,1024,0, &backgroundTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init background"));
	if (!instrTM.initialize(graphics, INSTR_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "My texture initialization failed"));
	if (!high_scoresTM.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "My texture initialization failed"));
	if (!instr.initialize(graphics, 2048,1024,0, &instrTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init instr"));
	if (!high_scores.initialize(graphics, 2048,1024,0, &high_scoresTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init instr"));
	if (!loseTM.initialize(graphics, GAME_OVER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "initialization failed"));
	if (!lose.initialize(graphics, 2048,1024,0, &loseTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error init my background"));
	if(dxFontSmall->initialize(graphics, 50, true, false, "Calibri")== false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	float height;
	float width;

	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		if (!enemy[i].initialize(this, SPACESHIP_SIZE, SPACESHIP_SIZE, 2,&enemyTM))
			throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
		if (!bonus[i].initialize(this, bonusNS::WIDTH, bonusNS::HEIGHT, 0,&bonusTM))
			throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
		if (!empPowerup[i].initialize(this, bonusNS::WIDTH, bonusNS::HEIGHT, 0,&empPowerupTM))
			throw(GameError(gameErrorNS::WARNING, "Enemy not initialized"));
		height = rand()%GAME_HEIGHT;
		width = rand()%GAME_WIDTH;
		enemy[i].setPosition(VECTOR2(height, width));
		bonus[i].setPos(width, height);
		bonus[i].setInvisible();
		empPowerup[i].setPos(width, height);
		empPowerup[i].setInvisible();
		enemy[i].setCollision(entityNS::BOX);
		enemy[i].setEdge(COLLISION_BOX_PUCK);
		enemy[i].setX(enemy[i].getPositionX());
		enemy[i].setY(enemy[i].getPositionY());
		enemy[i].setScale(.5);
	}

	hs.changeFileName("../games_project_2/highscores.txt");
	player.setFrames(0,1);
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setFrames(2,3);
	}

	boss.setFrames(2,3);

	player.setPosition(VECTOR2(rand()%GAME_WIDTH, rand()%GAME_HEIGHT));
    player.setCollisionType(entityNS::BOX);
    player.setEdge(COLLISION_BOX_player);
    player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(playerNS::SCALE);

	highScoreText = "";



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

	bossPatternStepIndex = rand()%maxBossPatternSteps;


	for (int i = 0; i< maxBossPatternSteps; i++)
	{
		bossPatterns[i].initializeBoss(&boss);
		bossPatterns[i].setActive();
	}


	bossPatterns[0].setAction(TRACK);
	bossPatterns[0].setTimeForStep(2);
	bossPatterns[1].setAction(RIGHT);
	bossPatterns[1].setTimeForStep(2);
	bossPatterns[2].setAction(DOWN);
	bossPatterns[2].setTimeForStep(2);
	bossPatterns[3].setAction(LEFT);
	bossPatterns[3].setTimeForStep(2);
	bossPatterns[4].setAction(UP);
	bossPatterns[4].setTimeForStep(2);
	bossPatterns[5].setAction(TRACK);
	bossPatterns[5].setTimeForStep(5);
	bossPatterns[6].setAction(TRACK);
	bossPatterns[6].setTimeForStep(3);
	bossPatterns[7].setAction(TRACK);
	bossPatterns[7].setTimeForStep(9);

	playerNextLaserIndex = 0;
	enemyNextLaserIndex = 0;
	bossNextLaserIndex = 0;
	bool shootKeyDownLastFrame = false;
	scoreMsg = "";
	score = 0;
	playerFrames=2;
	bool backCheatKeyPressedLastFrame = false;
	backAnyKeyPressedLastFrame = false;
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
	maiman = false;
	sp = false;
	LEVEL_UP_MSG = "LEVEL ";
	optionsScreenMSG = "MUSIC IS CURRENTLY";
	currentEnemyMaxHits = 0;
	currentBossMaxHits = 0;
	toggleKeyPressedLastFrame = false;
	for(int i = 0;i<24;i++)
	{
		anyCheatKeyPressedLastFrame[i] = false;
	}
	for(int i = 0;i<24;i++)
	{
		anyKeyPressedLastFrame[i] = false;
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
	if(input->isKeyDown(VK_ESCAPE))
		this->exitGame();
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
	case LOSE_SCREEN:
		for(int i = 0x41; i < 0x5A; i++)
		{
			if(input->isKeyDown(i)&&!anyKeyPressedLastFrame[x])
			{
				nameAttempt += i;
				keyPressedThisFrame[x] = true;
			}
			else if((input->isKeyDown(i)) && anyKeyPressedLastFrame[x])
			{
				keyPressedThisFrame[x] = true;
			}
			x++;
		}
		if(input->isKeyDown(VK_BACK)&&!backAnyKeyPressedLastFrame)
		{
			if(nameAttempt.length()>0)nameAttempt.pop_back();
			backKeyPressedThisFrame = true;
		}
		else if(input->isKeyDown(VK_BACK)&&anyKeyPressedLastFrame)
		{
			backKeyPressedThisFrame = true;
		}

		for(int j = 0;j<24;j++)
		{
			anyKeyPressedLastFrame[j] = keyPressedThisFrame[j];
		}
		
		backAnyKeyPressedLastFrame = backKeyPressedThisFrame;
		break;
	case CHEAT:
		for(int i = 0x41; i < 0x5A; i++)
		{
			if(input->isKeyDown(i)&&!anyCheatKeyPressedLastFrame[x])
			{
				cheatAttempt += i;
				keyPressedThisFrame[x] = true;
			}
			else if((input->isKeyDown(i)) && anyCheatKeyPressedLastFrame[x])
			{
				keyPressedThisFrame[x] = true;
			}
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
		enemiesRemain = false; //assume enemies dont remain until check for active enemies
		if(forcefield)
			player.getShield()->setVisible();
		if(sp)
			player.setHasEmp(true);
		if(input->isKeyDown(player_LEFT))
				player.left();
		if(input->isKeyDown(player_RIGHT))
				player.right();
		if(input->isKeyDown(player_UP) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y > 0))
				player.up();
		if(input->isKeyDown(player_DOWN) && (magSquared < playerNS::MAX_VELOCITY_SQUARED || playerVel.y < 0))
				player.down();
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
			if(!(rand()%10000)&&enemy[i].getVisible()&&!enemy[i].getEMP()->getActive()) 
			{
 				enemy[i].getEMP()->setActive(true);
				enemy[i].getEMP()->setVisible();
			}
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
			if(enemy[i].getEMP()->getActive())
			{
 				enemy[i].setEMPCounter(enemy[i].getEMPCounter()+frameTime);
				enemy[i].getEMP()->setScale(enemy[i].getEMP()->getScale()*EMPNS::growthRatePerFrame);
				if(enemy[i].getEMPCounter()>=EMPNS::maxEMPTime)
				{
					enemy[i].getEMP()->setActive(false);
					enemy[i].getEMP()->setInvisible();
					enemy[i].setEMPCounter(0);
					enemy[i].getEMP()->resetScale();
				}
			}
		if(input->isKeyDown(LAUNCH_EMP) && player.getHasEmp())
		{
			player.setHasEmp(false);
			player.getEMP()->setVisible();
		}
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

		
		if(boss.getVisible())
		{
			if(!(rand()%500))
			{
				(bossLaser[bossNextLaserIndex]).setVisible();
				(bossLaser[bossNextLaserIndex]).setPositionX((boss.getPositionX()+bossNS::WIDTH/4));//Center of the boss's width
				(bossLaser[bossNextLaserIndex]).setPositionY((boss.getPositionY()+bossNS::HEIGHT/4));//top of boss
				float rad = boss.getRadians();
				VECTOR2 laserVelocityBoss((boss.getVelocity()).x+bossLaserNS::SPEED_X*sin(boss.getRadians()),-boss.getVelocity().y+bossNS::SPEED_Y*cos(boss.getRadians()));
				(bossLaser[bossNextLaserIndex]).setVelocity(laserVelocityBoss);
				(bossLaser[bossNextLaserIndex]).setRadians(boss.getRadians());
				bossNextLaserIndex=(bossNextLaserIndex+1)%MAX_BOSS_LASERS;
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
		for(int i = 0; i < MAX_BOSS_LASERS; i++)
		{
			bossLaser[i].update(frameTime);
		}
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
		{
			enemy[i].update(frameTime);
			bonus[i].update(frameTime);
			bonus[i].setPos(enemy[i].getPositionX(),enemy[i].getPositionY());
			empPowerup[i].update(frameTime);
			empPowerup[i].setPos(enemy[i].getPositionX(),enemy[i].getPositionY());
		}

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
		for(int i = 0; i < NUM_ENEMIES_INITIAL;i++)
		{
			if((enemy[i]).getVisible())
			{
				enemiesRemain = true;
				break;
			}
		}
		if(!enemiesRemain)
		{
			boss.setVisible();
		}
		if(boss.getVisible())
		{
			boss.update(frameTime);
			if(boss.getHealth() >= 60.00)
			{
				boss.setFrames(2,3);
			}
			else if(boss.getHealth() <= 60.00 && boss.getHealth() > 30.00)
			{
				boss.setFrames(10,11);
			}
			else if(boss.getHealth() <= 30.00 && boss.getHealth() >= 0.00)
			{
				boss.setFrames(18,19);
			}
			else if(boss.getHealth() <= 0.00)
			{
				boss.setInvisible();
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
		nameAttempt = "";
		gameState = LOSE_SCREEN;
		timeInState = 0;
	}
	else if(gameState==LOSE_SCREEN && input->isKeyDown(VK_SPACE))
	{
		if(nameAttempt=="")nameAttempt="anonymous";
		hs.insertHighScore(nameAttempt,score);
		gameState = MENU;
		timeInState = 0;
	}
	else if(gameState==GAME_PLAY && !enemiesRemain && !boss.getVisible())
	{
		score+=25;
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
		if(!strcmp(cheatAttempt.c_str(),LASER_CHEAT))
		{
			cheatAttempt = "";
			maiman = true;
			for(int i = 0; i < MAX_PLAYER_LASERS; i++)
				playerLaser[i].setScale(10);
		}
		if(!strcmp(cheatAttempt.c_str(),EMP_CHEAT))
		{
			cheatAttempt = "";
			sp = true;
		}
	}
	else if(gameState==MENU && mainMenu->getSelectedItem()==3)
	{
		mainMenu->setSelectedItem(-1);
		gameState = INSTR;
		timeInState = 0;
	}
	else if(gameState == INSTR && input->isKeyDown(VK_SPACE))
	{
		gameState = MENU;
		timeInState = 0;
	}
	else if(gameState==MENU && mainMenu->getSelectedItem()==4)
	{
		mainMenu->setSelectedItem(-1);
		gameState = HIGH_SCORES;
		timeInState = 0;
		highScoreText = hs.getHighScoreString();
	}
	else if(gameState == HIGH_SCORES && input->isKeyDown(VK_SPACE))
	{
		gameState = MENU;
		timeInState = 0;
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
	boss.ai(frameTime, player);
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

	if (bossPatterns[bossPatternStepIndex].isFinished())
	{
		if((boss).getVisible())
		{
			bossPatterns[bossPatternStepIndex].setActive();
			bossPatternStepIndex=(rand())%maxBossPatternSteps;
		}				
	}

	bossPatterns[bossPatternStepIndex].updateBoss(frameTime);
}

//=============================================================================
// Handle collisions
//=============================================================================
void CollisionTypes::collisions()
{
    collisionVector = D3DXVECTOR2(0,0);
	collision = false;
	if(!player.getShield()->getActive()&&!player.getEMP()->getActive())//shield and emp are not active
	{
		if(boss.getVisible())
		{
			if (player.collidesWith(boss, collisionVector) && boss.getVisible() && player.getVisible())//player with boss collision
			{
				player.setHealth(player.getHealth() - bossKamikazeDamage);
			}
			for(int i = 0;i<MAX_BOSS_LASERS;i++)
			{
				if(bossLaser[i].getVisible())
				{
					if (player.collidesWith(bossLaser[i], collisionVector))
					{
						player.setHealth(player.getHealth() - BOSS_LASER_DAMAGE);
						bossLaser[i].setInvisible();
						audio->playCue(LASER);
					}
				}
			}
		}
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
			if (player.collidesWith(empPowerup[i], collisionVector) && empPowerup[i].getVisible() && player.getVisible())
			{
				player.setHasEmp(true);
				empPowerup[i].setInvisible();
				break;
			}
			if (player.collidesWith(*(enemy[i]).getEMP(), collisionVector) && enemy[i].getEMP()->getActive() && player.getVisible())
			{
				player.setHealth(player.getHealth() - empDamage);
				(enemy[i]).getEMP()->setActive(false);
				//audio->playCue(CRASH);
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
	else//shield and/or emp is active
	{
		for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
		{
			//shield with enemy collision
			if (player.getShield()->collidesWith(enemy[i], collisionVector) /*&& enemy[i].getVisible()*/ /*&& player.getVisible()*/)
			{	
				enemy[i].setInvisible();
				audio->playCue(CRASH);
				player.getShield()->setInvisible();
				break;
			}
			//forcefield with enemy collision
			if (player.getEMP()->collidesWith(enemy[i], collisionVector) /*&& enemy[i].getVisible()*/ /*&& player.getVisible()*/)
			{	
				enemy[i].setInvisible();
				break;
			}
			if (player.getShield()->collidesWith(boss, collisionVector) && boss.getVisible() && player.getVisible())//player with boss collision
			{
				audio->playCue(CRASH);
				player.getShield()->setInvisible();
				break;
			}
			if (player.getShield()->collidesWith(bonus[i], collisionVector) && bonus[i].getVisible() && player.getVisible())
			{
				player.getShield()->setVisible();
				bonus[i].setInvisible();
				break;
			}
			if (player.getShield()->collidesWith(empPowerup[i], collisionVector) && empPowerup[i].getVisible() && player.getVisible())
			{
				player.setHasEmp(true);
				empPowerup[i].setInvisible();
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
		for(int i = 0;i<MAX_ENEMY_LASERS;i++)
		{
			if(bossLaser[i].getVisible())
			{
				if (player.getShield()->collidesWith(bossLaser[i], collisionVector))
				{
					player.setHealth(player.getHealth() - laserDamage);
					bossLaser[i].setInvisible();
					audio->playCue(LASER);
				}
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
					int r = rand()%6;
					if(r==0)
					{
						bonus[j].setVisible();
					}
					else if(r==1)
					{
						empPowerup[j].setVisible();
					}
					score+=4;
				}
				//update score
			}
		}
	}
	//player laser boss collision
	if(boss.getActive())
	{
		for(int x = 0;x<MAX_PLAYER_LASERS;x++)
		{
			if(boss.collidesWith(playerLaser[x], collisionVector) && playerLaser[x].getVisible())
			{
				playerLaser[x].setInvisible();
				boss.wasHit();
				score++;
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
		ss2 << nameAttempt;
		graphics->spriteBegin();// begin drawing sprites
		lose.draw();
		dxFontSmall->print(scoreMsg,GAME_WIDTH*.85,GAME_HEIGHT*.01);
		levelOutput->print(ss2.str(), GAME_WIDTH*.53,GAME_HEIGHT*.75);
		levelOutput->print("Enter Name (Space to Continue): ", GAME_WIDTH*.1,GAME_HEIGHT*.75);
		graphics->spriteEnd();
		break;
	case INSTR:
		graphics->spriteBegin();// begin drawing sprites
		instr.draw();
		graphics->spriteEnd();
		break;
	case HIGH_SCORES:
		graphics->spriteBegin();// begin drawing sprites
		high_scores.draw();
		dxFontSmall->print(highScoreText,GAME_WIDTH*.4,GAME_HEIGHT*.2);
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
		dxFontSmall->print(scoreMsg,GAME_WIDTH*.85,GAME_HEIGHT*.01);//draw score message
		if(boss.getVisible())
		{
			for(int i = 0; i < MAX_BOSS_LASERS; i++)
			{
				bossLaser[i].draw();
			}
			boss.draw();
		}
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
			empPowerup[i].draw();
			if((enemy[i]).getEMP()->getActive())
				((enemy[i]).getEMP()->draw());
		}
		(*player.getEMP()).draw();
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
	boss.setHealth(100.00);
	boss.setInvisible();
	currentBossMaxHits += 15;
	boss.setMaxHits(currentBossMaxHits);
	audio->stopCue(BACKGROUND);
	if(musicOn)
		audio->playCue(BACKGROUND);
	currentEnemyMaxHits+=3;
	for(int i = 0; i < NUM_ENEMIES_INITIAL; i++)
	{
		(enemy[i]).setMaxHits(currentEnemyMaxHits);
		(enemy[i]).getEMP()->setActive(false);
		(enemy[i]).setEMPCounter(0);
		enemy[i].getEMP()->setInvisible();
		enemy[i].getEMP()->resetScale();
	}
	player.setVisible();
	player.getShield()->setInvisible();
	player.getEMP()->setInvisible();
	player.setHasEmp(false);
	player.getEMP()->resetScale();
	player.setPosition(VECTOR2(rand()%GAME_WIDTH, rand()%GAME_HEIGHT));
	int bossY =rand()%(GAME_HEIGHT-boss.getHeight());
	int bossX =rand()%(GAME_WIDTH-boss.getWidth());
	boss.setPosition(VECTOR2(bossX,bossY));
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
		empPowerup[i].setPos(width, height);
		empPowerup[i].setInvisible();
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
	for(int i = 0; i<MAX_ENEMY_LASERS;i++)
	{
		(bossLaser[i]).setInvisible();
	}
}
