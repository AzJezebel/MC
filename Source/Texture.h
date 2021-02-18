#pragma once

#include "glad/glad.h"
#include <iostream>

namespace Minecraft
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& path);
		bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

		GLuint GetTextureID() const { return m_textureId; }
		GLuint& GetTextureID();
		int& GetTextureWidth();
		int& GetTextureHeight();
	private:
		GLuint m_textureId = 0;
		int m_textureWidth = 0;
		int m_textureHeight = 0;
	};
}