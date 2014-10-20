#ifndef _BLENDTEST_H_
#define _BLENDTEST_H_

#include "cocos2d.h"

using namespace cocos2d;

class BlendTest : public Layer
{
public:
	BlendTest();
	~BlendTest();

	virtual bool init();

	void refreshCalendar();
	int getCalendarIdx() const;
	Sprite* createMaskedSprite(Sprite* textureSprite, Sprite* maskSprite);

	CREATE_FUNC(BlendTest);

protected:
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void addBlendSpriteWithShader();
	void addBlendSpriteWithoutShader();

private:
	int		_calendarIdx;
	Sprite*	_calendar;
};

#endif // _BLENDTEST_H_