#pragma once

class Vector3;

class Matrix4
{
public:
	float m[16];

public:
	Matrix4( ) { }

	Matrix4( float m0,	float m1,	float m2,	float m3, 
			 float m4,	float m5,	float m6,	float m7, 
			 float m8,	float m9,	float m10,	float m11, 
			 float m12,	float m13,	float m14,	float m15)
			{
				m[0] = m0;	m[1] = m1;	m[2] = m2;	m[3] = m3;
				m[4] = m4;	m[5] = m5;	m[6] = m6;	m[7] = m7;
				m[8] = m8;	m[9] = m9;	m[10]= m10;	m[11]= m11;
				m[12]= m12;	m[13]= m13;	m[14]= m14;	m[15]= m15;
			}
	static const Matrix4 identity;

	Matrix4 operator * ( const Matrix4& mat );
	Matrix4& operator *= ( const Matrix4& mat );

	static Matrix4 View( const Vector3& eye, const Vector3& look, const Vector3& up );
	static Matrix4 Perspective( float fov, float aspect, float znear, float zfar );

public:

	Matrix4& RotationX( float r );
	Matrix4& RotationY( float r );
	Matrix4& RotationZ( float r );
	Matrix4& Transpose( );
};