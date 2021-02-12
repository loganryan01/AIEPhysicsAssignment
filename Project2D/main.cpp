#include "PhysicsApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsApp();

	// initialise and loop
	app->run("AIE", 675, 900, false);

	// deallocation
	delete app;

	return 0;
}