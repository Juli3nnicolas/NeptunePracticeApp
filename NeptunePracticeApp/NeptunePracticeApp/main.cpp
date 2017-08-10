#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Factories/TriangleSpawner.h"
#include "Math/Vectors/Vec3.h"

#include "Graphics/Shader.h"
#include "Graphics/Color.h"
#include "Graphics/View.h"
#include "Graphics/VAOView.h"
#include "Graphics/UniformVarNames.h"

#include "Physics/Mechanics/Position.h"

#include <string>
#include <chrono>
#include <ratio>
#include <ctime>

using namespace Neptune;


int main(int argc, char* argv[])
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
	spawner.setWorldPosition({-0.5f, 0.25f, 0.0f});
	spawner.mapVertexData(PGM_NAME, 0);
	spawner.mapColorData(PGM_NAME, 1);
	spawner.useWorldMatrix(PGM_NAME, NEP_UNIVNAME_MV_MATRIX);

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

	return 0;
}