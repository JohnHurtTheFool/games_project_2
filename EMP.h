// file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef ENEMY_H                 // Prevent multiple definitions if this 
#define ENEMY_H   
#include "entity.h"
#include "constants.h"
#include "graphics.h"
class EMP :public Entity
{
public:
	EMP(void);
	~EMP(void);
};
#endif
