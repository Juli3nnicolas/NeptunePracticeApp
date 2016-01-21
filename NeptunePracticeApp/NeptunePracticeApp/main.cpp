#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/Factories/ModelFactory.h"

#include "Graphics/Camera.h"
#include "Math/Vectors/Vec3.h"

using namespace Neptune;

/*int main(int argc, char* argv[])
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);

	Camera camera( Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.25f, 0.0f), Vec3(0.0f, 1.0f, 0.0f) );
	camera.setScreenRatio( 1024.0f/768.0f );

	ModelFactory factory("Resources/Models/xwing.ply");
	View* cube = factory.create();
	cube->init();
	cube->getTransform().scale(0.25f, 0.25f, 0.25f);
	cube->bindToCamera( &camera );

	CubeFactory f2(0.0f,0.0f,1.0f);
	View* cube2 = f2.create();
	cube2->init();
	cube2->getTransform().translate(0.0f, 0.5f, 0.0f);
	cube2->getTransform().scale(0.25f,0.25f,0.25f);
	cube2->bindToCamera( &camera );

	CubeFactory f3(0.0f,0.5f,0.8f);
	View* cube3 = f3.create();
	cube3->init();
	cube3->getTransform().translate(0.0f,0.0f,5.0f);
	cube3->bindToCamera(&camera);

	float background[4] = { 0.0f,0.0f,0.0f,0.0f };
	while(true)
	{
		DisplayDeviceInterface::ClearBuffers(background);
		cube->update();
		cube2->update();
		cube3->update();
		cube->getTransform().rotate(0.0f, 1.0f, 0.0f);
		cube2->getTransform().rotate(1.0f, 0.0f, 0.0f);
		cube3->getTransform().rotate(0.0f, 1.0f, 0.0f);
		DisplayDeviceInterface::SwapBuffer(window);
	}

	cube->terminate();
	cube2->terminate();
	cube3->terminate();

	delete cube;
	delete cube2;
	delete cube3;

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);

	return 0;
}*/

int main(int argc, char* argv[])
{
	DisplayDeviceInterface::WindowHandle window = DisplayDeviceInterface::CreateWindow("Test",1024,768);
	DisplayDeviceInterface::GraphicalContextHandle ctxt = DisplayDeviceInterface::CreateGraphicalContext(window,3,4);

	Camera camera;//(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	camera.setScreenRatio( 1024.0f/768.0f );
	//camera.rotate((15.0f*3.14f/180.0f), Vec3(0.0f,1.0f,0.0f));
	//camera.translate(0.0f, 0.0f, -5.0f);
	camera.lookAt(Vec3(-5.0f, 5.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	camera.translate(5.0f, -5.0f, 0.0f);
	camera.rotate(-25.0f, Vec3(0.0f,1.0f,0.0f));
	camera.rotate(-20.0f, Vec3(1.0f,0.0f,0.0f));

	CubeFactory f1(1.0f, 0.0f, 0.0f);
	View* t1 = f1.create();
	t1->init();
	t1->bindToCamera(&camera);
	t1->getTransform().translate(0.0f, 0.0f, 0.9f);
	//t1->getTransform().scale(0.5f, 0.5f, 0.5f);

	CubeFactory f2(1.0f, 1.0f, 0.0f);
	View* t2 = f2.create();
	t2->init();
	t2->bindToCamera(&camera);
	t2->getTransform().translate(0.0f, 0.0f, -0.5f);
	//t2->getTransform().scale(3.0f, 3.0f, 3.0f);

	float zoom = 0.1f;
	float background[4] = { 0.0f,0.0f,0.0f,0.0f };
	while (true)
	{
		DisplayDeviceInterface::ClearBuffers(background);

		//camera.rotate(0.5f, Vec3(1.0f,1.0f,0.0f));
		//camera.translate(0.0f, 0.01f, 0.0f);
		//camera.zoom(zoom);
		zoom += 0.1f;
		t1->update();
		t2->update();
		//t1->getTransform().rotate(0.0f, 1.0f, 0.0f);
		//t2->getTransform().rotate(0.0f, 0.0f, 1.0f);

		DisplayDeviceInterface::SwapBuffer(window);
	}

	DisplayDeviceInterface::DestroyWindow(window);
	DisplayDeviceInterface::DestroyGraphicalContext(ctxt);

	return 0;
}