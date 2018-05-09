#include "TexturedModel.h"


TexturedModel::TexturedModel(void)
{
}


TexturedModel::~TexturedModel(void)
{
}

void TexturedModel::Draw()
{
	if (texture != NULL)
		texture->Bind();
	Model::Draw();
}