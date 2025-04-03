#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include<format>

// to use an enum either use its cardinality or class name scope res choesn term e.g. Genre::FICTION
class Book {
    //you can only overload the stream once with the same params so this is a print out for a customer and i have an admin print function for all of the info
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

public:
    std::string title;
    std::string author;
    Genre genre;
    double wholesalePrice;
    double retailPrice;
    float memberDiscount;
    std::string ISBN; //should be either 13 or 10 digits
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
            case Genre::FICTION:
                effectiveGenre = "Fiction";
                break;
            case Genre::NON_FICTION:
                effectiveGenre = "Non-Fiction";
                break;
            case Genre::MYSTERY:
                effectiveGenre = "Mystery";
                break;
            case Genre::SCIENCE_FICTION:
                effectiveGenre = "Science Fiction";
                break;
            case Genre::BIOGRAPHY:
                effectiveGenre = "Biography";
                break;
            case Genre::ROMANCE:
                effectiveGenre = "Romance";
                break;
            case Genre::TECH:
                effectiveGenre = "Tech";
                break;
            case Genre::HISTORY:
                effectiveGenre = "History";
                break;
            case Genre::HORROR:
                effectiveGenre = "Horror";
                break;
            case Genre::FANTASY:
                effectiveGenre = "Fantasy";
                break;
        }
    }
};

class Analytics {
    std::vector<Book> stock;
    
};

class FinancialData {
public:
    double Revenue;
    double Profits;
    long int TotalStock;

    // Constructor to initialize values
    FinancialData() : Revenue(0.0), Profits(0.0), TotalStock(0) {}

    // Method to update stock
    void UpdateStock(int newStock) {
        TotalStock = newStock;
    }

    // Method to display financial data
    void displayFinancials() const {
        std::cout << "Revenue: $" << Revenue << "\n"
                  << "Profits: $" << Profits << "\n"
                  << "Total Stock: " << TotalStock << std::endl;
    }
};


void customerMenu(Analytics &analytics) {
    bool exit = false;
    bool isMember = false;
    int choice = 0;
    while (!exit) {
        std::string menu = std::format(
            "Your are currently logged in as a {}\nPlease enter the number of your chosen action\n {}1.browse\n2.purchase book\n3.exit to previous menu",
            isMember ? "Member":"Guest"  , isMember ? "":"0.Login with member ID\n");
        std::cout << menu << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 0:
                //login with member ID
                isMember = true;
                break;
            case 1:
                //browse books
                break;
            case 2:
                //purchase book
                break;
            case 3:
                exit = true;
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }
    }
}

void adminMenu(Analytics &analytics, FinancialData &financialData) {
    bool exit = false;
    int choice = 0;
    while (!exit) {
        std::string menu =
            "Please enter the number of your chosen action\n"
            "1. Add book\n"
            "2. Remove book\n"
            "3. Update book\n"
            "4. View all books\n"
            "5. Help customer with return\n"
            "6. Save current stock and analytics to file / Display financials\n"
            "7. Load stock and analytics from file\n"
            "8. Exit to previous menu";
        std::cout << menu << std::endl;
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Add book logic
                break;
            case 2:
                // Remove book logic
                break;
            case 3:
                // Update book logic
                break;
            case 4:
                // View all books logic
                break;
            case 5:
                // Return logic
                break;
            case 6:
                // Display financials
                std::cout << "\n--- Financial Report ---\n";
                financialData.displayFinancials();

                // Update stock count based on analytics inventory
                std::cout << "Updating stock based on inventory..." << std::endl;
                financialData.calculateStock(analytics);
                std::cout << "Stock updated successfully!\n";
                break;
            case 7:
                // Load from file logic
                break;
            case 8:
                exit = true;
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }
    }
}
void topLevelMenu(Analytics &analytics) {
    bool exit = false;
    int choice = 0;
    std::string menu = "Welcome to the Bookstore are you a\n1. Customer\n2. Admin\n3. Exit";
    while (!exit) {
        std::cout << menu << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 1:
                customerMenu(analytics);
                break;
            case 2:
                adminMenu(analytics);
                break;
            case 3:
                exit = true;
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }
    }
    std::cout << "Thank you for using the bookstore system!" << std::endl;
}

int main() {
    Analytics analytics;
    Book book("The Great Gatsby", "F. Scott Fitzgerald", Book::Genre::FICTION, 10.99, 15.99, 0.1, "9780743273565", 100, 180);
    topLevelMenu(analytics);
    return 0;
}
