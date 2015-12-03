// file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef ENEMY_H                 // Prevent multiple definitions if this 
#define ENEMY_H   
#include "entity.h"
#include "constants.h"
#include "graphics.h"
namespace EMPNS
{
    const int WIDTH = 100;                   // image width
    const int HEIGHT = 100;                  // image height
	const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
	const float SHIELD_SCALE=0.5;
   const float maxShieldTime = 15.00f;
   const int MAX_HITS = 2;
}
class EMP :public Entity
{
private:
	float currentTime;
public:
	EMP(void);
	~EMP(void);
};
#endif
