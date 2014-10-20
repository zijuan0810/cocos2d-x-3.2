#ifndef _SHADERSPRITE_H_
#define _SHADERSPRITE_H_

#include "NextenMacros.h"
#include "cocos2d.h"

#include <string>

NS_NEXT_BEGIN

class ShaderSprite : public cocos2d::Sprite
{
public:
	static ShaderSprite* create(const char* vshFileName, const char* fshFileName, const char* key = "_SHADER_SPRITE_KEY");

public:
	ShaderSprite();
	~ShaderSprite();

	virtual bool initWithShader(const char* vshFileName, const char* fshFileName, const char* key = "_SHADER_SPRITE_KEY");

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);

protected:
	virtual void buildCustomUniforms() {};
	virtual void setCustomUniforms() {};

	//virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

private:
	void onDraw(const cocos2d::Mat4 &transform, bool transformUpdated);

protected:
	std::string _fshSourceFile;
	std::string _vshSourceFile;

	cocos2d::CustomCommand _customCommand;
};

NS_NEXT_END

#endif // _SHADERSPRITE_H_