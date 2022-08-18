#include "Matrix.h"

#include "../Model/Camera.h"
#include "../Model/Light.h"

namespace engine
{
	DirectX::XMMATRIX Matrix::CreateWorldMatrix(Vec3f pos_, Vec3f rote_, Vec3f scale_)
	{
		DirectX::XMMATRIX transrate = DirectX::XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
		DirectX::XMMATRIX rotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rote_.x));
		DirectX::XMMATRIX rotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rote_.y));
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rote_.z));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);

		return scale * rotateX * rotateY * rotateZ * transrate;
	}

	DirectX::XMMATRIX Matrix::CreateWorldMatrix2D(Vec2f pos_, float width_, float height_, float degree_)
	{
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(degree_ * (3.14f / 180.0f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos_.x, pos_.y, 0);
		return scale * rotateZ * translate;
	}

	DirectX::XMMATRIX Matrix::CreateViewMatrix()
	{
		Camera::View view = Camera::GetInstance()->GetView();
		return DirectX::XMMatrixLookAtLH(view.vecPos, view.vecFocus, view.vecUp);
	}

	DirectX::XMMATRIX Matrix::CreateProjMatrix()
	{
		Camera::Proj proj = Camera::GetInstance()->GetProj();
		return DirectX::XMMatrixPerspectiveFovLH(proj.fov, (viewport.x / viewport.y), proj.nearZ, proj.farZ);
	}

	DirectX::XMFLOAT4 Matrix::GetViewportMatrix()
	{
		return DirectX::XMFLOAT4(viewport.x, viewport.y, 0.0f, 1.0f);
	}
	DirectX::XMVECTOR Matrix::CreateLightPosMatrix()
	{
		Vec4f lightPos = Light::GetInstance()->GetPos();
		return DirectX::XMVector3Normalize(DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, lightPos.w));
	}
	DirectX::XMFLOAT4 Matrix::CreateLightColMatrix()
	{
		Vec4f LightCol = Light::GetInstance()->GetCol();
		return DirectX::XMFLOAT4(LightCol.x, LightCol.y, LightCol.z, LightCol.w);
	}
}