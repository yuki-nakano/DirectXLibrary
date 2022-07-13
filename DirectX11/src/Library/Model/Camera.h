#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

#include "../DirectX/DirectXUtility.h"
#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief �J�������ۊǃN���X
	*/
	class Camera : public Singleton<Camera>
	{
	public:
		struct View
		{
			DirectX::XMVECTOR vecPos;	/// �J�������W
			DirectX::XMVECTOR vecFocus;	/// �����_
			DirectX::XMVECTOR vecUp;	/// �J�����̏�����x�N�g��
		};

		struct Proj
		{
			float fov;		/// ����p
			float nearZ;	/// �ŒZ����
			float farZ;		/// �Œ�����
		};

	private:
		friend class Singleton<Camera>;

		Camera();
		~Camera();

	public:
		//�A�N�Z�T

		void SetPos(Vec4f pos_) { pos = pos_; view.vecPos = DirectX::XMVectorSet(pos.x, pos.y, pos.z, pos.w); }
		void SetFocus(Vec4f focus_) { focus = focus_; view.vecFocus = DirectX::XMVectorSet(focus.x, focus.y, focus.z, focus.w); }
		void SetFOV(float fov_) { proj.fov = DirectX::XMConvertToRadians(fov_); }
		void SetZ(float near_, float far_) { if (near_ < far_) { return; }; proj.nearZ = near_; proj.farZ = far_; }

		Vec4f GetPos() { return pos; }
		Vec4f GetFocus() { return focus; }
		Vec4f GetUp() { return up; }
		View GetView() { return view; }
		Proj GetProj() { return proj; }

	private:
		Vec4f pos;		/// �J�������W 
		Vec4f focus;	/// �����_
		Vec4f up;		/// �J�����̏�����x�N�g��
		View view;
		Proj proj;
	};
}

#endif // !CAMERA_H
