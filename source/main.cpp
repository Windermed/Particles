#include "Engine.h"

int main()
{
	// Gets a new instance of the engine directly and runs it.
	Engine::Init();
	Engine::GetEngine()->Run();
	//Engine::Shutdown();

	// Quit in the usual way when the engine is stopped
	return 0;
}