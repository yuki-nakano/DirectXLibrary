#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include <vector>
#include <map>

#include "../Utility/Singleton.h"

namespace engine
{
	/// keyの状態
	enum class KeyState
	{
		Not,	///押されていない
		Pushed,	///押した瞬間
		Held,	///押している
		Relesed	///離した
	};

	/**
	* @brief キー情報クラス
	*/
	class InputManager : public Singleton<InputManager>
	{
	private:
		friend class Singleton<InputManager>;

		InputManager() = default;
		~InputManager() = default;

	public:
		/**
		* @brief キーの押下状態の更新
		* @note ループごとに1回呼び出す
		*/
		void UpdateKeyState();

		/**
		* @brief 使用するキーの登録
		* @param 登録するキーのリスト
		*/
		void RegisterKey(std::vector<int> key_list_);

		/**
		* @brief 指定されたキーが押されているか
		* @param キーの指定
		* @return 押されていたらtrue
		*/
		bool IsHeldKey(int key_);

		/**
		* @brief 指定されたキーが押された瞬間なのか
		* @param キーの指定
		* @return 押された瞬間ならtrue
		*/
		bool IsPushedKey(int key_);

		/**
		* @brief 指定されたキーが離されたか
		* @param キーの指定
		* @return 離されていたらtrue
		*/
		bool IsRelesedKey(int key_);

		/**
		* @brief キーの状態の取得
		* @param キーの指定
		* @return キーの状態
		*/
		KeyState GetKeyState(int key_);

	private:
		/// 登録したキー情報保管
		std::map<int, KeyState> m_keyState;
	};
}

//WinUser.h参照でも可
#define KEY_LMOUSE	0x01	// left mouse
#define KEY_RMOUSE	0x02	// right mouse
#define KEY_TAB		0x09	// TAB key
#define KEY_ENTER	0x0D	// ENTER key
#define KEY_SHIFT	0x10	// SHIFT key
#define KEYCTRL		0x11	// CTRL key
#define KEY_ALT		0x12	// ALT key
#define KEY_ESC		0x1B	// ESC key
#define KEY_SPACE	0x20	// SPACEBAR
#define KEY_0		0x30	// 0 key
#define KEY_1		0x31	// 1 key
#define KEY_2		0x32	// 2 key
#define KEY_3		0x33	// 3 key
#define KEY_4		0x34	// 4 key
#define KEY_5		0x35	// 5 key
#define KEY_6		0x36	// 6 key
#define KEY_7		0x37	// 7 key
#define KEY_8		0x38	// 8 key
#define KEY_9		0x39	// 9 key
#define KEY_A		0x41	// A key
#define KEY_B		0x42	// B key
#define KEY_C		0x43	// C key
#define KEY_D		0x44	// D key
#define KEY_E		0x45	// E key
#define KEY_F		0x46	// F key
#define KEY_G		0x47	// G key
#define KEY_H		0x48	// H key
#define KEY_I		0x49	// I key
#define KEY_J		0x4A	// J key
#define KEY_K		0x4B	// K key
#define KEY_L		0x4C	// L key
#define KEY_M		0x4D	// M key
#define KEY_N		0x4E	// N key
#define KEY_O		0x4F	// O key
#define KEY_P		0x50	// P key
#define KEY_Q		0x51	// Q key
#define KEY_R		0x52	// R key
#define KEY_S		0x53	// S key
#define KEY_T		0x54	// T key
#define KEY_U		0x55	// U key
#define KEY_V		0x56	// V key
#define KEY_W		0x57	// W key
#define KEY_X		0x58	// X key
#define KEY_Y		0x59	// Y key
#define KEY_Z		0x5A	// Z key

#endif // !INPUT_H
