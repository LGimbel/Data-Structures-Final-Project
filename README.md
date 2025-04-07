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
