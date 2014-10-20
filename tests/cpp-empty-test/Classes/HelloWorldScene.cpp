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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

	// Blend Test
	auto aBlendTest = BlendTest::create();
	this->addChild(aBlendTest);
    
    return true;
}