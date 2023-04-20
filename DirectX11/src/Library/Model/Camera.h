#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

#include "../DirectX/DirectXUtility.h"
#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief カメラ情報保管クラス
	*/
	class Camera : public Singleton<Camera>
	{
	public:
		struct View
		{
			DirectX::XMVECTOR vecPos;	/// カメラ座標
			DirectX::XMVECTOR vecFocus;	/// 注視点
			DirectX::XMVECTOR vecUp;	/// カメラの上方向ベクトル
		};

		struct Proj
		{
			float fov;		/// 視野角
			float nearZ;	/// 最短距離
			float farZ;		/// 最長距離
		};

	private:
		friend class Singleton<Camera>;

		Camera();
		~Camera() = default;

	public:
		//アクセサ

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
		Vec4f pos{};	/// カメラ座標 
		Vec4f focus{};	/// 注視点
		Vec4f up{};		/// カメラの上方向ベクトル
		View view{};
		Proj proj{};
	};
}

#endif // !CAMERA_H
