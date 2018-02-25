#include "application.h"
#include "context.h"

LRESULT CALLBACK Application::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if ( msg == WM_CLOSE )
		PostQuitMessage( 0 );

	return DefWindowProc( hwnd, msg, wparam, lparam );
}

Application::Application( int width, int height, LPCWSTR name )
{
	WNDCLASS winclass;

	winclass.style			= CS_BYTEALIGNCLIENT;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= GetModuleHandle( NULL );
	winclass.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	winclass.hCursor		= LoadCursor( NULL, IDC_ARROW );
	winclass.hbrBackground	= (HBRUSH) GetStockObject( BLACK_BRUSH );
	winclass.lpszMenuName	= NULL;
	winclass.lpszClassName	= name;

	if (!RegisterClass(&winclass))
		return;

	int cx = ::GetSystemMetrics( SM_CXSCREEN );
	int cy = ::GetSystemMetrics( SM_CYSCREEN );

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	DWORD exstyle = 0;
	mWindow = CreateWindowEx( exstyle, name, name, style, 0, 0, width, height, NULL, NULL, winclass.hInstance, NULL );

	ShowWindow( mWindow, SW_SHOWNORMAL );
}

Application::~Application()
{
	CloseWindow( mWindow );
}

void Application::Create( )
{
	Context& context = Context::GetInstance( );
	context.CreateRenderDevice( );
	context.GetRenderDevice( ).Initialize( mWindow );

	OnCreate( );
}

void Application::Run()
{
	MSG msg = {0};
	while( true )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
				break;

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			OnRender( );
		}
	}
}