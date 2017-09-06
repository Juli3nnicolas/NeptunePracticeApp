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

	// Create a Texture
	Texture texture("Resources/Textures/Sonic.png");
	texture.init();

	// Creates the spawner
	const GraphicsProgram::ProgramName PGM_NAME = pgm.getName();
	PlanSpawner spawner(&pgm);
	spawner.createVertexData();
	spawner.create2DTextureMapData();
	spawner.setWorldPosition({-0.5f, -0.5f, 0.0f});
	spawner.mapVertexData(PGM_NAME,0);
	spawner.map2DTextureMapData(PGM_NAME, 1);
	spawner.useWorldMatrix(PGM_NAME, NEP_UNIVNAME_MV_MATRIX);

	// Create a view
	View* view = spawner.create();
	view->bindTexture(&texture);
	view->init();

	// main loop
	float background[4] = {255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
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

	//ViewSpawnerExamples::ModelViewMatrix();

	return 0;
}