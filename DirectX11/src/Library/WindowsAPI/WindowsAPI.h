#ifndef WINDOWS_API_H
#define WINDOWS_API_H

#include <Windows.h>
#include <string>

#include "../Utility/Singleton.h"

namespace engine
{
	/**
	* @brief Window�`��
	*/
	class WindowsAPI : public Singleton<WindowsAPI>
	{
	private:
		friend class Singleton<WindowsAPI>;

		WindowsAPI() {};
		~WindowsAPI() {};

	public:
		/**
		* @brief window����̃��b�Z�[�W�`�F�b�N
		* @return �I������false
		*/
		bool CheckMessage();

		/**
		* @brief �E�C���h�E�`��
		* @param window_name_ �E�B���h�E�̖��O
		* @param widht_ �E�B���h�E�̉���
		* @param height_ �E�B���h�E�̏c��
		*/
		bool Draw(const std::string& window_name_, const int& widht_, const int& height_);

		/**
		* @brief �v���V�[�W��
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
