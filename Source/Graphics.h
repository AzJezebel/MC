#pragma once

#include <glad/glad.h>

#include <string>

#include "World.h"

namespace Minecraft
{
	typedef GLuint ProgramName;
	typedef GLuint ShaderName;
	typedef GLuint TextureName;
	typedef GLuint SamplerName;

	class Graphics
	{
	public:
		Graphics() = delete;
		Graphics(World const& world);
		~Graphics();

		void Update(float dT);
		void DisplayWorld(const Camera& camera) const;
		void DisplayPlayerTarget(TargetBlock target) const;

	private:
		ProgramName CreateBlockShaderProgram(int uniformUnit, int textureUnit);
		ProgramName CreateEntityShaderProgram(int uniformUnit, int textureUnit);
		ProgramName CreateFallingShaderProgram(int uniformUnit, int textureUnit);
		ProgramName CreateFluidShaderProgram(int uniformUnit, int textureUnit);
		ProgramName CreateItemShaderProgram(int uniformUnit, int textureUnit);
		ProgramName CreateTargetShaderProgram(int uniformUnit);
		ProgramName CreateProgram(char const* vertexFile, char const* fragmentFile, char const* geometryFile = nullptr);
		ShaderName LoadShader(std::string const& filePath, GLuint type);

		TextureName LoadAtlasTexture(int textureUnit);

		GLint GetLocation(ProgramName program, char const* attributName) const;

		World const& m_world;

		int m_atlasTextureUnit;
		int m_itemTextureUnit;
		int m_uniformUnit;

		TextureName m_atlasTexture;
		SamplerName m_atlasSampler;

		ProgramName m_solidBlockProgram;
		ProgramName m_fluidBlockProgram;
		ProgramName m_fallingBlockProgram;
		ProgramName m_targetBlockProgram;
		ProgramName m_itemProgram;
		GLuint m_blockVAO;
		GLuint m_itemVAO;
		GLuint m_itemVBO;

		ProgramName m_entityProgram;

		GLuint m_matriceUBO;

		float m_timeElapse = 0.f;
	};
}
