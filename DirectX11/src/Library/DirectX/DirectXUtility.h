#ifndef DIRECTX_UTITLITY_H
#define DIRECTX_UTITLITY_H

namespace engine
{
	struct Vec4f
	{
		Vec4f(float x_ = 0, float y_ = 0, float z_ = 0, float w_ = 0)
			: x(x_)
			, y(y_)
			, z(z_)
			, w(w_)
		{}

		float x;
		float y;
		float z;
		float w;
	};

	struct Vec3f
	{
		Vec3f(float x_ = 0, float y_ = 0, float z_ = 0)
			: x(x_)
			, y(y_)
			, z(z_)
		{}

		float x;
		float y;
		float z;
	};

	struct Vec2f
	{
		Vec2f(float x_ = 0, float y_ = 0)
			: x(x_)
			, y(y_)
		{}

		float x;
		float y;
	};
}

#endif // !DIRECTX_UTITLITY_H
