#pragma once

#include "SandboxEngine/Render/Camera.h"
#include "SandboxEngine/Core/Core.h"

namespace snd
{
	class Camera
	{
	public:
		enum class Type
		{
			Orthographic,
			Perspective,
		};

	public:
		Camera(Camera::Type type);

		void Tick(float dt);

		void SetLocation(const glm::vec3& location);
		void SetTarget(const glm::vec3& target);

		void SetPerspective(float fov, float aspect, float n, float f);
		void SetOrthographic(float l, float r, float b, float t, float n, float f);
		void SetOrthographic(const glm::vec4& ortho, float n, float f); // ortho should be as follows - [l, r, b, t]

		const glm::vec3& GetLocation() const;
		const glm::vec3& GetTarget() const; const

		const glm::vec3& GetForwardVector();
		const glm::vec3& GetRightVector();

		const glm::mat4& GetViewMatrix();
		const glm::mat4& GetProjectionMatrix();

		inline void SetMoveStraightDelta(float delta) { m_MoveStraightDelta = delta; }
		inline void SetMoveSidewayDelta(float delta) { m_MoveSidewayDelta = delta; }
		inline void SetRotateHorizontallyAngle(float angle) { m_RotateHorizontallyAngle = angle; }
		inline void SetRotateVerticallyAngle(float angle) { m_RotateVerticallyAngle = angle; }

	private:
		void MoveStraight(float delta);			// positive delta - move forward
		void MoveSideway(float delta);			// positive delta - move right
		void RotateHorizontally(float angle);	// positive angle - rotate right
		void RotateVertically(float angle);		// positive angle - rotate up

	private:
		render::Camera m_Proxy = {};
		
		Dirty<glm::mat4> m_ViewMatrix = {};
		Dirty<glm::mat4> m_ProjectionMatrix = {};
		Dirty<glm::vec3> m_ForwardVector = {};
		Dirty<glm::vec3> m_RightVector = {};
		
		Type m_Type = Type::Perspective;

		float m_MoveStraightDelta = 0.0f;
		float m_MoveSidewayDelta = 0.0f;
		float m_RotateHorizontallyAngle = 0.0f;
		float m_RotateVerticallyAngle = 0.0f;
	};
}
