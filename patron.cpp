#include "patron.h"

// Default constructor
Patron::Patron()
{
    userName = "";
    cardNumber = "";
    owedFees = 0;
}

// Parameterized constructor
Patron::Patron(string name, string card, int fees)
{
    userName = name;
    cardNumber = card;
    owedFees = fees;
}

// Getters
string Patron::getUserName() const
{
    return userName;
}

string Patron::getCardNumber() const
{
    return cardNumber;
}

int Patron::getOwedFees() const
{
    return owedFees;
}

// Check fees
bool Patron::owesFees() const
{
    return owedFees > 0;
}

// Setter
void Patron::setFees(int fees)
{
    owedFees = fees;
}