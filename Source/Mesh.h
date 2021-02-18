#pragma once

#include <glad/glad.h>
#include <vector>

#include "DynamicBlock.h"

namespace Minecraft
{
	class Mesh
	{
	public:
		struct Entity
		{
			Entity() {}

			GLint first = 0;
			GLsizei count = 0;
		};

		Mesh();
		~Mesh();

		void UpdateMovingBlock(std::vector<DynamicBlock> const& blocks);

		GLsizei GetBlockVertexCount() const { return m_blockVertexCount; }
		GLuint GetBlockVAO() const
		{
			glVertexArrayVertexBuffer(m_VAO, 0, m_blockVBO, 0, m_vertexSize * sizeof(float));
			return m_VAO;
		}
		GLuint GetEntityVAO() const
		{
			glVertexArrayVertexBuffer(m_VAO, 0, m_entityVBO, 0, m_vertexSize * sizeof(float));
			return m_VAO;
		}

	private:
		void Back(DynamicBlock const& block, float spriteCol);
		void Bottom(DynamicBlock const& block, float spriteCol);
		void Front(DynamicBlock const& block, float spriteCol);
		void Left(DynamicBlock const& block, float spriteCol);
		void Normal(vec3 const& normal);
		void Position(float x, float y, float z);
		void Right(DynamicBlock const& block, float spriteCol);
		void Texel(float s, float t);
		void Top(DynamicBlock const& block, float spriteCol);

		int m_vertexSize = 0;
		GLuint m_VAO = 0;

		GLuint m_blockVBO = 0;
		std::vector<float> m_blockVertex = std::vector<float>();
		GLsizei m_blockVertexCount = 0;

		GLuint m_entityVBO = 0;
		Entity m_creeper = Entity();
		Entity m_zombie = Entity();
		Entity m_cow = Entity();

	public:
		Entity const& creeper = m_creeper;
		Entity const& zombie = m_zombie;
		Entity const& cow = m_cow;
	};
}