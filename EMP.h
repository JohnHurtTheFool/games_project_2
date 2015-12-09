// file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef EMP_H                 // Prevent multiple definitions if this 
#define EMP_H   
#include "entity.h"
#include "constants.h"

namespace EMPNS
{
    const int WIDTH = 512;                   // image width
    const int HEIGHT = 512;                  // image height
	const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
	const float maxEMPTime = .5f;
	const float originalScale = .1f;
	const float growthRatePerFrame = 1.02f;
}
class EMP :public Entity
{
private:
	float currentTime;
	bool counterActive;
	float maxEMPTime;
public:
	void setMaxEMPTime(float time){maxEMPTime = time;};
	float getMaxEMPTime(){return maxEMPTime;}
	float getCurrentEMPTime(){return currentTime;}
	void setCurrentEMPTime(float time){currentTime = time;}
	EMP(void);
	~EMP(void);
	virtual void draw();
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
	float getCurrWidth(){return EMPNS::WIDTH*getScale();}
	float getCurrHeight(){return EMPNS::HEIGHT*getScale();}
    void update(float frameTime);
	void resetScale(){setScale(EMPNS::originalScale);}
	void EMP::setInvisible()
	{
		counterActive = false;
		currentTime = 0.0f;
		Image::setVisible(false);
		active = false;
	}

	void EMP::setVisible()
	{
		counterActive = true;
		Image::setVisible(true);
		active = true;
	}

	virtual void setCollisionType(entityNS::COLLISION_TYPE ctype)
    {collisionType = ctype;}
	void setEdge(RECT e) {edge = e;}
	void setPos(double x, double y);
	entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}
};
#endif
