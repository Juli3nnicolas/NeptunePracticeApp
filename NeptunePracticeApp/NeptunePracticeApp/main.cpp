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

void MultiTexturedModelExample()
{
	const u32 WIDTH = 1024, HEIGHT = 768;

	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",WIDTH, HEIGHT);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
	EventSystemInterface::StartUp();

	// Set camera location
	Camera camera;							// Pos = (0,0,0)
	camera.translate(0.0f, 0.0f, -8.0f);	// Step back from 8 units
	camera.setScreenRatio(static_cast<float>(WIDTH) / HEIGHT);

	// Set camera controller
	TempFPSCameraController controller(&camera);
	controller.init();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PGM SET UP

	// Texture-based display
	std::string vertexShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/MultiTexturedDisplay.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag";
	
	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("MultiTexturedModel");
	const auto PGM_NAME = pgm.getName();
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create views
	Color color {1.0f, 1.0f, 0.0f, 1.0f};
	const char PLANS[]			= "Resources/Models/Objs/ColouredPolygones/TexturedPlans.obj";
	const char CLASSIC_SONIC[]	= "Resources/Models/ClassicSonic/ClassicSonic.DAE";
	const char SONIC_OBJ[]		= "Resources/Models/Objs/Sonic/sonic-the-hedgehog.obj";
	
	ModelSpawner factory(&pgm, SONIC_OBJ);			
	//ModelSpawner factory(&pgm, PLANS);		 
	//ModelSpawner factory(&pgm, CLASSIC_SONIC); 

	////////////////////////// DEBUG

	std::unordered_map<std::string, u8> texture_bindings;
	factory.getTextureBindingPoints(texture_bindings);

	// Test - set the same mapping as the hard-coded array (see top of the file)
	// Useful if one wants to share textures
	//texture_bindings["Resources/Models/Objs/Sonic/f1f6d3cb.jpg"] = 3;
	//texture_bindings["Resources/Models/Objs/Sonic/bab97353.jpg"] = 1;
	//factory.setTextureBindingPoints(texture_bindings);
	//

	std::vector<Texture> textures;
	for (const auto& binding : texture_bindings )
	{
		textures.emplace_back(binding.first.c_str());
		auto& texture = textures.back();
		texture.setIndex(binding.second);
		texture.init();
	}

	for (auto& t : textures)
		factory.setTexture(PGM_NAME, &t);

	static std::vector<u32> texture_map;
	factory.mapVerticesToTextureBindingPoints(texture_map);

	const u8 NB_MAX_TEXTURE_BINDING_POINTS = 13; // Max texture binding points supported by shader.
	NEP_ASSERT(NB_MAX_TEXTURE_BINDING_POINTS * 2 >= texture_map.size());

	GraphicsProgram::UniformVarInput texture_binding_index_array_uni(NEP_UNIVNAME_TEXTURE_BINDING_INDEX_ARRAY,
		GraphicsProgram::S32,
		NB_MAX_TEXTURE_BINDING_POINTS * 2,		// Each binding point goes with a vertex id, hence the multiplication
		1,										// An array, not a vector nor a matrix
		texture_map.size()*sizeof(texture_map[0]),
		texture_map.data());

	/////////


	factory.createVertexData();
	//factory.createColorData(color);
	//factory.createNormalData();
	factory.create2DTextureMapData();
	factory.mapVertexData(PGM_NAME, 0);
	//factory.mapColorData(PGM_NAME, 1);
	factory.map2DTextureMapData(PGM_NAME, 1);
	//factory.mapNormalData(PGM_NAME, 2);
	factory.useWorldAndProjectionMatrices(PGM_NAME);
	//factory.addUniformVariable(PGM_NAME, diffuse_light_dir_uni);
	//factory.addUniformVariable(PGM_NAME, diffuse_light_color_uni);
	factory.addUniformVariable(PGM_NAME, texture_binding_index_array_uni);

	
	/*Texture texture1("Resources/Models/Objs/Sonic/64124be4.jpg");
	//Texture texture1("Resources/Textures/Square1.png");
	texture1.setIndex(0);
	texture1.init();
	Texture texture2("Resources/Models/Objs/Sonic/bab97353.jpg");
	//Texture texture2("Resources/Textures/Square2.png");
	texture2.setIndex(1);
	texture2.init();
	Texture texture3("Resources/Models/Objs/Sonic/d1419efe.jpg");
	//Texture texture3("Resources/Textures/Square3.png");
	texture3.setIndex(2);
	texture3.init();
	Texture texture4("Resources/Models/Objs/Sonic/f1f6d3cb.jpg");
	//Texture texture4("Resources/Textures/Square4.png");
	texture4.setIndex(3);
	texture4.init();*/
	

	/*factory.setTexture(PGM_NAME, &texture1);
	factory.setTexture(PGM_NAME, &texture2);
	factory.setTexture(PGM_NAME, &texture3);
	factory.setTexture(PGM_NAME, &texture4);*/

	const u32 NB_VIEWS = 1;
	View* view_table[NB_VIEWS] = {nullptr}; 

	{
		const float OFFSET = 2.0f;

		view_table[0] = factory.create();
		view_table[0]->init();
		view_table[0]->getTransform().translate(0.0f, 0.0f, 4.0f);
		//view_table[0]->getTransform().rotate(90.0f, 0.0f, 0.0f);
		view_table[0]->getTransform().rotate(0.0f, 90.0f, 0.0f);
		//view_table[0]->getTransform().rotate(0.0f, 0.0f, 220.0f);
		view_table[0]->getTransform().scale(0.1f, 0.1f, 0.1f);

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
	float WHITE[4]		= {255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	float SKY_BLUE[4]	= {0.0f,162.0f/255.0f,232.0f/255.0f,0.0f};
	float BLACK[4]		= {0.0f, 0.0f, 0.0f, 0.0f};
	float* background	= SKY_BLUE;
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		controller.update();
		for (auto& v : view_table)
		{	
			v->getTransform().rotate(0.0f, 1.0f, 0.0f);
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
	MultiTexturedModelExample();
	//Mandelbrot::MandelbrotExample();
	//FactoryExamples::Display100PLYModels();

	return 0;
}