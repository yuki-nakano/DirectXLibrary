#include "WindowsAPI.h"

namespace engine
{
	const std::string WindowsAPI::m_windowClassName = "WindowClass";

	bool WindowsAPI::CheckMessage()
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return true;
	}

	bool WindowsAPI::Draw(const std::string& window_name_, const int& widht_, const int& height_)
	{
		m_windowClass = {
			sizeof(WNDCLASSEX),
			CS_HREDRAW | CS_VREDRAW,
			Procedure,
			0,
			0,
			GetModuleHandle(NULL),
			LoadIcon(NULL, IDI_APPLICATION),
			LoadCursor(NULL, IDC_ARROW),
			NULL,
			NULL,
			m_windowClassName.c_str(),
			NULL };

		if (RegisterClassEx(&m_windowClass) == 0)
		{
			return false;
		}

		//ウィンドウ作成
		HWND windowHandle = CreateWindow(
			m_windowClassName.c_str(),
			window_name_.c_str(),
			(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			widht_,
			height_,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL);

		if (windowHandle == NULL)
		{
			return false;
		}

		//ウィンドウのリサイズ
		RECT windowRect;
		GetWindowRect(windowHandle, &windowRect);

		RECT clientRect;
		GetClientRect(windowHandle, &clientRect);

		int frameSizeX = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
		int frameSizeY = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

		SetWindowPos(
			windowHandle,
			NULL,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			frameSizeX + widht_,
			frameSizeY + height_,
			SWP_NOMOVE);

		ShowWindow(windowHandle, SW_SHOW);

		return true;
	}

	LRESULT WindowsAPI::Procedure(HWND window_handle_, UINT message_id_, WPARAM wparam_, LPARAM lparam_)
	{
		switch (message_id_)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(window_handle_, message_id_, wparam_, lparam_);
			break;
		}

		return 0;
	}
}