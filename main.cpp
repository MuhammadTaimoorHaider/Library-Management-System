#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <map>

using namespace std;

// ANSI color codes for attractive interface
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";
const string BOLD = "\033[1m";

// Book class to store book information
class Book {
public:
    int bookId;
    string title;
    string author;
    string category;
    bool isIssued;
    string issuedTo;
    string issueDate;
    string returnDate;

    Book(int id, string t, string a, string c) {
        bookId = id;
        title = t;
        author = a;
        category = c;
        isIssued = false;
        issuedTo = "";
        issueDate = "";
        returnDate = "";
    }

    Book() {
        bookId = 0;
        title = "";
        author = "";
        category = "";
        isIssued = false;
        issuedTo = "";
        issueDate = "";
        returnDate = "";
    }
};

// Student class to store student information
class Student {
public:
    int studentId;
    string name;
    string email;
    string course;
    vector<int> issuedBooks;

    Student(int id, string n, string e, string c) {
        studentId = id;
        name = n;
        email = e;
        course = c;
    }

    Student() {
        studentId = 0;
        name = "";
        email = "";
        course = "";
    }
};

// Library Management System class
class LibrarySystem {
private:
    vector<Book> books;
    vector<Student> students;
    const string BOOKS_FILE = "books.txt";
    const string STUDENTS_FILE = "students.txt";

public:
    LibrarySystem() {
        loadData();
    }

    ~LibrarySystem() {
        saveData();
    }

    // Utility functions
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void printHeader(string title) {
        cout << CYAN << BOLD;
        cout << "\n" << string(60, '=') << "\n";
        cout << "           " << title << "\n";
        cout << string(60, '=') << "\n" << RESET;
    }

    void printLine() {
        cout << BLUE << string(60, '-') << RESET << "\n";
    }

    string getCurrentDate() {
        time_t now = time(0);
        struct tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }

    // File handling functions
    void saveData() {
        // Save books
        ofstream bookFile(BOOKS_FILE);
        for (const auto& book : books) {
            bookFile << book.bookId << "|" << book.title << "|" << book.author << "|"
                    << book.category << "|" << book.isIssued << "|" << book.issuedTo << "|"
                    << book.issueDate << "|" << book.returnDate << "\n";
        }
        bookFile.close();

        // Save students
        ofstream studentFile(STUDENTS_FILE);
        for (const auto& student : students) {
            studentFile << student.studentId << "|" << student.name << "|"
                       << student.email << "|" << student.course << "|";
            for (int bookId : student.issuedBooks) {
                studentFile << bookId << ",";
            }
            studentFile << "\n";
        }
        studentFile.close();
    }

    void loadData() {
        // Load books
        ifstream bookFile(BOOKS_FILE);
        if (bookFile.is_open()) {
            string line;
            while (getline(bookFile, line)) {
                if (line.empty()) continue;

                Book book;
                size_t pos = 0;
                vector<string> tokens;

                while ((pos = line.find('|')) != string::npos) {
                    tokens.push_back(line.substr(0, pos));
                    line.erase(0, pos + 1);
                }
                tokens.push_back(line);

                if (tokens.size() >= 4) {
                    book.bookId = stoi(tokens[0]);
                    book.title = tokens[1];
                    book.author = tokens[2];
                    book.category = tokens[3];
                    if (tokens.size() > 4) {
                        book.isIssued = (tokens[4] == "1");
                        if (tokens.size() > 5) book.issuedTo = tokens[5];
                        if (tokens.size() > 6) book.issueDate = tokens[6];
                        if (tokens.size() > 7) book.returnDate = tokens[7];
                    }
                    books.push_back(book);
                }
            }
            bookFile.close();
        }

        // Load students
        ifstream studentFile(STUDENTS_FILE);
        if (studentFile.is_open()) {
            string line;
            while (getline(studentFile, line)) {
                if (line.empty()) continue;

                Student student;
                size_t pos = 0;
                vector<string> tokens;

                while ((pos = line.find('|')) != string::npos) {
                    tokens.push_back(line.substr(0, pos));
                    line.erase(0, pos + 1);
                }
                tokens.push_back(line);

                if (tokens.size() >= 4) {
                    student.studentId = stoi(tokens[0]);
                    student.name = tokens[1];
                    student.email = tokens[2];
                    student.course = tokens[3];

                    if (tokens.size() > 4 && !tokens[4].empty()) {
                        string bookIds = tokens[4];
                        size_t commaPos = 0;
                        while ((commaPos = bookIds.find(',')) != string::npos) {
                            string bookId = bookIds.substr(0, commaPos);
                            if (!bookId.empty()) {
                                student.issuedBooks.push_back(stoi(bookId));
                            }
                            bookIds.erase(0, commaPos + 1);
                        }
                    }
                    students.push_back(student);
                }
            }
            studentFile.close();
        }
    }

    // Main menu functions
    void showMainMenu() {
        clearScreen();
        printHeader("LIBRARY MANAGEMENT SYSTEM");
        cout << GREEN << "\n\t️  Welcome to Digital Library \n" << RESET;
        printLine();
        cout << YELLOW << "\n\t1.  Librarian Portal (Admin)\n";
        cout << "\t2.  Student Portal\n";
        cout << "\t3.  Exit\n" << RESET;
        printLine();
        cout << WHITE << "Enter your choice: " << RESET;
    }

    // Librarian (Admin) functions
    void librarianLogin() {
        clearScreen();
        printHeader("LIBRARIAN LOGIN");

        string username, password;
        cout << WHITE << "Username: " << RESET;
        cin >> username;
        cout << WHITE << "Password: " << RESET;
        cin >> password;

        // Simple authentication (admin/admin)
        if (username == "admin" && password == "admin") {
            cout << GREEN << "\n Login Successful!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            librarianMenu();
        } else {
            cout << RED << "\n Invalid credentials!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void librarianMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("LIBRARIAN DASHBOARD");
            cout << GREEN << "\n\t Admin Control Panel\n" << RESET;
            printLine();
            cout << YELLOW << "\n\t1.  Book Management\n";
            cout << "\t2.  Student Management\n";
            cout << "\t3.  Issue/Return Books\n";
            cout << "\t4.  Reports\n";
            cout << "\t5.  Back to Main Menu\n" << RESET;
            printLine();
            cout << WHITE << "Enter your choice: " << RESET;
            cin >> choice;

            switch (choice) {
                case 1: bookManagementMenu(); break;
                case 2: studentManagementMenu(); break;
                case 3: issueReturnMenu(); break;
                case 4: reportsMenu(); break;
                case 5: break;
                default:
                    cout << RED << "Invalid choice!" << RESET;
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 5);
    }

    void bookManagementMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("BOOK MANAGEMENT");
            printLine();
            cout << YELLOW << "\n\t1. ➕ Add New Book\n";
            cout << "\t2.  View All Books\n";
            cout << "\t3.  Search Book\n";
            cout << "\t4.  Update Book\n";
            cout << "\t5.  Delete Book\n";
            cout << "\t6.  Back\n" << RESET;
            printLine();
            cout << WHITE << "Enter your choice: " << RESET;
            cin >> choice;

            switch (choice) {
                case 1: addBook(); break;
                case 2: viewAllBooks(); break;
                case 3: searchBook(); break;
                case 4: updateBook(); break;
                case 5: deleteBook(); break;
                case 6: break;
                default:
                    cout << RED << "Invalid choice!" << RESET;
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 6);
    }

    void addBook() {
        clearScreen();
        printHeader("ADD NEW BOOK");

        Book newBook;
        cout << WHITE << "Enter Book ID: " << RESET;
        cin >> newBook.bookId;

        // Check if book ID already exists
        for (const auto& book : books) {
            if (book.bookId == newBook.bookId) {
                cout << RED << " Book ID already exists!" << RESET;
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
        }

        cin.ignore();
        cout << WHITE << "Enter Title: " << RESET;
        getline(cin, newBook.title);
        cout << WHITE << "Enter Author: " << RESET;
        getline(cin, newBook.author);
        cout << WHITE << "Enter Category: " << RESET;
        getline(cin, newBook.category);

        books.push_back(newBook);
        cout << GREEN << "\n Book added successfully!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.get();
    }

    void viewAllBooks() {
        clearScreen();
        printHeader("ALL BOOKS");

        if (books.empty()) {
            cout << RED << "\n No books available!\n" << RESET;
        } else {
            cout << left << setw(8) << "ID" << setw(25) << "Title" << setw(20) << "Author"
                 << setw(15) << "Category" << setw(10) << "Status" << "\n";
            printLine();

            for (const auto& book : books) {
                cout << left << setw(8) << book.bookId << setw(25) << book.title.substr(0, 24)
                     << setw(20) << book.author.substr(0, 19) << setw(15) << book.category.substr(0, 14);

                if (book.isIssued) {
                    cout << RED << "Issued" << RESET;
                } else {
                    cout << GREEN << "Available" << RESET;
                }
                cout << "\n";
            }
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void searchBook() {
        clearScreen();
        printHeader("SEARCH BOOK");

        cout << YELLOW << "\n\t1. Search by ID\n";
        cout << "\t2. Search by Title\n";
        cout << "\t3. Search by Author\n" << RESET;

        int choice;
        cout << WHITE << "Enter choice: " << RESET;
        cin >> choice;
        cin.ignore();

        string searchTerm;
        vector<Book> results;

        switch (choice) {
            case 1: {
                cout << WHITE << "Enter Book ID: " << RESET;
                int id;
                cin >> id;
                for (const auto& book : books) {
                    if (book.bookId == id) {
                        results.push_back(book);
                    }
                }
                break;
            }
            case 2: {
                cout << WHITE << "Enter Title: " << RESET;
                getline(cin, searchTerm);
                for (const auto& book : books) {
                    if (book.title.find(searchTerm) != string::npos) {
                        results.push_back(book);
                    }
                }
                break;
            }
            case 3: {
                cout << WHITE << "Enter Author: " << RESET;
                getline(cin, searchTerm);
                for (const auto& book : books) {
                    if (book.author.find(searchTerm) != string::npos) {
                        results.push_back(book);
                    }
                }
                break;
            }
            default:
                cout << RED << "Invalid choice!" << RESET;
                cout << "\nPress Enter to continue...";
                cin.get();
                return;
        }

        if (results.empty()) {
            cout << RED << "\n No books found!\n" << RESET;
        } else {
            cout << "\n" << GREEN << "Search Results:" << RESET << "\n";
            printLine();
            for (const auto& book : results) {
                cout << "ID: " << book.bookId << " | Title: " << book.title
                     << " | Author: " << book.author << " | Status: ";
                if (book.isIssued) {
                    cout << RED << "Issued" << RESET;
                } else {
                    cout << GREEN << "Available" << RESET;
                }
                cout << "\n";
            }
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void updateBook() {
        clearScreen();
        printHeader("UPDATE BOOK");

        int id;
        cout << WHITE << "Enter Book ID to update: " << RESET;
        cin >> id;

        for (auto& book : books) {
            if (book.bookId == id) {
                cin.ignore();
                cout << WHITE << "Current Title: " << RESET << book.title << "\n";
                cout << WHITE << "Enter new Title (or press Enter to keep current): " << RESET;
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) book.title = newTitle;

                cout << WHITE << "Current Author: " << RESET << book.author << "\n";
                cout << WHITE << "Enter new Author (or press Enter to keep current): " << RESET;
                string newAuthor;
                getline(cin, newAuthor);
                if (!newAuthor.empty()) book.author = newAuthor;

                cout << WHITE << "Current Category: " << RESET << book.category << "\n";
                cout << WHITE << "Enter new Category (or press Enter to keep current): " << RESET;
                string newCategory;
                getline(cin, newCategory);
                if (!newCategory.empty()) book.category = newCategory;

                cout << GREEN << "\n Book updated successfully!\n" << RESET;
                cout << "Press Enter to continue...";
                cin.get();
                return;
            }
        }

        cout << RED << "\n Book not found!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void deleteBook() {
        clearScreen();
        printHeader("DELETE BOOK");

        int id;
        cout << WHITE << "Enter Book ID to delete: " << RESET;
        cin >> id;

        for (auto it = books.begin(); it != books.end(); ++it) {
            if (it->bookId == id) {
                if (it->isIssued) {
                    cout << RED << "\n Cannot delete! Book is currently issued.\n" << RESET;
                } else {
                    books.erase(it);
                    cout << GREEN << "\n Book deleted successfully!\n" << RESET;
                }
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
        }

        cout << RED << "\n Book not found!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void studentManagementMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("STUDENT MANAGEMENT");
            printLine();
            cout << YELLOW << "\n\t1. ➕ Add New Student\n";
            cout << "\t2.  View All Students\n";
            cout << "\t3.  Search Student\n";
            cout << "\t4.  Update Student\n";
            cout << "\t5.  Delete Student\n";
            cout << "\t6.  Back\n" << RESET;
            printLine();
            cout << WHITE << "Enter your choice: " << RESET;
            cin >> choice;

            switch (choice) {
                case 1: addStudent(); break;
                case 2: viewAllStudents(); break;
                case 3: searchStudent(); break;
                case 4: updateStudent(); break;
                case 5: deleteStudent(); break;
                case 6: break;
                default:
                    cout << RED << "Invalid choice!" << RESET;
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 6);
    }

    void addStudent() {
        clearScreen();
        printHeader("ADD NEW STUDENT");

        Student newStudent;
        cout << WHITE << "Enter Student ID: " << RESET;
        cin >> newStudent.studentId;

        // Check if student ID already exists
        for (const auto& student : students) {
            if (student.studentId == newStudent.studentId) {
                cout << RED << " Student ID already exists!" << RESET;
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
        }

        cin.ignore();
        cout << WHITE << "Enter Name: " << RESET;
        getline(cin, newStudent.name);
        cout << WHITE << "Enter Email: " << RESET;
        getline(cin, newStudent.email);
        cout << WHITE << "Enter Course: " << RESET;
        getline(cin, newStudent.course);

        students.push_back(newStudent);
        cout << GREEN << "\n Student added successfully!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.get();
    }

    void viewAllStudents() {
        clearScreen();
        printHeader("ALL STUDENTS");

        if (students.empty()) {
            cout << RED << "\n No students registered!\n" << RESET;
        } else {
            cout << left << setw(8) << "ID" << setw(25) << "Name" << setw(30) << "Email"
                 << setw(15) << "Course" << setw(10) << "Books" << "\n";
            printLine();

            for (const auto& student : students) {
                cout << left << setw(8) << student.studentId << setw(25) << student.name.substr(0, 24)
                     << setw(30) << student.email.substr(0, 29) << setw(15) << student.course.substr(0, 14)
                     << setw(10) << student.issuedBooks.size() << "\n";
            }
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void searchStudent() {
        clearScreen();
        printHeader("SEARCH STUDENT");

        int id;
        cout << WHITE << "Enter Student ID: " << RESET;
        cin >> id;

        for (const auto& student : students) {
            if (student.studentId == id) {
                cout << "\n" << GREEN << "Student Found:" << RESET << "\n";
                printLine();
                cout << "ID: " << student.studentId << "\n";
                cout << "Name: " << student.name << "\n";
                cout << "Email: " << student.email << "\n";
                cout << "Course: " << student.course << "\n";
                cout << "Books Issued: " << student.issuedBooks.size() << "\n";

                if (!student.issuedBooks.empty()) {
                    cout << "Issued Book IDs: ";
                    for (int bookId : student.issuedBooks) {
                        cout << bookId << " ";
                    }
                    cout << "\n";
                }

                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
        }

        cout << RED << "\n Student not found!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void updateStudent() {
        clearScreen();
        printHeader("UPDATE STUDENT");

        int id;
        cout << WHITE << "Enter Student ID to update: " << RESET;
        cin >> id;

        for (auto& student : students) {
            if (student.studentId == id) {
                cin.ignore();
                cout << WHITE << "Current Name: " << RESET << student.name << "\n";
                cout << WHITE << "Enter new Name (or press Enter to keep current): " << RESET;
                string newName;
                getline(cin, newName);
                if (!newName.empty()) student.name = newName;

                cout << WHITE << "Current Email: " << RESET << student.email << "\n";
                cout << WHITE << "Enter new Email (or press Enter to keep current): " << RESET;
                string newEmail;
                getline(cin, newEmail);
                if (!newEmail.empty()) student.email = newEmail;

                cout << WHITE << "Current Course: " << RESET << student.course << "\n";
                cout << WHITE << "Enter new Course (or press Enter to keep current): " << RESET;
                string newCourse;
                getline(cin, newCourse);
                if (!newCourse.empty()) student.course = newCourse;

                cout << GREEN << "\n Student updated successfully!\n" << RESET;
                cout << "Press Enter to continue...";
                cin.get();
                return;
            }
        }

        cout << RED << "\n Student not found!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void deleteStudent() {
        clearScreen();
        printHeader("DELETE STUDENT");

        int id;
        cout << WHITE << "Enter Student ID to delete: " << RESET;
        cin >> id;

        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->studentId == id) {
                if (!it->issuedBooks.empty()) {
                    cout << RED << "\n Cannot delete! Student has issued books.\n" << RESET;
                } else {
                    students.erase(it);
                    cout << GREEN << "\n Student deleted successfully!\n" << RESET;
                }
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
        }

        cout << RED << "\n Student not found!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void issueReturnMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("ISSUE/RETURN BOOKS");
            printLine();
            cout << YELLOW << "\n\t1.  Issue Book\n";
            cout << "\t2.  Return Book\n";
            cout << "\t3.  View Issued Books\n";
            cout << "\t4.  Back\n" << RESET;
            printLine();
            cout << WHITE << "Enter your choice: " << RESET;
            cin >> choice;

            switch (choice) {
                case 1: issueBook(); break;
                case 2: returnBook(); break;
                case 3: viewIssuedBooks(); break;
                case 4: break;
                default:
                    cout << RED << "Invalid choice!" << RESET;
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 4);
    }

    void issueBook() {
        clearScreen();
        printHeader("ISSUE BOOK");

        int bookId, studentId;
        cout << WHITE << "Enter Book ID: " << RESET;
        cin >> bookId;
        cout << WHITE << "Enter Student ID: " << RESET;
        cin >> studentId;

        // Find book
        Book* book = nullptr;
        for (auto& b : books) {
            if (b.bookId == bookId) {
                book = &b;
                break;
            }
        }

        if (!book) {
            cout << RED << "\n Book not found!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        if (book->isIssued) {
            cout << RED << "\n Book is already issued!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        // Find student
        Student* student = nullptr;
        for (auto& s : students) {
            if (s.studentId == studentId) {
                student = &s;
                break;
            }
        }

        if (!student) {
            cout << RED << "\n Student not found!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        // Check if student has reached maximum books limit (3 books)
        if (student->issuedBooks.size() >= 3) {
            cout << RED << "\n Student has reached maximum book limit (3 books)!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        // Issue the book
        book->isIssued = true;
        book->issuedTo = student->name;
        book->issueDate = getCurrentDate();
        student->issuedBooks.push_back(bookId);

        cout << GREEN << "\n Book issued successfully!\n" << RESET;
        cout << "Book: " << book->title << "\n";
        cout << "Issued to: " << student->name << "\n";
        cout << "Issue Date: " << book->issueDate << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void returnBook() {
        clearScreen();
        printHeader("RETURN BOOK");

        int bookId;
        cout << WHITE << "Enter Book ID to return: " << RESET;
        cin >> bookId;

        // Find book
        Book* book = nullptr;
        for (auto& b : books) {
            if (b.bookId == bookId) {
                book = &b;
                break;
            }
        }

        if (!book) {
            cout << RED << "\n Book not found!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        if (!book->isIssued) {
            cout << RED << "\n Book is not issued!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        // Find student and remove book from their issued list
        for (auto& student : students) {
            auto it = find(student.issuedBooks.begin(), student.issuedBooks.end(), bookId);
            if (it != student.issuedBooks.end()) {
                student.issuedBooks.erase(it);
                break;
            }
        }

        // Return the book
        string issuedTo = book->issuedTo;
        book->isIssued = false;
        book->issuedTo = "";
        book->returnDate = getCurrentDate();

        cout << GREEN << "\n Book returned successfully!\n" << RESET;
        cout << "Book: " << book->title << "\n";
        cout << "Returned by: " << issuedTo << "\n";
        cout << "Return Date: " << book->returnDate << "\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void viewIssuedBooks() {
        clearScreen();
        printHeader("ISSUED BOOKS");

        vector<Book> issuedBooks;
        for (const auto& book : books) {
            if (book.isIssued) {
                issuedBooks.push_back(book);
            }
        }

        if (issuedBooks.empty()) {
            cout << RED << "\n No books are currently issued!\n" << RESET;
        } else {
            cout << left << setw(8) << "ID" << setw(25) << "Title" << setw(20) << "Issued To"
                 << setw(25) << "Issue Date" << "\n";
            printLine();

            for (const auto& book : issuedBooks) {
                cout << left << setw(8) << book.bookId << setw(25) << book.title.substr(0, 24)
                     << setw(20) << book.issuedTo.substr(0, 19) << setw(25) << book.issueDate << "\n";
            }
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void reportsMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("REPORTS");
            printLine();
            cout << YELLOW << "\n\t1.  Total Books Report\n";
            cout << "\t2.  Total Students Report\n";
            cout << "\t3.  Books Issued Report\n";
            cout << "\t4.  Category-wise Books\n";
            cout << "\t5.  Back\n" << RESET;
            printLine();
            cout << WHITE << "Enter your choice: " << RESET;
            cin >> choice;

            switch (choice) {
                case 1: totalBooksReport(); break;
                case 2: totalStudentsReport(); break;
                case 3: booksIssuedReport(); break;
                case 4: categoryWiseReport(); break;
                case 5: break;
                default:
                    cout << RED << "Invalid choice!" << RESET;
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 5);
    }

    void totalBooksReport() {
        clearScreen();
        printHeader("TOTAL BOOKS REPORT");

        int totalBooks = books.size();
        int availableBooks = 0;
        int issuedBooks = 0;

        for (const auto& book : books) {
            if (book.isIssued) {
                issuedBooks++;
            } else {
                availableBooks++;
            }
        }

        cout << GREEN << "\n LIBRARY STATISTICS\n" << RESET;
        printLine();
        cout << "Total Books: " << CYAN << totalBooks << RESET << "\n";
        cout << "Available Books: " << GREEN << availableBooks << RESET << "\n";
        cout << "Issued Books: " << RED << issuedBooks << RESET << "\n";

        if (totalBooks > 0) {
            double availablePercentage = (double)availableBooks / totalBooks * 100;
            double issuedPercentage = (double)issuedBooks / totalBooks * 100;

            cout << "\nAvailability: " << GREEN << fixed << setprecision(1) << availablePercentage << "%" << RESET << "\n";
            cout << "Utilization: " << YELLOW << fixed << setprecision(1) << issuedPercentage << "%" << RESET << "\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void totalStudentsReport() {
        clearScreen();
        printHeader("TOTAL STUDENTS REPORT");

        int totalStudents = students.size();
        int activeStudents = 0;
        int totalBooksIssued = 0;

        for (const auto& student : students) {
            if (!student.issuedBooks.empty()) {
                activeStudents++;
            }
            totalBooksIssued += student.issuedBooks.size();
        }

        cout << GREEN << "\n STUDENT STATISTICS\n" << RESET;
        printLine();
        cout << "Total Students: " << CYAN << totalStudents << RESET << "\n";
        cout << "Active Students (with books): " << GREEN << activeStudents << RESET << "\n";
        cout << "Inactive Students: " << YELLOW << (totalStudents - activeStudents) << RESET << "\n";
        cout << "Total Books Issued: " << RED << totalBooksIssued << RESET << "\n";

        if (totalStudents > 0) {
            double activePercentage = (double)activeStudents / totalStudents * 100;
            double avgBooksPerStudent = (double)totalBooksIssued / totalStudents;

            cout << "\nActive Student Rate: " << GREEN << fixed << setprecision(1) << activePercentage << "%" << RESET << "\n";
            cout << "Average Books per Student: " << CYAN << fixed << setprecision(2) << avgBooksPerStudent << RESET << "\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void booksIssuedReport() {
        clearScreen();
        printHeader("BOOKS ISSUED REPORT");

        cout << "\n" << GREEN << "Currently Issued Books:" << RESET << "\n";
        printLine();

        bool hasIssuedBooks = false;
        for (const auto& book : books) {
            if (book.isIssued) {
                hasIssuedBooks = true;
                cout << "ID: " << book.bookId << " | Title: " << book.title
                     << " | Issued to: " << book.issuedTo << " | Date: " << book.issueDate << "\n";
            }
        }

        if (!hasIssuedBooks) {
            cout << RED << "No books are currently issued!" << RESET << "\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void categoryWiseReport() {
        clearScreen();
        printHeader("CATEGORY-WISE BOOKS");

        // Count books by category
        map<string, int> categoryCount;
        for (const auto& book : books) {
            categoryCount[book.category]++;
        }

        if (categoryCount.empty()) {
            cout << RED << "\n No books available!\n" << RESET;
        } else {
            cout << "\n" << GREEN << "Books by Category:" << RESET << "\n";
            printLine();
            cout << left << setw(20) << "Category" << setw(10) << "Count" << "\n";
            printLine();

            for (const auto& pair : categoryCount) {
                cout << left << setw(20) << pair.first << setw(10) << pair.second << "\n";
            }
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    // Student Portal functions
    void studentMenu() {
        int choice;
        do {
            clearScreen();
            printHeader("STUDENT PORTAL");
            cout << GREEN << "\n\t Welcome Student!\n" << RESET;
            printLine();
            cout << YELLOW << "\n\t1.  View Available Books\n";
            cout << "\t2.  Search Books\n";
            cout << "\t3.  My Issued Books\n";
            cout << "\t4.  My Profile\n";
            cout << "\t5.  Back to Main Menu\n" << RESET;
            printLine();
            cout << WHITE << "Enter your choice: " << RESET;
            cin >> choice;

            switch (choice) {
                case 1: studentViewBooks(); break;
                case 2: studentSearchBooks(); break;
                case 3: studentIssuedBooks(); break;
                case 4: studentProfile(); break;
                case 5: break;
                default:
                    cout << RED << "Invalid choice!" << RESET;
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 5);
    }

    void studentViewBooks() {
        clearScreen();
        printHeader("AVAILABLE BOOKS");

        vector<Book> availableBooks;
        for (const auto& book : books) {
            if (!book.isIssued) {
                availableBooks.push_back(book);
            }
        }

        if (availableBooks.empty()) {
            cout << RED << "\n No books available!\n" << RESET;
        } else {
            cout << left << setw(8) << "ID" << setw(25) << "Title" << setw(20) << "Author"
                 << setw(15) << "Category" << "\n";
            printLine();

            for (const auto& book : availableBooks) {
                cout << left << setw(8) << book.bookId << setw(25) << book.title.substr(0, 24)
                     << setw(20) << book.author.substr(0, 19) << setw(15) << book.category.substr(0, 14) << "\n";
            }

            cout << "\n" << GREEN << "Total Available Books: " << availableBooks.size() << RESET << "\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void studentSearchBooks() {
        clearScreen();
        printHeader("SEARCH BOOKS");

        cout << YELLOW << "\n\t1. Search by Title\n";
        cout << "\t2. Search by Author\n";
        cout << "\t3. Search by Category\n" << RESET;

        int choice;
        cout << WHITE << "Enter choice: " << RESET;
        cin >> choice;
        cin.ignore();

        string searchTerm;
        vector<Book> results;

        switch (choice) {
            case 1: {
                cout << WHITE << "Enter Title: " << RESET;
                getline(cin, searchTerm);
                for (const auto& book : books) {
                    if (book.title.find(searchTerm) != string::npos) {
                        results.push_back(book);
                    }
                }
                break;
            }
            case 2: {
                cout << WHITE << "Enter Author: " << RESET;
                getline(cin, searchTerm);
                for (const auto& book : books) {
                    if (book.author.find(searchTerm) != string::npos) {
                        results.push_back(book);
                    }
                }
                break;
            }
            case 3: {
                cout << WHITE << "Enter Category: " << RESET;
                getline(cin, searchTerm);
                for (const auto& book : books) {
                    if (book.category.find(searchTerm) != string::npos) {
                        results.push_back(book);
                    }
                }
                break;
            }
            default:
                cout << RED << "Invalid choice!" << RESET;
                cout << "\nPress Enter to continue...";
                cin.get();
                return;
        }

        if (results.empty()) {
            cout << RED << "\n No books found!\n" << RESET;
        } else {
            cout << "\n" << GREEN << "Search Results:" << RESET << "\n";
            printLine();
            cout << left << setw(8) << "ID" << setw(25) << "Title" << setw(20) << "Author"
                 << setw(15) << "Category" << setw(12) << "Status" << "\n";
            printLine();

            for (const auto& book : results) {
                cout << left << setw(8) << book.bookId << setw(25) << book.title.substr(0, 24)
                     << setw(20) << book.author.substr(0, 19) << setw(15) << book.category.substr(0, 14);
                if (book.isIssued) {
                    cout << RED << "Issued" << RESET;
                } else {
                    cout << GREEN << "Available" << RESET;
                }
                cout << "\n";
            }
        }

        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void studentIssuedBooks() {
        clearScreen();
        printHeader("MY ISSUED BOOKS");

        int studentId;
        cout << WHITE << "Enter your Student ID: " << RESET;
        cin >> studentId;

        // Find student
        Student* student = nullptr;
        for (auto& s : students) {
            if (s.studentId == studentId) {
                student = &s;
                break;
            }
        }

        if (!student) {
            cout << RED << "\n Student not found!\n" << RESET;
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            return;
        }

        if (student->issuedBooks.empty()) {
            cout << YELLOW << "\n You have no issued books!\n" << RESET;
        } else {
            cout << "\n" << GREEN << "Your Issued Books:" << RESET << "\n";
            printLine();
            cout << left << setw(8) << "ID" << setw(25) << "Title" << setw(20) << "Author"
                 << setw(25) << "Issue Date" << "\n";
            printLine();

            for (int bookId : student->issuedBooks) {
                for (const auto& book : books) {
                    if (book.bookId == bookId) {
                        cout << left << setw(8) << book.bookId << setw(25) << book.title.substr(0, 24)
                             << setw(20) << book.author.substr(0, 19) << setw(25) << book.issueDate << "\n";
                        break;
                    }
                }
            }

            cout << "\n" << CYAN << "Total Books Issued: " << student->issuedBooks.size() << "/3" << RESET << "\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void studentProfile() {
        clearScreen();
        printHeader("MY PROFILE");

        int studentId;
        cout << WHITE << "Enter your Student ID: " << RESET;
        cin >> studentId;

        // Find student
        for (const auto& student : students) {
            if (student.studentId == studentId) {
                cout << "\n" << GREEN << "Profile Information:" << RESET << "\n";
                printLine();
                cout << "Student ID: " << CYAN << student.studentId << RESET << "\n";
                cout << "Name: " << CYAN << student.name << RESET << "\n";
                cout << "Email: " << CYAN << student.email << RESET << "\n";
                cout << "Course: " << CYAN << student.course << RESET << "\n";
                cout << "Books Issued: " << YELLOW << student.issuedBooks.size() << "/3" << RESET << "\n";

                if (!student.issuedBooks.empty()) {
                    cout << "\nIssued Book IDs: ";
                    for (int bookId : student.issuedBooks) {
                        cout << MAGENTA << bookId << " " << RESET;
                    }
                    cout << "\n";
                }

                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                return;
            }
        }

        cout << RED << "\n Student not found!\n" << RESET;
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    // Main run function
    void run() {
        int choice;
        do {
            showMainMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    librarianLogin();
                    break;
                case 2:
                    studentMenu();
                    break;
                case 3:
                    cout << GREEN << "\n Thank you for using Library Management System!\n" << RESET;
                    saveData();
                    break;
                default:
                    cout << RED << "\n Invalid choice! Please try again.\n" << RESET;
                    cout << "Press Enter to continue...";
                    cin.ignore();
                    cin.get();
            }
        } while (choice != 3);
    }
};

// Main function
int main() {
    LibrarySystem library;
    library.run();
    return 0;
}
