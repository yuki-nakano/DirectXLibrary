#include "Camera.h"

namespace engine
{
	Camera::Camera()
	{
		pos = { 0.0f, 0.0f, -50.0f, 0.0f };
		view.vecPos = DirectX::XMVectorSet(pos.x, pos.y, pos.z, pos.w);

		focus = { 0.0f, 0.0f, 0.0f, 0.0f };
		view.vecFocus = DirectX::XMVectorSet(focus.x, focus.y, focus.z, focus.w);

		up = { 0.0f, 1.0f, 0.0f, 0.0f };
		view.vecUp = DirectX::XMVectorSet(up.x, up.y, up.z, up.w);

		proj.fov = DirectX::XMConvertToRadians(45.0f);
		proj.nearZ = 0.1f;
		proj.farZ = 100.0f;
	}
}

