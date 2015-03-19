#ifndef _HOE_H_
#define _HOE_H_

#include "Weapon.h"

class Hoe : public Weapon
{
public :
	Hoe();
	~Hoe();
	bool initAnimation();
};

#endif