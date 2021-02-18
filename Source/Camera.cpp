#include "Camera.h"

#include "Trigo.h"

namespace Minecraft
{
	Camera::Camera(vec3 const& playerPosition, vec2 const& playerRotation, float const& playerHeight, float fieldOfView, float aspectRatio, float far) :
		m_playerPosition(playerPosition),
		m_playerRotation(playerRotation),
		m_playerHeight(playerHeight),
		m_fieldOfView(fieldOfView),
		m_halfFov(fieldOfView* aspectRatio * .5f),
		m_aspectRatio(aspectRatio),
		m_far(far)
	{
		UpdateProjection();
	}

	void Camera::Update()
	{
		mat4x4 view = mat4x4(1.f);
		view = rotate(view, -m_playerRotation.y, vec3(1.f, 0.f, 0.f));
		view = rotate(view, -m_playerRotation.x, vec3(0.f, 1.f, 0.f));
		view = translate(view, -vec3(m_playerPosition.x, m_playerPosition.y + m_playerHeight, m_playerPosition.z));

		m_viewProjection = m_projection * view;

		// left
		float x = sinf(m_playerRotation.x - m_halfFov);
		float z = cosf(m_playerRotation.x - m_halfFov);
		m_right.x = -z; // do a -90 degree rotation
		m_right.z = x;
		m_right.w = m_playerPosition.x * z - m_playerPosition.z * x;

		// right
		x = sinf(m_playerRotation.x + m_halfFov);
		z = cosf(m_playerRotation.x + m_halfFov);
		m_left.x = z; // do a +90 degree rotation
		m_left.z = -x;
		m_left.w = m_playerPosition.z * x - m_playerPosition.x * z;
	}

	bool Camera::See(Chunk const& chunk) const
	{
		return See(float((chunk.chunkXPos + .5f) * CHUNK_X_SIZE), float((chunk.chunkZPos + .5f) * CHUNK_Z_SIZE), Chunk::s_radius);
	}

	bool Camera::See(float x, float z, float radius) const
	{
		// m_left.w -> distance from origine
		return (x * m_left.x + z * m_left.z + m_left.w + radius > 0)
			&& (x * m_right.x + z * m_right.z + m_right.w + radius > 0);
	}
}