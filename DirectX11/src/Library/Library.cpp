#include "Library.h"

namespace engine
{
	Library* Library::m_instance = nullptr;

	Library::Library()
	{
		window = WindowsAPI::CreateInstance();
		input = InputManager::CreateInstance();
		mouse = Mouse::CreateInstance();
		graphics = DirectXGraphics::CreateInstance();
		camera = Camera::CreateInstance();
		light = Light::CreateInstance();
		matrix = Matrix::CreateInstance();
		shader = ShaderManager::CreateInstance();
		graphics2d = Graphics2D::CreateInstance();
		direct2D = Direct2D::CreateInstance();
		texture = DirectXTexture::CreateInstance();
		obj = ObjManager::CreateInstance();
		fbx = FBXManager::CreateInstance();
	}

	Library::~Library()
	{
		input->DeleteInstance();
		mouse->DeleteInstance();
		direct2D->DeleteInstance();
		graphics->DeleteInstance();
		camera->DeleteInstance();
		light->DeleteInstance();
		matrix->DeleteInstance();
		shader->DeleteInstance();
		graphics2d->DeleteInstance();
		texture->DeleteInstance();
		obj->DeleteInstance();
		fbx->DeleteInstance();

		input = nullptr;
		mouse = nullptr;
		direct2D = nullptr;
		graphics = nullptr;
		camera = nullptr;
		light = nullptr;
		matrix = nullptr;
		shader = nullptr;
		graphics2d = nullptr;
		texture = nullptr;
		obj = nullptr;
		fbx = nullptr;
	}

	bool Library::Init(const std::string& window_name_, const int& widht_, const int& height_)
	{
		m_instance = new Library;

		m_instance->window->Draw(window_name_, widht_, height_);

		Vec2f viewport(static_cast<float>(widht_), static_cast<float>(height_));

		m_instance->matrix->SetViewport(viewport);

		if (!m_instance->graphics->Init()) { return false; }

		if (!m_instance->shader->Init()) { return false; }

		m_instance->graphics2d->Init();

		m_instance->texture->Init();

		m_instance->direct2D->Init();

		m_instance->fbx->Init();

		return true;
	}

	void Library::Release()
	{
		delete m_instance;
	}

	void Library::Update()
	{
		m_instance->input->UpdateKeyState();

		m_instance->mouse->Update();
	}
}
