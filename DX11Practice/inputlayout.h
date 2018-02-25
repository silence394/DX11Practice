#pragma once

class ShaderObject;
class Vector4;

struct BufferDesc
{
	void*	buffer;
	uint	length;
};

struct VertexBufferDesc
{
	BufferDesc	desc;
	uint		format;
};

class InputLayout
{
public:
	enum VertexFormat
	{
		VF_POSITION	= 1 << 0,
		VF_COLOR	= 1 << 1,
	};

	enum BufferType
	{
		BT_VERTEX	= 0x01,
		BT_INDEX	= 0x02,
		BT_CONSTANT	= 0x04,
	};

	enum VertexConstantFormat
	{
		VCF_WVP		= 0x01,
	};

private:
	void*		mVertexBuffer;
	uint		mVertexFormat;
	uint		mVertexCount;
	void*		mIndexBuffer;
	void*		mInputLayout;
	uint		mVSConstFormat;
	Vector4*	mVSConst;
	void*		mVSConstBuffer;
	void*		mPSConstBuffer;

public:
	InputLayout( );
	~InputLayout( );

	static uint GetVertexSize( uint format );

	void* GetVertexBuffer( )
	{
		return mVertexBuffer;
	}

	void* GetIndexBuffer( )
	{
		return mIndexBuffer;
	}

	void* GetInputLayout( )
	{
		return mInputLayout;
	}

	uint GetVertexFormat( )
	{
		return mVertexFormat;
	}

	uint GetVertexCount( )
	{
		return mVertexCount;
	}

	Vector4* GetVSConstArray( )
	{
		return mVSConst;
	}

	void* GetVSConstBuffer( )
	{
		return mVSConstBuffer;
	}

public:

	bool Load( VertexBufferDesc& desc );
	bool Load( VertexBufferDesc& vdesc, BufferDesc& idesc );
	bool SetLayout( ShaderObject* so );

	bool LoadVSConstantBuffer( uint format );
	void SetVSShaderConst( uint format, Vector4* con );
};