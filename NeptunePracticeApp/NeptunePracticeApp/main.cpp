#include "Graphics/DisplayDeviceInterface.h"
#include "Input/EventSystemInterface.h"

#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/Factories/ModelFactory.h"

#include "Graphics/Camera.h"
#include "Math/Vectors/Vec3.h"

#include "Debug/NeptuneDebug.h"

#include "Camera/Controller/TempFPSCameraController.h"

using namespace Neptune;

int main(int argc, char* argv[])
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
	EventSystemInterface::StartUp();

	Camera camera;
	camera.setScreenRatio(1024.0f/768.0f);
	camera.translate(0.0f, 0.0f, -5.0f);
	TempFPSCameraController ctrl(&camera);
	ctrl.init();

	CubeFactory cf(1.0f, 1.0f, 0.0f);
	View* view_table[100] = {0};

	for (u8 i = 0; i < 100; i++)
	{
		view_table[i] = cf.create();
		view_table[i]->init();
		view_table[i]->getTransform().translate(-2*(i%2) + 1.0f, 0.0f, i);
		view_table[i]->bindToCamera(&camera);
	}

	float background[4] = { 0.0f,0.0f,0.0f,0.0f };
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);
		ctrl.update();
		
		for (u8 i = 0; i < 100; i++)
			view_table[i]->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	ctrl.terminate();
	for(u8 i = 0; i < 100; i++)
		view_table[i]->terminate();

	for(u8 i = 0; i < 100; i++)
		delete view_table[i];

	EventSystemInterface::ShutDown();
	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);

	return 0;
}