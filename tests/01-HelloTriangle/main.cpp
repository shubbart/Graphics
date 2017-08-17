#include "graphics\Context.h"
#include <iostream>

int main()
{
	Context context;
	context.init();

	while (context.step())
	{
		//std::cout << context.getMouseButton(0) << std::endl;

	}

	context.term();
	return 0;
}