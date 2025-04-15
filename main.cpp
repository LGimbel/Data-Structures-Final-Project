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
        financialData.stock.emplace_back(title, author, genre, wholesalePrice, retailPrice, memberDiscount, ISBN, stock,
                                         pages);

        std::cout << "Added: " << title << " by " << author << "\n";
    }

    financialData.UpdateStock(financialData.stock.size());
}

#pragma region filtering
std::vector<Book> FilterByTitle(const std::vector<Book> &stock, const std::string &title) {
    std::vector<Book> foundBooks;
    for (const auto &book: stock) {
        if (book.title.find(title) != std::string::npos) {
            foundBooks.emplace_back(book);
        }
    }
    return foundBooks;
}

std::vector<Book> FilterByAuthor(const std::vector<Book> &stock, const std::string &author) {
    std::vector<Book> foundBooks;
    for (const auto &book: stock) {
        if (book.author.find(author) != std::string::npos) {
            foundBooks.emplace_back(book);
        }
    }
    return foundBooks;
}

std::vector<Book> filterBooksByGenre(const std::vector<Book> &stock, const int genreFilter) {
    std::vector<Book> filteredBooks;
    for (const auto &book: stock) {
        if (static_cast<int>(book.genre) == genreFilter) {
            filteredBooks.emplace_back(book);
        }
    }
    return filteredBooks;
}

std::vector<Book> filterBooksByPrice(const std::vector<Book> &stock, const double priceFloor,
                                     const double priceCeiling) {
    std::vector<Book> filteredBooks;
    for (const auto &book: stock) {
        if (book.retailPrice >= priceFloor && book.retailPrice <= priceCeiling) {
            filteredBooks.emplace_back(book);
        }
    }
    return filteredBooks;
}

std::vector<Book> filterBooksByPages(const std::vector<Book> &stock, const int pageFloor, const int pageCeiling) {
    std::vector<Book> filteredBooks;
    for (const auto &book: stock) {
        if (book.pages >= pageFloor && book.pages <= pageCeiling) {
            filteredBooks.emplace_back(book);
        }
    }
    return filteredBooks;
}
#pragma region sorting
//using quicksort for sorting
#pragma region titleSort
int partitionTitle(std::vector<Book> &books, int low, int high, const bool reverseFlag) {
    std::string pivot = books[high].title;
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if ((reverseFlag && books[j].title >= pivot) || (!reverseFlag && books[j].title <= pivot)) {
            i++;
            std::swap(books[i], books[j]);
        }
    }
    std::swap(books[i + 1], books[high]);
    return (i + 1);
}

void quickSortTitle(std::vector<Book> &arr, int low, int high, const bool reverseFlag) {
    if (low < high) {
        int partitionIndex = partitionTitle(arr, low, high, reverseFlag);

        quickSortTitle(arr, low, partitionIndex - 1, reverseFlag);
        quickSortTitle(arr, partitionIndex + 1, high, reverseFlag);
    }
}
#pragma endregion
#pragma region authorSort
int partitionAuthor(std::vector<Book> &books, int low, int high, const bool reverseFlag) {
    std::string pivot = books[high].author;
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        if ((reverseFlag && books[j].author >= pivot) || (!reverseFlag && books[j].author <= pivot)) {
            i++;
            std::swap(books[i], books[j]);
        }
    }
    std::swap(books[i + 1], books[high]);
    return (i + 1);
}

void quickSortAuthor(std::vector<Book> &arr, int low, int high, const bool reverseFlag) {
    if (low < high) {
        int partitionIndex = partitionAuthor(arr, low, high, reverseFlag);

        quickSortAuthor(arr, low, partitionIndex - 1, reverseFlag);
        quickSortAuthor(arr, partitionIndex + 1, high, reverseFlag);
    }
}

#pragma endregion
#pragma region genreSort
int partitionGenre(std::vector<Book> &books, int low, int high, const bool reverseFlag) {
    int pivot = static_cast<int>(books[high].genre);
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        int value = static_cast<int>(books[j].genre);
        if ((reverseFlag && value >= pivot) || (!reverseFlag && value <= pivot)) {
            i++;
            std::swap(books[i], books[j]);
        }
    }
    std::swap(books[i + 1], books[high]);
    return (i + 1);
}

void quickSortGenre(std::vector<Book> &arr, int low, int high, const bool reverseFlag) {
    if (low < high) {
        int partitionIndex = partitionGenre(arr, low, high, reverseFlag);

        quickSortGenre(arr, low, partitionIndex - 1, reverseFlag);
        quickSortGenre(arr, partitionIndex + 1, high, reverseFlag);
    }
}
#pragma endregion
#pragma region retailPriceSort
//price qs
int partitionPrice(std::vector<Book> &books, int low, int high, const bool reverseFlag) {
    double pivot = books[high].retailPrice;
    int i = (low - 1);

    for (int j = low; j < high; j++) {
        double value = books[j].retailPrice;
        if ((reverseFlag && value >= pivot) || (!reverseFlag && value <= pivot)) {
            i++;
            std::swap(books[i], books[j]);
        }
    }
    std::swap(books[i + 1], books[high]);
    return (i + 1);
}

void quickSortPrice(std::vector<Book> &arr, int low, int high, const bool reverseFlag) {
    if (low < high) {
        int partitionIndex = partitionPrice(arr, low, high, reverseFlag);

        quickSortPrice(arr, low, partitionIndex - 1, reverseFlag);
        quickSortPrice(arr, partitionIndex + 1, high, reverseFlag);
    }
}
#pragma endregion
#pragma endregion
std::vector<Book> sortDistributor(std::vector<Book> &foundBooks, const int sortCriterion, bool reverseFlag) {
    switch (sortCriterion) {
        case 1: {
            quickSortTitle(foundBooks, 0, foundBooks.size() - 1, reverseFlag);
            return foundBooks;
        }
        case 2: {
            // sort by author
            quickSortAuthor(foundBooks, 0, foundBooks.size() - 1, reverseFlag);
        }
        case 3: {
            // sort by genre
            quickSortGenre(foundBooks, 0, foundBooks.size() - 1, reverseFlag);
        }
        case 4: {
            // sort by retail price
            quickSortPrice(foundBooks, 0, foundBooks.size() - 1, reverseFlag);
        }
        default:
            std::cout << "Invalid sorting criterion. Defaulting to sort by title.\n";
            quickSortTitle(foundBooks, 0, foundBooks.size() - 1, reverseFlag);
            return foundBooks;
            break;
    }
}
void printResults (const std::vector<Book> &foundBooks) {
    for (const auto &book: foundBooks) {
        std::cout << book << "\n";
    }
}
#pragma endregion
void browseMenu(std::vector<Book> &stock) {
    bool exit = false;
    int choice = 0;
    int sortingCriterion = 1;
    int chosenGenereFilter = 0;
    double priceFilterFloor = 0.0;
    double priceFilterCeiling = 100.0;
    bool priceFiltering = false;
    bool genreFiltering = false;
    bool pagesFiltering = false;
    int pagesFilterFloor = 0;
    int pagesFilterCeiling = 1000;
    bool reverse = false;
    std::string menu =
            "Browsing Menu\n1. View all books sorted and filtered\n2. Search by title\n3. Search by author\n4.set filters\n5.Change sorting criterion\n6.Clear filters and sorts\n7.View Current Filters\n8.Exit\n";
    while (!exit) {
        std::cout << menu << "\nPlease enter an option:";
        std::cin >> choice;
        switch (choice) {
            // does not filter by user input only filters
            case 1: {
                std::vector<Book> foundBooks;
                if (priceFiltering) {
                    foundBooks = filterBooksByPrice(stock, priceFilterFloor, priceFilterCeiling);
                } else {
                    foundBooks = stock;
                }
                if (genreFiltering) {
                    foundBooks = filterBooksByGenre(foundBooks, chosenGenereFilter);
                }
                if (pagesFiltering) {
                    foundBooks = filterBooksByPages(foundBooks, pagesFilterFloor, pagesFilterCeiling);
                }
                printResults(sortDistributor(foundBooks, sortingCriterion, reverse));
                break;
            }
            // filters by user title Is case-sensitive
            case 2: {
                std::cout << "Please enter the title you wish to search for or a substring of that title:\n";
                if (priceFiltering || pagesFiltering || genreFiltering) {
                    std::cout << "Note: Filters are currently applied so search results may be limited\n";
                }
                std::string title;
                std::cin.ignore();
                std::getline(std::cin, title);
                std::vector<Book> foundBooks;
                if (priceFiltering) {
                    foundBooks = filterBooksByPrice(stock, priceFilterFloor, priceFilterCeiling);
                } else {
                    foundBooks = stock;
                }
                if (genreFiltering) {
                    foundBooks = filterBooksByGenre(foundBooks, chosenGenereFilter);
                }
                if (pagesFiltering) {
                    foundBooks = filterBooksByPages(foundBooks, pagesFilterFloor, pagesFilterCeiling);
                }
                std::vector<Book> searchResults = FilterByTitle(foundBooks, title);
                printResults(sortDistributor(foundBooks, sortingCriterion, reverse));
                break;
            }
            // filters by user input author is case-sensitive
            case 3: {
                std::cout << "Please enter the author you wish to search for or a substring of their name:\n";
                if (priceFiltering || pagesFiltering || genreFiltering) {
                    std::cout << "Note: Filters are currently applied so search results may be limited\n";
                }
                std::string author;
                std::cin.ignore();
                std::getline(std::cin, author);
                std::vector<Book> foundBooks;
                if (priceFiltering) {
                    foundBooks = filterBooksByPrice(stock, priceFilterFloor, priceFilterCeiling);
                } else {
                    foundBooks = stock;
                }
                if (genreFiltering) {
                    foundBooks = filterBooksByGenre(foundBooks, chosenGenereFilter);
                }
                if (pagesFiltering) {
                    foundBooks = filterBooksByPages(foundBooks, pagesFilterFloor, pagesFilterCeiling);
                }
                std::vector<Book> searchResults = FilterByAuthor(foundBooks, author);
                printResults(sortDistributor(foundBooks, sortingCriterion, reverse));
                break;
            }
            case 4: {
                std::cout << "what Filter do you want to set\n1.Price\n2.Genre\n3.Pages\n";
                std::cin >> choice;
                switch (choice) {
                    case 1:
                        std::cout << "Price filtering enabled; to turn off please reset current filters\n";
                        priceFiltering = true;
                        std::cout << "Please enter the floor of the price range you wish to filter by\n";
                        std::cin >> priceFilterFloor;
                        std::cout << "Please enter the ceiling of the price range you wish to filter by\n";
                        std::cin >> priceFilterCeiling;
                        break;
                    case 2: {
                        std::cout << "Genre filtering enabled; to turn off please reset current filters\n";
                        genreFiltering = true;
                        std::cout << "Please enter the number corresponding to the genre you want to filter by"
                                "\n1.Fiction\n2.Non-Fiction\n3.Mystery\n4.Science Fiction\n5.Biography\n6.Romance\n7.Tech\n8.History\n9.Horror\n10.Fantasy\n";
                        std::cin >> chosenGenereFilter;
                        chosenGenereFilter -= 1;
                        if (chosenGenereFilter < 1 || chosenGenereFilter > 10) {
                            std::cout << "Invalid choice. Defaulting to Fiction\n";
                            chosenGenereFilter = 0;
                            break;
                        }
                        break;
                    }
                    case 3:
                        std::cout << "Pages filtering enabled; to turn off please reset current filters\n";
                        pagesFiltering = true;
                        std::cout << "Please enter the floor of the pages range you wish to filter by\n";
                        std::cin >> pagesFilterFloor;
                        std::cout << "Please enter the ceiling of the pages range you wish to filter by\n";
                        std::cin >> pagesFilterCeiling;
                        break;
                    default:
                        std::cout << "Invalid choice. Defaulting to no filter\n";
                        priceFiltering = false;
                        genreFiltering = false;
                        pagesFiltering = false;
                }
                break;
            }
            case 5: {
                std::cout <<
                        "Enter the number corresponding to your desired sorting criterion\n1. Title\n2. Author\n3. Genre\n4. Retail Price\n";
                std::cin >> sortingCriterion;
                if (sortingCriterion < 1 || sortingCriterion > 4) {
                    std::cout << "Invalid choice. Defaulting to sort by Title A-Z\n";
                    sortingCriterion = 1;
                    break;
                }
                std::cout << "Do you want to reverse the order? (1 for yes, 0 for no): ";
                std::cin >> reverse;
                break;
            }
            case 6: {
                chosenGenereFilter = 0;
                sortingCriterion = 0;
                std::cout << "Filters and sorts cleared.\n";
                break;
            }
            case 7: {
                std::string filters = "Current Filters:\n";
                if (genreFiltering) {
                    switch (chosenGenereFilter + 1) {
                        case 1: filters += "Genre: Fiction\n";
                            break;
                        case 2: filters += "Genre: Non-Fiction\n";
                            break;
                        case 3: filters += "Genre: Mystery\n";
                            break;
                        case 4: filters += "Genre: Science Fiction\n";
                            break;
                        case 5: filters += "Genre: Biography\n";
                            break;
                        case 6: filters += "Genre: Romance\n";
                            break;
                        case 7: filters += "Genre: Tech\n";
                            break;
                        case 8: filters += "Genre: History\n";
                            break;
                        case 9: filters += "Genre: Horror\n";
                            break;
                        case 10: filters += "Genre: Fantasy\n";
                            break;
                        default: filters += "unexpected Error\n";
                            break;
                    }
                } else {
                    filters += "No genre filter\n";
                }
                if (priceFiltering) {
                    filters += "Price Filter: $" + std::to_string(priceFilterFloor) + " - $" + std::to_string(
                        priceFilterCeiling) + "\n";
                } else {
                    filters += "No price filter\n";
                }
                if (pagesFiltering) {
                    filters += "Pages Filter: " + std::to_string(pagesFilterFloor) + " - " + std::to_string(
                        pagesFilterCeiling) + "\n";
                } else {
                    filters += "No pages filter\n";
                }
                std::cout << filters;
                break;
            }
            case 8:
                exit = true;
                return;
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }
}

void customerMenu(FinancialData &financialData) {
    bool exit = false;
    bool isMember = false;
    int choice = 0;
    while (!exit) {
        std::string menu = isMember
                               ? "Your are currently logged in as a member\nPlease enter the number of your chosen action\n 1.browse\n2.purchase book\n3.exit to previous menu"
                               : "Your are currently logged in as a guest\nPlease enter the number of your chosen action\n0.Login in as member\n1.browse\n2.purchase book\n3.exit to previous menu";
        std::cout << menu << std::endl;
        std::cin >> choice;
        switch (choice) {
            case 0:
                isMember = true;
                break;
            case 1:
                browseMenu(financialData.stock);
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

// Add, Remove, and Edit Functions
void addBook(FinancialData &financialData) {
    std::string title, author, ISBN;
    double wholesalePrice, retailPrice;
    float memberDiscount;
    unsigned short stock;
    int pages, genreInt;

    std::cout << "Enter book details:\n";
    std::cin.ignore();
    std::cout << "Title: "; std::getline(std::cin, title);
    std::cout << "Author: "; std::getline(std::cin, author);
    std::cout << "Genre (0-Fiction, 1-Non-Fiction, etc.): "; std::cin >> genreInt;
    std::cout << "Wholesale Price: "; std::cin >> wholesalePrice;
    std::cout << "Retail Price: "; std::cin >> retailPrice;
    std::cout << "Member Discount: "; std::cin >> memberDiscount;
    std::cout << "ISBN: "; std::cin >> ISBN;
    std::cout << "Stock: "; std::cin >> stock;
    std::cout << "Pages: "; std::cin >> pages;

    Book::Genre genre = static_cast<Book::Genre>(genreInt);
    financialData.stock.emplace_back(title, author, genre, wholesalePrice, retailPrice, memberDiscount, ISBN, stock, pages);
    financialData.UpdateStock(financialData.stock.size());
    std::cout << "Book added successfully!\n";
}

void removeBook(FinancialData &financialData) {
    std::string ISBN;
    std::cout << "Enter ISBN of the book to remove: ";
    std::cin >> ISBN;

    auto it = std::remove_if(financialData.stock.begin(), financialData.stock.end(),
                             [&ISBN](const Book &book) { return book.ISBN == ISBN; });

    if (it != financialData.stock.end()) {
        financialData.stock.erase(it, financialData.stock.end());
        financialData.UpdateStock(financialData.stock.size());
        std::cout << "Book removed successfully!\n";
    } else {
        std::cout << "Book not found.\n";
    }
}

void editBook(FinancialData &financialData) {
    std::string ISBN;
    std::cout << "Enter ISBN of the book to edit: ";
    std::cin >> ISBN;

    for (auto &book : financialData.stock) {
        if (book.ISBN == ISBN) {
            std::cout << "Editing book: " << book.title << " by " << book.author << "\n";

            std::cout << "New Title (leave empty for no change): ";
            std::cin.ignore();
            std::string newTitle; std::getline(std::cin, newTitle);
            if (!newTitle.empty()) book.title = newTitle;

            std::cout << "New Author (leave empty for no change): ";
            std::string newAuthor; std::getline(std::cin, newAuthor);
            if (!newAuthor.empty()) book.author = newAuthor;

            int newGenreInt;
            std::cout << "New Genre (0-Fiction, 1-Non-Fiction, etc., -1 for no change): ";
            std::cin >> newGenreInt;
            if (newGenreInt != -1) book.genre = static_cast<Book::Genre>(newGenreInt);

            std::cout << "New Wholesale Price (-1 for no change): ";
            double newWholesalePrice; std::cin >> newWholesalePrice;
            if (newWholesalePrice != -1) book.wholesalePrice = newWholesalePrice;

            std::cout << "New Retail Price (-1 for no change): ";
            double newRetailPrice; std::cin >> newRetailPrice;
            if (newRetailPrice != -1) book.retailPrice = newRetailPrice;

            std::cout << "New Member Discount (-1 for no change): ";
            float newMemberDiscount; std::cin >> newMemberDiscount;
            if (newMemberDiscount != -1) book.memberDiscount = newMemberDiscount;

            std::cout << "New Stock (-1 for no change): ";
            unsigned short newStock; std::cin >> newStock;
            if (newStock != -1) book.stock = newStock;

            std::cout << "New Pages (-1 for no change): ";
            int newPages; std::cin >> newPages;
            if (newPages != -1) book.pages = newPages;

            std::cout << "Book updated successfully!\n";
            return;
        }
    }
    std::cout << "Book not found.\n";
}

// function to save the current database to a file
void saveToFile(const FinancialData &financialData) {
    std::ofstream outFile("bookstore_data.txt");
    if (!outFile) {
        std::cerr << "Error: Could not open file for saving.\n";
        return;
    }

    // save financial data
    outFile << std::fixed << std::setprecision(2); // ensure consistent formatting for floating-point numbers
    outFile << financialData.Revenue << "\n";
    outFile << financialData.Profits << "\n";
    outFile << financialData.TotalStock << "\n";

    // save book data
    for (const auto &book : financialData.stock) {
        outFile << book.title << "\n"
                << book.author << "\n"
                << static_cast<int>(book.genre) << "\n"
                << book.wholesalePrice << "\n"
                << book.retailPrice << "\n"
                << book.memberDiscount << "\n"
                << book.ISBN << "\n"
                << book.stock << "\n"
                << book.pages << "\n";
    }

    outFile.close();
    std::cout << "Data successfully saved to bookstore_data.txt\n";
}

// function to load the database from a file
void loadFromFile(FinancialData &financialData) {
    std::ifstream inFile("bookstore_data.txt");
    if (!inFile) {
        std::cerr << "Error: Could not open file for loading.\n";
        return;
    }

    // clear current stock
    financialData.stock.clear();

    // load financial data
    inFile >> financialData.Revenue;
    inFile >> financialData.Profits;
    inFile >> financialData.TotalStock;
    inFile.ignore(); // ignore the newline character after TotalStock

    // load book data
    while (true) {
        std::string title, author, ISBN;
        int genreInt;
        double wholesalePrice, retailPrice;
        float memberDiscount;
        unsigned short stock;
        int pages;

        if (!std::getline(inFile, title)) break; // break only on EOF or input failure
        if (title.empty()) {
            std::cerr << "Warning: Encountered a book with an empty title. Skipping this record.\n";
            // Read and discard the remaining fields for this record
            std::getline(inFile, author);
            inFile >> genreInt >> wholesalePrice >> retailPrice >> memberDiscount;
            inFile.ignore(); // ignore the newline character after memberDiscount
            std::getline(inFile, ISBN);
            inFile >> stock >> pages;
            inFile.ignore(); // ignore the newline character after pages
            continue; // skip this record and move to the next
        }
        if (!std::getline(inFile, author)) break;
        if (!(inFile >> genreInt)) break;
        if (!(inFile >> wholesalePrice)) break;
        if (!(inFile >> retailPrice)) break;
        if (!(inFile >> memberDiscount)) break;
        inFile.ignore(); // ignore the newline character after memberDiscount
        if (!std::getline(inFile, ISBN)) break;
        if (!(inFile >> stock)) break;
        if (!(inFile >> pages)) break;
        inFile.ignore(); // ignore the newline character after pages

        Book::Genre genre = static_cast<Book::Genre>(genreInt);
        financialData.stock.emplace_back(title, author, genre, wholesalePrice, retailPrice, memberDiscount, ISBN, stock, pages);
    }

    inFile.close();
    std::cout << "Data successfully loaded from bookstore_data.txt\n";
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
                addBook(financialData); // add book
                break;
            case 2:
                removeBook(financialData); // remove book
                break;
            case 3:
                editBook(financialData); // edit book
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
                saveToFile(financialData); // save to file
                break;
            case 7:
                loadFromFile(financialData); // load from file
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
