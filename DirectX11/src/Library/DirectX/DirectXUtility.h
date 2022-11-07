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

		Vec4f& operator+=(const Vec4f& vec4f_)
		{
			x += vec4f_.x;
			y += vec4f_.y;
			z += vec4f_.z;
			w += vec4f_.w;
			return *this;
		}
		Vec4f& operator-=(const Vec4f& vec4f_)
		{
			x -= vec4f_.x;
			y -= vec4f_.y;
			z -= vec4f_.z;
			w -= vec4f_.w;
			return *this;
		}
		Vec4f& operator*=(const Vec4f& vec4f_)
		{
			x *= vec4f_.x;
			y *= vec4f_.y;
			z *= vec4f_.z;
			w *= vec4f_.w;
			return *this;
		}
		Vec4f& operator/=(const Vec4f& vec4f_)
		{
			x /= vec4f_.x;
			y /= vec4f_.y;
			z /= vec4f_.z;
			w /= vec4f_.w;
			return *this;
		}

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

		Vec3f& operator+=(const Vec3f& vec3f_)
		{
			x += vec3f_.x;
			y += vec3f_.y;
			z += vec3f_.z;
			return *this;
		}
		Vec3f& operator-=(const Vec3f& vec3f_)
		{
			x -= vec3f_.x;
			y -= vec3f_.y;
			z -= vec3f_.z;
			return *this;
		}
		Vec3f& operator*=(const Vec3f& vec3f_)
		{
			x *= vec3f_.x;
			y *= vec3f_.y;
			z *= vec3f_.z;
			return *this;
		}
		Vec3f& operator/=(const Vec3f& vec3f_)
		{
			x /= vec3f_.x;
			y /= vec3f_.y;
			z /= vec3f_.z;
			return *this;
		}

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

		Vec2f& operator+=(const Vec2f& vec2f_)
		{
			x += vec2f_.x;
			y += vec2f_.y;
			return *this;
		}
		Vec2f& operator-=(const Vec2f& vec2f_)
		{
			x -= vec2f_.x;
			y -= vec2f_.y;
			return *this;
		}
		Vec2f& operator*=(const Vec2f& vec2f_)
		{
			x *= vec2f_.x;
			y *= vec2f_.y;
			return *this;
		}
		Vec2f& operator/=(const Vec2f& vec2f_)
		{
			x /= vec2f_.x;
			y /= vec2f_.y;
			return *this;
		}

		float x;
		float y;
	};
}

#endif // !DIRECTX_UTITLITY_H
