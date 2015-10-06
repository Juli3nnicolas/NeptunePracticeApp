#include "ExApp.h"

#include "Graphics/DisplayDeviceInterface.h"

using namespace Neptune;

int main(int argc, char* argv[])
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow( "Test", 1024, 768 );
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext( window, 3, 4 );

	Ex::ExApp app;

	while (true)
	{
		app.execute();
		DisplayDeviceInterface::SwapBuffer( window );
	}

	return 0;
}