#include "ViewSpawnerExamples.h"
#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Factories/TriangleSpawner.h"
#include "Math/Vectors/Vec3.h"

#include "Graphics/Shader.h"
#include "Graphics/Color.h"
#include "Graphics/View.h"
#include "Graphics/VAOView.h"

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

