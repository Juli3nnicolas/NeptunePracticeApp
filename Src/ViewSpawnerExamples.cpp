#include "ViewSpawnerExamples.h"
#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Spawners/PlanSpawner.h"
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

#include "Math/Vectors/Mat4x4.h"
#include "Physics/Mechanics/Position.h"

#include "Input/EventSystemInterface.h"
#include "Graphics/Camera.h"
#include "Camera/Controller/TempFPSCameraController.h"

#include "Debug/NeptuneDebug.h"
#include "Profiling/Chrono.h"

#include <string>
#include <chrono>
#include <ratio>
#include <ctime>

#include "Math/Vectors/Vec4.h"


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
	spawner.movePgmParameters();

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
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("ModelViewMatrix",1024,768);
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
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);
	spawner.movePgmParameters();

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

void ViewSpawnerExamples::Display100XWings()
{
	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Display100XWings", WIDTH, HEIGHT, DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window, 3, 4);
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

	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PassThrough.frag";

	Shader vert(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("Display100XWings");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create views
	NEP_PROFILING_CHRONO_INIT;
	NEP_PROFILING_CHRONO_START;
		Color color{ 1.0f, 1.0f, 0.0f, 1.0f };
		ModelSpawner factory(&pgm, "Resources/Models/xwing.ply");
	double t = NEP_PROFILING_CHRONO_STOP;
	NEP_LOG("Loading time for first instance %f ms", t);

	factory.createVertexData();
	factory.createColorData(color);
	factory.mapVertexData(PGM_NAME, 0);
	factory.mapColorData(PGM_NAME, 1);
	factory.useModelViewAndProjectionMatrices(PGM_NAME);
	factory.movePgmParameters();

	const u32 NB_VIEWS = 100;
	View* view_table[NB_VIEWS] = { nullptr };
	view_table[0] = factory.create();
	view_table[0]->init();
	view_table[0]->getTransform().translate(0.0f, 0.0f, 0.0f);
	view_table[0]->bindToCamera(&camera);

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
	float WHITE[4] = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f };
	float BLACK[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float* background = BLACK;
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		for (auto& v : view_table)
		{
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

void ViewSpawnerExamples::Display20Cubes()
{
	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Display20Cubes",WIDTH, HEIGHT, DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16);
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
	float diffuse_light_dir[3] = {0.0f, 1.0f, 1.0f};
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

	Mat4 World;
	GraphicsProgram::UniformVarInput world_matrix_uni("World",
		GraphicsProgram::FLOAT,
		4,
		4,
		16 * sizeof(float),
		World.getPtr());

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
	factory.useModelViewAndProjectionMatrices(PGM_NAME);
	factory.addUniformVariable(PGM_NAME, diffuse_light_dir_uni);
	factory.addUniformVariable(PGM_NAME, diffuse_light_color_uni);
	factory.addUniformVariable(PGM_NAME, world_matrix_uni);
	factory.movePgmParameters();


	const u32 NB_VIEWS = 20;
	View* view_table[NB_VIEWS] = {nullptr}; 
	view_table[0] = factory.create();
	view_table[0]->init();
	view_table[0]->getTransform().translate(0.0f, 0.0f, 0.0f);
	view_table[0]->bindToCamera(&camera);

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
	Vec4 light_source(diffuse_light_dir[0], diffuse_light_dir[1], diffuse_light_dir[2], 0);
	Transform rot;

	float WHITE[4] = {255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	float BLACK[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	float* background = BLACK;
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		for (auto& v : view_table)
		{	
			v->getTransform().rotate(0.0f, 1.0f, 0.0f);

			// Rotate light source
			//rot.rotate(0.0f, 1.0f, 0.0f);
			//Vec4 rot_light = rot.getMatrix() * light_source;
			//auto light = pgm.getUniformVar("DiffuseLightDirection");
			//light->second.setData(rot_light.getPtr());

			v->updateUniform("World", v->getTransform().getDataPtr());
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
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);

	// Add custom uniforms - Texture index table to be used for selecting the right texture to apply (ApplyTexture.frag)
	std::vector<Texture> textures;
	CreateAndMapTextures(PGM_NAME, spawner, textures);

	// Move program parameters from the spawner to the program
	spawner.movePgmParameters();


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

void ViewSpawnerExamples::XWing()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APPLICATION SET TUP

	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::OffScreenRenderingSettings settings;
	settings.m_antiAliasing = DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16;
	settings.m_frameBufferHeight = HEIGHT;
	settings.m_frameBufferWidth = WIDTH;
	//settings.m_enableReversedZ = true;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("XWing", WIDTH, HEIGHT);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window, 3, 4, settings);
	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;												// Pos = (0,0,0)
	camera.translate(0.0f, 2.0f, -3.0f);						// Step back from 3 units and move up from 2
	camera.setScreenRatio(static_cast<float>(WIDTH) / HEIGHT);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();

	// Uniforms
	float diffuse_light_dir[3] = { 0.0f, 0.0f, 1.0f };
	GraphicsProgram::UniformVarInput diffuse_light_dir_uni("DiffuseLightDirection",
		GraphicsProgram::FLOAT,
		3,
		1,
		3 * sizeof(float),
		diffuse_light_dir);

	float diffuse_light_color[3] = { 1.0f, 1.0f, 1.0f };
	GraphicsProgram::UniformVarInput diffuse_light_color_uni("DiffuseLightColor",
		GraphicsProgram::FLOAT,
		3,
		1,
		3 * sizeof(float),
		diffuse_light_color);

	Mat4 World;
	GraphicsProgram::UniformVarInput world_matrix_uni("World",
		GraphicsProgram::FLOAT,
		4,
		4,
		16 * sizeof(float),
		World.getPtr());


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	CREATE GRAPHICS PROGRAM

	//std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/MultiTexturedDisplayWithLight.vert";
	//std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTextureWithLight.frag";

	//std::string vertexShaderName   = /*"../../../Neptune/Engine/Multiplatform/Core/*/"Shaders/Vertex/PhongLightingNoMultiTexturing.vert";
	//std::string fragmentShaderName = /*"../../../Neptune/Engine/Multiplatform/Core/*/"Shaders/Fragment/PhongLightingNoMultiTexturing.frag";

	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/PhongLightingNoMultiTexturing.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PhongLightingNoMultiTexturing.frag";

	Shader vert(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("MultiTexturedModelWithLight");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SET UP MODEL SPAWNER

	// Set model paths
	const char XWING[] = "Resources/Models/xwing.ply";

	// Load model
	ModelSpawner spawner(&pgm, XWING);

	// Tell the spawner to prepare common data for graphics programs
	spawner.createVertexData();							// Get model's vertices ready
	spawner.createColorData({ 1, 0, 0, 1 });							// Get model's color data ready
	spawner.createNormalData();							// Get model's normal data ready

	// Map the data
	spawner.mapVertexData(PGM_NAME, 0);					// Use vertex data in the graphics program
	spawner.mapColorData(PGM_NAME, 1);					// Use colour data
	spawner.mapNormalData(PGM_NAME, 2);					// Use normal data with graphics program

	// Set the spawner to send a world and projection matrix to the graphics program
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);

	// Add lighting-related uniforms
	spawner.addUniformVariable(PGM_NAME, diffuse_light_dir_uni);
	spawner.addUniformVariable(PGM_NAME, diffuse_light_color_uni);
	spawner.addUniformVariable(PGM_NAME, world_matrix_uni);

	// Move program parameters from the spawner to the program
	spawner.movePgmParameters();



	////////////////////////////////////////////////////////////////////////////
	// INSTANTIATE VIEW

	View* view{ nullptr };
	view = spawner.create();
	view->init();
	view->getTransform().translate(0.0f, 2.0f, 0.0f);
	view->getTransform().rotate(0.0f, 90.0f, 0.0f);
	//view->getTransform().scale(0.1f, 0.1f, 0.1f);

	view->bindToCamera(&camera);


	////////////////////////////////////////////////////////////////////////////
	// MAIN LOOP

	float WHITE[4] = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f };
	float SKY_BLUE[4] = { 0.0f, 162.0f / 255.0f, 232.0f / 255.0f, 0.0f };
	float PURPLE[4] = { 0.5f, 0.0f, 0.5f, 0.0f };
	float BLACK[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float* background = PURPLE;
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		view->getTransform().rotate(0.0f, 1.0f, 0.0f);

		view->updateUniform("World", view->getTransform().getDataPtr());

		view->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}
}

void ViewSpawnerExamples::MultiTexturedModelWithSimpleLightingExample()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APPLICATION SET TUP

	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::OffScreenRenderingSettings settings;
	settings.m_antiAliasing = DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16;
	settings.m_enableReversedZ = true;
	settings.m_frameBufferHeight = HEIGHT;
	settings.m_frameBufferWidth = WIDTH;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("MultiTexturedModelWithSimpleLightingExample", WIDTH, HEIGHT);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window, 3, 4, settings);

	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;												// Pos = (0,0,0)
	camera.translate(0.0f, 2.0f, -3.0f);						// Step back from 3 units and move up from 2
	camera.setScreenRatio(static_cast<float>(WIDTH) / HEIGHT);
	camera.updateProjection(Camera::ProjectionType::REVERSED_Z_PERSPECTIVE);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();

	// Uniforms
	float diffuse_light_dir[3] = { 0.0f, 0.0f, 1.0f };
	GraphicsProgram::UniformVarInput diffuse_light_dir_uni("DiffuseLightDirection",
		GraphicsProgram::FLOAT,
		3,
		1,
		3 * sizeof(float),
		diffuse_light_dir);

	float diffuse_light_color[3] = { 1.0f, 1.0f, 1.0f };
	GraphicsProgram::UniformVarInput diffuse_light_color_uni("DiffuseLightColor",
		GraphicsProgram::FLOAT,
		3,
		1,
		3 * sizeof(float),
		diffuse_light_color);

	Mat4 World;
	GraphicsProgram::UniformVarInput world_matrix_uni("World",
		GraphicsProgram::FLOAT,
		4,
		4,
		16 * sizeof(float),
		World.getPtr());


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	CREATE GRAPHICS PROGRAM

	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/PhongLighting.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PhongLighting.frag";

	Shader vert(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("MultiTexturedModelWithLight");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SET UP MODEL SPAWNER

	// Set model paths
	const char SONIC_OBJ[] = "Resources/Models/Objs/Sonic/sonic-the-hedgehog.obj";
	const char CLASSIC_SONIC[] = "Resources/Models/ClassicSonic/ClassicSonic.DAE";

	// Load model
	ModelSpawner spawner(&pgm, SONIC_OBJ);

	// Tell the spawner to prepare common data for graphics programs
	spawner.createVertexData();							// Get model's vertices ready
	spawner.create2DTextureMapData();					// Get model's texture coordinates ready
	spawner.createNormalData();							// Get model's normal data ready

	// Map the data
	spawner.mapVertexData(PGM_NAME, 0);					// Use vertex data in the graphics program
	spawner.map2DTextureMapData(PGM_NAME, 1);			// Use 2D texture map coordinates with graphics program
	spawner.mapNormalData(PGM_NAME, 2);					// Use normal data with graphics program

	// Set the spawner to send a world and projection matrix to the graphics program
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);

	// Add lighting-related uniforms
	spawner.addUniformVariable(PGM_NAME, diffuse_light_dir_uni);
	spawner.addUniformVariable(PGM_NAME, diffuse_light_color_uni);
	spawner.addUniformVariable(PGM_NAME, world_matrix_uni);

	// Add custom uniforms - Texture index table to be used for selecting the right texture to apply (ApplyTexture.frag)
	std::vector<Texture> textures;
	CreateAndMapTextures(PGM_NAME, spawner, textures);

	// Move program parameters from the spawner to the program
	spawner.movePgmParameters();



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
	float PURPLE[4] = {0.5f, 0.0f , 0.5f, 0.0f};
	float BLACK[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float* background = PURPLE;
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		view->getTransform().rotate(0.0f, 1.0f, 0.0f);

		view->updateUniform("World", view->getTransform().getDataPtr());

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

static void SpawnModel(GraphicsProgram& _pgm, const char* _modelPath, u32 _amount,
	const GraphicsProgram::UniformVarInput& diffuse_light_dir_uni,
	const GraphicsProgram::UniformVarInput& diffuse_light_color_uni,
	const GraphicsProgram::UniformVarInput& world_matrix_uni,
	std::vector<View*>& _outViews,
	std::vector<Texture>& _outTextures)
{
	ModelSpawner spawner(&_pgm, _modelPath);

	// Tell the spawner to prepare common data for graphics programs
	spawner.createVertexData();							// Get model's vertices ready
	spawner.create2DTextureMapData();					// Get model's texture coordinates ready
	spawner.createNormalData();							// Get model's normal data ready

	// Map the data
	const GraphicsProgram::ProgramName PGM_NAME = _pgm.getName();
	spawner.mapVertexData(PGM_NAME, 0);					// Use vertex data in the graphics program
	spawner.map2DTextureMapData(PGM_NAME, 1);			// Use 2D texture map coordinates with graphics program
	spawner.mapNormalData(PGM_NAME, 2);					// Use normal data with graphics program

	// Set the spawner to send a world and projection matrix to the graphics program
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);

	// Add lighting-related uniforms
	spawner.addUniformVariable(PGM_NAME, diffuse_light_dir_uni);
	spawner.addUniformVariable(PGM_NAME, diffuse_light_color_uni);
	spawner.addUniformVariable(PGM_NAME, world_matrix_uni);

	// Add custom uniforms - Texture index table to be used for selecting the right texture to apply (ApplyTexture.frag)
	CreateAndMapTextures(PGM_NAME, spawner, _outTextures);

	// Move parameters from the spawner to the model
	spawner.movePgmParameters();

	// Spawn views
	for (u32 i = 0; i < _amount; i++)
	{
		Neptune::View* view = spawner.create();
		view->init();
		_outViews.push_back(view);
	}
}

void ViewSpawnerExamples::SceneExample1()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APPLICATION SET TUP

	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("SceneExample1", WIDTH, HEIGHT, DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window, 3, 4);
	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;												// Pos = (0,0,0)
	camera.translate(0.0f, 2.0f, -3.0f);						// Step back from 8 units
	camera.rotate(10.0f, Vec3(1.0f, 0.0f, 0.0f));
	camera.setScreenRatio(static_cast<float>(WIDTH) / HEIGHT);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();

	// Uniforms
	float diffuse_light_dir[3] = { 1.0f, -1.0f, 0.0f };
	GraphicsProgram::UniformVarInput diffuse_light_dir_uni("DiffuseLightDirection",
		GraphicsProgram::FLOAT,
		3,
		1,
		3 * sizeof(float),
		diffuse_light_dir);

	float diffuse_light_color[3] = { 1.0f, 1.0f, 1.0f };
	GraphicsProgram::UniformVarInput diffuse_light_color_uni("DiffuseLightColor",
		GraphicsProgram::FLOAT,
		3,
		1,
		3 * sizeof(float),
		diffuse_light_color);

	Mat4 World;
	GraphicsProgram::UniformVarInput world_matrix_uni("World",
		GraphicsProgram::FLOAT,
		4,
		4,
		16 * sizeof(float),
		World.getPtr());


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	CREATE GRAPHICS PROGRAM

	//std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/MultiTexturedDisplayWithLight.vert";
	//std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTextureWithLight.frag";

	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/PhongLighting.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/PhongLighting.frag";

	Shader vert(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("MultiTexturedModelWithLight");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SET UP MODEL SPAWNER

	// Set model paths
	const char SONIC_OBJ[] = "Resources/Models/Objs/Sonic/sonic-the-hedgehog.obj";
	const char CLASSIC_SONIC[] = "Resources/Models/ClassicSonic/ClassicSonic.DAE";

	// Resources

	std::vector<View*> views;
	std::vector<Texture> textures(50);

	// Spawn sonic
	SpawnModel(pgm, SONIC_OBJ, 1,
		diffuse_light_dir_uni, diffuse_light_color_uni, world_matrix_uni,
		views, textures
		);

	View* view = views[0];
	view->getTransform().translate(0.0f, 0.0f, 4.0f);
	view->getTransform().rotate(0.0f, 90.0f, 0.0f);
	view->getTransform().scale(0.1f, 0.1f, 0.1f);
	view->bindToCamera(&camera);

	// Spawn the ground
	Texture ground_texture("Resources/Textures/CheckboardHD.jpg");
	ground_texture.init();

	u32 ground_texture_table[32] = { 0 };
	GraphicsProgram::UniformVarInput ground_texture_table_uni =
	{
		"VerticesToTextureBindingPointMap",
		GraphicsProgram::U32,
		32,
		1,
		32 * sizeof(float),
		ground_texture_table
	};

	Shader ground_vert("../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/MultiTexturedDisplay.vert", GL_VERTEX_SHADER);
	Shader ground_frag("../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag", GL_FRAGMENT_SHADER);
	GraphicsProgram ground_pgm("Ground");
	const auto GROUND_PGM_NAME = ground_pgm.getName();
	ground_pgm.add(ground_vert.getId());
	ground_pgm.add(ground_frag.getId());

	PlanSpawner ground_spawner(&ground_pgm);
	ground_spawner.createVertexData();
	ground_spawner.create2DTextureMapData();

	ground_spawner.mapVertexData(GROUND_PGM_NAME, 0);
	ground_spawner.map2DTextureMapData(GROUND_PGM_NAME, 1);

	ground_spawner.setTexture(GROUND_PGM_NAME, &ground_texture);

	ground_spawner.useModelViewAndProjectionMatrices(GROUND_PGM_NAME);
	ground_spawner.addUniformVariable(GROUND_PGM_NAME, ground_texture_table_uni);
	ground_spawner.movePgmParameters();


	View* ground = ground_spawner.create();
	ground->init();
	ground->bindToCamera(&camera);
	ground->getTransform().translate(-25.0f, 0.0f, 25.0f);
	ground->getTransform().rotate(90.0f, 0.0f, 0.0f);
	ground->getTransform().scale(100.0f, 100.0f, 100.0f);

	////////////////////////////////////////////////////////////////////////////
	// MAIN LOOP

	float WHITE[4] = { 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 0.0f };
	float SKY_BLUE[4] = { 0.0f, 162.0f / 255.0f, 232.0f / 255.0f, 0.0f };
	float BLACK[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float TWILIGHT[4] = { 224.0f / 255.0f, 152.0f / 255.0f, 72.0f / 255.0f, 0.0f };
	float* background = TWILIGHT;
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();

		ground->update();
		for (View* v : views)
		{
			//view->getTransform().rotate(0.0f, 1.0f, 0.0f);

			v->updateUniform("World", view->getTransform().getDataPtr());
			v->update();
		}

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

void ViewSpawnerExamples::SkyBox()
{
	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("SkyBoxExample", WIDTH, HEIGHT, DisplayDeviceInterface::MULTI_SAMPLE_ANTI_ALLIASING::X16);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window, 3, 4);
	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;							// Pos = (0,0,0)
	float screen_ratio = static_cast<float>(WIDTH) / HEIGHT;
	camera.setScreenRatio(screen_ratio);
	camera.setViewFrustum(45.0f, screen_ratio, 0.1f, 1500.0f);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PGM SET UP

	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/SkyBox.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/SkyBox.frag";

	Shader vert(vertexShaderName.c_str(), GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(), GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("Skybox");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create texture
	Texture texture("Resources/Textures/SkyBox.ktx", Texture::Type::CUBE_MAP);
	texture.init();

	// Create views
	CubeSpawner spawner(&pgm);
	spawner.createVertexData();
	spawner.mapVertexData(PGM_NAME, 0);
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);
	spawner.setTexture(PGM_NAME, &texture);
	spawner.movePgmParameters();

	View* view = spawner.create();
	view->init();
	view->bindToCamera(&camera);
	view->getTransform().scale(10, 10, 10);

	// main loop
	float RED[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
	float* background = RED;
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		view->update();
		DisplayDeviceInterface::SwapBuffer(window);
	}

	view->terminate();
	delete view;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
	EventSystemInterface::ShutDown();
}