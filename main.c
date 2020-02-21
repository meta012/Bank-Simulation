/* Problem: make a simulation and comparison of two banks. One with more cashiers, who work slowly
   and the other with less fast working cashiers. Probability of teller coming to bank in one minute
   is x. Simulation run time is y. Compare both banks - which strategy works best.
   Simulation must be executed using custom made queue and long int implementations.
   ------------------------------------------------------------------------------------------------
   Execution:
   * make two banks - slow(more tellers) and fast(less tellers)
   * compare work efficiency of both banks simultaneously
   * print results depending of the probability of customer walking into bank in one minute
   */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "QueueF.h"
#include "Customer.h"
#include "Teller.h"
#include "func.h"

#define SLOW_TELLERS 10        /* count of slow tellers in bank 1 */
#define FAST_TELLERS 5         /* count of fast tellers in bank 2 */
#define SLOW_SERVICE_TIME 20   /* max time for a slow teller to serve a client per min */
#define FAST_SERVICE_TIME 10   /* max time for a fast teller to serve a client per min */
#define BANK_RUN_TIME 70       /* overall simulation run time */

int main(void)
{
    float       likely_arrivals[] = {0.2, 0.5, 0.8};

    printf("Simulation runs %d minutes.\n", BANK_RUN_TIME);
    printf("Number 1 bank has %d tellers that serve clients %d minutes each.\n", SLOW_TELLERS, SLOW_SERVICE_TIME);
    printf("Number 2 bank has %d tellers that serve clients %d minutes each.\n", FAST_TELLERS, FAST_SERVICE_TIME);
    printf("Customer arrival probability is ");
    for (int l = 0; l < sizeof(likely_arrivals)/sizeof(likely_arrivals[0]); l++)
    {
        printf("%4.1f ", likely_arrivals[l]);
    }
    printf("\n---------------------------------------------------------------");
    for (int h = 0; h < sizeof(likely_arrivals)/sizeof(likely_arrivals[0]); h++)
    {
        float		random_arrivals = 0;
        Queue*     	slow_queue;
        Queue*     	fast_queue;
        int	        slow_number_of_customers = 0;
        int	        fast_number_of_customers = 0;
        int         i = 0, s = 0;

        int	        slow_overall_busy_time = 0;
        int	        fast_overall_busy_time = 0;
        int	        slow_overall_inactive_time = 0;
        int	        fast_overall_inactive_time = 0;

        int         slow_all_busy_tellers = 0;
        int         fast_all_busy_tellers = 0;

        Customer	slow_customer;
        Customer	fast_customer;
        Teller      slow_tellers[SLOW_TELLERS];
        Teller      fast_tellers[FAST_TELLERS];
        int		    slow_average_time = 0;
        int 		fast_average_time = 0;

        int         customer_in = 0;
        time_t		t;
//---------------------------------------
        char s1[1000];
        char s2[1000];
        char s3[1000];
        char s4[1000];
        char s5[1000];
        char s6[1000];
        char s7[1000];
        char s8[1000];
        char s9[1000];

        int clock_ = 0;
        int num_ = 1;
        int slow_overall_waiting_time_ = 0;
        int fast_overall_waiting_time_ = 0;
        int slow_max_waiting_time_ = 0;
        int fast_max_waiting_time_ = 0;

        sprintf(s1, "%d", BANK_RUN_TIME);
        sprintf(s2, "%d", clock_);
        sprintf(s3, "%d", num_);
        sprintf(s4, "%d", slow_overall_waiting_time_);
        sprintf(s5, "%d", fast_overall_waiting_time_);
        sprintf(s6, "%d", slow_max_waiting_time_);
        sprintf(s7, "%d", fast_max_waiting_time_);
        BigInt* bank_run_time = naujasSkaicius(s1);
        BigInt* clock = naujasSkaicius(s2);
        BigInt* num = naujasSkaicius(s3);
        BigInt* slow_overall_waiting_time = naujasSkaicius(s4);
        BigInt* fast_overall_waiting_time = naujasSkaicius(s5);
        BigInt* slow_max_waiting_time = naujasSkaicius(s6);
        BigInt* fast_max_waiting_time = naujasSkaicius(s7);

        slow_queue = createEmptyQueue(0);
        fast_queue = createEmptyQueue(0);

        srand(time(&t));


        for (i = 0; i < SLOW_TELLERS; i++)
        {
            createTeller(&slow_tellers[i]);
        }
        for (i = 0; i < FAST_TELLERS; i++)
        {
            createTeller(&fast_tellers[i]);
        }

//------------------------------------------------------------------------------------------------
        while ((palyginti(bank_run_time, clock) == 1 || palyginti(bank_run_time, clock) == 0))  /* run while banks are working */
        {
            random_arrivals = (float)rand()/(float)RAND_MAX;	/* random client arrival probability */

            if (random_arrivals < likely_arrivals[h] && palyginti(bank_run_time, clock) == 1) /* if our likely probability is greater than random */
            {
                customerSetServiceTime(&slow_customer, SLOW_SERVICE_TIME);
                customerSetServiceTime(&fast_customer, FAST_SERVICE_TIME);
                enqueue(slow_queue, s);                      /* put clients to queue by arrival time */
                enqueue(fast_queue, s);
                customer_in++;
            }
            for (i = 0; i < SLOW_TELLERS; i++)
            {
                if (slow_tellers[i].remaining_time == 0)  /* if teller is free */
                {
                    if (!checkEmpty(slow_queue))         /* if queue is not empty */
                    {
                        sprintf(s8, "%d", getFirst(slow_queue));
                        BigInt* slow_entry_time = naujasSkaicius(s8);
                        slow_overall_waiting_time = atimti((sudeti(slow_overall_waiting_time, clock)), slow_entry_time);
                        if (palyginti(atimti(clock, slow_entry_time), slow_max_waiting_time) == 1)
                        {
                            slow_max_waiting_time = atimti(clock, slow_entry_time);
                        }
                        dequeue(slow_queue);
                        tellerNewCustomer(&slow_tellers[i]);
                        setTellerRemainingTime(&slow_tellers[i], customerGetServiceTime(&slow_customer));
                        tellerBusy(&slow_tellers[i]);
                        if (customerGetServiceTime(&slow_customer) > 1)
                        {
                            notFreeTeller(&slow_tellers[i]);
                        }
                    }
                    else
                    {
                        tellerInactive(&slow_tellers[i]);
                    }
                }
                else
                {
                    slow_tellers[i].remaining_time--;
                    tellerBusy(&slow_tellers[i]);
                    if (slow_tellers[i].remaining_time == 0)
                    {
                        freeTeller(&slow_tellers[i]);
                    }
                }
            }
            for (i = 0; i < FAST_TELLERS; i++)  /* repeat, but with fast teller parameters */
            {
                if (fast_tellers[i].remaining_time == 0)
                {
                    if (!checkEmpty(fast_queue))
                    {
                        sprintf(s9, "%d", getFirst(fast_queue));
                        BigInt* fast_entry_time = naujasSkaicius(s9);
                        fast_overall_waiting_time = atimti((sudeti(fast_overall_waiting_time, clock)), fast_entry_time);
                        if (palyginti(atimti(clock, fast_entry_time), fast_max_waiting_time) == 1)
                        {
                            fast_max_waiting_time = atimti(clock, fast_entry_time);
                        }
                        dequeue(fast_queue);
                        tellerNewCustomer(&fast_tellers[i]);
                        setTellerRemainingTime(&fast_tellers[i], customerGetServiceTime(&fast_customer));
                        tellerBusy(&fast_tellers[i]);
                        if (customerGetServiceTime(&fast_customer) > 1)
                        {
                            notFreeTeller(&fast_tellers[i]);
                        }
                    }
                    else
                    {
                        tellerInactive(&fast_tellers[i]);
                    }
                }
                else
                {
                    fast_tellers[i].remaining_time--;
                    tellerBusy(&fast_tellers[i]);
                    if (fast_tellers[i].remaining_time == 0)
                    {
                        freeTeller(&fast_tellers[i]);
                    }
                }
            }
            clock = sudeti(clock, num);
            s++;
        }
        printf("\nWith %4.1f arrival probability came %d customers.\n", likely_arrivals[h], customer_in);
//---------------------------------------------------------------------------------------------------------------------
        printf("\n--Bank number 1 (slow).\n");
        for(i = 0; i < SLOW_TELLERS; i++)       /* how many customers are overall served */
        {
            slow_number_of_customers += getTellerCustomersNumber(&slow_tellers[i]);
        }
        if (slow_number_of_customers == 0 || yraNulis(slow_overall_waiting_time) == 1)
        {
            slow_average_time = 0.0;
            printf("The average wait time for a customer was %d min.\n",slow_average_time);
        }
        else
        {
            char s10[1000];
            sprintf(s10, "%d", slow_number_of_customers);
            BigInt* slow_num_of_cust = naujasSkaicius(s10);
            BigInt* slow_av_time = padalinti(slow_overall_waiting_time, slow_num_of_cust);
            char* slow_av_print = toString(slow_av_time);
            printf("The average wait time for a customer was %s min.\n", slow_av_print);
        }
        printf("Max waiting time for a served customer was %s min.\n", toString(slow_max_waiting_time));
        for (i = 0; i < SLOW_TELLERS; i++)
        {
            if (isTellerBusy(&slow_tellers[i]))
            {
                slow_all_busy_tellers++;
            }
        }
        printf("%d customers were served.\n", slow_number_of_customers-slow_all_busy_tellers);
        printf("%d customers were being served during closing time.\n", slow_all_busy_tellers);
        printf("%d customers were left standing in line.\n", getQueueLength(slow_queue));
        for (i = 0; i < SLOW_TELLERS; i++)
        {
            slow_overall_busy_time += + slow_tellers[i].busy_time;
            slow_overall_inactive_time += slow_tellers[i].inactive_time;
        }
        int slow_time = slow_overall_busy_time + slow_overall_inactive_time;
        printf("Tellers worked an average %4.2f %% of all bank simulation time\n", ((float)slow_overall_busy_time)/((float)slow_time)*100);

//------------------------------------------------------------------------------------------------------------------------
        printf("\n--Bank number 2 (fast).\n");
        for(i = 0; i < FAST_TELLERS; i++)
        {
            fast_number_of_customers += getTellerCustomersNumber(&fast_tellers[i]);
        }
        if (fast_number_of_customers == 0 || yraNulis(fast_overall_waiting_time) == 1)
        {
            fast_average_time = 0.0;
            printf("The average wait time for a customer was %d min.\n", fast_average_time);
        }
        else
        {
            char s11[1000];
            sprintf(s11, "%d", fast_number_of_customers);
            BigInt* fast_num_of_cust = naujasSkaicius(s11);
            BigInt* fast_av_time = padalinti(fast_overall_waiting_time, fast_num_of_cust);
            char* fast_av_print = toString(fast_av_time);
            printf("The average wait time for a customer was %s min.\n", fast_av_print);
        }
        printf("Max waiting time for a served customer was %s min.\n", toString(fast_max_waiting_time));
        for (i = 0; i < FAST_TELLERS; i++)
        {
            if (isTellerBusy(&fast_tellers[i]))
            {
                fast_all_busy_tellers++;
            }
        }
        printf("%d customers were served.\n", fast_number_of_customers-fast_all_busy_tellers);
        printf("%d customers were being served during closing time.\n", fast_all_busy_tellers);
        printf("%d customers were left standing in line.\n", getQueueLength(fast_queue));
        for (i = 0; i < FAST_TELLERS; i++)
        {
            fast_overall_busy_time += + fast_tellers[i].busy_time;
            fast_overall_inactive_time += fast_tellers[i].inactive_time;
        }
        int fast_time = fast_overall_busy_time + fast_overall_inactive_time;
        printf("Tellers worked an average %4.2f %% of all bank simulation time\n", ((float)fast_overall_busy_time)/((float)fast_time)*100);
        printf("---------------------------------------------------------------\n");
    }

	return 0;
}
