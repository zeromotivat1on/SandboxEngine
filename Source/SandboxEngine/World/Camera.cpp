#include "sndpch.h"
#include "SandboxEngine/World/Camera.h"
#include "SandboxEngine/Core/Input.h"
#include <glm/gtx/rotate_vector.hpp>

snd::Camera::Camera(Type type)
	: m_Type(type)
{
	m_Proxy.Up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void snd::Camera::Tick(float dt)
{
	if (input::ButtonDown(input::KeyboardBit::A))
	{
		MoveRight(-1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::D))
	{
		MoveRight(1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::S))
	{
		MoveForward(-1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::W))
	{
		MoveForward(1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::Left))
	{
		RotateRight(-1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::Right))
	{
		RotateRight(1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::Down))
	{
		RotateUp(-1.0f);
	}

	if (input::ButtonDown(input::KeyboardBit::Up))
	{
		RotateUp(1.0f);
	}
}

void snd::Camera::SetLocation(const glm::vec3& location)
{
	m_Proxy.Eye = location;

	m_ViewMatrix.MakeDirty();
	m_RightVector.MakeDirty();
	m_ForwardVector.MakeDirty();
}

void snd::Camera::SetTarget(const glm::vec3& target)
{
	m_Proxy.At = target;

	m_ViewMatrix.MakeDirty();
	m_RightVector.MakeDirty();
	m_ForwardVector.MakeDirty();
}

void snd::Camera::SetPerspective(float fov, float aspect, float n, float f)
{
	SND_ASSERT(m_Type == Type::Perspective, "Camera is not perspective");

	m_Proxy.Fov = fov;
	m_Proxy.Aspect = aspect;
	m_Proxy.Near = n;
	m_Proxy.Far = f;

	m_ProjectionMatrix.MakeDirty();
}

void snd::Camera::SetOrthographic(float l, float r, float b, float t, float n, float f)
{
	SND_ASSERT(m_Type == Type::Orthographic, "Camera is not orthographic");

	m_Proxy.Left = l;
	m_Proxy.Right = r;
	m_Proxy.Bottom = b;
	m_Proxy.Top = t;
	m_Proxy.Near = n;
	m_Proxy.Far = f;

	m_ProjectionMatrix.MakeDirty();
}

void snd::Camera::SetOrthographic(const glm::vec4& ortho, float n, float f)
{
	SetOrthographic(ortho[0], ortho[1], ortho[2], ortho[3], n, f);
}

glm::vec3 snd::Camera::Location() const
{
	return m_Proxy.Eye;
}

glm::vec3 snd::Camera::Target() const
{
	return m_Proxy.At;
}

glm::vec3 snd::Camera::ForwardVector()
{
	if (m_ForwardVector.IsDirty())
	{
		m_ForwardVector = m_Proxy.ForwardVector();
		m_ForwardVector.ClearDirty();
	}

	return m_ForwardVector.Current();
}

glm::vec3 snd::Camera::RightVector()
{
	if (m_RightVector.IsDirty())
	{
		m_RightVector = m_Proxy.RightVector();
		m_RightVector.ClearDirty();
	}

	return m_RightVector.Current();
}

glm::mat4 snd::Camera::ViewMatrix()
{
	if (m_ViewMatrix.IsDirty())
	{
		m_ViewMatrix = m_Proxy.ViewMatrix();
		m_ViewMatrix.ClearDirty();
	}

	return m_ViewMatrix.Current();
}

glm::mat4 snd::Camera::ProjectionMatrix()
{
	if (m_ProjectionMatrix.IsDirty())
	{
		switch (m_Type)
		{
		case Type::Orthographic:
			m_ProjectionMatrix = m_Proxy.OrthographicProjectionMatrix();
			break;
		case Type::Perspective:
			m_ProjectionMatrix = m_Proxy.PerpsectiveProjectionMatrix();
			break;
		default:
			SND_LOG_ERROR("Unknown camera type {}", static_cast<int32_t>(m_Type));
		}

		m_ProjectionMatrix.ClearDirty();
	}

	return m_ProjectionMatrix.Current();
}

void snd::Camera::MoveForward(float delta)
{
	const glm::vec3 forward = ForwardVector();
	SetLocation(Location() + forward * delta);
	SetTarget(Target() + forward * delta);
}

void snd::Camera::MoveRight(float delta)
{
	const glm::vec3 right = RightVector();
	SetLocation(Location() + right * delta);
	SetTarget(Target() + right * delta);
}

void snd::Camera::RotateUp(float angle)
{
	const glm::vec3 rotateDirection = glm::rotate(ForwardVector(), glm::radians(angle), m_Proxy.RightVector());
	SetTarget(Location() + rotateDirection);
}

void snd::Camera::RotateRight(float angle)
{
	const glm::vec3 rotateDirection = glm::rotateY(ForwardVector(), glm::radians(-angle));
	SetTarget(Location() + rotateDirection);
}
