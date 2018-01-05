#include "FactoryExamples.h"
#include "Graphics/DisplayDeviceInterface.h"
#include "Input/EventSystemInterface.h"

#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/Factories/ModelFactory.h"

#include "Graphics/Camera.h"
#include "Math/Vectors/Vec3.h"

#include "Debug/NeptuneDebug.h"

#include "Camera/Controller/TempFPSCameraController.h"
#include "Graphics/AmbientLight.h"

#include "Profiling/Chrono.h"

using namespace Neptune;

void FactoryExamples::ViewRows()
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
	EventSystemInterface::StartUp();

	AmbientLight ambientLight({1.0f,1.0f,1.0f,1.0f});

	Camera camera;
	camera.setScreenRatio(1024.0f/768.0f);
	camera.translate(0.0f,0.0f,-5.0f);
	TempFPSCameraController ctrl(&camera);
	ctrl.init();

	TriangleFactory cf({1.0f,1.0f,0.0f,1.0f});
	//const u8 OFFSET = 1;
	//ModelFactory cf("Resources/Models/LowPolyTree.ply");
	const u8 OFFSET = 5;
	const u8 NB_OBJECTS = 100;
	View* view_table[NB_OBJECTS] ={0};

	for(u8 i = 0; i < NB_OBJECTS; i++)
	{
		view_table[i] = cf.create();
		view_table[i]->init();
		view_table[i]->getTransform().translate(-2*OFFSET*(i%2) + 1.0f,0.0f,i*OFFSET);
		view_table[i]->bindToCamera(&camera);
	}

	float background[4] ={0.0f/255.0f,0.0f/255.0f,0.0f/255.0f,0.0f};
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);
		ctrl.update();

		for(u8 i = 0; i < NB_OBJECTS; i++)
		{
			view_table[i]->getTransform().rotate(0.0f,1.0f,0.0f);
			view_table[i]->update();
		}
		DisplayDeviceInterface::SwapBuffer(window);
	}

	ctrl.terminate();
	for(u8 i = 0; i < 100; i++)
	{
		view_table[i]->unbindFromCamera();
		view_table[i]->terminate();
		delete view_table[i];
	}

	EventSystemInterface::ShutDown();
	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
}

void FactoryExamples::Display100PLYModels()
{
	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",WIDTH, HEIGHT);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;							// Pos = (0,0,0)
	camera.translate(0.0f, 1.0f, -5.0f);	// Step back from 5 units
	camera.setScreenRatio(static_cast<float>(WIDTH) / HEIGHT);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();

	// Create views

	NEP_PROFILING_CHRONO_INIT;
	NEP_PROFILING_CHRONO_START;
		Color color {1.0f, 1.0f, 0.0f, 1.0f};
		ModelFactory  factory("Resources/Models/xwing.ply");
	double t = NEP_PROFILING_CHRONO_STOP;
	NEP_LOG("Loading time %f", t);

	const u32 NB_VIEWS = 100;
	View* view_table[NB_VIEWS] = {nullptr}; 

	{
		const float OFFSET = 2.0f;

		view_table[0] = factory.create();
		view_table[0]->init();
		view_table[0]->getTransform().translate(0, 0.0f, 0);

		view_table[0]->bindToCamera(&camera);
	}

	for (u32 i = 1; i < NB_VIEWS; i++)
	{
		const float OFFSET = 2.0f;

		view_table[i] = factory.create();
		view_table[i]->init();
		//view_table[i]->cloneInit(*view_table[0]);
		view_table[i]->getTransform().translate(0, 0.0f, i*OFFSET);

		view_table[i]->bindToCamera(&camera);
	}

	// main loop
	float WHITE[4] = {255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	float BLACK[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	float* background = BLACK;
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		for (auto& v : view_table)
			v->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	for (u32 i = 0; i < NB_VIEWS; i++)
	{
		view_table[i]->terminate();
		delete view_table[i];
	}

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
	EventSystemInterface::ShutDown();
}
