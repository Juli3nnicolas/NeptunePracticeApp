#include "ViewSpawnerExamples.h"
#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Spawners/TriangleSpawner.h"
#include "Graphics/Spawners/CubeSpawner.h"
#include "Graphics/Spawners/ModelSpawner.h"
#include "Math/Vectors/Vec3.h"

#include "Graphics/Shader.h"
#include "Graphics/Color.h"
#include "Graphics/View.h"
#include "Graphics/VAOView.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/Texture.h"

#include "Physics/Mechanics/Position.h"

#include "Input/EventSystemInterface.h"
#include "Graphics/Camera.h"
#include "Camera/Controller/TempFPSCameraController.h"

#include "Debug/NeptuneDebug.h"

#include <string>
#include <chrono>
#include <ratio>
#include <ctime>


using namespace Neptune;

std::chrono::high_resolution_clock::time_point s_t_start = std::chrono::high_resolution_clock::now();

namespace Example
{
	float GetCurrentTime()
	{
		using namespace std::chrono;

		high_resolution_clock::time_point t_now   = high_resolution_clock::now();
		duration<float> time_span                = duration_cast<duration<float>>(t_now - s_t_start);

		return time_span.count();
	}
}

void ViewSpawnerExamples::TimeControledTriangleColor()
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);

	// Creates the graphics programs for the ViewSpawner
	std::string vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/FunWithUniforms.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";

	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("Bob");
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	// Creates the uniforms

	float current_time = Example::GetCurrentTime();

	GraphicsProgram::UniformVarInput current_time_uni("Time",
		GraphicsProgram::FLOAT,
		1,
		1,
		sizeof(float),
		&current_time);

	// Creates the spawner
	const GraphicsProgram::ProgramName PGM_NAME = pgm.getName();
	TriangleSpawner spawner(&pgm);
	spawner.createVertexData();
	spawner.createColorData({1.0f,0.8f,0.0f,1.0f});
	spawner.mapVertexData(PGM_NAME,0);
	spawner.mapColorData(PGM_NAME,1);
	spawner.addUniformVariable(PGM_NAME,current_time_uni);

	// Create a view
	View* view = spawner.create();
	view->init();

	// main loop
	float background[4] ={255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		current_time = Example::GetCurrentTime();
		view->updateUniform(PGM_NAME,"Time",&current_time);
		view->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	// Cleanup/shutdown
	view->terminate();
	delete view;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
}

void ViewSpawnerExamples::ModelViewMatrix()
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
	
	// Creates the graphics programs for the ViewSpawner
	std::string vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";
	
	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("Bob");
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	// Creates the spawner
	const GraphicsProgram::ProgramName PGM_NAME = pgm.getName();
	TriangleSpawner spawner(&pgm);
	spawner.createVertexData();
	spawner.createColorData({1.0f, 0.8f, 0.0f, 1.0f});
	spawner.setWorldPosition({-0.5f, -0.5f, 0.0f});
	spawner.mapVertexData(PGM_NAME, 0);
	spawner.mapColorData(PGM_NAME, 1);
	spawner.useWorldAndProjectionMatrices(PGM_NAME);

	// Create a view
	View* view = spawner.create();
	view->init();

	// main loop
	float background[4] ={255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);
		
		view->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	// Cleanup/shutdown
	view->terminate();
	delete view;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
}

void ViewSpawnerExamples::Display100Cubes()
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

static void CreateAndMapTextures(const GraphicsProgram::ProgramName& _pgmName, ModelSpawner& _spawner, std::vector<Texture>& _textures)
{
	// Get texture binding points - VRAM index allocated for models' textures
	std::unordered_map<std::string, u8> texture_bindings;
	_spawner.getTextureBindingPoints(texture_bindings);		// Map of {texture name, texture binding point}

	// Create textures and give them their binding points
	for (const auto& binding : texture_bindings)
	{
		_textures.emplace_back(binding.first.c_str());
		auto& texture = _textures.back();
		texture.setIndex(binding.second);
		texture.init();
	}

	// Tell the spawner to use the newly created textures with program _pgmName
	for (auto& t : _textures)
		_spawner.setTexture(_pgmName, &t);

	// Create a map from models' vertices to texture-binding-points 
	// so that the graphics program knows what texture to apply for every vertex.
	std::vector<u32> texture_map;
	_spawner.mapVerticesToTextureBindingPoints(texture_map);								// Sets a map of {last vertex index (to be used with), binding point}

	// Add the map as an uniform to the graphics program
	GraphicsProgram::UniformVarInput texture_binding_index_array_uni(NEP_UNIVNAME_VERTICES_TO_TEXTURE_BINDING_POINT_MAP,
		GraphicsProgram::U32,
		texture_map.size(),							// Number of values
		1,											// An array, not a vector (2,3) nor a matrix
		texture_map.size()*sizeof(texture_map[0]),	// Data size
		texture_map.data());						// Data

	_spawner.addUniformVariable(_pgmName, texture_binding_index_array_uni);
}

void ViewSpawnerExamples::MultiTexturedModelExample()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APPLICATION SET TUP

	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("MultiTexturedModelExample", WIDTH, HEIGHT);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window, 3, 4);
	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;												// Pos = (0,0,0)
	camera.translate(0.0f, 0.0f, -8.0f);						// Step back from 8 units
	camera.setScreenRatio(static_cast<float>(WIDTH) / HEIGHT);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	CREATE GRAPHICS PROGRAM

	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/MultiTexturedDisplay.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag";

	Shader vert(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("MultiTexturedModel");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SET UP MODEL SPAWNER

	// Set model paths
	const char SONIC_OBJ[] = "Resources/Models/Objs/Sonic/sonic-the-hedgehog.obj";

	// Load model
	ModelSpawner spawner(&pgm, SONIC_OBJ);

	// Tell the spawner to prepare common data for graphics programs
	spawner.createVertexData();							// Get model's vertices ready
	spawner.create2DTextureMapData();					// Get model's texture coordinates ready

	// Map the data
	spawner.mapVertexData(PGM_NAME, 0);					// Use vertex data in the graphics program
	spawner.map2DTextureMapData(PGM_NAME, 1);			// Use 2D texture map coordinates with graphics program

	// Set the spawner to send a world and projection matrix to the graphics program
	spawner.useWorldAndProjectionMatrices(PGM_NAME);

	// Add custom uniforms - Texture index table to be used for selecting the right texture to apply (ApplyTexture.frag)
	std::vector<Texture> textures;
	CreateAndMapTextures(PGM_NAME, spawner, textures);


	////////////////////////////////////////////////////////////////////////////
	// INSTANTIATE VIEW

	View* view{ nullptr };
	view = spawner.create();
	view->init();
	view->getTransform().translate(0.0f, 0.0f, 4.0f);
	view->getTransform().rotate(0.0f, 90.0f, 0.0f);
	view->getTransform().scale(0.1f, 0.1f, 0.1f);

	view->bindToCamera(&camera);


	////////////////////////////////////////////////////////////////////////////
	// MAIN LOOP

	float WHITE[4] = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f };
	float SKY_BLUE[4] = { 0.0f, 162.0f / 255.0f, 232.0f / 255.0f, 0.0f };
	float BLACK[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float* background = SKY_BLUE;
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		view->getTransform().rotate(0.0f, 1.0f, 0.0f);
		view->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}


	////////////////////////////////////////////////////////////////////////////
	// CLEAN UP

	view->terminate();
	delete view;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
	EventSystemInterface::ShutDown();
}