#ifndef _COMMON_H_
#define _COMMON_H_

#include "cocos2d.h"
using namespace cocos2d;

enum InDirState {
	IN_DIR_RIGHT = 0, IN_DIR_UP, 
	IN_DIR_LEFT, IN_DIR_DOWN, 
	IN_DIR_NO_PRESSED};

enum InBtnState {
	IN_BTN_PRESSED, IN_BTN_NO_PRESSED};

enum EnemyState {
	E_IDLE = 0, E_RUN, E_ATTACK, E_HURT, E_DIE, E_NONE};

enum DirectState {
	LEFT = 0, RIGHT};

enum HeroBodyState {
	RUN = 0, IDLE, NONE};

enum WeaponState {
	WEAPON_READY, NO_BULLET, NO_COOL_DOWN};

typedef struct _BoundingBox
{
	CCRect actual;
	CCRect original;
} BoundingBox;

const float WIN_WIDTH = 480;
const float WIN_HEIGHT = 320;

#endif