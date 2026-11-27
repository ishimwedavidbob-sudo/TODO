#ifndef PATRON_H
#define PATRON_H

#include <string>

using namespace std;

class Patron
{
private:
    string userName;
    string cardNumber;
    int owedFees;

public:
    // Constructors
    Patron();
    Patron(string name, string card, int fees);

    // Getters
    string getUserName() const;
    string getCardNumber() const;
    int getOwedFees() const;

    // Returns true if patron owes fees
    bool owesFees() const;

    // Setter
    void setFees(int fees);
};

#endif