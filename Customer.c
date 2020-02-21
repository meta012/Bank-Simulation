#include <stdio.h>
#include "Customer.h"

void customerSetServiceTime (Customer *target, int duration)
{
	target->serviceTime = duration;
}

int customerGetServiceTime (Customer *target)
{
	return target->serviceTime;
}
