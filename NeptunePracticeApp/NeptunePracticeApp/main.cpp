#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Factories/TriangleSpawner.h"
#include "Graphics/Factories/PlanSpawner.h"
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

	// Creates the graphics programs for the ViewSpawner
	std::string vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/BasicDisplay.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/ApplyTexture.frag";

	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("Bob");
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	// Create Textures
	Texture grass_texture("Resources/Textures/Grass.png");
	grass_texture.init();
	grass_texture.setIndex(0);

	Texture sonic_texture("Resources/Textures/SonicKTX.ktx");
	sonic_texture.init();
	sonic_texture.setIndex(0);

	View* grass_square1 = nullptr, *grass_square2 = nullptr, *sonic = nullptr;

	// Creates the spawner
	// Test that it is effectively a proxy between views and their data
	{
		const GraphicsProgram::ProgramName PGM_NAME = pgm.getName();
		PlanSpawner spawner(&pgm);
		spawner.createVertexData();
		spawner.create2DTextureMapData();
		spawner.setWorldPosition({-0.5f, -0.5f, 0.0f});
		spawner.mapVertexData(PGM_NAME,0);
		spawner.map2DTextureMapData(PGM_NAME, 1);
		spawner.useWorldMatrix(PGM_NAME, NEP_UNIVNAME_MV_MATRIX);
		spawner.setTexture(PGM_NAME, &sonic_texture);


		// Create sonic
		sonic = spawner.create();
		sonic->init();

		// Swap texture: all views will be rendered with grass_texture even sonic
		spawner.setTexture(PGM_NAME, &grass_texture); // swaps the texture for all instances

		// Create grass_square1
		grass_square1 = spawner.create();
		grass_square1->init();
		grass_square1->getTransform().translate(-0.5f, -0.5f, 0.0f);

		// Create grass_square2
		grass_square2 = spawner.create();
		grass_square2->init();
		grass_square2->getTransform().translate(0.2f, -0.5f, 0.0f);
	}

	// main loop
	float background[4] = {255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		// Texture swap at update time to limit memory consumption
		// The same program is used to render different objects
		// only the textures are swapped.
		pgm.setTexture(&grass_texture);
		grass_square1->update();
		grass_square2->update();
		
		pgm.setTexture(&sonic_texture);
		sonic->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	// Cleanup/shutdown
	grass_square1->terminate();
	grass_square2->terminate();
	sonic->terminate();

	delete grass_square1;
	delete grass_square2;
	delete sonic;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);

	return 0;
}