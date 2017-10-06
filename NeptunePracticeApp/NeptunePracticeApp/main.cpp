#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Spawners/TriangleSpawner.h"
#include "Graphics/Spawners/PlanSpawner.h"
#include "Graphics/Spawners/CubeSpawner.h"

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

#include "Input/EventSystemInterface.h"
#include "Graphics/Camera.h"
#include "Camera/Controller/TempFPSCameraController.h"

#include "Physics/Mechanics/Position.h"

#include <string>
#include <chrono>
#include <ratio>
#include <ctime>

#include "ViewSpawnerExamples.h"

using namespace Neptune;

static void FactoryExample()
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

	Color color {1.0f, 1.0f, 0.0f, 1.0f};
	//TriangleFactory factory(color);
	//TriangleFactory factory("Resources/Textures/Grass.png");
	//PlanFactory factory(color);
	//PlanFactory factory("Resources/Textures/Grass.png");
	//CubeFactory factory(color);
	//CubeFactory factory("Resources/Textures/Grass.png");
	ModelFactory  factory("Resources/Models/xwing.ply");
	
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

static void SpawnerExample()
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PGM SET UP

	std::string vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/DiffuseLight.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
	
	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("DiffuseLight");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());


	// Uniforms
	float diffuse_light_dir[3] = {-1.0f, -2.0f, 1.0f};
	GraphicsProgram::UniformVarInput diffuse_light_dir_uni("DiffuseLightDirection",
		GraphicsProgram::FLOAT,
		3,
		1,
		3*sizeof(float),
		diffuse_light_dir);

	float diffuse_light_color[3] = {1.0f, 1.0f, 1.0f};
	GraphicsProgram::UniformVarInput diffuse_light_color_uni("DiffuseLightColor",
		GraphicsProgram::FLOAT,
		3,
		1,
		3*sizeof(float),
		diffuse_light_color);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create views
	Color color {1.0f, 1.0f, 0.0f, 1.0f};
	CubeSpawner factory(&pgm);
	factory.createVertexData();
	factory.createColorData(color);
	factory.createNormalData();
	factory.mapVertexData(PGM_NAME, 0);
	factory.mapColorData(PGM_NAME, 1);
	factory.mapNormalData(PGM_NAME, 2);
	factory.useWorldAndProjectionMatrices(PGM_NAME);
	factory.addUniformVariable(PGM_NAME, diffuse_light_dir_uni);
	factory.addUniformVariable(PGM_NAME, diffuse_light_color_uni);

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
		//view_table[i]->init();					// Instantiate new buffers for every view
		view_table[i]->cloneInit(*view_table[0]);	// Share view[0] buffers
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
		{	v->getTransform().rotate(0.0f, 1.0f, 0.0f);
			v->update();
		}

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

int main(int argc, char* argv[])
{
	//FactoryExample();
	SpawnerExample();
	
	return 0;
}