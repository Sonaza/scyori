#ifndef SZEN_MAT4_HPP
#define SZEN_MAT4_HPP

#include <szen/System/vec3.hpp>
#include <szen/System/vec4.hpp>

#include <cassert>
#include <cmath>

namespace sz
{

	class mat4
	{
	public:
		mat4();
		mat4(float m00, float m01, float m02, float m03,
			 float m10, float m11, float m12, float m13,
			 float m20, float m21, float m22, float m23,
			 float m30, float m31, float m32, float m33);

		// Retrieve current matrix
		const float* getMatrix() const;

		// Calculate inverse matrix
		mat4 getInverse() const;

		// Calculate a lookat matrix 
		mat4& lookAt(const vec3f& eye, const vec3f& target, const vec3f& up);

		// Calculates 3D projection matrix
		mat4& perspective(float fov, float aspect, float znear, float zfar);
	
		// Calculates orthographic projection matrix
		mat4& ortho(float left, float right, float bottom, float top, float znear = -1.f, float zfar = 1.f);
	
		// Matrix multiplication
		mat4& combine(const mat4& matrix);

		// Translate matrix by offset
		mat4& translate(const vec3f& offset);

		// Rotate matrix by amount on specified axis
		mat4& rotate(float amount, const vec3f& axis);

		// Scale matrix by amount
		mat4& scale(const vec3f &amount);

		// Calculate matrix transpose
		mat4& transpose();

		// Row utility class for bracket access
		class row
		{
		public:
			row(float a, float b, float c, float d)
			{
				m_row[0] = a;
				m_row[1] = b;
				m_row[2] = c;
				m_row[3] = d;
			}

			float operator[](unsigned int i) const { assert(i >= 0 && i < 4); return m_row[i]; }

		private:
			float m_row[4];
		};

		// Overload brackets to retrieve a single row
		const row operator[](unsigned int i) { assert(i >= 0 && i < 4); return row(m_matrix[i*4], m_matrix[i*4+1], m_matrix[i*4+2], m_matrix[i*4+3]); }

	private:

		float m_matrix[16];

	};

	////////////////////////////////////////////////////
	mat4 operator*(const mat4& lhs, const mat4& rhs);
	
	////////////////////////////////////////////////////
	// Transform vector by matrix
	vec4f operator*(const mat4& mat, const vec4f& v);

	#include "mat4.inl"

}

#endif MAT4_HPP
