#include "SneakyButton.h"

using namespace cocos2d;

void SneakyButton::onEnterTransitionDidFinish()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchCancelled = CC_CALLBACK_2(SneakyButton::ccTouchCancelled, this);
	listener->onTouchMoved = CC_CALLBACK_2(SneakyButton::ccTouchMoved, this);
	listener->onTouchBegan = CC_CALLBACK_2(SneakyButton::ccTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(SneakyButton::ccTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SneakyButton::onExit()
{
	_eventDispatcher->removeAllEventListeners();
}

bool SneakyButton::initWithRect(CCRect rect)
{
	bool pRet = false;
	//if(CCSprite::init()){
		
		bounds = CCRectMake(0, 0, rect.size.width, rect.size.height);
		center = CCPointMake(rect.size.width/2, rect.size.height/2);
		status = true; //defaults to enabled
		active = false;
		value = false;
		isHoldable = false;
		isToggleable = false;
		radius = 32.0f;
		rateLimit = 1.0f/120.0f;
		
		setPosition(rect.origin); //not sure about this
		pRet = true;
	//}
	return pRet;
}

void SneakyButton::limiter(float delta)
{
	value = false;
	this->unschedule(schedule_selector(SneakyButton::limiter));
	active = false;
}

void SneakyButton::setRadius(float r)
{
	radius = r;
	radiusSq = r*r;
}

bool SneakyButton::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	if (active) return false;
	
	CCPoint location = 
		CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = this->convertToNodeSpace(location);
		//Do a fast rect check before doing a circle hit check:
	if(location.x < -radius || location.x > radius 
		|| location.y < -radius || location.y > radius){
		return false;
	}else{
		float dSq = location.x*location.x + location.y*location.y;
		if(radiusSq > dSq){
			active = true;
			if (!isHoldable && !isToggleable){
				value = true;
				this->schedule(schedule_selector(SneakyButton::limiter), rateLimit);
			}
			if (isHoldable) value = true;
			if (isToggleable) value = !value;
			return true;
		}
	}
	return false;
}

void SneakyButton::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
	if (!active) return;
	
	CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
	location = this->convertToNodeSpace(location);
		//Do a fast rect check before doing a circle hit check:
	if(location.x < -radius || location.x > radius 
		|| location.y < -radius || location.y > radius){
		return;
	}else{
		float dSq = location.x*location.x + location.y*location.y;
		if(radiusSq > dSq){
			if (isHoldable) value = true;
		}
		else {
			if (isHoldable) value = false; active = false;
		}
	}
}

void SneakyButton::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
	if (!active) return;
	if (isHoldable) value = false;
	if (isHoldable||isToggleable) active = false;
}

void SneakyButton::ccTouchCancelled(CCTouch *touch, CCEvent *event)
{
	this->ccTouchEnded(touch, event);
}

void SneakyButton::touchDelegateRelease()
{
	this->release();
}

void SneakyButton::touchDelegateRetain()
{
	this->retain();
}
