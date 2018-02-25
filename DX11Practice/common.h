#pragma once

// TypeDef
typedef unsigned int uint;

// Includes.
// #include "color.h"
// #include "vector3.h"
// #include "geometry.h"
// #include "renderdevice.h"
// #include "application.h"
// #include "context.h"

template<typename T>
T* MakePtr()
{
	return new T;
}

template<typename T>
void FreePtr( T* t )
{
	delete t;
}