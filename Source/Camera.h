#pragma once

#include "glmType.h"
#pragma warning( push )
#pragma warning( disable : 4201 )
#include <glm/gtc/matrix_transform.hpp>
#pragma warning( pop )

#include "Chunk.h"

namespace Minecraft
{
	class Camera
	{
	public:
		Camera() = delete;
		Camera(vec3 const& playerPosition, vec2 const& playerRotation, float const& playerHeight, float fieldOfView, float aspectRatio, float far);
		~Camera() = default;

		void Update();
		mat4x4 const& GetViewProjection() const { return m_viewProjection; }

		void UpdateFieldOfView(float fieldOfView) { m_fieldOfView = fieldOfView; m_halfFov = fieldOfView * m_aspectRatio * .5f; UpdateProjection(); }
		void UpdateAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; m_halfFov = m_fieldOfView * aspectRatio * .5f;; UpdateProjection(); }
		void UpdateFar(float far) { m_far = far; UpdateProjection(); }

		bool See(Chunk const& chunk) const;
		bool See(float x, float z, float radius) const;

	private:
		void UpdateProjection() { m_projection = glm::perspective(m_fieldOfView, m_aspectRatio, 0.1f, m_far); }

		vec3 const& m_playerPosition;
		vec2 const& m_playerRotation;
		float const& m_playerHeight;

		float m_fieldOfView;
		float m_halfFov;
		float m_aspectRatio;
		float m_far;
		mat4x4 m_projection = glm::identity<mat4x4>();
		mat4x4 m_viewProjection = glm::identity<mat4x4>();

		vec4 m_left = vec4();
		vec4 m_right = vec4();
	};
}
