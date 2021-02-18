#pragma once
#include <map>
#include <string>
#include "glad/glad.h"
#include "Texture.h"

namespace Minecraft
{
	class TextureManager
	{
	public:
		TextureManager() = default;
		~TextureManager() = default;

		Texture& GetTexture(const std::string& str);

	private:

		std::map<std::string, Texture>	m_textureStore;

	};
}

