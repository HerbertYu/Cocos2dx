#ifndef _CARROT_H_
#define _CARROT_H_

#include "Enemy.h"

class Carrot : public Enemy
{
public :
	Carrot();
	~Carrot();
	bool initAnimation();
};

#endif