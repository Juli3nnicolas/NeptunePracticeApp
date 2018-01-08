#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Spawners/TriangleSpawner.h"
#include "Graphics/Spawners/PlanSpawner.h"
#include "Graphics/Spawners/CubeSpawner.h"
#include "Graphics/Spawners/ModelSpawner.h"

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

#include "FactoryExamples.h"
#include "ViewSpawnerExamples.h"

#include "Mandelbrot.h"

#include "Debug/NeptuneDebug.h"

using namespace Neptune;

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
	_spawner.mapVerticesToTextureBindingPoints(texture_map);				// Sets a map of {last vertex index (to be used with), binding point}

	const u8 NB_MAX_TEXTURE_BINDING_POINTS = 16;							// Max texture binding points supported by shader.
	NEP_ASSERT(NB_MAX_TEXTURE_BINDING_POINTS * 2 >= texture_map.size());	// Each binding point goes with a vertex id, hence the multiplication

	// Add the map as an uniform to the graphics program
	GraphicsProgram::UniformVarInput texture_binding_index_array_uni(NEP_UNIVNAME_TEXTURE_BINDING_INDEX_ARRAY,
		GraphicsProgram::U32,
		NB_MAX_TEXTURE_BINDING_POINTS * 2,			// Each binding point goes with a vertex id, hence the multiplication
		1,											// An array, not a vector (2,3) nor a matrix
		texture_map.size()*sizeof(texture_map[0]),
		texture_map.data());

	_spawner.addUniformVariable(_pgmName, texture_binding_index_array_uni);
}

void MultiTexturedModelExample()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APPLICATION SET TUP

	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("MultiTexturedModelExample",WIDTH, HEIGHT);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
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
	
	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

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

	View* view{nullptr}; 
	view = spawner.create();
	view->init();
	view->getTransform().translate(0.0f, 0.0f, 4.0f);
	view->getTransform().rotate(0.0f, 90.0f, 0.0f);
	view->getTransform().scale(0.1f, 0.1f, 0.1f);

	view->bindToCamera(&camera);


	////////////////////////////////////////////////////////////////////////////
	// MAIN LOOP

	float WHITE[4]		= {255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	float SKY_BLUE[4]	= {0.0f,162.0f/255.0f,232.0f/255.0f,0.0f};
	float BLACK[4]		= {0.0f, 0.0f, 0.0f, 0.0f};
	float* background	= SKY_BLUE;
	while(true)
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

int main(int argc, char* argv[])
{
	MultiTexturedModelExample();
	//Mandelbrot::MandelbrotExample();
	//FactoryExamples::Display100PLYModels();

	return 0;
}