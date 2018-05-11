#include "Graphics/DisplayDeviceInterface.h"

#include "Graphics/Spawners/TriangleSpawner.h"
#include "Graphics/Spawners/PlanSpawner.h"
#include "Graphics/Spawners/CubeSpawner.h"
#include "Graphics/Spawners/ModelSpawner.h"

#include "Graphics/Factories/TriangleFactory.h"
#include "Graphics/Factories/PlanFactory.h"
#include "Graphics/Factories/CubeFactory.h"
#include "Graphics/Factories/ModelFactory.h"

#include "Math/Vectors/Vec3.h"
#include "Graphics/Shader.h"
#include "Graphics/Color.h"
#include "Graphics/View.h"
#include "Graphics/VAOView.h"
#include "Graphics/UniformVarNames.h"
#include "Graphics/Texture.h"

#include "Input/EventSystemInterface.h"
#include "Graphics/Camera.h"
#include "Camera/Controller/TempFPSCameraController.h"

#include "Physics/Mechanics/Position.h"

#include <string>
#include <chrono>
#include <ratio>
#include <ctime>

#include "FactoryExamples.h"
#include "ViewSpawnerExamples.h"

#include "Mandelbrot.h"

#include "Debug/NeptuneDebug.h"

using namespace Neptune;


int main(int argc, char* argv[])
{
	//ViewSpawnerExamples::Display20Cubes();
	//FactoryExamples::Display100PLYModels();
	//ViewSpawnerExamples::Display100XWings();
	//ViewSpawnerExamples::XWing();
	ViewSpawnerExamples::MultiTexturedModelWithSimpleLightingExample();
	ViewSpawnerExamples::SceneExample1();
	ViewSpawnerExamples::SkyBox();
	Mandelbrot::MandelbrotExample();

	return 0;
}