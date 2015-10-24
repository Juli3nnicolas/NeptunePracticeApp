#include "Graphics/DisplayDeviceInterface.h"
#include "Graphics/Shader.h"
#include "Graphics/GraphicalProgram.h"
#include "Graphics/VAORenderer.h"

using namespace Neptune;

int main(int argc, char* argv[])
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow( "Test", 1024, 768 );
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext( window, 3, 4 );

	// First triangle's data

	float t1[] = 
	{
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	float c1[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GraphicalProgram::ShaderAttribute t1_data =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              //nb components per value
		sizeof( t1 ),                  // data size
		t1                            // data
	};

	GraphicalProgram::ShaderAttribute c1_data =
	{
		1,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              //nb components per value
		sizeof(c1),                    // data size
		c1                            // data
	};

	// Create the triangle's renderer
	VAORenderer r1;
	r1.setDrawingPrimitve(Renderer::DrawingPrimitive::TRIANGLES);
	r1.setNbverticesToRender( 3 );
	
	// Create the shader to display the triangle
	Shader vert("Resources/Shaders/VertexBuffer.vert",GL_VERTEX_SHADER);
	Shader frag("Resources/Shaders/VertexBuffer.frag",GL_FRAGMENT_SHADER);

	// Create the program to display a triangle
	GraphicalProgram& pgm = r1.createProgram();
	pgm.add(vert.getId());
	pgm.add(frag.getId());
	pgm.addShaderAttribute(t1_data);
	pgm.addShaderAttribute(c1_data);
	pgm.build();

	// Rendering loop

	r1.init();
	while ( true )
	{
		r1.update();
		DisplayDeviceInterface::SwapBuffer( window );
	}
	r1.terminate();

	return 0;
}