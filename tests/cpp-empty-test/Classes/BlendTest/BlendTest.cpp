#include "BlendTest.h"

BlendTest::BlendTest()
{

}

BlendTest::~BlendTest()
{

}

bool BlendTest::init()
{
	if (!Layer::init()) {
		return false;
	}

	return true;
}
