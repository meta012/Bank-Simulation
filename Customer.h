/* Customer.c header file */

#ifndef __customer__
#define __customer__

typedef struct
{
	int serviceTime;
} Customer;

void customerSetServiceTime (Customer *target, int duration);
int customerGetServiceTime (Customer *target);

#endif /* not _customer_ */
