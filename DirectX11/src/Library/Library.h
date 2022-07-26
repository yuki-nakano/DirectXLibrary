/**
* @file Library.h
* @brief directX関数群
*/

#ifndef LIBRARY_H
#define LIBRARY_H

#include "DirectX/DirectXGraphics.h"
#include "DirectX/Graphics2D.h"
#include "Shader/ShaderManager.h"
#include "WindowsAPI/WindowsAPI.h"
#include "WindowsAPI/Input.h"
#include "WindowsAPI/Mouse.h"
#include "Model/ObjManager.h"
#include "Model/Camera.h"
#include "Model/Light.h"
#include "DirectX/Matrix.h"
#include "DirectX/DirectXUtility.h" 
#include "DirectX/DirectXTexture.h"

#include <memory>

namespace engine
{
	/**
	* @brief ライブラリクラス
	*/
	class Library
	{
	private:
		Library();
		~Library();

	public:
		/**
		* @brief ライブラリ初期化
		* @param window_name_　ウィンドウの名前
		* @param widht_ ウィンドウ横サイズ
		* @param height_ ウィンドウ縦サイズ
		* @return ライブラリの初期化成功時true
		* @note ゲームループ前に必ず1度呼び出す
		*/
		static bool Init(const std::string& window_name_, const int& widht_, const int& height_);

		/**
		* @brief ライブラリの解放
		* @note ゲームループ終了時に呼び出し
		* @attention この関数呼び出し後はInit関数以外の呼び出し禁止
		*/
		static void Release();

		/**
		* @brief ライブラリの更新
		* @note ゲームループ毎に呼び出す
		*/
		static void Update();

		//------------------------------------------------------------
		//   WindowsAPI
		//------------------------------------------------------------

		/**
		* @brief windowからのメッセージチェック
		* @return ループ終了時false
		* @note ゲームループ毎に呼び出す
		*/
		static bool CheckMwssage() { return m_instance->window->CheckMessage(); }


		//------------------------------------------------------------
		//   Input
		//------------------------------------------------------------

		/**
		* @brief 使用するキーの登録
		* @param key_list_ 登録するキーのリスト
		*/
		static void RegisterKey(std::vector<int> key_list_) { m_instance->input->RegisterKey(key_list_); }

		/**
		* @brief 指定されたキーが押されているか
		* @param key_ キーの指定
		* @return 押されていたらtrue
		*/
		static bool IsHeldKey(int key_) { return m_instance->input->IsHeldKey(key_); }

		/**
		* @brief 指定されたキーが押された瞬間なのか
		* @param key_ キーの指定
		* @return 押された瞬間ならtrue
		*/
		static bool IsPushedKey(int key_) { return m_instance->input->IsPushedKey(key_); }

		/**
		* @brief 指定されたキーが離されたか
		* @param key_ キーの指定
		* @return 離されていたらtrue
		*/
		static bool IsRelesedKey(int key_) { return m_instance->input->IsRelesedKey(key_); }

		/**
		* @brief キーの状態の取得
		* @param key_ キーの指定
		* @return キーの状態
		*/
		static KeyState GetKeyState(int key_) { return m_instance->input->GetKeyState(key_); }


		//------------------------------------------------------------
		//   mouse
		//------------------------------------------------------------

		/**
		* @brief マウスのx座標の取得
		* @return マウスのx座標
		*/
		static long GetMousePosX() { return m_instance->mouse->GetPosX(); };

		/**
		* @brief マウスのy座標の取得
		* @return マウスのy座標
		*/
		static long GetMousePosY() { return m_instance->mouse->GetPosY(); };


		//------------------------------------------------------------
		//   DirectXGraphics
		//------------------------------------------------------------

		/**
		* @brief レンダリングの開始
		* @note バッファの初期化を行うのでゲームループ毎の描画処理の前に呼び出す
		*/
		static void StartRendering() { m_instance->graphics->StartRendering(); }

		/**
		* @brief レンダリングの終了
		* @note バッファの切り替えを行うのでゲームループ毎の描画処理の終了の後に呼び出す
		*/
		static void FinishRendering() { m_instance->graphics->FinishRendering(); }


		//------------------------------------------------------------
		//   Graphics2D
		//------------------------------------------------------------

		/**
		* @brief 三角形の描画
		* @param pos_x_ 描画するx座標
		* @param pos_y_ 描画するy座標
		* @param widht_	三角形の横の辺の長さ
		* @param height_ 三角形の縦の辺の長さ
		* @param angle_ 描画する三角形の角度
		* @note 左上の頂点が90°の三角形の描画
		*/
		static void DrawPorigon(float pos_x_, float pos_y_, float width_, float height_, float angle_ = 0.0f) { m_instance->graphics2d->DrawPorigon(pos_x_, pos_y_, width_, height_, angle_); }

		/**
		* @brief 矩形の描画
		* @param pos_x_ 描画する矩形の左上のx座標
		* @param pos_y_ 描画する矩形の左上のy座標
		* @param widht_ 描画する矩形の横幅
		* @param height_ 描画する矩形の縦幅
		* @param angle_ 描画する矩形の角度
		* @note 各頂点の角度が90°の矩形の描画
		*/
		static void DrawRect(float pos_x_, float pos_y_, float width_, float height_, float angle_) { m_instance->graphics2d->DrawRect(pos_x_, pos_y_, width_, height_, angle_); }


		//------------------------------------------------------------
		//   GraphicsTexture
		//------------------------------------------------------------

		/**
		* @brief テクスチャの読み込み
		* @param file_name_　読み込むテクスチャのファイルパス
		* @param name_　テクスチャ呼び出し時の名前
		* @return テクスチャの読み込み成功時にtrue
		*/
		static bool LoadTexture(const std::wstring& file_name_, const std::string& name_) { return m_instance->texture->LoadTexture(file_name_, name_); }

		/**
		* @brief テクスチャの描画
		* @param name_ LoadTexture関数で登録した名前
		* @param pos_x_ 描画するテクスチャの左上のx座標
		* @param pos_y_ 描画するテクスチャの左上のy座標
		* @param widht_ 描画するテクスチャの横幅
		* @param height_ 描画するテクスチャの縦幅
		* @param angle_ 描画するテクスチャの角度
		*/
		static void DrawTexture(const std::string& name_, float pos_x_, float pos_y_, float width_, float height_, float angle_ = 0.0f) { m_instance->texture->DrawTexture(name_, pos_x_, pos_y_, width_, height_, angle_); }


		//------------------------------------------------------------
		//   Obj
		//------------------------------------------------------------

		/**
		* @brief objファイルの読み込み
		* @param file_name_ 読み込むobjのファイルパス
		* @param name_　obj呼び出し時の名前
		* @return obj読み込み成功時true
		*/
		static bool LoadObj(const std::string& file_name_, const std::string& name_) { return m_instance->obj->LoadObj(file_name_, name_); };

		/**
		* @brief 読み込んだobjの削除
		* @param LoadObj関数で登録した名前
		*/
		static void DeleteObj(const std::string& name_) { m_instance->obj->DeleteObj(name_); };

		/**
		* @brief obj描画
		* @param obj呼び出し時の名前
		*/
		static void RenderObj(const std::string& name_, Vec3f pos_, Vec3f rote_, Vec3f scale_) { m_instance->obj->RenderObj(name_, pos_, rote_, scale_); }

		/**
		* @brief 読み込んだobjファイルの解放
		* @param name_ LoadObj関数で登録した名前
		*/
		static void ReleseObj(const std::string& name_) { m_instance->obj->DeleteObj(name_); }


		//------------------------------------------------------------
		//   Camera
		//------------------------------------------------------------

		/**
		* @brief カメラの座標の変更
		* @param pos_ カメラ座標
		*/
		static void SetCameraPos(const Vec4f& pos_) { m_instance->camera->SetPos(pos_); }

		/**
		* @brief 注視点座標の変更
		* @param focus_ 注視点座標
		*/
		static void SetFocusPos(const Vec4f& focus_) { m_instance->camera->SetFocus(focus_); }

		/**
		* @brief 視野角の変更
		* @param fov_ 視野角
		*/
		static void SetFOV(const float& fov_) { m_instance->camera->SetFOV(fov_); }

		/**
		* @brief 描画距離の変更
		* @param near_ 手前のz座標
		* @param far_ 奥のz座標
		*/
		static void SetZ(const float& near_, const float& far_) { m_instance->camera->SetZ(near_, far_); }

		/**
		* @brief カメラの座標の取得
		* @return 現在のカメラ座標
		*/
		static Vec4f GetCameraPos() { return m_instance->camera->GetPos(); }

		/**
		* @brief 注視点座標の取得
		* @return 現在の注視点座標
		*/
		static Vec4f GetFocusPos() { return m_instance->camera->GetFocus(); }

	private:
		// インスタンス
		static Library* m_instance;

		WindowsAPI* window{ nullptr };
		InputManager* input{ nullptr };
		Mouse* mouse{ nullptr };
		DirectXGraphics* graphics{ nullptr };
		ShaderManager* shader{ nullptr };
		Graphics2D* graphics2d{ nullptr };
		DirectXTexture* texture{ nullptr };
		ObjManager* obj{ nullptr };
		Camera* camera{ nullptr };
		Light* light{ nullptr };
		Matrix* matrix{ nullptr };
	};
}

#endif // !LIBRARY_H
