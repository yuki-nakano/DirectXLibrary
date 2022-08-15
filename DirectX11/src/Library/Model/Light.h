#ifndef LIGHT_H
#define LIGHT_H

#include "../Utility/Singleton.h"
#include "../DirectX/DirectXUtility.h"

namespace engine
{
	/**
	* @brief ライト情報保管
	*/
	class Light : public Singleton<Light>
	{
	private:
		friend class Singleton<Light>;

		Light() {};
		~Light() {};

	public:

		// アクセサ

		Vec4f GetPos() { return pos; }
		Vec4f GetCol() { return col; }

		void SetPos(Vec4f pos_) { pos = pos_; pos.w = 1.0f; }
		void SetCol(Vec4f col_) { col = col_; }
	private:
		/// ライト座標
		Vec4f pos{ 0.0f, 100.0f, 0.0f, 1.0f };

		/// ライトの色
		Vec4f col{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}

#endif // !LIGHT_H
