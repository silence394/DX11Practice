#pragma once

#include "windows.h"
#include "common.h"
#include "color.h"
#include "inputlayout.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment ( lib, "d3d11.lib" )
#pragma comment ( lib, "d3dcompiler.lib" )

class RenderDevice
{
private:
	IDXGISwapChain*			mSwapChain;
	ID3D11Device*			mRenderDevice;
	ID3D11DeviceContext*	mDeviceContext;
	ID3D11RenderTargetView*	mDefaultTarget;
	uint					mWidth;
	uint					mHeight;

public:
	RenderDevice( );
	~RenderDevice( );

public:
	bool Initialize( HWND window );

	inline int GetDeviceWidth( ) const
	{
		return mWidth;
	}
	inline int GetDeviceHeight( ) const
	{
		return mHeight;
	}

	inline void Clear( Color c )
	{
		mDeviceContext->ClearRenderTargetView( mDefaultTarget, (float*) &c );
	}

	inline void Present( )
	{
		mSwapChain->Present( 0, 0 );
	}

	void*	CreateVertexShader( LPCWSTR filename, const char* entry, BufferDesc& shaderbuffer );
	void*	CreatePixelShader( LPCWSTR filename, const char* entry );
	void*	CreateBuffer( BufferDesc& desc, uint type, uint usage = 0, uint cpuflag = 0 );
	void*	CreateInputLayout( uint format, uint count, const BufferDesc& shader );

	void	Render( ShaderObject& so, InputLayout& layout );
};