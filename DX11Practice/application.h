#pragma once

#include "windows.h"
#include "renderdevice.h"

class Application
{
private:
	HWND			mWindow;
	RenderDevice*	mRenderDevice;

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	Application( int width = 800, int height = 600, LPCWSTR name = L"SoftRender" );
	~Application();

public:
	virtual void OnCreate( ) { }
	virtual void OnRender( ) { }

	void Create( );
	void Run( );
};