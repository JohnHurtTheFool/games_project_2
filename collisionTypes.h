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
#include "EMPpowerup.h"
#include "highscore.h"
#include "boss.h"
#include "bossLaser.h"
#include "particleManager.h"

#define maxPatternSteps 7
#define maxBossPatternSteps 8

//=============================================================================
// This class is the core of the game
//=============================================================================
class CollisionTypes : public Game
{
private:
    // game items
	GAME_STATES gameState;
	highscore hs;
    TextureManager playerTM, puckTM, enemyTM, backgroundTM,
		playerLaserTM, enemyLaserTM, shieldTM, bonusTM, splashTM,
		loseTM, instrTM,empPowerupTM,high_scoresTM,EMPTM,bossTM,bossLaserTM;   // game texture
	Image background, splash, lose, instr, high_scores;
	bool enemiesRemain;
    VECTOR2 collisionVector;    // collision vector
	ParticleManager pm;
	Player player;
	Puck puck;
	Bonus bonus[NUM_ENEMIES_INITIAL];
	EMPpowerup empPowerup[NUM_ENEMIES_INITIAL];
	int score;
	bool collision;
	Enemy enemy[NUM_ENEMIES_INITIAL];
	Boss boss;
	PlayerLaser playerLaser [MAX_PLAYER_LASERS];
	EnemyLaser enemyLaser [MAX_ENEMY_LASERS];
	BossLaser bossLaser [MAX_ENEMY_LASERS];
	PatternStep allPatterns[NUM_ENEMIES_INITIAL][maxPatternSteps];
	PatternStep bossPatterns[maxBossPatternSteps];
	int bossPatternStepIndex;
	int patternStepIndex[NUM_ENEMIES_INITIAL];
	int playerNextLaserIndex;//The index of the next laser to shoot.
	int enemyNextLaserIndex;//The index of the next laser to shoot.
	int bossNextLaserIndex;//The index of the next laser to shoot.
	bool shootKeyDownLastFrame;
	TextDX *dxFontSmall;
	std::string scoreMsg;
	string highScoreText;
	int playerFrames;
	float timeInState;
	Menu *mainMenu;
	TextDX *output;
	TextDX *levelOutput;
	std::string outString;
	void levelReset();
	int currentEnemyMaxHits;
	int currentBossMaxHits;
	int levelNumber;
	float gameEndTime;
	std::string LEVEL_UP_MSG;
	std::string optionsScreenMSG;
	std::string cheatAttempt;
	std::string nameAttempt;
	std::string cheatMSG;
	bool musicOn;
	bool toggleKeyPressedLastFrame;
	bool anyCheatKeyPressedLastFrame[24];
	bool anyKeyPressedLastFrame[24];
	bool backAnyKeyPressedLastFrame;
	bool backCheatKeyPressedLastFrame;
	bool forcefield;
	bool maiman;
	bool sp;
public:
    // Constructor
    CollisionTypes();

    // Destructor
    virtual ~CollisionTypes();

	void createParticleEffect(VECTOR2 pos, VECTOR2 vel, int numParticles);

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
