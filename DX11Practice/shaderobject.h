#pragma once

struct ShaderDesc
{
	LPCWSTR	mFileName;
	char*	mEntry;
	uint	mType;
};

class ShaderObject
{
private:
	void*		mVertexShader;
	void*		mPixelShader;
	BufferDesc*	mVertexCodes;

public:
	ShaderObject( );
	~ShaderObject( );

	void* GetVertexShader( )
	{
		return mVertexShader;
	}

	void* GetPixelShader( )
	{
		return mPixelShader;
	}

	const BufferDesc& GetVertexCode( )
	{
		return *mVertexCodes;
	}

public:
	bool LoadShader( const ShaderDesc& desc );
};