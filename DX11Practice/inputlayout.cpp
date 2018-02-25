#include "common.h"
#include "inputlayout.h"
#include "vector3.h"
#include "context.h"
#include "shaderobject.h"
#include "Vector4.h"

InputLayout::InputLayout( ) : mVertexBuffer( nullptr ), mVertexFormat( 0 ), mVertexCount( 0 ), mIndexBuffer( nullptr ), mInputLayout( nullptr ), mVSConstFormat( 0 ), mVSConst( nullptr ), mVSConstBuffer( nullptr ), mPSConstBuffer( nullptr )
{
}

InputLayout::~InputLayout( )
{
	// TODO.
}

uint InputLayout::GetVertexSize( uint format )
{
	uint size = 0;
	if ( format & VF_POSITION )
		size += sizeof( Vector3 );
	if ( format & VF_COLOR )
		size += sizeof( uint );

	return size;
}

bool InputLayout::Load( VertexBufferDesc& desc )
{
	if ( mVertexBuffer != nullptr )
		return false;

	RenderDevice& rd = Context::GetInstance( ).GetRenderDevice( );
	mVertexBuffer = rd.CreateBuffer( desc.desc, BT_VERTEX );

	// TODO. Like shit.
	mVertexFormat = desc.format;
	mVertexCount = desc.desc.length / GetVertexSize( desc.format );

	return true;
}

bool InputLayout::Load( VertexBufferDesc& vdesc, BufferDesc& idesc )
{
	// TODO.

	return true;
}

bool InputLayout::SetLayout( ShaderObject* so )
{
	if ( so == nullptr )
		return false;

	RenderDevice& rd = Context::GetInstance( ).GetRenderDevice( );

	uint num = 0;
	if ( mVertexFormat& VF_POSITION )
		num ++;
	if ( mVertexFormat & VF_COLOR )
		num ++;

	mInputLayout = rd.CreateInputLayout( mVertexFormat, num, so->GetVertexCode( ) );

	return true;
}

bool InputLayout::LoadVSConstantBuffer( uint format )
{
	if ( format == 0 || mVSConstBuffer != nullptr )
		return false;

	uint len = 0;
	if ( format & VCF_WVP )
		len += 64;

	mVSConstFormat = format;
	mVSConst = new Vector4[ len / 16 ];
// 	mVSConst[0] = Vector4(0.1f, 0.1f, 0.1f, 0.1);
// 	mVSConst[1] = Vector4(0.2f, 0.2f, 0.2f, 0.2);
// 	mVSConst[2] = Vector4(0.4f, 0.4f, 0.4f, 0.4);
// 	mVSConst[3] = Vector4(0.8f, 0.8f, 0.8f, 0.8);

	RenderDevice& rd = Context::GetInstance( ).GetRenderDevice( );

	BufferDesc desc;
	desc.length = len;
	desc.buffer = mVSConst;

	mVSConstBuffer = rd.CreateBuffer( desc, BT_CONSTANT );

	return true;
}

void InputLayout::SetVSShaderConst( uint format, Vector4* con )
{
	uint fmt = mVSConstFormat & format;
	if ( fmt== 0 )
		return;

	// TODO. Search list.
	uint size = 0, endindex = 0;
	if ( fmt & VCF_WVP )
	{
		size = 4;
		endindex += 4;
	}

	for( uint i = endindex - size; i < size; i ++ )
		mVSConst[i] = *( con + i );

// 	mVSConst[0] = Vector4(0.1f, 0.1f, 0.1f, 0.1);
// 	mVSConst[1] = Vector4(0.2f, 0.2f, 0.2f, 0.2);
// 	mVSConst[2] = Vector4(0.4f, 0.4f, 0.4f, 0.4);
// 	mVSConst[3] = Vector4(0.8f, 0.8f, 0.8f, 0.8);
}