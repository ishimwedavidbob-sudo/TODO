#include <iostream>

#include "book.h"
#include "patron.h"
#include "date.h"
#include "transaction.h"
#include "library.h"

using namespace std;

int main()
{
    Library lib;

    // Books
    Book b1(
        "ISBN001",
        "C++ Programming",
        "Bjarne Stroustrup",
        2013,
        Book::nonfiction);

    Book b2(
        "ISBN002",
        "Harry Potter",
        "J.K Rowling",
        2000,
        Book::fiction);

    // Patrons
    Patron p1(
        "David",
        "CARD001",
        0);

    Patron p2(
        "John",
        "CARD002",
        3000);

    // Add books
    lib.addBook(b1);
    lib.addBook(b2);

    // Add patrons
    lib.addPatron(p1);
    lib.addPatron(p2);

    // Date
    Date d1(
        6,
        11,
        2026);

    // Checkout
    lib.checkOutBook(
        "ISBN001",
        "CARD001",
        d1);

    // Show patrons owing fees
    vector<string> debtors =
        lib.getPatronsWithFees();

    cout << "\nPatrons Owing Fees\n";

    for(int i = 0; i < debtors.size(); i++)
    {
        cout << debtors[i] << endl;
    }

    return 0;
}