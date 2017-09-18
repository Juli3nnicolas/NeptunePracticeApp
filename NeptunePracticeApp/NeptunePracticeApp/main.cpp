#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Factories/TriangleSpawner.h"
#include "Graphics/Factories/PlanSpawner.h"

#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/Factories/PlanFactory.h"
#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/Factories/ModelFactory.h"

#include "Math/Vectors/Vec3.h"
#include "Graphics/Shader.h"
#include "Graphics/Color.h"
#include "Graphics/View.h"
#include "Graphics/VAOView.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/Texture.h"

#include "Physics/Mechanics/Position.h"

#include <string>
#include <chrono>
#include <ratio>
#include <ctime>

#include "ViewSpawnerExamples.h"

using namespace Neptune;



int main(int argc, char* argv[])
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);


	Color color {1.0f, 0.0f, 0.0f, 1.0f};

	//TriangleFactory factory(color);
	//TriangleFactory factory("Resources/Textures/Grass.png");
	//PlanFactory factory(color);
	//PlanFactory factory("Resources/Textures/Grass.png");
	//CubeFactory factory(color);
	//CubeFactory factory("Resources/Textures/Grass.png");
	ModelFactory  factory("Resources/Models/xwing.ply");
	
	View* v1 = factory.create();
	View* v2 = factory.create();
	v1->init();
	v1->getTransform().rotate(0.0f, 0.0f, 45.0f);
	v1->getTransform().rotate(45.0f, 0.0f, 45.0f);
	v1->getTransform().scale(0.25f, 0.25f, 0.25f);

	v2->init();
	v2->getTransform().translate(-0.6f, 0.0f, 0.0f);
	v2->getTransform().rotate(45.0f, 0.0f, 45.0f);
	v2->getTransform().scale(0.25f, 0.25f, 0.25f);

	// main loop
	float background[4] = {0.0f/255.0f,0.0f/255.0f,0.0f/255.0f,0.0f};
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		v1->update();
		v2->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	// Cleanup/shutdown
	v1->terminate();
	v2->terminate();
	delete v1;
	delete v2;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);

	return 0;
}