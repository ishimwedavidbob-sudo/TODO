#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>

#include "book.h"
#include "patron.h"
#include "transaction.h"

using namespace std;

class Library
{
private:
    vector<Book> books;
    vector<Patron> patrons;
    vector<Transaction> transactions;

public:
    void addBook(const Book& book);

    void addPatron(const Patron& patron);

    void checkOutBook(
        string isbn,
        string cardNumber,
        Date date);

    vector<string> getPatronsWithFees() const;
};

#endif