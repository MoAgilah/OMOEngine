////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////

/////////////////
// MY INCLUDES //
/////////////////
#include <memory>
#include <assert.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "../Headers/Framework.h"

int main()
{
	std::unique_ptr<Framework> p_System = std::make_unique<Framework>();
	assert(p_System != nullptr);

	// Initialize and run the system object.
	p_System->Run();

	return 0;
}