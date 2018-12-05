#include "Shader.h"


Shader::~Shader()
{
}

void Shader::UseTexture( std::string textureName )
{
	Use();
	glActiveTexture( GL_TEXTURE0 + textures[textureName]->GetTextureNum() );
	glBindTexture( GL_TEXTURE_2D, textures[textureName]->GetTextureId() );
	glUniform1i(glGetUniformLocation(program, "texture1"), textures[textureName]->GetTextureNum());
}

void Shader::LoadTexture( const char* path, const char* textureUniformName, std::string textureName )
{
	//Define o uso do shader atual
	Use(); 
	//Carrega a textura
	Texture* tempTexture = new Texture();
	tempTexture->Load( path, textureUniformName, program, textureQtd );
	textures[textureName] = tempTexture;
	textureQtd += 1;
}
