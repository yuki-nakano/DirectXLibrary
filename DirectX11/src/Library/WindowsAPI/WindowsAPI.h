#ifndef WINDOWS_API_H
#define WINDOWS_API_H

#include <Windows.h>
#include <string>

#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief Window描画
	*/
	class WindowsAPI : public Singleton<WindowsAPI>
	{
	private:
		friend class Singleton<WindowsAPI>;

		WindowsAPI() {};
		~WindowsAPI() {};

	public:
		/**
		* @brief windowからのメッセージチェック
		* @return 終了時にfalse
		*/
		bool CheckMessage();

		/**
		* @brief ウインドウ描画
		* @param window_name_ ウィンドウの名前
		* @param widht_ ウィンドウの横幅
		* @param height_ ウィンドウの縦幅
		*/
		bool Draw(const std::string& window_name_, const int& widht_, const int& height_);

		/**
		* @brief プロシージャ
		*/
		static LRESULT CALLBACK Procedure(HWND window_handle_, UINT message_id_, WPARAM wparam, LPARAM lparam);

	public:
		static const std::string m_windowClassName;


	private:
		WNDCLASSEX m_windowClass{};

		MSG msg{};
	};
}
#endif // !WINDOW_API_H
