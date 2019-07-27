#include "InputSystemExamples.h"
#include "Application/SimpleApp.h"
#include "Input/InputConsumer.h"
#include "Input/SDLInputProducer.h"
#include "Input/InputTypes.h"
#include "Graphics/Factories/ModelFactory.h"
#include "Graphics/Camera.h"


using namespace Neptune;


void InputSystemExamples::InputExample()
{
	const u32 WIDTH = 1024;
	const u32 HEIGHT = 768;

	// Starts both the input and graphical systems
	SimpleApp app(WIDTH, HEIGHT, "InputExample");

	// Add a view to the application
	ModelFactory factory("Resources/Models/xwing.ply");
	View* v = factory.create();
	v->init();
	v->getTransform().translate(0.0f, 0.0f, 2.0f);

	app.add(v);

	// Run the main loop
	app.loop();
}