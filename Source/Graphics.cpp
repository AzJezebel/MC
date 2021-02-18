#include "Graphics.h"

#include <iostream>
#include <fstream>
#include <sstream>

#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/gtc/type_ptr.hpp>
#pragma warning ( pop )

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "BlockId.h"

namespace Minecraft
{
	Graphics::Graphics(World const& world) :
		m_world(world),
		m_uniformUnit(0),
		m_atlasTextureUnit(1),
		m_itemTextureUnit(2),
		m_atlasTexture(LoadAtlasTexture(m_atlasTextureUnit)),
		m_solidBlockProgram(CreateBlockShaderProgram(m_uniformUnit, m_atlasTextureUnit)),
		m_fluidBlockProgram(CreateFluidShaderProgram(m_uniformUnit, m_atlasTextureUnit)),
		m_fallingBlockProgram(CreateFallingShaderProgram(m_uniformUnit, m_atlasTextureUnit)),
		m_targetBlockProgram(CreateTargetShaderProgram(m_uniformUnit)),
		m_entityProgram(CreateEntityShaderProgram(m_uniformUnit, m_atlasTextureUnit)),
		m_itemProgram(CreateItemShaderProgram(m_uniformUnit, m_itemTextureUnit))
	{
		glCreateBuffers(1, &m_matriceUBO);
		glBindBufferBase(GL_UNIFORM_BUFFER, m_uniformUnit, m_matriceUBO);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Graphics::~Graphics()
	{
		glDeleteTextures(1, &m_atlasTexture);
		glDeleteProgram(m_solidBlockProgram);
		glDeleteProgram(m_fallingBlockProgram);
		glDeleteProgram(m_fluidBlockProgram);
		glDeleteProgram(m_targetBlockProgram);
		glDeleteProgram(m_entityProgram);
		glDeleteProgram(m_itemProgram);

		glDeleteVertexArrays(1, &m_blockVAO);
		glDeleteVertexArrays(1, &m_itemVAO);

		glDeleteBuffers(1, &m_matriceUBO);
		glDeleteBuffers(1, &m_itemVBO);
	}

	void Graphics::Update(float dT)
	{
		m_timeElapse += dT;
	}

	void Graphics::DisplayWorld(const Camera& camera) const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glNamedBufferData(m_matriceUBO, sizeof(mat4x4), value_ptr(camera.GetViewProjection()), GL_STATIC_DRAW);

		// Draw Entity (mob)
		glUseProgram(m_entityProgram);
		glBindVertexArray(m_world.GetMesh().GetEntityVAO());
		for (Enemy const& enemy : m_world.GetEnemies())
		{
			glProgramUniformMatrix4fv(m_entityProgram, GetLocation(m_entityProgram, "model"), 1, GL_FALSE, value_ptr(enemy.GetModelMatrix()));

			glDrawArrays(GL_TRIANGLES, enemy.m_mesh.first, enemy.m_mesh.count);
		}

		// Draw Falling Block
		glUseProgram(m_fallingBlockProgram);
		glBindVertexArray(m_world.GetMesh().GetBlockVAO());
		glDrawArrays(GL_TRIANGLES, 0, m_world.GetMesh().GetBlockVertexCount());

		// Draw Block
		glBindVertexArray(m_blockVAO);
		glUseProgram(m_solidBlockProgram);
		for (const auto& c : m_world.renderedChunks)
			for (const auto& chunk : c)
			{
				if (camera.See(*chunk))
				{
					chunk->Link(0);

					glProgramUniform1i(m_solidBlockProgram, GetLocation(m_solidBlockProgram, "highestBlock"), chunk->highestBlock + 1);
					glDrawArrays(GL_POINTS, 0, 16 * (chunk->highestBlock + 1) * 16);
				}
			}
		/*for (int x = 0; x < CHUNK_ARRAY_SIZE; x++)
			for (int z = 0; z < CHUNK_ARRAY_SIZE; z++)
			{
				if (x > 0) m_world.renderedChunks[x - 1][z]->Link(1); // left
				if (x < CHUNK_ARRAY_SIZE - 1) m_world.renderedChunks[x + 1][z]->Link(2); // right
				if (z > 0) m_world.renderedChunks[x][z - 1]->Link(3); // front
				if (z < CHUNK_ARRAY_SIZE - 1) m_world.renderedChunks[x][z + 1]->Link(4); // back
				m_world.renderedChunks[x][z]->Link(0);

				glProgramUniform1i(m_solidBlockProgram, GetLocation(m_solidBlockProgram, "highestBlock"), m_world.renderedChunks[x][z]->highestBlock + 1);
				glDrawArrays(GL_POINTS, 0, 16 * (m_world.renderedChunks[x][z]->highestBlock + 1) * 16);
			}*/

		// Draw Water
		glDepthMask(GL_FALSE);
		glProgramUniform1i(m_fluidBlockProgram, GetLocation(m_fluidBlockProgram, "ticks"), ((int)(m_timeElapse * 4)) % 16);
		glUseProgram(m_fluidBlockProgram);
		for (const auto& c : m_world.renderedChunks)
			for (const auto& chunk : c)
			{
				if (chunk != nullptr && camera.See(*chunk))
				{
					chunk->Link(0);

					glProgramUniform1i(m_fluidBlockProgram, GetLocation(m_fluidBlockProgram, "highestBlock"), chunk->highestBlock + 1);
					glDrawArrays(GL_POINTS, 0, 16 * (chunk->highestBlock + 1) * 16);
				}
			}
		glDepthMask(GL_TRUE);

		// Draw current item
		glDisable(GL_DEPTH_TEST);
		Item const& itemInHand = m_world.GetPlayer().GetInventory().GetItemInventoryBarArray()[m_world.GetPlayer().GetInventoryBarItemIndex()];
		if (itemInHand.GetToolType() != Tools::NONE)
		{
			glUseProgram(m_itemProgram);
			glBindVertexArray(m_itemVAO);
			GLuint test = itemInHand.GetTexture().GetTextureID();
			glBindTextureUnit(m_itemTextureUnit, test);

			mat4x4 move = glm::identity<mat4x4>();
			move = glm::translate(move, vec3(.8f, -1.3f, .6f));

			float timer = m_world.GetPlayer().GetAtkTimer();
			float ratio = 0.f;
			if (timer > 0.f)
			{
				ratio = timer <= HALF_ATTACK_TIMER ? timer / HALF_ATTACK_TIMER : (ATTACK_TIMER - timer) / HALF_ATTACK_TIMER;
			}
			move = glm::rotate(move, ratio * SWING_ANGLE*2.f, vec3(-2.f, 1.f, 0.f));
			move = glm::rotate(move, -80.f * Trigo::DEGREE_TO_RADIAN, vec3(0.f, 1.f, 0.f));

			glProgramUniformMatrix4fv(m_itemProgram, GetLocation(m_itemProgram, "movement"), 1, GL_FALSE, value_ptr(move));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glEnable(GL_DEPTH_TEST);
	}

	void Graphics::DisplayPlayerTarget(TargetBlock target) const
	{
		glProgramUniform1f(m_targetBlockProgram, GetLocation(m_targetBlockProgram, "x"), (float)target.x);
		glProgramUniform1f(m_targetBlockProgram, GetLocation(m_targetBlockProgram, "y"), (float)target.y);
		glProgramUniform1f(m_targetBlockProgram, GetLocation(m_targetBlockProgram, "z"), (float)target.z);
		glProgramUniform1i(m_targetBlockProgram, GetLocation(m_targetBlockProgram, "face"), (int)target.face);

		glUseProgram(m_targetBlockProgram);
		glBindVertexArray(m_blockVAO);
		glDrawArrays(GL_POINTS, 0, 1);
	}

	ProgramName Graphics::CreateBlockShaderProgram(int uniformUnit, int textureUnit)
	{
		//		.vert - a vertex shader
		//		.tesc - a tessellation control shader
		//		.tese - a tessellation evaluation shader
		//		.geom - a geometry shader
		//		.frag - a fragment shader
		//		.comp - a compute shader

		ProgramName program = CreateProgram("shaders/Block.vert", "shaders/Block.frag", "shaders/Block.geom");

		glProgramUniform1i(program, GetLocation(program, "chunkWidth"), CHUNK_X_SIZE);
		glProgramUniform1i(program, GetLocation(program, "chunkHeight"), CHUNK_Y_SIZE);
		glProgramUniform1i(program, GetLocation(program, "chunkDepth"), CHUNK_Z_SIZE);
		glProgramUniform1i(program, GetLocation(program, "atlas"), textureUnit);
		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "matrice"), uniformUnit);

		glGenVertexArrays(1, &m_blockVAO);

		return program;
	}

	ProgramName Graphics::CreateEntityShaderProgram(int uniformUnit, int textureUnit)
	{
		ProgramName program = CreateProgram("shaders/Entity.vert", "shaders/Block.frag");

		glProgramUniform1i(program, GetLocation(program, "atlas"), textureUnit);
		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "matrice"), uniformUnit);

		return program;
	}

	ProgramName Graphics::CreateFallingShaderProgram(int uniformUnit, int textureUnit)
	{
		ProgramName program = CreateProgram("shaders/Falling.vert", "shaders/Block.frag");

		glProgramUniform1i(program, GetLocation(program, "atlas"), textureUnit);
		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "matrice"), uniformUnit);

		return program;
	}

	ProgramName Graphics::CreateFluidShaderProgram(int uniformUnit, int textureUnit)
	{
		ProgramName program = CreateProgram("shaders/Block.vert", "shaders/Block.frag", "shaders/Fluid.geom");

		glProgramUniform1i(program, GetLocation(program, "chunkWidth"), CHUNK_X_SIZE);
		glProgramUniform1i(program, GetLocation(program, "chunkHeight"), CHUNK_Y_SIZE);
		glProgramUniform1i(program, GetLocation(program, "chunkDepth"), CHUNK_Z_SIZE);
		glProgramUniform1i(program, GetLocation(program, "atlas"), textureUnit);
		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "matrice"), uniformUnit);

		return program;
	}

	ProgramName Graphics::CreateItemShaderProgram(int uniformUnit, int textureUnit)
	{
		ProgramName program = CreateProgram("shaders/Item.vert", "shaders/Item.frag");

		glProgramUniform1i(program, GetLocation(program, "item"), textureUnit);
		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "matrice"), uniformUnit);


		glCreateVertexArrays(1, &m_itemVAO);
		GLuint bindingPoint = 0;

		GLuint position = 0;
		glEnableVertexArrayAttrib(m_itemVAO, position);
		glVertexArrayAttribFormat(m_itemVAO, position, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float));
		glVertexArrayAttribBinding(m_itemVAO, position, bindingPoint);

		GLuint texture = 1;
		glEnableVertexArrayAttrib(m_itemVAO, texture);
		glVertexArrayAttribFormat(m_itemVAO, texture, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		glVertexArrayAttribBinding(m_itemVAO, texture, bindingPoint);

		float data[] = {
			/*1.f, -.3f, -.9f,  0.f,  0.f, // right
			.6f, -.3f, -.1f,  1.f,  1.f, // left
			.8f,-1.4f, -.5f,  0.f,  1.f, // top

			1.f, -.3f, -.9f,  0.f,  0.f, // right
			.8f,  .6f, -.5f,  1.f,  0.f, // bottom
			.6f, -.3f, -.1f,  1.f,  1.f, // left*/
			 .9f,  1.f, -.0f,  0.f,  0.f, // right
			 .0f,  2.f, -.0f,  1.f,  0.f, // top
			-.9f,  1.f, -.0f,  1.f,  1.f, // left

			 .9f,  1.f, -.0f,  0.f,  0.f, // right
			-.9f,  1.f, -.0f,  1.f,  1.f, // left
			 .0f,  .0f, -.0f,  0.f,  1.f, // bottom
		};
		glCreateBuffers(1, &m_itemVBO);
		glNamedBufferData(m_itemVBO, sizeof(data), data, GL_STATIC_DRAW);

		glVertexArrayVertexBuffer(m_itemVAO, 0, m_itemVBO, 0, 5 * sizeof(float));

		return program;
	}

	ProgramName Graphics::CreateTargetShaderProgram(int uniformUnit)
	{
		ProgramName program = CreateProgram("shaders/Block.vert", "shaders/Simple.frag", "shaders/Target.geom");
		glUniformBlockBinding(program, glGetUniformBlockIndex(program, "matrice"), uniformUnit);
		return program;
	}

	ProgramName Graphics::CreateProgram(char const* vertexFile, char const* fragmentFile, char const* geometryFile)
	{
		ShaderName vertex = LoadShader(vertexFile, GL_VERTEX_SHADER);
		ShaderName geometry = (geometryFile != nullptr) ? LoadShader(geometryFile, GL_GEOMETRY_SHADER) : 0;
		ShaderName fragment = LoadShader(fragmentFile, GL_FRAGMENT_SHADER);

		ProgramName program = glCreateProgram();
		glAttachShader(program, vertex);
		if (geometryFile != nullptr)
			glAttachShader(program, geometry);
		glAttachShader(program, fragment);

		glLinkProgram(program);
#ifdef _DEBUG
		int  success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "\tERROR : \tglLinkProgram(" << program << ") ->\n" << infoLog << std::endl;
		}
#endif //_DEBUG

		glDeleteShader(vertex);
		if (geometryFile != nullptr)
			glDeleteShader(geometry);
		glDeleteShader(fragment);

		return program;
	}

	ShaderName Graphics::LoadShader(std::string const& filePath, GLuint type)
	{
		// Read Shader File
		std::ifstream file(filePath);

		if (!file.is_open())
		{
			char infoLog[40];
			strerror_s(infoLog, 39, errno);
			std::cout << "\tERROR : \tfile(" << filePath << ") ->\n" << infoLog << std::endl;
		}

		std::stringstream ss;
		ss << file.rdbuf();
		std::string str = ss.str();
		char const* source = str.c_str();

		// Create Shader
		ShaderName shader(glCreateShader((int)type));

		glShaderSource(shader, 1, &source, NULL);

		glCompileShader(shader);
#ifdef _DEBUG
		int  success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "\tERROR : \tglCompileShader(" << shader << ") ->\n" << infoLog << std::endl;
		}
#endif //_DEBUG

		file.close();
		return shader;
	}

	TextureName Graphics::LoadAtlasTexture(int textureUnit)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("assets/Atlas.png", &width, &height, &nrChannels, 0);

		if (!data)
		{
			std::cout << "\tERROR : \tstbi_load(assets/Atlas.png, ...) ->\n" << std::endl;
			stbi_image_free(data);
			return 0;
		}

		TextureName texture;
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 5, (nrChannels == 3 ? GL_RGB8 : GL_RGBA8), width, height);
		glTextureSubImage2D(texture, 0, 0, 0, width, height, (nrChannels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(texture);
		glBindTextureUnit(textureUnit, texture);

		glGenSamplers(1, &m_atlasSampler);
		glSamplerParameteri(m_atlasSampler, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glSamplerParameteri(m_atlasSampler, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glSamplerParameteri(m_atlasSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // TODO: test if GL_NEAREST render result closer to original minecraft
		glSamplerParameteri(m_atlasSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glSamplerParameterf(m_atlasSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 5.f); // TODO: understand param 5.f
		glBindSampler(textureUnit, m_atlasSampler);

		stbi_image_free(data);
		return texture;
	}

	GLint Graphics::GetLocation(ProgramName program, char const* attributName) const
	{
		GLint location = glGetUniformLocation(program, attributName);
#ifdef _DEBUG
		if (location == -1)
		{
			std::cout << "\tWARNING : \tglGetUniformLocation(" << program << ", \"" << attributName << "\") -> not found" << std::endl;
		}
#endif // _DEBUG
		return location;
	}
}