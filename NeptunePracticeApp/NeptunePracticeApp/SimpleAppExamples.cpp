#include "SimpleAppExamples.h"
#include "Graphics/Spawners/ModelSpawner.h"
#include "Graphics/Shader.h"
#include "Graphics/Color.h"
#include "Graphics/View.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "Graphics/UniformVarNames.h"
#include "Debug/NeptuneDebug.h"
#include "Application/SimpleApp.h"
#include "Graphics/Factories/ModelFactory.h"

#include <string>

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

void SimpleAppExamples::MultiTexturedModelWithSimpleLightingExample()
{
	// Create APPLICATION
	u32 WIDTH = 1024, HEIGHT = 768;
	SimpleApp app(WIDTH, HEIGHT, "MultiTexturedModelWithSimpleLightingExample");


	//////////////////////// INIT MODEL /////////////////////////////////////////////////////////////////////////////
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// APPLICATION
	app.add(view);

	Color PURPLE = { 0.5f, 0.0f, 0.5f, 0.0f };
	app.setBackgroundColor(PURPLE);

	// Set camera location
	Camera& camera = app.getCamera();							// Pos = (0,0,0)
	camera.translate(0.0f, 2.0f, -3.0f);						// Step back from 3 units and move up from 2

	// Set model-on-frame-rotation
	app.setOnViewUpdateCallBack(
		[](View* _view){
		_view->getTransform().rotate(0.0f, 1.0f, 0.0f);
		_view->updateUniform("World", _view->getTransform().getDataPtr());
		return true;
	}
	);

	// Main loop
	app.loop();
}

void SimpleAppExamples::ModelFactoryExample()
{
	const u32 WIDTH = 1024;
	const u32 HEIGHT = 768;

	SimpleApp app(WIDTH, HEIGHT, "ModelFactoryExample");

	ModelFactory factory("Resources/Models/xwing.ply");
	View* v = factory.create();
	v->init();
	v->getTransform().translate(0.0f, 0.0f, 2.0f);
	
	app.add(v);
	app.loop();
}