#include <iostream>
#include "library.h"

using namespace std;

// Add book
void Library::addBook(const Book& book)
{
    books.push_back(book);
}

// Add patron
void Library::addPatron(const Patron& patron)
{
    patrons.push_back(patron);
}

// Check out book
void Library::checkOutBook(
    string isbn,
    string cardNumber,
    Date date)
{
    int bookIndex = -1;
    int patronIndex = -1;

    // Find book
    for(int i = 0; i < books.size(); i++)
    {
        if(books[i].getISBN() == isbn)
        {
            bookIndex = i;
            break;
        }
    }

    // Find patron
    for(int i = 0; i < patrons.size(); i++)
    {
        if(patrons[i].getCardNumber() == cardNumber)
        {
            patronIndex = i;
            break;
        }
    }

    // Check if found
    if(bookIndex == -1)
    {
        cout << "Error: Book not found." << endl;
        return;
    }

    if(patronIndex == -1)
    {
        cout << "Error: Patron not found." << endl;
        return;
    }

    // Check fees
    if(patrons[patronIndex].owesFees())
    {
        cout << "Error: Patron owes library fees." << endl;
        return;
    }

    // Check if already checked out
    if(books[bookIndex].isCheckedOut())
    {
        cout << "Error: Book already checked out." << endl;
        return;
    }

    // Checkout book
    books[bookIndex].checkOut();

    // Create transaction
    Transaction t(
        books[bookIndex],
        patrons[patronIndex],
        "Check Out",
        date
    );

    transactions.push_back(t);

    cout << "Book checked out successfully." << endl;
}

// Patrons with fees
vector<string> Library::getPatronsWithFees() const
{
    vector<string> feePatrons;

    for(int i = 0; i < patrons.size(); i++)
    {
        if(patrons[i].owesFees())
        {
            feePatrons.push_back(
                patrons[i].getUserName()
            );
        }
    }

    return feePatrons;
}