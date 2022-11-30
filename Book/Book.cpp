#include <iostream>
#include <string>
#include <ostream>
#include <vector>
#include <algorithm>

#pragma region Book

class ISBN {


public:

    ISBN(std::string s) {
        if (isValidISBNNumber(s))
        {
            ISBN_number = s;
        }
        else
        {
            std::cout << "not a valid ISBN";

        }
    }
    ISBN() {}

    std::string get_ISBN_number() const { return ISBN_number; }


private:

    std::string ISBN_number = "0-0-0-x";

    static bool isValidISBNNumber(const std::string& ISBN_number);


};

bool ISBN::isValidISBNNumber(const std::string& ISBN_number) {

    int part{ 0 };

    for (char const& ch : ISBN_number) {
        if (ch == '-')
        {
            ++part;
        }
        else if (part > 2)
        {
            if (std::isalpha(ch) == 0) return false;
        }
        else
        {
            if (std::isdigit(ch) == 0) return false;
        }
    }

    return true;
}


class Title {


public:

    Title(const std::string& s) : BookTitle{ s } {};
    Title() {};
    const std::string& get_BookTitle()const { return BookTitle; };

private:

    std::string BookTitle = "Keloglan Masallari";

};

class Author {
public:
    Author(const std::string& s) : BookAuthor{ s } {};
    Author() {};
    const std::string& get_BookAuthor()const { return BookAuthor; };

private:

    std::string BookAuthor = "Alper Gunes";

};

class CopyrightDate {


};

enum class Genre {

    FICTION,
    NONFICTION,
    PERIODICAL,
    BIOGRAPHY,
    CHILDREN,

    DEFAULT = 0
};



class Book {


public:

    Book(std::string ISBN_number, std::string title, std::string author, Genre book_genre) : MyISBN{ ISBN_number }, mytitle{ title }
        , myauthor{ author }, myGenre{ book_genre } {}

    Book() {}


    const ISBN& get_ISBN() const { return MyISBN; }
    const Title& get_Title() const { return mytitle; }
    const Author& get_Author() const { return myauthor; }
    const CopyrightDate& get_CopyrightDate() const { return myCopyrightDate; }

    bool get_CheckedOut() { return isCheckedOut; }

    void CheckOutBook() { isCheckedOut = !isCheckedOut; }


private:

    ISBN MyISBN;
    Title mytitle;
    Author myauthor;
    CopyrightDate myCopyrightDate;
    Genre myGenre = Genre::DEFAULT;

    bool isCheckedOut = false;
};


//Operator overloadings for type Book

bool operator==(const Book& BookA, const Book& BookB) {

    if (BookA.get_ISBN().get_ISBN_number() == BookB.get_ISBN().get_ISBN_number())
    {
        return true;
    }

    return false;
}
bool operator!=(const Book& BookA, const Book& BookB) {

    if (BookA.get_ISBN().get_ISBN_number() != BookB.get_ISBN().get_ISBN_number())
    {
        return true;
    }

    return false;
}
std::ostream& operator<<(std::ostream& os, const Book& myBook) {

    os << "Title: " << myBook.get_Title().get_BookTitle() << std::endl << "Author: " << myBook.get_Author().get_BookAuthor() <<
        std::endl << "ISBN: " << myBook.get_ISBN().get_ISBN_number();

    return os;
}


#pragma endregion

#pragma region Patron

class Patron {

public:

    Patron(std::string username, long long int LibraryCardNumber, double LibraryFee) : UserName{ username }, LibraryCardNumber{ LibraryCardNumber }, LibraryFees{ LibraryFee } {}
    Patron() {};
    Patron(std::string username, long long int LibraryCardNumber) : UserName{ username }, LibraryCardNumber{LibraryCardNumber} {}

    const std::string& get_username() const { return UserName; }
    long long int get_LibraryCardNumber()const { return LibraryCardNumber; }
    double get_LibraryFees()const { return LibraryFees; }

    const std::vector<Book>& get_Books() const { return Books; }

    void set_LibraryFee(double LibraryFee) { LibraryFees = LibraryFee; }
    void set_PatronBooks(const Book& newBook) { Books.push_back(newBook); }


private:
    std::string UserName;
    long long int LibraryCardNumber{ 99 };
    double LibraryFees{ 0.0 };

    std::vector<Book> Books;


};

bool operator==(const Patron& p1, const Patron& p2) {

    if (p1.get_LibraryCardNumber() == p2.get_LibraryCardNumber())
    {
        return true;
    }
    return false;
}


bool DoesOwnMoney(const Patron& patron) {

    if (patron.get_LibraryFees() < 0.0)
    {
        return true;
    }

    return false;
}

#pragma endregion

#pragma region Library

class Library {

    struct Transaction;

public:

    std::vector<Transaction> Transactions;

    void AddBook(const Book& NewBook);
    void AddPatron(const Patron& NewPatron);
    void CheckOutBook(Book& BookToCheckOut, Patron& Patron);

    static void UpdatePatronList();

    struct Transaction
    {
        Book Lib_Book;
        Patron Patron;

    };

    void print_debtors() const;

private:

    std::vector<Book> Books;
    std::vector<Patron> Patrons;
    
    
    Patron find_bookOwner(const Book& Book);

    bool CheckBook(const Book& Book);
    bool CheckUser(const Patron& Patron);

};

void Library::UpdatePatronList() {
    

}

void Library::AddBook(const Book& NewBook) {

    Books.push_back(NewBook);
}

void Library::AddPatron(const Patron& NewPatron) {
   
    Patrons.push_back(NewPatron);
}

void Library::CheckOutBook(Book& BookToCheckOut, Patron& Patron) {
   
    if (CheckBook(BookToCheckOut) && CheckUser(Patron))
    {
        if (DoesOwnMoney(Patron)) { 
            /*if (find_bookOwner(BookToCheckOut) == Patron)
            {
                std::cout << "Thanks for returning the book you brokie!" << std::endl;
            }*/

            std::cout << Patron.get_username() << ": User owns money to the library! Therefore can not get the book!" << std::endl;
            return;
        }
        else {
        
            std::string BookTitle = BookToCheckOut.get_Title().get_BookTitle();

            if (!BookToCheckOut.get_CheckedOut())
            {
                BookToCheckOut.CheckOutBook();
                std::cout << Patron.get_username() << " has the book: " << BookTitle << std::endl;
                Patron.set_PatronBooks(BookToCheckOut);

                Transaction newTransaction;
                newTransaction.Lib_Book = BookToCheckOut;
                newTransaction.Patron = Patron;
                Transactions.push_back(newTransaction);
            }
            else if (find_bookOwner(BookToCheckOut) == Patron)
            {
                BookToCheckOut.CheckOutBook();
                std::cout << BookTitle << " is returned by: " << Patron.get_username() << std::endl;
            }
            else
            {

                std::cout << BookTitle << " is already checked out by: " << find_bookOwner(BookToCheckOut).get_username() << ". Sorry " << Patron.get_username() << std::endl;
            }
        }
        
    }
    else
    {
        std::cout << "Wrong book or wrong user name!" << std::endl;
    }  
    
}

bool Library::CheckBook(const Book& Book) {

    for (auto findBook: Books)
    {
        if (findBook == Book)
        {
            return true;
        }
    }

    return false;
}

bool Library::CheckUser(const Patron& Patron) {

    for (size_t i = 0; i < Patrons.size(); i++)
    {
        if (Patrons[i].get_LibraryCardNumber() == Patron.get_LibraryCardNumber()) {
            return true;
        }
    }
    return false;
}

Patron Library::find_bookOwner(const Book& Book) {
    for (auto t: Transactions)
    {
        if (Book == t.Lib_Book)
        {
            return t.Patron;
        }
    }
}

void Library::print_debtors() const {

    std::cout << "Patrons who owns money to the library: " << std::endl;

    for (auto Patron: Patrons)
    {
        std::cout << Patron.get_username() << ": " << Patron.get_LibraryFees() << std::endl;
    }

}

#pragma endregion


int main()
{
   
    Library Library;

    Book Booka = Book("111-3233-444-nnn","Dede Korkut Masallari","Dede Korkut",Genre::FICTION);
    Book Bookb = Book("111-3233-445-nnn", "Keloglan Masallari", "Kel Dede Korkut", Genre::BIOGRAPHY);

    Patron Alper{"Alper",12559761124,-5.0};
    Patron Oguz{"Oguz", 12595061125};
    Patron Bilal{ "Bilal",  12590661126 };

    Library.AddBook(Booka);
    Library.AddBook(Bookb);

    Library.AddPatron(Alper);
    Library.AddPatron(Oguz);
    Library.AddPatron(Bilal);

    Library.CheckOutBook(Booka, Alper);
    Alper.set_LibraryFee(5.0);          //FIX: This function won't update this object in the Library's Patron list. Make it update!
    Library.CheckOutBook(Bookb, Alper);
    Library.CheckOutBook(Bookb, Bilal);
    Library.CheckOutBook(Booka, Alper);
    Library.CheckOutBook(Booka, Oguz);


    std::cout << std::endl << "Alper's books: " << std::endl;
    for (auto book: Alper.get_Books())
    {
        std::cout << book.get_Title().get_BookTitle() << std::endl;
    }

    Library.print_debtors();
}



