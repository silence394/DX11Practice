#pragma once

class Color
{
public:
	float r, g, b, a;

public:
	Color( ) : r( 0.0 ), g( 0.0f ), b( 0.0f ), a( 0.0f ) { }

	Color( float rr, float gg, float bb, float aa ) : r( rr ), g( gg ), b( bb ), a( aa ) { }
};