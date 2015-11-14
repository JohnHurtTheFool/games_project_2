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
#include <string>
#include "player.h"
#include "puck.h"
#include "enemy.h"
#include "shield.h"
#include "bonus.h"
#include "patternStep.h"
#include "PlayerLaser.h"
#include "testDx.h"
#include "EnemyLaser.h"
#include "menu.h"

#define maxPatternSteps 7

//=============================================================================
// This class is the core of the game
//=============================================================================
class CollisionTypes : public Game
{
private:
    // game items
	GAME_STATES gameState;
    TextureManager playerTM, puckTM, enemyTM, backgroundTM, playerLaserTM, enemyLaserTM, shieldTM, bonusTM, splashTM, loseTM;   // game texture
	Image background, splash, lose;
    VECTOR2 collisionVector;    // collision vector
	Player player;
	Puck puck;
	Bonus bonus[NUM_ENEMIES_INITIAL];
	int score;
	bool collision;
	Enemy enemy[NUM_ENEMIES_INITIAL];
	PlayerLaser playerLaser [MAX_PLAYER_LASERS];
	EnemyLaser enemyLaser [MAX_ENEMY_LASERS];
	PatternStep allPatterns[NUM_ENEMIES_INITIAL][maxPatternSteps];
	int patternStepIndex[NUM_ENEMIES_INITIAL];
	int playerNextLaserIndex;//The index of the next laser to shoot.
	int enemyNextLaserIndex;//The index of the next laser to shoot.
	bool shootKeyDownLastFrame;
	TextDX *dxFontSmall;
	std::string scoreMsg;
	int playerFrames;
	float timeInState;
	Menu *mainMenu;
	TextDX *output;
	TextDX *levelOutput;
	std::string outString;
	void levelReset();
	int currentEnemyMaxHits;
	int levelNumber;
	std::string LEVEL_UP_MSG;
	std::string optionsScreenMSG;
	std::string cheatAttempt;
	std::string cheatMSG;
	bool musicOn;
	bool toggleKeyPressedLastFrame;
	bool anyCheatKeyPressedLastFrame;
	bool forcefield;
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
	void updateState();
    void releaseAll();
    void resetAll();
};

#endif
