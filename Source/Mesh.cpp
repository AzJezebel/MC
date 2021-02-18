#include "Mesh.h"

namespace Minecraft
{
	Mesh::Mesh()
	{
		glCreateVertexArrays(1, &m_VAO);

		GLuint bindingPoint = 0;

		GLuint position = 0;
		glEnableVertexArrayAttrib(m_VAO, position);
		glVertexArrayAttribFormat(m_VAO, position, 3, GL_FLOAT, GL_FALSE, 0 * sizeof(float));
		glVertexArrayAttribBinding(m_VAO, position, bindingPoint);

		GLuint texture = 1;
		glEnableVertexArrayAttrib(m_VAO, texture);
		glVertexArrayAttribFormat(m_VAO, texture, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		glVertexArrayAttribBinding(m_VAO, texture, bindingPoint);

		GLuint normal = 2;
		glEnableVertexArrayAttrib(m_VAO, normal);
		glVertexArrayAttribFormat(m_VAO, normal, 3, GL_FLOAT, GL_FALSE, (3 + 2) * sizeof(float));
		glVertexArrayAttribBinding(m_VAO, normal, bindingPoint);


		glCreateBuffers(1, &m_blockVBO);


		float pixelW = 1.f / (32.f * 16.f); // TODO: create define
		float pixelH = 1.f / (8.f * 16.f);

		//float pixelX = 0.f;
		//float pixelY = 0.f;

		float th16 = 0.0625;

		float data[] = {
			// creeper
			// ffeet front
			-4.f * th16,  0.f * th16,  6.f * th16,  4.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 0.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,

			 0.f * th16,  0.f * th16,  6.f * th16,  4.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,

			// ffeet top
			-4.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16,  6.f * th16,  2.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 0.f * th16,  6.f * th16,  2.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16,  6.f * th16,  2.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			 0.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16,  2.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16,  6.f * th16,  2.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16,  2.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			// ffeet back
			-4.f * th16,  0.f * th16,  2.f * th16, 12.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16,  6.f * th16,  2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  0.f * th16,  2.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,

			 0.f * th16,  6.f * th16,  2.f * th16, 16.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  0.f * th16,  2.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16,  6.f * th16,  2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			 0.f * th16,  0.f * th16,  2.f * th16, 12.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  6.f * th16,  2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16,  6.f * th16,  2.f * th16, 16.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  6.f * th16,  2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// ffeet bottom
			-4.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  0.f * th16,  2.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16,  6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 0.f * th16,  0.f * th16,  2.f * th16, 12.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16,  6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  0.f * th16,  2.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			 0.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16,  2.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  0.f * th16,  6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16,  0.f * th16,  2.f * th16, 12.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  0.f * th16,  6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16,  2.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// ffeet right
			 4.f * th16,  0.f * th16,  6.f * th16,  8.f * pixelW,  6.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 12.f * pixelW,  6.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16,  6.f * th16,  2.f * th16, 12.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  6.f * th16,  6.f * th16,  8.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 12.f * pixelW,  6.f * pixelH,  1.f,  0.f,  0.f,

			// ffeet left
			-4.f * th16,  0.f * th16,  6.f * th16,  4.f * pixelW,  6.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  0.f * th16,  2.f * th16,  0.f * pixelW,  6.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16,  6.f * th16,  2.f * th16,  0.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  0.f * th16,  2.f * th16,  0.f * pixelW,  6.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  6.f * th16,  6.f * th16,  4.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,


			// bfeet front
			-4.f * th16,  0.f * th16, -2.f * th16,  4.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 0.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,

			 0.f * th16,  0.f * th16, -2.f * th16,  4.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW,  6.f * pixelH,  0.f,  0.f,  1.f,

			// bfeet top
			-4.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16,  6.f * th16, -6.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 0.f * th16,  6.f * th16, -6.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16,  6.f * th16, -6.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			 0.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16, -6.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16,  6.f * th16, -6.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 0.f * th16,  6.f * th16, -6.f * th16,  4.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			// bfeet back
			-4.f * th16,  0.f * th16, -6.f * th16, 12.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16,  6.f * th16, -6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  0.f * th16, -6.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,

			 0.f * th16,  6.f * th16, -6.f * th16, 16.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  0.f * th16, -6.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16,  6.f * th16, -6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			 0.f * th16,  0.f * th16, -6.f * th16, 12.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  6.f * th16, -6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  0.f * th16, -6.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16,  6.f * th16, -6.f * th16, 16.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  0.f * th16, -6.f * th16, 16.f * pixelW,  6.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  6.f * th16, -6.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// bfeet bottom
			-4.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  0.f * th16, -6.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16, -2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 0.f * th16,  0.f * th16, -6.f * th16, 12.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16, -2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  0.f * th16, -6.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			 0.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16, -6.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  0.f * th16, -2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16,  0.f * th16, -6.f * th16, 12.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  0.f * th16, -2.f * th16, 12.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16, -6.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// bfeet right
			 4.f * th16,  0.f * th16, -2.f * th16,  8.f * pixelW,  6.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  0.f * th16, -6.f * th16, 12.f * pixelW,  6.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16,  6.f * th16, -6.f * th16, 12.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  6.f * th16, -2.f * th16,  8.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  0.f * th16, -6.f * th16, 12.f * pixelW,  6.f * pixelH,  1.f,  0.f,  0.f,

			// bfeet left
			-4.f * th16,  0.f * th16, -2.f * th16,  4.f * pixelW,  6.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  0.f * th16, -6.f * th16,  0.f * pixelW,  6.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16,  6.f * th16, -6.f * th16,  0.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  0.f * th16, -6.f * th16,  0.f * pixelW,  6.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  6.f * th16, -2.f * th16,  4.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,



			// body front
			-4.f * th16,  6.f * th16,  2.f * th16, 20.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  6.f * th16,  2.f * th16, 28.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 18.f * th16,  2.f * th16, 20.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16, 18.f * th16,  2.f * th16, 28.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 18.f * th16,  2.f * th16, 20.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  6.f * th16,  2.f * th16, 28.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
				
			// body top
			-4.f * th16, 18.f * th16,  2.f * th16, 20.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 18.f * th16,  2.f * th16, 28.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 18.f * th16, -2.f * th16, 20.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16, 18.f * th16, -2.f * th16, 28.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 18.f * th16, -2.f * th16, 20.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 18.f * th16,  2.f * th16, 28.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			// body back
			-4.f * th16,  6.f * th16, -2.f * th16, 32.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 18.f * th16, -2.f * th16, 32.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  6.f * th16, -2.f * th16, 40.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16, 18.f * th16, -2.f * th16, 40.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  6.f * th16, -2.f * th16, 40.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 18.f * th16, -2.f * th16, 32.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// body bottom
			-4.f * th16,  6.f * th16,  2.f * th16, 28.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  6.f * th16, -2.f * th16, 28.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  6.f * th16,  2.f * th16, 36.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16,  6.f * th16, -2.f * th16, 36.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  6.f * th16,  2.f * th16, 36.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  6.f * th16, -2.f * th16, 28.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// body right
			 4.f * th16,  6.f * th16,  2.f * th16, 28.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  6.f * th16, -2.f * th16, 32.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 18.f * th16,  2.f * th16, 28.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 18.f * th16, -2.f * th16, 32.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 18.f * th16,  2.f * th16, 28.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  6.f * th16, -2.f * th16, 32.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			// body left
			-4.f * th16,  6.f * th16,  2.f * th16, 20.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 18.f * th16,  2.f * th16, 20.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  6.f * th16, -2.f * th16, 16.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16, 18.f * th16, -2.f * th16, 16.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  6.f * th16, -2.f * th16, 16.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 18.f * th16,  2.f * th16, 20.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,



			// head front
			-4.f * th16, 18.f * th16,  4.f * th16,  8.f * pixelW, 16.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 18.f * th16,  4.f * th16, 16.f * pixelW, 16.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 26.f * th16,  4.f * th16,  8.f * pixelW, 24.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16, 26.f * th16,  4.f * th16, 16.f * pixelW, 24.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 26.f * th16,  4.f * th16,  8.f * pixelW, 24.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 18.f * th16,  4.f * th16, 16.f * pixelW, 16.f * pixelH,  0.f,  0.f,  1.f,
				
			// head top
			-4.f * th16, 26.f * th16,  4.f * th16,  8.f * pixelW, 24.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 26.f * th16,  4.f * th16, 16.f * pixelW, 24.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 26.f * th16, -4.f * th16,  8.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16, 26.f * th16, -4.f * th16, 16.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 26.f * th16, -4.f * th16,  8.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 26.f * th16,  4.f * th16, 16.f * pixelW, 24.f * pixelH,  0.f,  1.f,  0.f,

			// head back
			-4.f * th16, 18.f * th16, -4.f * th16, 24.f * pixelW, 16.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 26.f * th16, -4.f * th16, 24.f * pixelW, 24.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 18.f * th16, -4.f * th16, 32.f * pixelW, 16.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16, 26.f * th16, -4.f * th16, 32.f * pixelW, 24.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 18.f * th16, -4.f * th16, 32.f * pixelW, 16.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 26.f * th16, -4.f * th16, 24.f * pixelW, 24.f * pixelH,  0.f,  0.f, -1.f,

			// head bottom
			-4.f * th16, 18.f * th16,  4.f * th16, 16.f * pixelW, 24.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 18.f * th16, -4.f * th16, 16.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 18.f * th16,  4.f * th16, 24.f * pixelW, 24.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16, 18.f * th16, -4.f * th16, 24.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 18.f * th16,  4.f * th16, 24.f * pixelW, 24.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 18.f * th16, -4.f * th16, 16.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,

			// head right
			 4.f * th16, 18.f * th16,  4.f * th16, 16.f * pixelW, 16.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 18.f * th16, -4.f * th16, 24.f * pixelW, 16.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 26.f * th16,  4.f * th16, 16.f * pixelW, 24.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 26.f * th16, -4.f * th16, 24.f * pixelW, 24.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 26.f * th16,  4.f * th16, 16.f * pixelW, 24.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 18.f * th16, -4.f * th16, 24.f * pixelW, 16.f * pixelH,  1.f,  0.f,  0.f,

			// head left
			-4.f * th16, 18.f * th16,  4.f * th16,  8.f * pixelW, 16.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 26.f * th16,  4.f * th16,  8.f * pixelW, 24.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 18.f * th16, -4.f * th16,  0.f * pixelW, 16.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16, 26.f * th16, -4.f * th16,  0.f * pixelW, 24.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 18.f * th16, -4.f * th16,  0.f * pixelW, 16.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 26.f * th16,  4.f * th16,  8.f * pixelW, 24.f * pixelH, -1.f,  0.f,  0.f,


		//---------------------------------------------------------------------------------------------
			// zombie
			// feet front
			-4.f * th16,  0.f * th16,  2.f * th16, 44.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 12.f * th16,  2.f * th16, 44.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 0.f * th16, 12.f * th16,  2.f * th16, 48.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 12.f * th16,  2.f * th16, 44.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,

			 0.f * th16,  0.f * th16,  2.f * th16, 44.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16, 12.f * th16,  2.f * th16, 44.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16, 12.f * th16,  2.f * th16, 48.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 0.f * th16, 12.f * th16,  2.f * th16, 44.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,

			// feet back
			-4.f * th16,  0.f * th16, -2.f * th16, 52.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 12.f * th16, -2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  0.f * th16, -2.f * th16, 56.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 0.f * th16, 12.f * th16, -2.f * th16, 56.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16,  0.f * th16, -2.f * th16, 56.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 12.f * th16, -2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			 0.f * th16,  0.f * th16, -2.f * th16, 52.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16, 12.f * th16, -2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  0.f * th16, -2.f * th16, 56.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16, 12.f * th16, -2.f * th16, 56.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16,  0.f * th16, -2.f * th16, 56.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 0.f * th16, 12.f * th16, -2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// feet bottom
			-4.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  0.f * th16, -2.f * th16, 48.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16,  2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 0.f * th16,  0.f * th16, -2.f * th16, 52.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16,  2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16,  0.f * th16, -2.f * th16, 48.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			 0.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16, -2.f * th16, 48.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16,  0.f * th16, -2.f * th16, 52.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16,  0.f * th16,  2.f * th16, 52.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 0.f * th16,  0.f * th16, -2.f * th16, 48.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// feet right
			 4.f * th16,  0.f * th16,  2.f * th16, 48.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  0.f * th16, -2.f * th16, 52.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 12.f * th16,  2.f * th16, 48.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 12.f * th16, -2.f * th16, 52.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 12.f * th16,  2.f * th16, 48.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16,  0.f * th16, -2.f * th16, 52.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			// feet left
			-4.f * th16,  0.f * th16,  2.f * th16, 44.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 12.f * th16,  2.f * th16, 44.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  0.f * th16, -2.f * th16, 40.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16, 12.f * th16, -2.f * th16, 40.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16,  0.f * th16, -2.f * th16, 40.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 12.f * th16,  2.f * th16, 44.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,



			// body front
			-4.f * th16, 12.f * th16,  2.f * th16, 60.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 12.f * th16,  2.f * th16, 68.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 24.f * th16,  2.f * th16, 60.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16, 24.f * th16,  2.f * th16, 68.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 24.f * th16,  2.f * th16, 60.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 12.f * th16,  2.f * th16, 68.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
				
			// body top
			-4.f * th16, 24.f * th16,  2.f * th16, 60.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 24.f * th16,  2.f * th16, 68.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 24.f * th16, -2.f * th16, 60.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16, 24.f * th16, -2.f * th16, 68.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 24.f * th16, -2.f * th16, 60.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 24.f * th16,  2.f * th16, 68.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			// body back
			-4.f * th16, 12.f * th16, -2.f * th16, 72.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 24.f * th16, -2.f * th16, 72.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 12.f * th16, -2.f * th16, 80.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16, 24.f * th16, -2.f * th16, 80.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 12.f * th16, -2.f * th16, 80.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 24.f * th16, -2.f * th16, 72.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// body bottom
			-4.f * th16, 12.f * th16,  2.f * th16, 68.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 12.f * th16, -2.f * th16, 68.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 12.f * th16,  2.f * th16, 76.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16, 12.f * th16, -2.f * th16, 76.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 12.f * th16,  2.f * th16, 76.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 12.f * th16, -2.f * th16, 68.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// body right
			 4.f * th16, 12.f * th16,  2.f * th16, 68.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 12.f * th16, -2.f * th16, 72.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16,  2.f * th16, 68.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 24.f * th16, -2.f * th16, 72.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16,  2.f * th16, 68.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 12.f * th16, -2.f * th16, 72.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			// body left
			-4.f * th16, 12.f * th16,  2.f * th16, 60.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16,  2.f * th16, 60.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 12.f * th16, -2.f * th16, 56.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16, 24.f * th16, -2.f * th16, 56.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 12.f * th16, -2.f * th16, 56.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16,  2.f * th16, 60.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,



			// head front
			-4.f * th16, 24.f * th16,  4.f * th16, 48.f * pixelW, 16.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 24.f * th16,  4.f * th16, 56.f * pixelW, 16.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 32.f * th16,  4.f * th16, 48.f * pixelW, 24.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16, 32.f * th16,  4.f * th16, 56.f * pixelW, 24.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 32.f * th16,  4.f * th16, 48.f * pixelW, 24.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 24.f * th16,  4.f * th16, 56.f * pixelW, 16.f * pixelH,  0.f,  0.f,  1.f,
				
			// head top
			-4.f * th16, 32.f * th16,  4.f * th16, 48.f * pixelW, 24.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 32.f * th16,  4.f * th16, 56.f * pixelW, 24.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 32.f * th16, -4.f * th16, 48.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16, 32.f * th16, -4.f * th16, 56.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 32.f * th16, -4.f * th16, 48.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 32.f * th16,  4.f * th16, 56.f * pixelW, 24.f * pixelH,  0.f,  1.f,  0.f,

			// head back
			-4.f * th16, 24.f * th16, -4.f * th16, 64.f * pixelW, 16.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 32.f * th16, -4.f * th16, 64.f * pixelW, 24.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 24.f * th16, -4.f * th16, 72.f * pixelW, 16.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16, 32.f * th16, -4.f * th16, 72.f * pixelW, 24.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 24.f * th16, -4.f * th16, 72.f * pixelW, 16.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 32.f * th16, -4.f * th16, 64.f * pixelW, 24.f * pixelH,  0.f,  0.f, -1.f,

			// head bottom
			-4.f * th16, 24.f * th16,  4.f * th16, 56.f * pixelW, 24.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 24.f * th16, -4.f * th16, 56.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 24.f * th16,  4.f * th16, 64.f * pixelW, 24.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16, 24.f * th16, -4.f * th16, 64.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 24.f * th16,  4.f * th16, 64.f * pixelW, 24.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 24.f * th16, -4.f * th16, 56.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,

			// head right
			 4.f * th16, 24.f * th16,  4.f * th16, 56.f * pixelW, 16.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16, -4.f * th16, 64.f * pixelW, 16.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 32.f * th16,  4.f * th16, 56.f * pixelW, 24.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 32.f * th16, -4.f * th16, 64.f * pixelW, 24.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 32.f * th16,  4.f * th16, 56.f * pixelW, 24.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16, -4.f * th16, 64.f * pixelW, 16.f * pixelH,  1.f,  0.f,  0.f,

			// head left
			-4.f * th16, 24.f * th16,  4.f * th16, 48.f * pixelW, 16.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 32.f * th16,  4.f * th16, 48.f * pixelW, 24.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16, -4.f * th16, 40.f * pixelW, 16.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16, 32.f * th16, -4.f * th16, 40.f * pixelW, 24.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16, -4.f * th16, 40.f * pixelW, 16.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 32.f * th16,  4.f * th16, 48.f * pixelW, 24.f * pixelH, -1.f,  0.f,  0.f,



			// larm front
			 4.f * th16, 24.f * th16, -2.f * th16, 84.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 24.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 8.f * th16, 24.f * th16, 10.f * th16, 88.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 24.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
				
			// larm top
			 4.f * th16, 24.f * th16, -2.f * th16, 84.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 20.f * th16, -2.f * th16, 84.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			 8.f * th16, 20.f * th16, -2.f * th16, 88.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 20.f * th16, -2.f * th16, 84.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			 8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			// larm back
			 4.f * th16, 20.f * th16, -2.f * th16, 92.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 8.f * th16, 20.f * th16, -2.f * th16, 96.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 8.f * th16, 20.f * th16, 10.f * th16, 96.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 8.f * th16, 20.f * th16, -2.f * th16, 96.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// larm bottom
			 4.f * th16, 24.f * th16, 10.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 20.f * th16, 10.f * th16, 88.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 8.f * th16, 24.f * th16, 10.f * th16, 92.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 8.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 8.f * th16, 24.f * th16, 10.f * th16, 92.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 20.f * th16, 10.f * th16, 88.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// larm right
			 8.f * th16, 24.f * th16, 10.f * th16, 88.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 8.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 8.f * th16, 20.f * th16, -2.f * th16, 92.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 8.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			// larm left
			 4.f * th16, 24.f * th16, 10.f * th16, 80.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16, -2.f * th16, 80.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 20.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 20.f * th16, -2.f * th16, 84.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 20.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16, -2.f * th16, 80.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,



			// rarm front
			-4.f * th16, 24.f * th16, -2.f * th16, 84.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 24.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,

			-8.f * th16, 24.f * th16, 10.f * th16, 88.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 24.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
				
			// rarm top
			-4.f * th16, 24.f * th16, -2.f * th16, 84.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 20.f * th16, -2.f * th16, 84.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			-8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,

			-8.f * th16, 20.f * th16, -2.f * th16, 88.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,
			-8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 20.f * th16, -2.f * th16, 84.f * pixelW, 16.f * pixelH,  0.f,  1.f,  0.f,

			// rarm back
			-4.f * th16, 20.f * th16, -2.f * th16, 92.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-8.f * th16, 20.f * th16, -2.f * th16, 96.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			-8.f * th16, 20.f * th16, 10.f * th16, 96.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-8.f * th16, 20.f * th16, -2.f * th16, 96.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			// rarm bottom
			-4.f * th16, 24.f * th16, 10.f * th16, 88.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-8.f * th16, 24.f * th16, 10.f * th16, 92.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 20.f * th16, 10.f * th16, 88.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			-8.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 20.f * th16, 10.f * th16, 88.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			-8.f * th16, 24.f * th16, 10.f * th16, 92.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			// rarm right
			-8.f * th16, 24.f * th16, 10.f * th16, 88.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-8.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			-8.f * th16, 20.f * th16, -2.f * th16, 92.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-8.f * th16, 20.f * th16, 10.f * th16, 92.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-8.f * th16, 24.f * th16, -2.f * th16, 88.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			// rarm left
			-4.f * th16, 24.f * th16, 10.f * th16, 80.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 20.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16, -2.f * th16, 80.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			-4.f * th16, 20.f * th16, -2.f * th16, 84.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16, -2.f * th16, 80.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 20.f * th16, 10.f * th16, 84.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,


		//---------------------------------------------------------------------------------------------
			// cow
			// feet front
				 //lf
			-6.f * th16,  0.f * th16,  9.f * th16,100.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-2.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-6.f * th16, 12.f * th16,  9.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			-2.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-6.f * th16, 12.f * th16,  9.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-2.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
				 //rf
			 2.f * th16,  0.f * th16,  9.f * th16,100.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 6.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 2.f * th16, 12.f * th16,  9.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 6.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 2.f * th16, 12.f * th16,  9.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 6.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
				 //lb
			-6.f * th16,  0.f * th16, -5.f * th16,100.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-2.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			-6.f * th16, 12.f * th16, -5.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			-2.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-6.f * th16, 12.f * th16, -5.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			-2.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
				 //rb
			 2.f * th16,  0.f * th16, -5.f * th16,100.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 6.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,
			 2.f * th16, 12.f * th16, -5.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,

			 6.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 2.f * th16, 12.f * th16, -5.f * th16,100.f * pixelW, 12.f * pixelH,  0.f,  0.f,  1.f,
			 6.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH,  0.f,  0.f,  1.f,

			// feet back
				 //lf
			-6.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-6.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			-2.f * th16,  0.f * th16,  5.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			-2.f * th16, 12.f * th16,  5.f * th16,112.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			-2.f * th16,  0.f * th16,  5.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-6.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
				 //rf
			 2.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 2.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 6.f * th16,  0.f * th16,  5.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 6.f * th16, 12.f * th16,  5.f * th16,112.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 6.f * th16,  0.f * th16,  5.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 2.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
				 //lb
			-6.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-6.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			-2.f * th16,  0.f * th16, -9.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			-2.f * th16, 12.f * th16, -9.f * th16,112.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			-2.f * th16,  0.f * th16, -9.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			-6.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
				 //rb
			 2.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 2.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 6.f * th16,  0.f * th16, -9.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,

			 6.f * th16, 12.f * th16, -9.f * th16,112.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,
			 6.f * th16,  0.f * th16, -9.f * th16,112.f * pixelW,  0.f * pixelH,  0.f,  0.f, -1.f,
			 2.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f,  0.f, -1.f,

			// feet bottom
				 //lf
			-6.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-6.f * th16,  0.f * th16,  5.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			-2.f * th16,  0.f * th16,  9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			-2.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			-2.f * th16,  0.f * th16,  9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-6.f * th16,  0.f * th16,  5.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
				//rf
			 2.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 2.f * th16,  0.f * th16,  5.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 6.f * th16,  0.f * th16,  9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 6.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 6.f * th16,  0.f * th16,  9.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 2.f * th16,  0.f * th16,  5.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
				 //lb
			-6.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-6.f * th16,  0.f * th16, -9.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			-2.f * th16,  0.f * th16, -5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			-2.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			-2.f * th16,  0.f * th16, -5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			-6.f * th16,  0.f * th16, -9.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
				//rb
			 2.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 2.f * th16,  0.f * th16, -9.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 6.f * th16,  0.f * th16, -5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,

			 6.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,
			 6.f * th16,  0.f * th16, -5.f * th16,108.f * pixelW, 12.f * pixelH,  0.f, -1.f,  0.f,
			 2.f * th16,  0.f * th16, -9.f * th16,104.f * pixelW, 16.f * pixelH,  0.f, -1.f,  0.f,

			// feet right
				 //lf
			-2.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			-2.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
				 //rf
			 6.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 6.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
				 //lb
			-2.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			-2.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			-2.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
				 //rb
			 6.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,

			 6.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			// feet left
				 //lf
			-6.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			-6.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
				 //rf
			 2.f * th16,  0.f * th16,  9.f * th16,104.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			 2.f * th16, 12.f * th16,  5.f * th16,108.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16,  0.f * th16,  5.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16, 12.f * th16,  9.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
				 //lb
			-6.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			-6.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
				 //rb
			 2.f * th16,  0.f * th16, -5.f * th16,104.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			 2.f * th16, 12.f * th16, -9.f * th16,108.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16,  0.f * th16, -9.f * th16,108.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			 2.f * th16, 12.f * th16, -5.f * th16,104.f * pixelW, 12.f * pixelH, -1.f,  0.f,  0.f,



			// body front
			-6.f * th16, 12.f * th16,  9.f * th16,124.f * pixelW, 18.f * pixelH,  0.f,  0.f,  1.f,
			 6.f * th16, 12.f * th16,  9.f * th16,136.f * pixelW, 18.f * pixelH,  0.f,  0.f,  1.f,
			-6.f * th16, 22.f * th16,  9.f * th16,124.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,

			 6.f * th16, 22.f * th16,  9.f * th16,136.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
			-6.f * th16, 22.f * th16,  9.f * th16,124.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
			 6.f * th16, 12.f * th16,  9.f * th16,136.f * pixelW, 18.f * pixelH,  0.f,  0.f,  1.f,
				
			// body top
			-6.f * th16, 22.f * th16,  9.f * th16,158.f * pixelW, 18.f * pixelH,  0.f,  1.f,  0.f,
			 6.f * th16, 22.f * th16,  9.f * th16,146.f * pixelW, 18.f * pixelH,  0.f,  1.f,  0.f,
			-6.f * th16, 22.f * th16, -9.f * th16,158.f * pixelW,  0.f * pixelH,  0.f,  1.f,  0.f,

			 6.f * th16, 22.f * th16, -9.f * th16,146.f * pixelW,  0.f * pixelH,  0.f,  1.f,  0.f,
			-6.f * th16, 22.f * th16, -9.f * th16,158.f * pixelW,  0.f * pixelH,  0.f,  1.f,  0.f,
			 6.f * th16, 22.f * th16,  9.f * th16,146.f * pixelW, 18.f * pixelH,  0.f,  1.f,  0.f,

			// body back
			-6.f * th16, 12.f * th16, -9.f * th16,136.f * pixelW, 18.f * pixelH,  0.f,  0.f, -1.f,
			-6.f * th16, 22.f * th16, -9.f * th16,136.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,
			 6.f * th16, 12.f * th16, -9.f * th16,148.f * pixelW, 18.f * pixelH,  0.f,  0.f, -1.f,

			 6.f * th16, 22.f * th16, -9.f * th16,148.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,
			 6.f * th16, 12.f * th16, -9.f * th16,148.f * pixelW, 18.f * pixelH,  0.f,  0.f, -1.f,
			-6.f * th16, 22.f * th16, -9.f * th16,136.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,

			// body bottom
			-6.f * th16, 12.f * th16,  9.f * th16,124.f * pixelW, 18.f * pixelH,  0.f, -1.f,  0.f,
			-6.f * th16, 12.f * th16, -9.f * th16,124.f * pixelW,  0.f * pixelH,  0.f, -1.f,  0.f,
			 6.f * th16, 12.f * th16,  9.f * th16,136.f * pixelW, 18.f * pixelH,  0.f, -1.f,  0.f,

			 6.f * th16, 12.f * th16, -9.f * th16,136.f * pixelW,  0.f * pixelH,  0.f, -1.f,  0.f,
			 6.f * th16, 12.f * th16,  9.f * th16,136.f * pixelW, 18.f * pixelH,  0.f, -1.f,  0.f,
			-6.f * th16, 12.f * th16, -9.f * th16,124.f * pixelW,  0.f * pixelH,  0.f, -1.f,  0.f,

			// body right
			 6.f * th16, 12.f * th16,  9.f * th16,136.f * pixelW, 18.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 12.f * th16, -9.f * th16,136.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 22.f * th16,  9.f * th16,146.f * pixelW, 18.f * pixelH,  1.f,  0.f,  0.f,

			 6.f * th16, 22.f * th16, -9.f * th16,146.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 22.f * th16,  9.f * th16,146.f * pixelW, 18.f * pixelH,  1.f,  0.f,  0.f,
			 6.f * th16, 12.f * th16, -9.f * th16,136.f * pixelW,  0.f * pixelH,  1.f,  0.f,  0.f,

			// body left
			-6.f * th16, 12.f * th16,  9.f * th16,114.f * pixelW, 18.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 22.f * th16,  9.f * th16,124.f * pixelW, 18.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 12.f * th16, -9.f * th16,114.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,

			-6.f * th16, 22.f * th16, -9.f * th16,124.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 12.f * th16, -9.f * th16,114.f * pixelW,  0.f * pixelH, -1.f,  0.f,  0.f,
			-6.f * th16, 22.f * th16,  9.f * th16,124.f * pixelW, 18.f * pixelH, -1.f,  0.f,  0.f,

				

			// udder front
			-2.f * th16, 11.f * th16, -3.f * th16,149.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,
			 2.f * th16, 11.f * th16, -3.f * th16,153.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,
			-2.f * th16, 12.f * th16, -3.f * th16,149.f * pixelW, 32.f * pixelH,  0.f,  0.f,  1.f,

			 2.f * th16, 12.f * th16, -3.f * th16,153.f * pixelW, 32.f * pixelH,  0.f,  0.f,  1.f,
			-2.f * th16, 12.f * th16, -3.f * th16,149.f * pixelW, 32.f * pixelH,  0.f,  0.f,  1.f,
			 2.f * th16, 11.f * th16, -3.f * th16,153.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,

			// udder back
			-2.f * th16, 11.f * th16, -9.f * th16,153.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,
			-2.f * th16, 12.f * th16, -9.f * th16,153.f * pixelW, 32.f * pixelH,  0.f,  0.f, -1.f,
			 2.f * th16, 11.f * th16, -9.f * th16,157.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,

			 2.f * th16, 12.f * th16, -9.f * th16,157.f * pixelW, 32.f * pixelH,  0.f,  0.f, -1.f,
			 2.f * th16, 11.f * th16, -9.f * th16,157.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,
			-2.f * th16, 12.f * th16, -9.f * th16,153.f * pixelW, 32.f * pixelH,  0.f,  0.f, -1.f,

			// udder bottom
			-2.f * th16, 11.f * th16, -3.f * th16,149.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,
			-2.f * th16, 11.f * th16, -9.f * th16,149.f * pixelW, 25.f * pixelH,  0.f, -1.f,  0.f,
			 2.f * th16, 11.f * th16, -3.f * th16,153.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,

			 2.f * th16, 11.f * th16, -9.f * th16,153.f * pixelW, 25.f * pixelH,  0.f, -1.f,  0.f,
			 2.f * th16, 11.f * th16, -3.f * th16,153.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,
			-2.f * th16, 11.f * th16, -9.f * th16,149.f * pixelW, 25.f * pixelH,  0.f, -1.f,  0.f,

			// udder right
			 2.f * th16, 11.f * th16, -3.f * th16,153.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,
			 2.f * th16, 11.f * th16, -9.f * th16,153.f * pixelW, 25.f * pixelH,  1.f,  0.f,  0.f,
			 2.f * th16, 12.f * th16, -3.f * th16,154.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,

			 2.f * th16, 12.f * th16, -9.f * th16,154.f * pixelW, 25.f * pixelH,  1.f,  0.f,  0.f,
			 2.f * th16, 12.f * th16, -3.f * th16,154.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,
			 2.f * th16, 11.f * th16, -9.f * th16,153.f * pixelW, 25.f * pixelH,  1.f,  0.f,  0.f,

			// udder left
			-2.f * th16, 11.f * th16, -3.f * th16,148.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
			-2.f * th16, 12.f * th16, -3.f * th16,149.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
			-2.f * th16, 11.f * th16, -9.f * th16,148.f * pixelW, 25.f * pixelH, -1.f,  0.f,  0.f,

			-2.f * th16, 12.f * th16, -9.f * th16,149.f * pixelW, 25.f * pixelH, -1.f,  0.f,  0.f,
			-2.f * th16, 11.f * th16, -9.f * th16,148.f * pixelW, 25.f * pixelH, -1.f,  0.f,  0.f,
			-2.f * th16, 12.f * th16, -3.f * th16,149.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,



			// head front
			-4.f * th16, 16.f * th16, 15.f * th16,102.f * pixelW, 18.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 16.f * th16, 15.f * th16,110.f * pixelW, 18.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 24.f * th16, 15.f * th16,102.f * pixelW, 26.f * pixelH,  0.f,  0.f,  1.f,

			 4.f * th16, 24.f * th16, 15.f * th16,110.f * pixelW, 26.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 24.f * th16, 15.f * th16,102.f * pixelW, 26.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 16.f * th16, 15.f * th16,110.f * pixelW, 18.f * pixelH,  0.f,  0.f,  1.f,
				
			// head top
			-4.f * th16, 24.f * th16, 15.f * th16,102.f * pixelW, 26.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 24.f * th16, 15.f * th16,110.f * pixelW, 26.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 24.f * th16,  9.f * th16,102.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,

			 4.f * th16, 24.f * th16,  9.f * th16,110.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 24.f * th16,  9.f * th16,102.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 24.f * th16, 15.f * th16,110.f * pixelW, 26.f * pixelH,  0.f,  1.f,  0.f,

			// head back
			-4.f * th16, 16.f * th16,  9.f * th16,116.f * pixelW, 18.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 24.f * th16,  9.f * th16,116.f * pixelW, 26.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 16.f * th16,  9.f * th16,124.f * pixelW, 18.f * pixelH,  0.f,  0.f, -1.f,

			 4.f * th16, 24.f * th16,  9.f * th16,124.f * pixelW, 26.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 16.f * th16,  9.f * th16,124.f * pixelW, 18.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 24.f * th16,  9.f * th16,116.f * pixelW, 26.f * pixelH,  0.f,  0.f, -1.f,

			// head bottom
			-4.f * th16, 16.f * th16, 15.f * th16,110.f * pixelW, 26.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 16.f * th16,  9.f * th16,110.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 16.f * th16, 15.f * th16,118.f * pixelW, 26.f * pixelH,  0.f, -1.f,  0.f,

			 4.f * th16, 16.f * th16,  9.f * th16,118.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 16.f * th16, 15.f * th16,118.f * pixelW, 26.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 16.f * th16,  9.f * th16,110.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,

			// head right
			 4.f * th16, 16.f * th16, 15.f * th16,110.f * pixelW, 18.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 16.f * th16,  9.f * th16,116.f * pixelW, 18.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16, 15.f * th16,110.f * pixelW, 26.f * pixelH,  1.f,  0.f,  0.f,

			 4.f * th16, 24.f * th16,  9.f * th16,116.f * pixelW, 26.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 24.f * th16, 15.f * th16,110.f * pixelW, 26.f * pixelH,  1.f,  0.f,  0.f,
			 4.f * th16, 16.f * th16,  9.f * th16,116.f * pixelW, 18.f * pixelH,  1.f,  0.f,  0.f,

			// head left
			-4.f * th16, 16.f * th16, 15.f * th16,102.f * pixelW, 18.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16, 15.f * th16,102.f * pixelW, 26.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 16.f * th16,  9.f * th16, 96.f * pixelW, 18.f * pixelH, -1.f,  0.f,  0.f,

			-4.f * th16, 24.f * th16,  9.f * th16, 96.f * pixelW, 26.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 16.f * th16,  9.f * th16, 96.f * pixelW, 18.f * pixelH, -1.f,  0.f,  0.f,
			-4.f * th16, 24.f * th16, 15.f * th16,102.f * pixelW, 26.f * pixelH, -1.f,  0.f,  0.f,



			// lhorn front
			-5.f * th16, 22.f * th16, 13.f * th16,119.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
			-5.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,

			-4.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,
			-5.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,
			-4.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
				
			// lhorn top
			-5.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f,  1.f,  0.f,
			-5.f * th16, 25.f * th16, 12.f * th16,119.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,

			-4.f * th16, 25.f * th16, 12.f * th16,120.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			-5.f * th16, 25.f * th16, 12.f * th16,119.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			-4.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f,  1.f,  0.f,

			// lhorn back
			-5.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,
			-5.f * th16, 25.f * th16, 12.f * th16,121.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 22.f * th16, 12.f * th16,122.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,

			-4.f * th16, 25.f * th16, 12.f * th16,122.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,
			-4.f * th16, 22.f * th16, 12.f * th16,122.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,
			-5.f * th16, 25.f * th16, 12.f * th16,121.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,

			// lhorn bottom
			-5.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,
			-5.f * th16, 22.f * th16, 12.f * th16,120.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 22.f * th16, 13.f * th16,121.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,

			-4.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			-4.f * th16, 22.f * th16, 13.f * th16,121.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,
			-5.f * th16, 22.f * th16, 12.f * th16,120.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,

			// lhorn right
			-4.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 28.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 28.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,

			-4.f * th16, 25.f * th16, 12.f * th16,121.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,
			-4.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 28.f * pixelH,  1.f,  0.f,  0.f,

			// lhorn left
			-5.f * th16, 22.f * th16, 13.f * th16,119.f * pixelW, 28.f * pixelH, -1.f,  0.f,  0.f,
			-5.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
			-5.f * th16, 22.f * th16, 12.f * th16,118.f * pixelW, 28.f * pixelH, -1.f,  0.f,  0.f,

			-5.f * th16, 25.f * th16, 12.f * th16,118.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
			-5.f * th16, 22.f * th16, 12.f * th16,118.f * pixelW, 28.f * pixelH, -1.f,  0.f,  0.f,
			-5.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,



			// rhorn front
			 4.f * th16, 22.f * th16, 13.f * th16,119.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
			 5.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,

			 5.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,
			 4.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH,  0.f,  0.f,  1.f,
			 5.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 28.f * pixelH,  0.f,  0.f,  1.f,
				
			// rhorn top
			 4.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH,  0.f,  1.f,  0.f,
			 5.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 25.f * th16, 12.f * th16,119.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,

			 5.f * th16, 25.f * th16, 12.f * th16,120.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			 4.f * th16, 25.f * th16, 12.f * th16,119.f * pixelW, 32.f * pixelH,  0.f,  1.f,  0.f,
			 5.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f,  1.f,  0.f,

			// rhorn back
			 4.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 25.f * th16, 12.f * th16,121.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,
			 5.f * th16, 22.f * th16, 12.f * th16,122.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,

			 5.f * th16, 25.f * th16, 12.f * th16,122.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,
			 5.f * th16, 22.f * th16, 12.f * th16,122.f * pixelW, 28.f * pixelH,  0.f,  0.f, -1.f,
			 4.f * th16, 25.f * th16, 12.f * th16,121.f * pixelW, 31.f * pixelH,  0.f,  0.f, -1.f,

			// rhorn bottom
			 4.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 22.f * th16, 12.f * th16,120.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 5.f * th16, 22.f * th16, 13.f * th16,121.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,

			 5.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,
			 5.f * th16, 22.f * th16, 13.f * th16,121.f * pixelW, 31.f * pixelH,  0.f, -1.f,  0.f,
			 4.f * th16, 22.f * th16, 12.f * th16,120.f * pixelW, 32.f * pixelH,  0.f, -1.f,  0.f,

			// rhorn right
			 5.f * th16, 22.f * th16, 13.f * th16,120.f * pixelW, 28.f * pixelH,  1.f,  0.f,  0.f,
			 5.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 28.f * pixelH,  1.f,  0.f,  0.f,
			 5.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,

			 5.f * th16, 25.f * th16, 12.f * th16,121.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,
			 5.f * th16, 25.f * th16, 13.f * th16,120.f * pixelW, 31.f * pixelH,  1.f,  0.f,  0.f,
			 5.f * th16, 22.f * th16, 12.f * th16,121.f * pixelW, 28.f * pixelH,  1.f,  0.f,  0.f,

			// rhorn left
			 4.f * th16, 22.f * th16, 13.f * th16,119.f * pixelW, 28.f * pixelH, -1.f,  0.f,  0.f,
			 4.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
			 4.f * th16, 22.f * th16, 12.f * th16,118.f * pixelW, 28.f * pixelH, -1.f,  0.f,  0.f,

			 4.f * th16, 25.f * th16, 12.f * th16,118.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
			 4.f * th16, 22.f * th16, 12.f * th16,118.f * pixelW, 28.f * pixelH, -1.f,  0.f,  0.f,
			 4.f * th16, 25.f * th16, 13.f * th16,119.f * pixelW, 31.f * pixelH, -1.f,  0.f,  0.f,
		};


		m_creeper.first = 0;
		m_creeper.count = 192;
		m_zombie.first = 192;
		m_zombie.count = 192;
		m_cow.first = 384;
		m_cow.count = 294;

		m_vertexSize = 8;
		glCreateBuffers(1, &m_entityVBO);
		glNamedBufferData(m_entityVBO, sizeof(data), data, GL_STATIC_DRAW);
	}

	Mesh::~Mesh()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_blockVBO);
		glDeleteBuffers(1, &m_entityVBO);
	}

	const float halfSide = 0.5f;
	const float spriteWidth = 1.f / 32.f;
	const float spriteHeight = 1.f / 8.f;
	void Mesh::UpdateMovingBlock(std::vector<DynamicBlock> const& blocks)
	{
		m_blockVertexCount = (GLsizei)(36 * blocks.size());
		m_blockVertex.resize(0);

		for (DynamicBlock const& block : blocks)
		{
			float id = (float)block.currBlock - 1.f;
			Back(block, id);
			Front(block, id);
			Bottom(block, id);
			Top(block, id);
			Left(block, id);
			Right(block, id);
		}

		if (m_blockVertex.size() > 0)
		{
			glNamedBufferData(m_blockVBO, m_blockVertex.size() * sizeof(float), &m_blockVertex[0], GL_STATIC_DRAW);
		}
	}

	void Mesh::Back(DynamicBlock const& block, float spriteCol)
	{
		const float zBack = block.currPos.z - halfSide;
		const float spriteRow = 4.f;
		vec3 normal = vec3(0.f, 0.f, -1.f);

		Position(block.currPos.x - halfSide, block.currPos.y - halfSide, zBack);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, block.currPos.y + halfSide, zBack);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, block.currPos.y - halfSide, zBack);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);


		Position(block.currPos.x + halfSide, block.currPos.y + halfSide, zBack);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, block.currPos.y - halfSide, zBack);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, block.currPos.y + halfSide, zBack);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);
	}

	void Mesh::Bottom(DynamicBlock const& block, float spriteCol)
	{
		const float yBottom = block.currPos.y - halfSide;
		const float spriteRow = 2.f;
		vec3 normal = vec3(0.f, -1.f, 0.f);

		Position(block.currPos.x - halfSide, yBottom, block.currPos.z + halfSide);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, yBottom, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, yBottom, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);


		Position(block.currPos.x + halfSide, yBottom, block.currPos.z - halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, yBottom, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, yBottom, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);
	}

	void Mesh::Front(DynamicBlock const& block, float spriteCol)
	{
		const float zFront = block.currPos.z + halfSide;
		const float spriteRow = 6.f;
		vec3 normal = vec3(0.f, 0.f, 1.f);

		Position(block.currPos.x - halfSide, block.currPos.y - halfSide, zFront);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, block.currPos.y - halfSide, zFront);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, block.currPos.y + halfSide, zFront);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);


		Position(block.currPos.x + halfSide, block.currPos.y + halfSide, zFront);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, block.currPos.y + halfSide, zFront);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, block.currPos.y - halfSide, zFront);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);
	}

	void Mesh::Left(DynamicBlock const& block, float spriteCol)
	{
		const float xLeft = block.currPos.x - halfSide;
		const float spriteRow = 3.f;
		vec3 normal = vec3(-1.f, 0.f, 0.f);

		Position(xLeft, block.currPos.y - halfSide, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f)* spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(xLeft, block.currPos.y + halfSide, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(xLeft, block.currPos.y - halfSide, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);


		Position(xLeft, block.currPos.y + halfSide, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, (spriteRow +1.f) * spriteHeight);
		Normal(normal);

		Position(xLeft, block.currPos.y - halfSide, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(xLeft, block.currPos.y + halfSide, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);
	}

	void Mesh::Normal(vec3 const& normal)
	{
		m_blockVertex.push_back(normal.x);
		m_blockVertex.push_back(normal.y);
		m_blockVertex.push_back(normal.z);
	}

	void Mesh::Position(float x, float y, float z)
	{
		m_blockVertex.push_back(x);
		m_blockVertex.push_back(y);
		m_blockVertex.push_back(z);
	}

	void Mesh::Right(DynamicBlock const& block, float spriteCol)
	{
		const float xRight = block.currPos.x + halfSide;
		const float spriteRow = 5.f;
		vec3 normal = vec3(1.f, 0.f, 0.f);

		Position(xRight, block.currPos.y - halfSide, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(xRight, block.currPos.y - halfSide, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(xRight, block.currPos.y + halfSide, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);


		Position(xRight, block.currPos.y + halfSide, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(xRight, block.currPos.y + halfSide, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(xRight, block.currPos.y - halfSide, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);
	}

	void Mesh::Texel(float s, float t)
	{
		m_blockVertex.push_back(s);
		m_blockVertex.push_back(t);
	}

	void Mesh::Top(DynamicBlock const& block, float spriteCol)
	{
		const float yBottom = block.currPos.y + halfSide;
		const float spriteRow = 7.f;
		vec3 normal = vec3(0.f, 1.f, 0.f);

		Position(block.currPos.x - halfSide, yBottom, block.currPos.z + halfSide);
		Texel(spriteCol * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, yBottom, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, yBottom, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);


		Position(block.currPos.x + halfSide, yBottom, block.currPos.z - halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x - halfSide, yBottom, block.currPos.z - halfSide);
		Texel(spriteCol * spriteWidth, (spriteRow + 1.f) * spriteHeight);
		Normal(normal);

		Position(block.currPos.x + halfSide, yBottom, block.currPos.z + halfSide);
		Texel((spriteCol + 1.f) * spriteWidth, spriteRow * spriteHeight);
		Normal(normal);
	}
}
