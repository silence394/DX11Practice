#include "renderdevice.h"
#include "windows.h"
#include "stdio.h"
#include "vector3.h"
#include "shaderobject.h"
#include <Mmsystem.h>
#include "matrix4.h"
#pragma comment(lib, "Winmm.lib")  

RenderDevice::RenderDevice( )
{
}

RenderDevice::~RenderDevice( )
{
	mSwapChain->Release( );
	mDeviceContext->Release( );
	mDeviceContext->Release( );
}

bool RenderDevice::Initialize( HWND window )
{
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window;
	scd.SampleDesc.Count = 4;
	scd.Windowed = true;

	D3D11CreateDeviceAndSwapChain( NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&mSwapChain,
		&mRenderDevice,
		NULL,
		&mDeviceContext );

	ID3D11Texture2D* backbuffer;
	mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*) &backbuffer );
	mRenderDevice->CreateRenderTargetView( backbuffer, NULL, &mDefaultTarget );
	backbuffer->Release( );

	mDeviceContext->OMSetRenderTargets( 1, &mDefaultTarget, NULL );

	RECT rect = { 0 };
	GetClientRect( window, &rect );
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float) rect.right;
	viewport.Height = (float) rect.bottom;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	mDeviceContext->RSSetViewports( 1, &viewport );

	mWidth = (uint) viewport.Width;
	mHeight = (uint) viewport.Height;

	return true;
}

void* RenderDevice::CreateVertexShader( LPCWSTR filename, const char* entry, BufferDesc& shaderbuffer )
{
	ID3D10Blob* codes;
	ID3D11VertexShader* shader;
	DWORD start;
	DWORD end;
	start = ::timeGetTime( );
	D3DCompileFromFile( filename, 0, 0, entry, "vs_4_0", D3D10_SHADER_SKIP_VALIDATION | D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &codes, 0 );

	shaderbuffer.buffer = codes->GetBufferPointer( );
	shaderbuffer.length = codes->GetBufferSize( );

	mRenderDevice->CreateVertexShader( codes->GetBufferPointer( ), codes->GetBufferSize( ), nullptr, &shader );

	end = ::timeGetTime( );
	printf( "compile %ls  cost %ld\n", filename, end - start );

// 
// 	ID3D11ShaderReflection* reflector = nullptr;
// 
// 	D3DReflect( codes->GetBufferPointer( ), codes->GetBufferSize( ), IID_ID3D11ShaderReflection, (void**) &reflector );
// 
// 	D3D11_SHADER_DESC desc;
// 	reflector->GetDesc( &desc );
// 
// 	for ( uint i = 0; i < desc.ConstantBuffers; i ++ )
// 	{
// 		ID3D11ShaderReflectionConstantBuffer* rcb = reflector->GetConstantBufferByIndex( i );
// 
// 		D3D11_SHADER_BUFFER_DESC cbdesc;
// 		rcb->GetDesc( &cbdesc );
// 
// 
// 		
// 	}

	return (void*) shader;
}

void* RenderDevice::CreatePixelShader( LPCWSTR filename, const char* entry )
{
	ID3D10Blob* codes;
	ID3D11PixelShader* shader;

	DWORD start;
	DWORD end;
	start = ::timeGetTime( );

	D3DCompileFromFile( filename, 0, 0, entry, "ps_4_0", D3D10_SHADER_SKIP_VALIDATION | D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &codes, 0 );

	mRenderDevice->CreatePixelShader( codes->GetBufferPointer( ), codes->GetBufferSize( ), nullptr, &shader );
	end = ::timeGetTime( );

	printf( "compile %ls  cost %ld, codes buffer size is %d\n", filename, end - start, codes->GetBufferSize( ) );

	return (void*) shader;
}

void* RenderDevice::CreateBuffer( BufferDesc& desc, uint type, uint usage, uint cpuflag )
{
	D3D11_BUFFER_DESC bufferdesc = { 0 };
	bufferdesc.Usage = D3D11_USAGE_DEFAULT;
	bufferdesc.BindFlags = (D3D11_BIND_FLAG) type;
	bufferdesc.CPUAccessFlags = 0;
	bufferdesc.ByteWidth = desc.length;

	D3D11_SUBRESOURCE_DATA srd = { 0 };
	srd.pSysMem = desc.buffer;

	ID3D11Buffer* buffer = nullptr;
	mRenderDevice->CreateBuffer( &bufferdesc, &srd, &buffer );

	return (void*) buffer;
}

void* RenderDevice::CreateInputLayout( uint format, uint count, const BufferDesc& codebuffer )
{
 	D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[count];
 	uint index = 0, offset = 0;
	if ( format & InputLayout::VF_POSITION )
	{
		D3D11_INPUT_ELEMENT_DESC input = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		desc[index ++] = input;
		offset += sizeof(Vector3);
	}
	if ( format & InputLayout::VF_COLOR )
	{
		D3D11_INPUT_ELEMENT_DESC input = { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		desc[index ++] = input;
		offset += sizeof(Vector3);
	}

	ID3D11InputLayout* layout = nullptr;

	mRenderDevice->CreateInputLayout( desc, count, codebuffer.buffer, codebuffer.length, &layout );

	return (void*) layout;
}

void RenderDevice::Render( ShaderObject& so, InputLayout& layout )
{
	ID3D11Buffer* vconbuffer = (ID3D11Buffer*) layout.GetVSConstBuffer( );
	Matrix4* mat = (Matrix4*) layout.GetVSConstArray( );

	mDeviceContext->UpdateSubresource( vconbuffer, 0, NULL, mat, 0, 0 );

	mDeviceContext->VSSetConstantBuffers( 0, 1, &vconbuffer );
	mDeviceContext->IASetInputLayout( (ID3D11InputLayout*) ( layout.GetInputLayout( ) ) );
	uint offset = 0;
	uint stride = InputLayout::GetVertexSize( layout.GetVertexFormat( ) );
	ID3D11Buffer* vb = (ID3D11Buffer*) layout.GetVertexBuffer( );
	mDeviceContext->IASetVertexBuffers( 0, 1, &vb, &stride,  &offset );
	mDeviceContext->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	mDeviceContext->VSSetShader( (ID3D11VertexShader*) so.GetVertexShader( ), 0, 0 );
	mDeviceContext->PSSetShader( (ID3D11PixelShader*) so.GetPixelShader( ), 0, 0 );

	mDeviceContext->Draw( layout.GetVertexCount( ), 0 );
}
