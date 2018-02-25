#pragma once

class Vector4
{
public:
	float x, y, z, w;

public:
	Vector4( ) : x( 0.0 ), y( 0.0 ), z( 0.0 ), w( 0.0 ) { }
	Vector4( float xx, float yy, float zz, float ww ) : x( xx ), y( yy ), z( zz ), w( ww ) { }
};