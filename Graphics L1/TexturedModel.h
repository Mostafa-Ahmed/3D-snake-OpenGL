#ifndef TexturedModel_h__
#define TexturedModel_h__

#include "glm/glm.hpp"
#include <vector>
#include "gl\glew.h"
#include "texture.h"
#include "ShaderProgram.h"
#include "Model.h"

class TexturedModel : public Model
{

public:
	TexturedModel();
	~TexturedModel();
	Texture* texture;
	void Draw();
};
#endif // Model_h__

