#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Dirty.h"
#include "SandboxEngine/Render/Camera.h"

namespace snd
{
	class Camera
	{
	public:
		enum class Type : uint8_t
		{
			Orthographic,
			Perspective,
		};

	public:
								Camera(Type type);

		void					Tick(float dt);

		glm::vec3 				Location() const;
		glm::vec3 				Target() const;

		glm::vec3 				ForwardVector();
		glm::vec3 				RightVector();

		glm::mat4 				ViewMatrix();
		glm::mat4 				ProjectionMatrix();
		
		void 					SetLocation(const glm::vec3& location);
		void 					SetTarget(const glm::vec3& target);

		void 					SetPerspective(float fov, float aspect, float n, float f);
		void 					SetOrthographic(float l, float r, float b, float t, float n, float f);
		void 					SetOrthographic(const glm::vec4& ortho, float n, float f); // ortho should be as follows - [l, r, b, t]

		void 					MoveForward(float delta);
		void 					MoveRight(float delta);
		
		void 					RotateUp(float angle);
		void 					RotateRight(float angle);

	private:
		render::Camera			m_Proxy;
		Dirty<glm::vec3> 		m_ForwardVector;
		Dirty<glm::vec3> 		m_RightVector;
		Dirty<glm::mat4> 		m_ViewMatrix;
		Dirty<glm::mat4> 		m_ProjectionMatrix;
		Type					m_Type;
	};

	SND_INLINE glm::vec3 Camera::Location() const
	{
		return m_Proxy.Eye;
	}

	SND_INLINE glm::vec3 Camera::Target() const
	{
		return m_Proxy.At;
	}
}
