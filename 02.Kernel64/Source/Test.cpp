#include "Test.h"


void Hello(void)
{
	char* temp = (char *)0xB8000;

	*temp = '0';

	kTest1();
}

