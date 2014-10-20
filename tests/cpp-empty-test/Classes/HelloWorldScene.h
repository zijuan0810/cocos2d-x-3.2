#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* scene();

public:
    virtual bool init();  

    CREATE_FUNC(HelloWorld);
protected:
    void menuCloseCallback(Ref* sender);
};

#endif // __HELLOWORLD_SCENE_H__
