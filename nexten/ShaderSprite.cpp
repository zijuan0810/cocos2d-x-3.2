#include "ShaderSprite.h"

using namespace cocos2d;


NS_NEXT_BEGIN

ShaderSprite* ShaderSprite::create(const char* vshFileName, const char* fshFileName, const char* key /* = "_SHADER_SPRITE_KEY" */) 
{
	ShaderSprite* s = new ShaderSprite();
	if (s && s->initWithShader(vshFileName, fshFileName, key)) {
		s->autorelease();
		return s;
	}

	CC_SAFE_DELETE(s);
	return nullptr;
}

ShaderSprite::ShaderSprite()
{
}

ShaderSprite::~ShaderSprite()
{
}

bool ShaderSprite::initWithShader(const char* vshFileName, const char* fshFileName, 
	const char* key /* = "_SHADER_SPRITE_KEY" */)
{
	if (!Sprite::init()) {
		return false;
	}

	if (vshFileName != nullptr) {
		_vshSourceFile = vshFileName;
	}

	if (fshFileName == nullptr) {
		return false;
	}
	_fshSourceFile = fshFileName;

	GLProgram* program = ShaderCache::getInstance()->programForKey(key);
	if (program == nullptr) {
		auto fshFile = FileUtils::getInstance()->fullPathForFilename(_fshSourceFile);
		GLchar* fshSource = (GLchar*)String::createWithContentsOfFile(fshFile)->getCString();

		if (vshFileName != nullptr) {
			auto vshFile = FileUtils::getInstance()->fullPathForFilename(_fshSourceFile);
			auto vshSource = (GLchar*)String::createWithContentsOfFile(vshFile)->getCString();
			program = GLProgram::createWithByteArrays(vshSource, fshSource);
		}
		else {
			program = GLProgram::createWithByteArrays(ccPositionTextureColor_vert, fshSource);
		}
		CHECK_GL_ERROR_DEBUG();

		program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		program->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
		CHECK_GL_ERROR_DEBUG();

		program->link();
		CHECK_GL_ERROR_DEBUG();

		program->updateUniforms();
		CHECK_GL_ERROR_DEBUG();

		this->buildCustomUniforms();
		CHECK_GL_ERROR_DEBUG();

		ShaderCache::getInstance()->addProgram(program, key);
	}

	setShaderProgram(program);
	CHECK_GL_ERROR_DEBUG();

	return true;
}

void ShaderSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(ShaderSprite::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void ShaderSprite::onDraw(const kmMat4 &transform, bool transformUpdated)
{
	auto shader = this->getShaderProgram();
	shader->use();
	shader->setUniformsForBuiltins(transform);

	this->setCustomUniforms();

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::blendFunc(_blendFunc.src, _blendFunc.dst);
	GL::bindTexture2D(this->getTexture()->getName());


	//
	// Attributes
	//
#define kQuadSize sizeof(_quad.bl)
	size_t offset = (size_t)&_quad;

	// vertex
	int diff = offsetof(V3F_C4B_T2F, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// texCoods
	diff = offsetof(V3F_C4B_T2F, texCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof(V3F_C4B_T2F, colors);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
}


NS_NEXT_END