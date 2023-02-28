#include "Library/Library.h"

#include <string>

#include "Library/Model/Fbx.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstace,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmpLine,
	_In_ int nCmdshow)
{
	// ライブラリ初期化
	if (!engine::Library::Init("test", 640, 480)) { return 0; }
	engine::Library::LoadObj("res/BirchTree_1.obj", "hikouki");
	engine::Library::LoadTexture(L"res/test.png", "texture");

	std::vector<int> keyList = { KEY_A ,KEY_D, KEY_W, KEY_S };

	float count{ 0.0f };

	engine::Library::SetCameraPos(engine::Vec4f(0.0f, 100.0f, 200.0f));
	engine::Library::SetFocusPos(engine::Vec4f(0.0f, 100.0f, 0.0f));
	engine::Library::SetDrawingDistance(0.1f, 1000.0f);

	while (engine::Library::CheckMessage())
	{
		// ライブラリ更新
		engine::Library::Update();

		// 描画開始
		engine::Library::StartRendering();


		//描画終了
		engine::Library::FinishRendering();
	}

	//engine::Library::ReleseObj("hikouki");

	engine::Library::Release();
}
