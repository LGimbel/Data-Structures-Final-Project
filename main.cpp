#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <random>

class Book {
    friend std::ostream &operator<<(std::ostream &os, const Book &obj) {
        return os
               << "title: " << obj.title << "\n"
               << " author: " << obj.author << "\n"
               << " genre: " << obj.effectiveGenre << "\n"
               << " retailPrice: " << obj.retailPrice << "\n"
               << " memberDiscount: " << (obj.memberDiscount * 100) << "%" << "\n"
               << " ISBN: " << obj.ISBN << "\n"
               << " stock: " << obj.stock << "\n"
               << " pages: " << obj.pages << std::endl;
    }

public:
    enum class Genre {
        FICTION,
        NON_FICTION,
        MYSTERY,
        SCIENCE_FICTION,
        BIOGRAPHY,
        ROMANCE,
        TECH,
        HISTORY,
        HORROR,
        FANTASY
    };

    std::string title;
    std::string author;
    Genre genre;
    double wholesalePrice;
    double retailPrice;
    float memberDiscount;
    std::string ISBN;
    unsigned short stock;
    int pages;

    void printAdmin() const {
        std::cout << "Title = " << title << "\n"
                << "Author = " << author << "\n"
                << "Genre = " << effectiveGenre << "\n"
                << "Wholesale Price = " << wholesalePrice << "\n"
                << "Retail Price = " << retailPrice << "\n"
                << "Member Discount = " << memberDiscount * 100 << "%\n"
                << "ISBN = " << ISBN << "\n"
                << "Stock = " << stock << "\n"
                << "Pages = " << pages << std::endl;
    }

private:
    std::string effectiveGenre;

public:
    Book(std::string title, std::string author, Genre genre, double wholesalePrice, double retailPrice,
         float memberDiscount, std::string ISBN, unsigned short stock, int pages)
        : title(std::move(title)), author(std::move(author)), genre(genre), wholesalePrice(wholesalePrice),
          retailPrice(retailPrice), memberDiscount(memberDiscount), ISBN(std::move(ISBN)),
          stock(stock), pages(pages) {
        switch (genre) {
            case Genre::FICTION: effectiveGenre = "Fiction";
                break;
            case Genre::NON_FICTION: effectiveGenre = "Non-Fiction";
                break;
            case Genre::MYSTERY: effectiveGenre = "Mystery";
                break;
            case Genre::SCIENCE_FICTION: effectiveGenre = "Science Fiction";
                break;
            case Genre::BIOGRAPHY: effectiveGenre = "Biography";
                break;
            case Genre::ROMANCE: effectiveGenre = "Romance";
                break;
            case Genre::TECH: effectiveGenre = "Tech";
                break;
            case Genre::HISTORY: effectiveGenre = "History";
                break;
            case Genre::HORROR: effectiveGenre = "Horror";
                break;
            case Genre::FANTASY: effectiveGenre = "Fantasy";
                break;
        }
    }
};

class FinancialData {
public:
    std::vector<Book> stock;
    double Revenue;
    double Profits;
    long int TotalStock;

    FinancialData() : Revenue(0.0), Profits(0.0), TotalStock(0) {
    }

    void UpdateStock(int newStock) {
        TotalStock = newStock;
    }

    void displayFinancials() const {
        std::cout << "Revenue: $" << Revenue << "\n"
                << "Profits: $" << Profits << "\n"
                << "Total Stock: " << TotalStock << std::endl;
    }
};

void populateRandomBooks(FinancialData &financialData, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> priceDist(5.0, 100.0);
    std::uniform_int_distribution<> stockDist(1, 100);
    std::uniform_int_distribution<> pagesDist(100, 1000);
    std::uniform_int_distribution<> genreDist(0, 9);

    for (int i = 0; i < count; ++i) {
        std::string title = "RandomTitle_" + std::to_string(i + 1);
        std::string author = "Author_" + std::to_string(i + 1);
        Book::Genre genre = static_cast<Book::Genre>(genreDist(gen));
        double wholesalePrice = priceDist(gen);
        double retailPrice = wholesalePrice * 1.2;
        float memberDiscount = 0.1f;
        std::string ISBN = "978000000" + std::to_string(i + 1);
        unsigned short stock = stockDist(gen);
        int pages = pagesDist(gen);
        //use emplace back to allow for move semantics and embed the constructor
        financialData.stock.emplace_back(title, author, genre, wholesalePrice, retailPrice, memberDiscount, ISBN, stock, pages);

        std::cout << "Added: " << title << " by " << author << "\n";
    }

    financialData.UpdateStock(financialData.stock.size());
}

void customerMenu(FinancialData &financialData) {
    bool exit = false;
    bool isMember = false;
    int choice = 0;
    while (!exit) {
        std::string menu = isMember ? "Your are currently logged in as a member\nPlease enter the number of your chosen action\n 1.browse\n2.purchase book\n3.exit to previous menu" :
        "Your are currently logged in as a guest\nPlease enter the number of your chosen action\n0.Login in as member 1.browse\n2.purchase book\n3.exit to previous menu";
        std::cout << menu << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 0:
                isMember = true;
                break;
            case 1:
                /* browse */
                break;
            case 2:
                /* purchase */
                break;
            case 3:
                exit = true;
                break;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
}

void adminMenu(FinancialData &financialData) {
    bool exit = false;
    int choice = 0;
    while (!exit) {
        std::string menu =
                "Admin Menu:\n"
                "1. Add book\n"
                "2. Remove book\n"
                "3. Update book\n"
                "4. View all books\n"
                "5. Help customer with return\n"
                "6. Save to file\n"
                "7. Load from file\n"
                "8. Populate random books\n"
                "9. Exit\n";
        std::cout << menu << "\nPlease enter an option:";
        std::cin >> choice;
        switch (choice) {
            case 1:
                /* add book */
                break;
            case 2:
                /* remove book */
                break;
            case 3:
                /* update book */
                break;
            case 4:
                for (const auto &book: financialData.stock) {
                    book.printAdmin();
                    std::cout << "----------------------\n";
                }
                break;
            case 5:
                /* return logic */
                break;
            case 6:
                /* save */
                break;
            case 7:
                /* load */
                break;
            case 8: {
                int count;
                std::cout << "Enter number of random books to generate: ";
                std::cin >> count;
                populateRandomBooks(financialData, count);
                break;
            }
            case 9:
                exit = true;
                break;

            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
}

void topLevelMenu(FinancialData &financialData) {
    bool exit = false;
    int choice = 0;
    while (!exit) {
        std::cout << "Welcome to the Bookstore!\n1. Customer\n2. Admin\n3. Exit\nEnter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                customerMenu(financialData);
                break;
            case 2:
                adminMenu(financialData);
                break;
            case 3:
                exit = true;
                break;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
    std::cout << "Thank you for using the bookstore system!" << std::endl;
}

int main() {
    FinancialData financialData;
    topLevelMenu(financialData);
    return 0;
}
