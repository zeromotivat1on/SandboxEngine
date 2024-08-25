#pragma once

#include "SandboxEngine/Render/Camera.h"
#include "SandboxEngine/Core/Core.h"

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

		void Tick(float dt);

		void SetLocation(const glm::vec3& location);
		void SetTarget(const glm::vec3& target);

		void SetPerspective(float fov, float aspect, float n, float f);
		void SetOrthographic(float l, float r, float b, float t, float n, float f);
		void SetOrthographic(const glm::vec4& ortho, float n, float f); // ortho should be as follows - [l, r, b, t]

		glm::vec3 GetLocation() const;
		glm::vec3 GetTarget() const;

		glm::vec3 GetForwardVector();
		glm::vec3 GetRightVector();

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

		void MoveForward(float delta);
		void MoveRight(float delta);
		
		void RotateUp(float angle);
		void RotateRight(float angle);

	private:
		render::Camera m_Proxy = {};
		
		Dirty<glm::mat4> m_ViewMatrix = {};
		Dirty<glm::mat4> m_ProjectionMatrix = {};
		
		Dirty<glm::vec3> m_ForwardVector = {};
		Dirty<glm::vec3> m_RightVector = {};
		
		Type m_Type = Type::Perspective;
	};
}
