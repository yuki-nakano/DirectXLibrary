#ifndef MATRIX_H
#define MATRIX_H

#include <DirectXMath.h>

#include "DirectXUtility.h"
#include "../Utility/Singleton.h"

namespace engine
{
	class Matrix : public Singleton<Matrix>
	{
	private:
		friend class Singleton<Matrix>;

		Matrix();
		~Matrix();

	public:
		/**
		* @breif ���[���h���W�ϊ��s��̐���
		* @param pos_ �ړ���
		* @param rote_ �p�x
		* @param scale_ �傫��
		* @retun ���[���h���W�ϊ��s��
		*/
		DirectX::XMMATRIX CreateWorldMatrix(Vec3f pos_, Vec3f rote_, Vec3f scale_);

		/**
		* @breif �r���[���W�ϊ��s��̐���
		* @retun �r���[���W�ϊ��s��
		*/
		DirectX::XMMATRIX CreateViewMatrix();

		/**
		* @breif �v���W�F�N�V�������W�ϊ��s��̐���
		* @retun �v���W�F�N�V�������W�ϊ��s��
		*/
		DirectX::XMMATRIX CreateProjMatrix();

		DirectX::XMFLOAT4 GetViewportMatrix();

		void SetViewport(Vec2f viewport_) { viewport = viewport_; }

	private:
		Vec2f viewport;
	};
}

#endif // !MATRIX_H
