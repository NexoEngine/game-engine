//// Camera.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Header file for the camera component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Transform.hpp"
#include "math/Vector.hpp"
#include "core/event/Input.hpp"
#include "renderer/Framebuffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace nexo::components {

	enum class CameraType {
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	struct CameraComponent {
		unsigned int width;
		unsigned int height;
		float fov = 45.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
		CameraType type = CameraType::PERSPECTIVE;

		glm::vec4 clearColor = {0.0f, 0.0f, 0.0f, 1.0f};

		bool active = true;
		bool main = true;
		bool resizing = false; // Internal use only

		std::shared_ptr<renderer::Framebuffer> m_renderTarget = nullptr;

		glm::mat4 getProjectionMatrix() const
		{
			if (type == CameraType::PERSPECTIVE) {
				return glm::perspective(glm::radians(fov), static_cast<float>(width) / height, nearPlane, farPlane);
			} else {
				return glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, nearPlane, farPlane);
			}
		}

		glm::mat4 getViewMatrix(const TransformComponent &transf) const
		{
			glm::vec3 front;
			glm::vec3 right;
			glm::vec3 up;

			math::extractCameraComponents(transf.rotation, front, right, up);
			return glm::lookAt(transf.pos, transf.pos + front, up);
		}

		void resize(unsigned int newWidth, unsigned int newHeight)
		{
			width = newWidth;
			height = newHeight;
			resizing = true;
			if (m_renderTarget) {
				m_renderTarget->resize(newWidth, newHeight);
			}
		}
	};

	struct PerspectiveCameraController {
		PerspectiveCameraController() { lastMousePosition = event::getMousePosition();}
		glm::vec2 lastMousePosition;
		float mouseSensitivity = 0.1f;
	};

	struct CameraContext {
		glm::mat4 viewProjectionMatrix;
		glm::vec3 cameraPosition;
		glm::vec4 clearColor;
		std::shared_ptr<renderer::Framebuffer> renderTarget;
	};
}
