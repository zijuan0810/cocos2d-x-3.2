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

	CREATE_FUNC(BlendTest);
};

#endif // _BLENDTEST_H_