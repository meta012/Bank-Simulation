/* Teller.c header file */

#ifndef __teller__
#define __teller__

typedef struct
{
    int number_of_customers;
	int busy_time;
	int inactive_time;
	int remaining_time;
	int present;
} Teller;

void createTeller (Teller *teller);
void setTellerRemainingTime (Teller *teller, int duration);
void tellerNewCustomer (Teller *teller);
int getTellerCustomersNumber (Teller *teller);
void tellerInactive (Teller *teller);
void tellerBusy (Teller *teller);
void notFreeTeller (Teller *teller);
void freeTeller (Teller *teller);
int isTellerBusy (Teller *teller);


#endif /* not _teller_ */
