#include "LinkedList.h"


void checkDiv(int a, int b, int s[])
{

	int result = (a / b);

	if ((result * b) != b)
	{
		int lost = a - (result * b);

		for (int i = 0; i < lost; i++)
			s[i] = 1;
	}
}
