#include "windows.h"
#include "common.h"
#include "context.h"
#include "renderdevice.h"
#include "shaderobject.h"

ShaderObject::ShaderObject( ) : mVertexShader( nullptr ), mPixelShader( nullptr ), mVertexCodes( nullptr )
{
}

ShaderObject::~ShaderObject( )
{
	FreePtr( mVertexCodes );
}

bool ShaderObject::LoadShader( const ShaderDesc& desc )
{
	// VS
	if ( desc.mType == 0 && mVertexShader != nullptr )
		return false;
	if ( desc.mType == 1 && mPixelShader != nullptr )
		return false;

	RenderDevice& rd = Context::GetInstance( ).GetRenderDevice( );
	switch( desc.mType )
	{
		case 0:
		{
			mVertexCodes = MakePtr<BufferDesc>( );
			mVertexShader = rd.CreateVertexShader( desc.mFileName, desc.mEntry, *mVertexCodes );
			break;
		}

		case 1:
		{
			mPixelShader = rd.CreatePixelShader( desc.mFileName, desc.mEntry );
			break;
		}
	}

	return true;
}