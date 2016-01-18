#include "ExApp.h"

#include "Graphics/Renderer.h"
#include "Graphics/PLYLoader.h"
#include "System/Type/Integers.h"
#include "Graphics/Shader.h"
#include "Graphics/GraphicsProgram.h"
#include "Graphics/Camera.h"

using namespace Neptune;

/*// Terribly ineffective but insightful
static void ClumsilyDrawTriangle(Neptune::Renderer& renderer)
{
	// Create data

	float position[] ={-1,0,0,0,1,0,1,0,0};
	float color[]    ={0,0,0.5,0,0,0.5,0,0,0.5};

	// Set the VBOs

	GLuint vbos_handle[2];
	glGenBuffers(2,vbos_handle);
	GLuint pos_vbo = vbos_handle[0];
	GLuint col_vbo = vbos_handle[1];

	glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER,col_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);

	// Set The VAO

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	// Bind the vbos to the vao

	glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,col_vbo);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind the vao (all the drawing state has been saved)


	// Attempt to mess up the drawing process
	GLuint vao2;
	{
		float position[] ={-1,0,0,0,1,0,1,0,0};
		float color[]    ={0,1,0,0,1,0,0,1,0};

		// Set the VBOs

		GLuint vbos_handle[2];
		glGenBuffers(2,vbos_handle);
		GLuint pos_vbo = vbos_handle[0];
		GLuint col_vbo = vbos_handle[1];

		glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER,col_vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);

		// Set The VAO

		glGenVertexArrays(1,&vao2);
		glBindVertexArray(vao2);

		// Bind the vbos to the vao

		glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER,col_vbo);
		glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	
	}

	// Bind the triangle's VAO to load its shader-inputs

	glBindVertexArray(vao);   // Stage the dark blue triangle's drawing state for rendering
	glBindVertexArray(vao2);  // Stage the green triangle's drawing state for rendering
	glBindVertexArray(vao);   // Stage the dark blue triangle's drawing state for rendering

	// Build the display program

	Neptune::Shader vertex("Resources/Shaders/VertexBuffer.vert",GL_VERTEX_SHADER);
	Neptune::Shader fragment("Resources/Shaders/VertexBuffer.frag",GL_FRAGMENT_SHADER);

	Neptune::GraphicalProgram program;
	program.add(vertex.getId());
	program.add(fragment.getId());
	program.build();

	// Set up the renderer

	renderer.setRenderingPgm(program.getId());
	renderer.setNbverticesToRender(3);
	renderer.setBackgroundColor(153.0f/255.0f,217.0f/255.0f,234.0f/255.0f,1.0f);
}

static void DrawTriangle(Neptune::Renderer& renderer)
{
	// Create data

	float position[] ={-1,0,0,0,1,0,1,0,0};
	float color[]    ={1,0,0,1,0,0,1,0,0};

	// Set VAO

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray( vao );

	// Set VBOs

	GLuint vbos_handle[2];
	glGenBuffers(2, vbos_handle);
	GLuint pos_vbo = vbos_handle[0];
	GLuint col_vbo = vbos_handle[1];

	glBindBuffer( GL_ARRAY_BUFFER, pos_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW );
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,col_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);
	glEnableVertexAttribArray(1);

	// Build the display program

	Neptune::Shader vertex("Resources/Shaders/VertexBuffer.vert",GL_VERTEX_SHADER);
	Neptune::Shader fragment("Resources/Shaders/VertexBuffer.frag",GL_FRAGMENT_SHADER);

	Neptune::GraphicalProgram program;
	program.add(vertex.getId());
	program.add(fragment.getId());
	program.build();

	// Set up the renderer

	renderer.setRenderingPgm(program.getId());
	renderer.setNbverticesToRender(3);
	renderer.setBackgroundColor(153.0f/255.0f,217.0f/255.0f,234.0f/255.0f,1.0f);
}

static void DrawTriangleWithGlElement(Neptune::Renderer& renderer)
{
	// data
	float position[] ={-1,0,0,0,1,0,1,0,0};
	float color[]    ={1,1,0,1,1,0,1,1,0};
	u8 list[]        ={0,1,2};

	// Shader's parameters
	glEnableVertexAttribArray(0);	// Position
	glEnableVertexAttribArray(1);	// Colors

	// VBO setting
	GLuint vbo_handle[3];
	glGenBuffers(3,vbo_handle);

	GLuint pos_vbo_handler      = vbo_handle[0];
	GLuint col_vbo_handler      = vbo_handle[1];
	GLuint list_vbo_handler     = vbo_handle[2];

	// VBO binding and filling
	glBindBuffer(GL_ARRAY_BUFFER,pos_vbo_handler);
	glBufferData(GL_ARRAY_BUFFER,sizeof(position),position,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);

	glBindBuffer(GL_ARRAY_BUFFER,col_vbo_handler);
	glBufferData(GL_ARRAY_BUFFER,sizeof(color),color,GL_STATIC_DRAW);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,list_vbo_handler);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(list),list,GL_STATIC_DRAW);

	// Build the display program

	Neptune::Shader vertex("Resources/Shaders/VertexBuffer.vert",GL_VERTEX_SHADER);
	Neptune::Shader fragment("Resources/Shaders/VertexBuffer.frag",GL_FRAGMENT_SHADER);

	Neptune::GraphicalProgram program;
	program.add(vertex.getId());
	program.add(fragment.getId());
	program.build();

	// Set up the renderer

	renderer.setRenderingPgm(program.getId());
	renderer.setNbverticesToRender(3);
	renderer.setVertexIndicesType(GL_UNSIGNED_BYTE);
	renderer.setBackgroundColor(153.0f/255.0f,217.0f/255.0f,234.0f/255.0f,1.0f);
}

static void InitXWingBuffers(Neptune::PLYLoader& loader)
{
	// Set the buffers

	GLuint vao_handle = 0;
	GLuint vbo_handle[4];
	glGenBuffers(4,vbo_handle);
	GLuint pos_vbo_handler      = vbo_handle[0];
	GLuint col_vbo_handler      = vbo_handle[1];
	GLuint nor_vbo_handler      = vbo_handle[2];
	GLuint pos_list_vbo_handler = vbo_handle[3];

	// Fill the position buffer

	glBindBuffer(GL_ARRAY_BUFFER,pos_vbo_handler);
	glBufferData(GL_ARRAY_BUFFER,loader.getPropertyBuffer(PLYLoader::PropertyType::POSITION).m_bufferSize,
		loader.getPropertyBuffer(PLYLoader::PropertyType::POSITION).m_buffer,GL_STATIC_DRAW);

	// Fill the color buffer

	glBindBuffer(GL_ARRAY_BUFFER,col_vbo_handler);
	glBufferData(GL_ARRAY_BUFFER,loader.getPropertyBuffer(PLYLoader::PropertyType::COLOR).m_bufferSize,
		loader.getPropertyBuffer(PLYLoader::PropertyType::COLOR).m_buffer,GL_STATIC_DRAW);

	// Fill the normal buffer

	glBindBuffer(GL_ARRAY_BUFFER,nor_vbo_handler);
	glBufferData(GL_ARRAY_BUFFER,loader.getPropertyBuffer(PLYLoader::PropertyType::NORMAL).m_bufferSize,
		loader.getPropertyBuffer(PLYLoader::PropertyType::NORMAL).m_buffer,GL_STATIC_DRAW);

	// Fill the position-list buffer

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pos_list_vbo_handler);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,loader.getPropertyBuffer(PLYLoader::PropertyType::INDEX).m_bufferSize,
		loader.getPropertyBuffer(PLYLoader::PropertyType::INDEX).m_buffer,GL_STATIC_DRAW);

	// Enable the vertex-attribute objects (inputs of the vertex buffer)

	glEnableVertexAttribArray(0);	// Position
	glEnableVertexAttribArray(1);	// Colors
	glEnableVertexAttribArray(2);	// Normals

	// Map index 0 to the position buffer

	glBindBuffer(GL_ARRAY_BUFFER,pos_vbo_handler);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,NULL);

	// Map index 1 to the color buffer

	glBindBuffer(GL_ARRAY_BUFFER,col_vbo_handler);
	glVertexAttribPointer(1,3,GL_UNSIGNED_BYTE,GL_FALSE,0,NULL);

	// Map index 2 to the normal buffer

	glBindBuffer(GL_ARRAY_BUFFER,nor_vbo_handler);
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,NULL);

	// Bind the element array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pos_list_vbo_handler);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

static void DrawXWing(Neptune::Renderer& renderer)
{
	Neptune::PLYLoader model_loader;
	model_loader.load("Resources/Models/xwing.ply");
	Neptune::Shader vertex("Resources/Shaders/SelfPractice/xwingDiffuseLightingUniform.vert",GL_VERTEX_SHADER);
	Neptune::Shader fragment("Resources/Shaders/SelfPractice/testView.frag",GL_FRAGMENT_SHADER);

	Neptune::GraphicalProgram program;
	program.add(vertex.getId());
	program.add(fragment.getId());
	program.build();

	InitXWingBuffers(model_loader);

	renderer.setRenderingPgm(program.getId());
	renderer.setNbverticesToRender(model_loader.getNbverticesToRender());
	renderer.setVertexIndicesType(model_loader.getIndicesType());
	renderer.setBackgroundColor(153.0f/255.0f,217.0f/255.0f,234.0f/255.0f,1.0f);

	// Using glm
	{
		Neptune::Camera camera(glm::vec3(2.0f,2.0f,2.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
		camera.setScreenRatio(4.0f/3.0f);

		glm::mat4 proj = glm::perspective(camera.getFieldOfView(),camera.getScreenRatio(),camera.getNearPlan(),camera.getFarPlan());

		GLfloat MV[4][4];
		for(u8 i = 0; i < 4; i++)
		for(u8 j = 0; j < 4; j++)
			MV[i][j] = camera.getMatrix()[i][j];

		GLfloat Proj[4][4];
		for(u8 i = 0; i < 4; i++)
		for(u8 j = 0; j < 4; j++)
			Proj[i][j] = proj[i][j];

		renderer.setMVMatrix(MV);
		renderer.setProjMatrix(Proj);
	}
}






Ex::ExApp::ExApp()
{
	ClumsilyDrawTriangle( m_renderer );
	//DrawTriangle( m_renderer );
	//DrawTriangleWithGlElement( m_renderer );
	//DrawXWing( m_renderer );
}

Ex::ExApp::~ExApp()
{

}

*/