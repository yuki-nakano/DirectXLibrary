﻿#include "Library/Library.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstace,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmpLine,
	_In_ int nCmdshow)
{
	// ライブラリ初期化
	if (!engine::Library::Init("test", 640, 480)) { return 0; }
	engine::Library::LoadObj("res/hikouki.obj", "hikouki");

	std::vector<int> keyList = { KEY_A };
	engine::Library::RegisterKey(keyList);

	engine::Vec3f pos(0, -10, 0);
	engine::Vec3f rote(-20, 0, -30);
	engine::Vec3f scale(1, 1, 1);

	while (engine::Library::CheckMwssage())
	{
		// ライブラリ更新
		engine::Library::Update();

		// 描画開始
		engine::Library::StartRendering();

		//engine::Library::DrawRect(0, 0, 100, 100, 0.0f);

		rote.y += 0.01f;

		engine::KeyState keystate = engine::Library::GetKeyState(KEY_A);

		switch (keystate)
		{
		case engine::KeyState::Not:
			//engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		case engine::KeyState::Held:
			//engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		case engine::KeyState::Pushed:
			engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		case engine::KeyState::Relesed:
			engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		default:
			break;
		}

		engine::Library::RenderObj("hikouki", pos, rote, scale);

		//描画終了
		engine::Library::FinishRendering();
	}

	//engine::Library::ReleseObj("hikouki");

	engine::Library::Release();
}
