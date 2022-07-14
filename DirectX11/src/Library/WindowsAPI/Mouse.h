#ifndef MOUSUE_H

#include <Windows.h>

#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief マウス情報クラス
	*/
	class Mouse : public Singleton<Mouse>
	{
	private:
		friend class Singleton<Mouse>;

		Mouse() = default;
		~Mouse() = default;

	public:
		/**
		* @brief マウス情報の更新
		*/
		void Update();

		// アクセサ

		long GetPosX() { return screenPos.x; }
		long GetPosY() { return screenPos.y; }

	private:
		POINT screenPos{};	/// 画面上の座標
		POINT clientPos{};	/// ウインドウ上の座標


	};
}
#endif // !MOUSUE_H
