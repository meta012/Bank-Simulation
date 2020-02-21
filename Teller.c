#include "teller.h"

void createTeller (Teller *teller)
{
	teller->number_of_customers = 0;
	teller->busy_time = 0;
	teller->inactive_time = 0;
	teller->remaining_time = 0;
	teller->present = 1;
}

void setTellerRemainingTime (Teller *teller, int duration)
{
	teller->remaining_time = duration;
}

void tellerNewCustomer (Teller *teller)
{

	teller->number_of_customers++;
}

int getTellerCustomersNumber (Teller *teller)
{
	return teller->number_of_customers;
}

void tellerInactive (Teller *teller)
{
	teller->inactive_time++;
}

void tellerBusy (Teller *teller)
{
	teller->busy_time++;
}

void notFreeTeller (Teller *teller)
{
    teller->present = 1;
}
void freeTeller (Teller *teller)
{
	teller->present = 0;
}

int isTellerBusy (Teller *teller)
{
	return teller->present;
}
