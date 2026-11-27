#include "Book.h"

// Default constructor
Book::Book()
{
    ISBN = "";
    title = "";
    author = "";
    copyrightDate = 0;
    checkedOut = false;
    genre = fiction;
}

// Parameterized constructor
Book::Book(string isbn,
           string title,
           string author,
           int copyrightDate,
           Genre genre)
{
    this->ISBN = isbn;
    this->title = title;
    this->author = author;
    this->copyrightDate = copyrightDate;
    this->genre = genre;
    checkedOut = false;
}

// Getters
string Book::getISBN() const
{
    return ISBN;
}

string Book::getTitle() const
{
    return title;
}

string Book::getAuthor() const
{
    return author;
}

int Book::getCopyrightDate() const
{
    return copyrightDate;
}

bool Book::isCheckedOut() const
{
    return checkedOut;
}

Book::Genre Book::getGenre() const
{
    return genre;
}

// Check out
void Book::checkOut()
{
    checkedOut = true;
}

// Check in
void Book::checkIn()
{
    checkedOut = false;
}

// ==
bool Book::operator==(const Book& other) const
{
    return ISBN == other.ISBN;
}

// !=
bool Book::operator!=(const Book& other) const
{
    return !(*this == other);
}

// << operator
ostream& operator<<(ostream& os, const Book& book)
{
    os << "Title: " << book.title << endl;
    os << "Author: " << book.author << endl;
    os << "ISBN: " << book.ISBN << endl;

    os << "Genre: ";

    switch(book.genre)
    {
        case Book::fiction:
            os << "Fiction";
            break;

        case Book::nonfiction:
            os << "Nonfiction";
            break;

        case Book::periodical:
            os << "Periodical";
            break;

        case Book::biography:
            os << "Biography";
            break;

        case Book::children:
            os << "Children";
            break;
    }

    os << endl;

    return os;
}
#include "book.h"

