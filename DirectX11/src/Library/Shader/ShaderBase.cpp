#include "ShaderBase.h"

#include <fstream>
#include <sstream>

namespace engine
{

	bool ShaderBase::LoadFile(const std::string& file_name_)
	{
		FILE* file = nullptr;
		fopen_s(&file, file_name_.c_str(), "rb");
		if (file == nullptr) { return false; }

		fseek(file, 0, SEEK_END);
		m_size = ftell(file);
		fseek(file, 0, SEEK_SET);

		m_data = new char[m_size];
		fread_s(m_data, m_size, m_size, 1, file);
		fclose(file);

		return true;
	}
}