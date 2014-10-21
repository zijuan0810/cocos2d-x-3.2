#include "BlendTest.h"
#include "nexten.h"

#include <random>
#include <functional>

using namespace nexten;

BlendTest::BlendTest()
	: _calendarIdx(1)
	, _calendar(nullptr)
{
}

BlendTest::~BlendTest()
{
}

/*
bool BlendTest::init()
{
	if (!Layer::init()) {
		return false;
	}

	// add touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(BlendTest::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(BlendTest::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(BlendTest::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->refreshCalendar();

	return true;
}
*/

int BlendTest::getCalendarIdx() const
{
	std::default_random_engine generator(time(nullptr));
	std::uniform_int_distribution<int> dis(1, 3);
	auto dice = std::bind(dis, generator);

	int i = _calendarIdx;
	do {
		i = dice();
	} while (i == _calendarIdx);

	return i;
}

bool BlendTest::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
	_calendarIdx = this->getCalendarIdx();
	this->refreshCalendar();
	return true;
}

void BlendTest::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void BlendTest::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{

}

void BlendTest::refreshCalendar()
{
	char image[100] = { 0 };
	sprintf(image, "Calendar%d.png", _calendarIdx);

	if (_calendar == nullptr) {
		_calendar = Sprite::create(image);
		//_calendar->setPosition(Point(Director::getInstance()->getWinSize() / 2.0f));
		//this->addChild(_calendar);

		Sprite* mask = Sprite::create("CalendarMask.png");

		Sprite* spMask = this->createMaskedSprite(_calendar, mask);
		spMask->setPosition(Point(Director::getInstance()->getWinSize() / 2.0f));
		this->addChild(spMask);
	}
	else {
		_calendar->setTexture(image);
	}
}

Sprite* BlendTest::createMaskedSprite(Sprite* textureSprite, Sprite* maskSprite)
{
	const Size& maskedSize = maskSprite->getContentSize();
	RenderTexture* renderTex = RenderTexture::create(maskedSize.width, maskedSize.height);

	maskSprite->setPosition(Point(maskedSize / 2.0f));
	textureSprite->setPosition(Point(textureSprite->getContentSize() / 2.0f));

	maskSprite->setBlendFunc(BlendFunc{ GL_ONE, GL_ZERO });
	textureSprite->setBlendFunc(BlendFunc{ GL_DST_ALPHA, GL_ZERO });

	renderTex->begin();
	maskSprite->visit();
	textureSprite->visit();
	renderTex->end();

	Sprite* ret = Sprite::createWithTexture(renderTex->getSprite()->getTexture());
	ret->setFlippedY(true);
	return ret;
}

bool BlendTest::init()
{
	if (!Layer::init()) {
		return false;
	}

	this->addBlendSpriteWithoutShader();

	return true;
}

void BlendTest::addBlendSpriteWithoutShader()
{
	const Size& winSize = Director::getInstance()->getWinSize();

	// 创建遮罩图片
	Sprite *maskSprite = Sprite::create("CalendarMask.png");
	maskSprite->retain();
	maskSprite->setPosition(Point(maskSprite->getContentSize() / 2.0f));

	// 创建被遮罩图片
	Sprite *flowerSprite = Sprite::create("Calendar1.png");
	flowerSprite->retain();
	flowerSprite->setPosition(Point(flowerSprite->getContentSize() / 2.0f));

	// 创建干净的画板，在后台进行渲染
	const Size& textureSize = flowerSprite->getContentSize();
	RenderTexture *renderTexture = RenderTexture::create(textureSize.width, textureSize.height);
	renderTexture->retain();
	renderTexture->setPosition(Point(textureSize / 2.0f));

	// 先设置好 遮罩精灵 和 被遮罩精灵 在被渲染的时候采用什么样的颜色混合法则 
	maskSprite->setBlendFunc(BlendFunc{ GL_ONE, GL_ZERO });
	flowerSprite->setBlendFunc(BlendFunc{ GL_DST_ALPHA, GL_ZERO });

	// 开始把各种精灵渲染到画板上
	renderTexture->setKeepMatrix(true);
	renderTexture->begin();
	// 先渲染遮罩精灵。但是因为有个画板先被渲染。所以pMask是第二个被渲染的，即后被渲染。
	// 所以在这一刻pMask是源颜色。调用pMask->visit()的时候吧精灵pMask上的每个像素的RGBA分量和1.0相乘。
	// 所以遮罩图片被元模原样的渲染出来.
	maskSprite->visit();
	// 再渲染被遮罩的精灵.在这一刻,之前先有pMask被渲染。所以pFlower后被渲染。pFlower就是源颜色。
	// 之前的pMask就是目标颜色。
	// 调用pFlower->visit()的时候,精灵pFlower上的对应像素的RGBA分量和pMask上的对应像素的A分量相乘.
	// 因为前面设置了GL_DST_ALPHA。
	flowerSprite->visit();
	// 停止渲染到画板  
	renderTexture->end();

	Sprite* newSprite = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	newSprite->setPosition(Point(winSize / 2.0f));
	newSprite->setFlippedY(true);
	this->addChild(newSprite);

	// 必须延迟删除，因为visit()有调用渲染命令，可能会将上面创建的对象删除掉
	Sequence	*seq = Sequence::create(DelayTime::create(0.0f), CallFunc::create([=] {
		maskSprite->release();
		flowerSprite->release();
		renderTexture->release();
	}), nullptr);

	newSprite->runAction(seq);
}

void BlendTest::addBlendSpriteWithShader()
{
	const Size& winSize = Director::getInstance()->getWinSize();

	Sprite* spCalendar1 = Sprite::create("Calendar1.png");
	spCalendar1->setAnchorPoint(Point(0, 0));
	spCalendar1->setPosition(Point(0, 0));
	spCalendar1->retain();

	ShaderSprite* spCalendarMask = ShaderSprite::create(nullptr, "mask.fsh");
	spCalendarMask->setTexture("CalendarMask.png");
	spCalendarMask->setBlendFunc(BlendFunc{ GL_ZERO, GL_SRC_ALPHA });
	spCalendarMask->setAnchorPoint(Point(0, 0));
	spCalendarMask->setPosition(Point(0, 0));
	spCalendarMask->retain();

	const Size& textureSize = spCalendar1->getContentSize();
	spCalendar1->setPosition(Point(textureSize / 2.0f));
	spCalendarMask->setPosition(Point(textureSize / 2.0f));

	RenderTexture* tmpRender = RenderTexture::create(textureSize.width, textureSize.height);
	tmpRender->setKeepMatrix(true);
	tmpRender->retain();

	tmpRender->begin();
	spCalendar1->visit();
	spCalendarMask->visit();
	tmpRender->end();

	Sprite* newSprite = Sprite::createWithTexture(tmpRender->getSprite()->getTexture());
	newSprite->setPosition(Point(winSize / 2.0f));

	Sequence* seq = Sequence::create(DelayTime::create(0.0f), CallFunc::create([=] {
		spCalendarMask->release();
		spCalendar1->release();
		tmpRender->release();
	}), NULL);

	newSprite->runAction(seq);

	newSprite->setFlippedY(true);
	this->addChild(newSprite);
}
