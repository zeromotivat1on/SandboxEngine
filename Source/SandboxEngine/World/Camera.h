#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Dirty.h"
#include "SandboxEngine/Render/Camera.h"

namespace snd
{
	class Camera
	{
	public:
		enum class Type : u8
		{
			Orthographic,
			Perspective,
		};

	public:
								Camera(Type type);

		void					Tick(f32 dt);

		glm::vec3 				Location() const;
		glm::vec3 				Target() const;

		glm::vec3 				ForwardVector();
		glm::vec3 				RightVector();

		glm::mat4 				ViewMatrix();
		glm::mat4 				ProjectionMatrix();
		
		void 					SetLocation(const glm::vec3& location);
		void 					SetTarget(const glm::vec3& target);

		void 					SetPerspective(f32 fov, f32 aspect, f32 n, f32 f);
		void 					SetOrthographic(f32 l, f32 r, f32 b, f32 t, f32 n, f32 f);
		void 					SetOrthographic(const glm::vec4& ortho, f32 n, f32 f); // ortho should be as follows - [l, r, b, t]

		void 					MoveForward(f32 delta);
		void 					MoveRight(f32 delta);
		
		void 					RotateUp(f32 angle);
		void 					RotateRight(f32 angle);

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
