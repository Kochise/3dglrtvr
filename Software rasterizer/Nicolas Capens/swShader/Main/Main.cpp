#include "Application.hpp"
using namespace swShader;

#include "Error.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Application application(hInstance);

		application.mainLoop();
	}
	catch(const Error &error)
	{
		MessageBox(0, error.getString(), 0, MB_OK);
	}
	catch(...)
	{
		MessageBox(0, "Unknown fatal application error. Click to terminate.", 0, MB_OK);
	}

	return 0;
}