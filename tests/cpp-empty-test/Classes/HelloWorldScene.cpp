#include "HelloWorldScene.h"

#include "BlendTest/BlendTest.h"

USING_NS_CC;


Scene* HelloWorld::scene()
{
    auto scene = Scene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() ) {
        return false;
    }
    
	const Size& winSize = Director::getInstance()->getWinSize();

	// Blend Test
	//auto aBlendTest = BlendTest::create();
	//this->addChild(aBlendTest);

	auto icon = Sprite::create("Calendar2.png");
	icon->setPosition(Vec2(winSize / 2.0f));
	this->addChild(icon);
    
    return true;
}