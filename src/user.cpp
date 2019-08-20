#include "user.h"

bank::bank()
{
	name = "user";
	balance = 0;
}

int bank::getBalance()
{
	return balance;
}

void bank::deposit(int i)
{
	balance = balance + i;
}
