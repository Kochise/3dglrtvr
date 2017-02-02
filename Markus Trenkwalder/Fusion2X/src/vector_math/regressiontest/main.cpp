#include "../vector_math.h"
#include <cassert>
#include <iostream>

using namespace std;

typedef vmath::vec2<float> vec2f;
typedef vmath::vec3<float> vec3f;
typedef vmath::vec4<float> vec4f;
typedef vmath::quat<float> quatf;

typedef vmath::mat2<float> mat2f;
typedef vmath::mat3<float> mat3f;
typedef vmath::mat4<float> mat4f;

template <typename T>
inline bool equal(const T& lhs, const T& rhs)
{
	return abs(lhs - rhs) < T(1e-5);
}

#define EQUAL_TEMPLATE(CLASS, COUNT) \
	template <typename T> \
	inline bool equal(const vmath::CLASS & lhs, const vmath::CLASS & rhs) \
	{ \
		for (int i = 0; i < COUNT; ++i) \
			if (abs(rhs[i] - lhs[i]) >= T(1e-5)) return false; \
		return true; \
	}

EQUAL_TEMPLATE(vec2<T>, 2)
EQUAL_TEMPLATE(vec3<T>, 3)
EQUAL_TEMPLATE(vec4<T>, 4)
EQUAL_TEMPLATE(quat<T>, 4)

EQUAL_TEMPLATE(mat2<T>, 4)
EQUAL_TEMPLATE(mat3<T>, 9)
EQUAL_TEMPLATE(mat4<T>, 16)

int main()
{
	{
		vec3f a(1.0f, 0.0f, 0.0f);
		vec3f b(0.0f, 1.0f, 0.0f);
		vec3f c = cross(a, b);
		assert(equal(c, vec3f(0.0f, 0.0f, 1.0f)));
		assert(length(a) == 1.0f);
		assert(lerp(a, b, 0.5f) == vec3f(0.5f, 0.5f, 0.0f));
		vec4f d(1.0f, 2.0f, 3.0f, 4.0f);
		assert(length(normalize(d)) == 1.0f);
	}

	mat4f m4 = vmath::euler(30.0f, 40.0f, 50.0f);
	mat3f m3(m4);
	mat2f m2(m3);
	assert(transpose(transpose(m4)) == m4);
	assert(equal(det(m4), 1.0f));
	assert(equal(inverse(m2) * m2, vmath::identity2<float>()));
	assert(equal(inverse(m3) * m3, vmath::identity3<float>()));
	assert(equal(inverse(m4) * m4, vmath::identity4<float>()));

	vec4f v(1.0f, 2.0f, 3.0f, 1.0f);
	vec4f v2(1.0f, 2.0f, 3.0f, 0.0f);
	assert(equal(vec3f(m4 * v), transform_point(m4, vec3f(v))));
	assert(equal(vec3f(m4 * v2), transform_vector(m4, vec3f(v2))));
	assert(equal(vec3f(v * m4), transform_point_transpose(m4, vec3f(v))));
	assert(equal(vec3f(v2 * m4), transform_vector_transpose(m4, vec3f(v2))));
	assert(equal(fast_inverse(m4), inverse(m4)));

	quatf q = mat_to_quat(m4);
	assert(equal(quat_to_mat4(q), m4));

	mat4f r = vmath::rotation_matrix(90.0f, 1.0f, 0.0f, 0.0f);
	vec4f v3(0.0f, 1.0f, 0.0f, 1.0f);
	vec4f t = r * v3;
	assert(equal(t, vec4f(0.0f, 0.0f, 1.0f, 1.0f)));

	{
		float angle = 45.0f * float(M_PI/180);
		q = vmath::quat_from_axis_angle(1.0f, 0.0f, 0.0f, angle);
		vec3f v;
		float a;
		vmath::quat_to_axis_angle(q, &v, &a);
		assert(equal(v, vec3f(1.0f, 0.0f, 0.0f)));
		assert(equal(a, angle));
	}

	{
		mat4f m(2.0f);
		assert(trace(m) == 4.0f * 2.0f);
	}
}