#ifndef LIGHT_H
#define LIGHT_H

#include "../Utility/Singleton.h"
#include "../DirectX/DirectXUtility.h"

namespace engine
{
	class Light : public Singleton<Light>
	{
	private:
		friend class Singleton<Light>;

		Light() {};
		~Light() {};

	public:

		// �A�N�Z�T

		Vec4f GetPos() { return pos; }
		Vec4f GetCol() { return col; }


	private:
		/// ���C�g���W
		Vec4f pos{ 0.0f, 10.0f, 0.0f, 1.0f };

		/// ���C�g�̐F
		Vec4f col{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}

#endif // !LIGHT_H
