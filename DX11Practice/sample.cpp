#include "application.h"
#include "shaderobject.h"
#include "inputlayout.h"
#include "common.h"
#include "renderdevice.h"
#include "vector3.h"
#include "camera.h"
#include "context.h"
#include "matrix4.h"

class DemoApp : public Application
{
private:
	ShaderObject*	so;
	InputLayout*	layout;
	Camera			mCamera;
	Matrix4			mWorldTransform;
	Matrix4			mViewTransform;
	Matrix4			mPerspectiveTransform;
	float			rotateangle;

public:
	DemoApp( int width = 800, int height = 600, LPCWSTR name = L"Demo" )
		: Application( width, height, name ) { }

	~DemoApp( );

public:
	virtual void OnCreate( );
	virtual void OnRender( );
};

DemoApp::~DemoApp( )
{
	FreePtr( so );
	FreePtr( layout );
}

void DemoApp::OnCreate( )
{
	RenderDevice&rd = Context::GetInstance( ).GetRenderDevice( );

	so = MakePtr<ShaderObject>( );

	ShaderDesc vsdesc;
	vsdesc.mEntry = "VS";
	vsdesc.mFileName = L"sample.sf";
	vsdesc.mType = 0;
	so->LoadShader( vsdesc );

	ShaderDesc psdesc;
	psdesc.mEntry = "PS";
	psdesc.mFileName = L"sample.sf";
	psdesc.mType = 1;
	so->LoadShader( psdesc );

	struct Vertex
	{
		Vector3 pos;
		uint	color;
	};

	Vertex inputs[] =
	{
		{ Vector3( 0.0f, 0.0, 0.942f ), 0xffff0000 },
		{ Vector3( 1.0f, -0.574f, 0.0f ), 0xff0000ff },
		{ Vector3( -1.0f, -0.574f, 0.0f ), 0xff00ff00 },
		{ Vector3( 1.0f, -0.574f, 0.0f  ), 0xff0000ff },
		{ Vector3( 0.0f, 1.15f, 0.0f ), 0Xffff00ff },
		{ Vector3( -1.0f, -0.574f, 0.0f ), 0xff00ff00 },
		{ Vector3( 0.0f, 0.0, 0.942f ), 0xffff0000 },
		{ Vector3( -1.0f, -0.574f, 0.0f ), 0xff00ff00 },
		{ Vector3(  0.0f, 1.15f, 0.0f ), 0Xffff00ff },
		{ Vector3( 0.0f, 0.0, 0.942f ), 0xffff0000 },
		{ Vector3(  0.0f, 1.15f, 0.0f ), 0Xffff00ff },
		{ Vector3( 1.0f, -0.574f, 0.0f ), 0xff0000ff },
	};

	VertexBufferDesc bufferdesc;
	bufferdesc.desc.buffer = (void*) inputs;
	bufferdesc.desc.length = sizeof( inputs );
	bufferdesc.format = InputLayout::VF_POSITION | InputLayout::VF_COLOR;

	layout = MakePtr<InputLayout>( );
	layout->Load( bufferdesc );
	layout->SetLayout( so );

	layout->LoadVSConstantBuffer( InputLayout::VCF_WVP );

	mCamera.eye = Vector3( 3.0f, 0.0f, 0.0f );
	mCamera.look = Vector3( 0.0f, 0.0f, 0.0f );
	mCamera.up = Vector3( 0.0f, 0.0f, 1.0f );

	mWorldTransform = Matrix4::identity;
	mViewTransform = Matrix4::View( mCamera.eye, mCamera.look, mCamera.up );
	mPerspectiveTransform = Matrix4::Perspective( 1.57f, (float) rd.GetDeviceWidth( ) / (float) rd.GetDeviceHeight( ), 0.001f, 1000.0f );
	rotateangle = 0.1f;
}

void DemoApp::OnRender( )
{
	rotateangle += 0.001f;
	mWorldTransform.RotationZ( rotateangle );

	Matrix4 wvp = mWorldTransform * mViewTransform * mPerspectiveTransform;

	RenderDevice& rd = Context::GetInstance( ).GetRenderDevice( );
	rd.Clear( Color( 0.5, 0.5, 0.5, 0.5 ) );

	layout->SetVSShaderConst( InputLayout::VCF_WVP, (Vector4*) &wvp );
	rd.Render( *so, *layout );

	rd.Present( );
}

int main( )
{
	DemoApp app( 800, 600, L"Demo" );
	app.Create( );

	app.Run( );

	return 0;
}