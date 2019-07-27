#include "Mandelbrot.h"

#include "Graphics/DisplayDeviceInterface.h"
#include "Graphics/Spawners/PlanSpawner.h"
#include "Graphics/Shader.h"
#include "Graphics/View.h"
#include "Graphics/UniformVarNames.h"

#include <chrono>
#include <ratio>
#include <ctime>

using namespace Neptune;

namespace Mandelbrot
{
	std::chrono::high_resolution_clock::time_point s_t_start = std::chrono::high_resolution_clock::now();

	float GetCurrentTime()
	{
		using namespace std::chrono;
	
		high_resolution_clock::time_point t_now   = high_resolution_clock::now();
		duration<float> time_span                 = duration_cast<duration<float>>(t_now - s_t_start);
	
		return time_span.count();
	}
}


void Mandelbrot::MandelbrotExample()
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test", 1280, 760);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);
	
	// Creates the graphics programs for the ViewSpawner
	std::string vertexShaderName   = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Vertex/Display.vert";
	std::string fragmentShaderName = "../../../Neptune/Engine/Multiplatform/Core/Shaders/Fragment/Mandelbrot.frag";
	
	Shader vert(vertexShaderName.c_str(),GL_VERTEX_SHADER);
	Shader frag(fragmentShaderName.c_str(),GL_FRAGMENT_SHADER);

	GraphicsProgram pgm("Mandelbrot");
	pgm.add(vert.getId());
	pgm.add(frag.getId());

	// Creates the uniforms
	float current_time = Mandelbrot::GetCurrentTime();

	GraphicsProgram::UniformVarInput current_time_uni("_Time",
		GraphicsProgram::FLOAT,
		1,
		1,
		sizeof(float),
		&current_time);

	// Creates the spawner
	const GraphicsProgram::ProgramName PGM_NAME = pgm.getName();
	PlanSpawner spawner(&pgm);
	spawner.createVertexData();
	spawner.createColorData({0.0f, 0.0f, 0.0f, 1.0f});
	//spawner.setWorldPosition({0.0f, 0.0f, 0.0f});
	spawner.mapVertexData(PGM_NAME, 0);
	spawner.mapColorData(PGM_NAME, 1);
	spawner.useModelViewAndProjectionMatrices(PGM_NAME);
	spawner.addUniformVariable(PGM_NAME,current_time_uni);
	spawner.movePgmParameters();

	// Create a view
	View* view = spawner.create();
	view->init();
	view->getTransform().translate(-1.0f, -1.0f, 0.0f);
	view->getTransform().scale(4.0f, 4.0f, 0.0f);

	// main loop
	float background[4] ={255.0f/255.0f,255.0f/255.0f,255.0f/255.0f,0.0f};
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);
		
		current_time = Mandelbrot::GetCurrentTime();
		view->updateUniform(PGM_NAME,"_Time",&current_time);
		view->update();

		DisplayDeviceInterface::SwapBuffer(window);
	}

	// Cleanup/shutdown
	view->terminate();
	delete view;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);
}

