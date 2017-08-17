#include "graphics\Context.h"
#include <iostream>

int main()
{
	Context context;
	context.init();

	while (context.step())
	{
		std::cout << context.getKey(65) << std::endl;
	}

	context.term();
	return 0;
}