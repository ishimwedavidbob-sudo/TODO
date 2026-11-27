#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include "book.h"
#include "patron.h"
#include "date.h"

using namespace std;

class Transaction
{
private:
    Book book;
    Patron patron;
    string activity;
    Date date;

public:
    Transaction();

    Transaction(Book b,
                Patron p,
                string act,
                Date d);

    Book getBook() const;
    Patron getPatron() const;
    string getActivity() const;
    Date getDate() const;
};

#endif