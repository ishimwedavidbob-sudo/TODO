#include "transaction.h"

Transaction::Transaction()
{
}

Transaction::Transaction(Book b,
                         Patron p,
                         string act,
                         Date d)
{
    book = b;
    patron = p;
    activity = act;
    date = d;
}

Book Transaction::getBook() const
{
    return book;
}

Patron Transaction::getPatron() const
{
    return patron;
}

string Transaction::getActivity() const
{
    return activity;
}

Date Transaction::getDate() const
{
    return date;
}