#include "Library/Library.h"

#include <string>

int WINAPI WinMain(
	_In_ HINSTANCE hInstace,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmpLine,
	_In_ int nCmdshow)
{
	// ライブラリ初期化
	if (!engine::Library::Init("test", 640, 480)) { return 0; }
	engine::Library::LoadObj("res/Cube.obj", "hikouki");
	engine::Library::LoadTexture(L"res/test.png", "texture");

	std::vector<int> keyList = { KEY_A ,KEY_D, KEY_W, KEY_S };
	engine::Library::RegisterKey(keyList);

	engine::Vec3f pos(0, -10, 0);
	engine::Vec3f rote(0, 0, 0);
	engine::Vec3f scale(8, 8, 8);

	int count = 0;

	while (engine::Library::CheckMessage())
	{
		// ライブラリ更新
		engine::Library::Update();

		// 描画開始
		engine::Library::StartRendering();

		//engine::Library::DrawRect(engine::Vec2f(100.0f, 100.0f), 100, 100, engine::Vec3f(0.0f, 0.0f, 1.0f), 0.3f, 0.0f);

		rote.y += 1;
		engine::Library::RenderObj("hikouki", pos, rote, scale);

		//engine::Library::DrawTexture("texture", engine::Vec2f(0.0f, 0.0f), 100, 100, 0.0f, 0.2f);

		engine::KeyState keystate = engine::Library::GetKeyState(KEY_A);

		switch (keystate)
		{
		case engine::KeyState::Not:
			//engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		case engine::KeyState::Held:
			pos.z -= 1.0f;
			//engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		case engine::KeyState::Pushed:
			pos.x += 1.0f;
			engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		case engine::KeyState::Relesed:
			engine::Library::RenderObj("hikouki", pos, rote, scale);
			break;
		default:
			break;
		}

		count++;
		int tmp = count / 60;
		std::wstring time = std::to_wstring(tmp);
		engine::Library::DrawTextData(time, engine::Vec2f(0, 0));
		engine::Library::DrawTextData(time, engine::Vec2f(0, 100));


		//描画終了
		engine::Library::FinishRendering();
	}

	//engine::Library::ReleseObj("hikouki");

	engine::Library::Release();
}
