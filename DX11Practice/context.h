#pragma once

#include "renderdevice.h"

class Context
{
private:
	RenderDevice* mRenderDevice;

private:
	Context( ) : mRenderDevice( NULL ) { }
	Context( const Context& context ) { }
	Context& operator = (const Context&) { }

public:
	static Context& GetInstance( )
	{
		static Context context;
		return context;
	}

	void CreateRenderDevice( )
	{
		if ( mRenderDevice == NULL )
			mRenderDevice = new RenderDevice;
	}

	RenderDevice& GetRenderDevice( )
	{
		return *mRenderDevice;
	}
};