#include "TextureManager.h"


#include <iostream>

Minecraft::Texture& Minecraft::TextureManager::GetTexture(const std::string& str)
{
	auto [texIt, inserted] = m_textureStore.emplace(str, str);
	return texIt->second;
}
	