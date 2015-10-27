#include "Graphics/DisplayDeviceInterface.h"
#include "Graphics/Shader.h"
#include "Graphics/GraphicalProgram.h"
#include "Graphics/VAORenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	
	// Create the shaders to display the triangle
	Shader vert("Resources/Shaders/VertexBuffer.vert",GL_VERTEX_SHADER);
	Shader frag("Resources/Shaders/VertexBuffer.frag",GL_FRAGMENT_SHADER);

	// Create the program to display a triangle
	{
		GraphicalProgram& pgm = r1.createProgram();
		pgm.add(vert.getId());
		pgm.add(frag.getId());
		pgm.addShaderAttribute(t1_data);
		pgm.addShaderAttribute(c1_data);
		pgm.build();
	}

	//////////////////////////////////////////////

	float t2[] =
	{
		0.0f,0.0f,0.0f,
		-0.5f,0.0f,0.0f,
		0.0f,0.0f,0.5f
	};

	float c2[] =
	{
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f
	};

	GraphicalProgram::ShaderAttribute t2_data =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              //nb components per value
		sizeof(t2),                    // data size
		t2                            // data
	};

	GraphicalProgram::ShaderAttribute c2_data =
	{
		1,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              //nb components per value
		sizeof(c2),                    // data size
		c2                            // data
	};

	// Instantiate a shader using rotation_matrix
	Shader u_vert2("Resources/Shaders/UniformVariable.vert",GL_VERTEX_SHADER);

	// Create a model matrix uniform
	{
		glm::mat4 model;
		model = glm::rotate(model,-90.0f,glm::vec3(1.0f,0.0f,0.0f));

		GraphicalProgram::UniformVarInput u2(	"rotation_matrix",
												GraphicalProgram::FLOAT,
												4,
												4,
												16*sizeof(float),
												glm::value_ptr(model));

		// Create the program to display a triangle
		{
			GraphicalProgram& pgm = r1.createProgram();
			pgm.add(u_vert2.getId());
			pgm.add(frag.getId());
			pgm.addShaderAttribute(t2_data);
			pgm.addShaderAttribute(c2_data);
			pgm.addUniformVariable(u2);
			pgm.build();
		}
	}

	////////////////////////////////////////////////

	//////////////////////////////////////////////

	float t3[] =
	{
		0.0f,0.0f,0.0f,
		0.5f,0.0f,0.0f,
		0.0f,0.0f,0.5f
	};

	float c3[] =
	{
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f
	};

	GraphicalProgram::ShaderAttribute t3_data =
	{
		0,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              //nb components per value
		sizeof(t3),                    // data size
		t3                            // data
	};

	GraphicalProgram::ShaderAttribute c3_data =
	{
		1,                                // layout
		GraphicalProgram::Types::FLOAT,  // Type
		3,                              //nb components per value
		sizeof(c3),                    // data size
		c3                            // data
	};

	// Create a model matrix uniform
	glm::mat4 model;
	model = glm::rotate( model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );

	GraphicalProgram::UniformVarInput u3(	"rotation_matrix",
											GraphicalProgram::FLOAT,
											4,
											4,
											16*sizeof(float),
											glm::value_ptr( model ) );

	// Create a second renderer
	VAORenderer r2;
	r2.setDrawingPrimitve(Renderer::DrawingPrimitive::TRIANGLES);
	r2.setNbverticesToRender(3);

	// Instantiate a shader using rotation_matrix
	Shader u_vert("Resources/Shaders/UniformVariable.vert", GL_VERTEX_SHADER);

	// Create the program to display a triangle
	{
		GraphicalProgram& pgm = r2.createProgram();
		pgm.add(u_vert.getId());
		pgm.add(frag.getId());
		pgm.addShaderAttribute(t3_data);
		pgm.addShaderAttribute(c3_data);
		pgm.addUniformVariable(u3);
		pgm.build();
	}

	////////////////////////////////////////////////

	// Init the renderers
	r1.init();
	r2.init();

	// Rendering loop
	float background[4] = { 0.0f,0.8f,1.0f,1.0f };
	while ( true )
	{
		DisplayDeviceInterface::ClearBuffers( background );
		r1.update();
		r2.update();
		DisplayDeviceInterface::SwapBuffer( window );
	}
	r1.terminate();
	r2.terminate();

	return 0;
}