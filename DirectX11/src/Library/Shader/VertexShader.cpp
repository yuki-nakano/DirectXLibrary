#include "VertexShader.h"

namespace engine
{
	bool VertexShader::CreateShader()
	{
		if (FAILED(DirectXGraphics::GetInstance()->GetDevice()->CreateVertexShader(m_data, m_size, nullptr, &m_interface)))
		{
			return false;
		}

		return true;
	}

	void VertexShader::DeleteShader()
	{
		if (m_interface != nullptr) { m_interface->Release(); }
	}
}