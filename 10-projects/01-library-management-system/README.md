# Dự Án: Hệ Thống Quản Lý Thư Viện

## 🎯 Tổng Quan Dự Án

Đây là dự án capstone kết hợp **tất cả các khái niệm** đã học trong khóa C++. Bạn sẽ xây dựng một hệ thống quản lý thư viện hoàn chỉnh, demonstrating best practices từ 15 năm kinh nghiệm.

### 🏗️ Kiến Trúc Hệ Thống

```
📦 Library Management System
├── 📁 src/
│   ├── 📁 core/           # Core business logic
│   │   ├── book.h/cpp
│   │   ├── member.h/cpp
│   │   ├── library.h/cpp
│   │   └── transaction.h/cpp
│   ├── 📁 utils/          # Utility classes
│   │   ├── date_time.h/cpp
│   │   ├── file_manager.h/cpp
│   │   └── validator.h/cpp
│   ├── 📁 interfaces/     # Abstract interfaces
│   │   ├── searchable.h
│   │   └── persistable.h
│   └── main.cpp
├── 📁 data/              # Data files
├── 📁 tests/            # Unit tests
├── CMakeLists.txt       # Build configuration
└── README.md
```

## 🎯 Mục Tiêu Học Tập

Sau khi hoàn thành dự án, bạn sẽ demonstrate:

### Core C++ Concepts
- ✅ **Classes & Objects**: Well-designed class hierarchy
- ✅ **Inheritance & Polymorphism**: Abstract base classes
- ✅ **Encapsulation**: Proper data hiding
- ✅ **Operator Overloading**: Custom operators

### Modern C++ Features
- ✅ **Smart Pointers**: Proper memory management
- ✅ **Move Semantics**: Efficient resource transfer
- ✅ **Lambda Expressions**: Functional programming
- ✅ **Auto & Type Deduction**: Modern syntax

### Advanced Concepts
- ✅ **Templates**: Generic programming
- ✅ **STL Containers**: Vector, map, set usage
- ✅ **Exception Handling**: Robust error management
- ✅ **File I/O**: Data persistence

### Best Practices
- ✅ **RAII**: Resource management
- ✅ **Const Correctness**: Immutability
- ✅ **Coding Standards**: Professional style
- ✅ **Testing**: Unit test coverage

## 📋 Functional Requirements

### 1. Book Management
- Add new books to library
- Remove books from inventory
- Update book information
- Search books by title, author, ISBN
- Track book availability status

### 2. Member Management
- Register new library members
- Update member information
- Track member borrowing history
- Calculate member fees and fines

### 3. Transaction Management
- Issue books to members
- Return books from members
- Calculate due dates and overdue fines
- Generate transaction reports

### 4. Search & Reports
- Search functionality across all entities
- Generate various reports (overdue books, popular books, etc.)
- Export data to different formats

### 5. Data Persistence
- Save/load data from files
- Backup and restore functionality
- Data validation and error recovery

## 🛠️ Technical Requirements

### Programming Standards
```cpp
// Follow these coding standards throughout the project:

// 1. Naming Convention
class LibraryManager {           // PascalCase for classes
private:
    int member_count_;           // snake_case for variables
    static const int MAX_BOOKS;  // ALL_CAPS for constants

public:
    void add_member();           // snake_case for functions
    bool is_book_available();    // Boolean functions with is_/has_/can_
};

// 2. Header Guards
#pragma once                     // Use pragma once

// 3. Include Order
#include <iostream>              // System includes first
#include <memory>
#include "book.h"               // Project includes last

// 4. Documentation
/**
 * @brief Manages library operations
 * @author Your Name
 * @date Current Date
 */
```

### Memory Management
- Use **smart pointers** exclusively (no raw new/delete)
- Implement **RAII** for all resources
- Proper **exception safety** guarantees

### Error Handling
- Custom exception hierarchy
- Input validation at all boundaries
- Graceful failure recovery

## 📊 Class Design

### Core Classes

#### 1. Book Class
```cpp
class Book {
public:
    enum class Status { Available, Borrowed, Reserved, Maintenance };
    
private:
    std::string isbn_;
    std::string title_;
    std::string author_;
    std::string publisher_;
    int publication_year_;
    Status status_;
    std::unique_ptr<BorrowingInfo> borrowing_info_;
    
public:
    // Constructor with validation
    Book(const std::string& isbn, const std::string& title, 
         const std::string& author);
    
    // Move semantics support
    Book(Book&& other) noexcept;
    Book& operator=(Book&& other) noexcept;
    
    // Getters (const-correct)
    const std::string& get_isbn() const;
    const std::string& get_title() const;
    Status get_status() const;
    
    // Operations
    bool is_available() const;
    void set_status(Status new_status);
    
    // Operators
    bool operator==(const Book& other) const;
    bool operator<(const Book& other) const;  // For std::set
    
    // Serialization
    std::string to_string() const;
    static Book from_string(const std::string& data);
};
```

#### 2. Member Class
```cpp
class LibraryMember {
private:
    int member_id_;
    std::string name_;
    std::string email_;
    std::string phone_;
    std::vector<std::shared_ptr<Book>> borrowed_books_;
    double outstanding_fees_;
    
public:
    // Constructor with validation
    LibraryMember(int id, const std::string& name, const std::string& email);
    
    // Book operations
    bool can_borrow_book() const;
    void borrow_book(std::shared_ptr<Book> book);
    bool return_book(const std::string& isbn);
    
    // Fee management
    double calculate_fees() const;
    void pay_fees(double amount);
    
    // Information
    size_t get_borrowed_count() const;
    std::vector<std::shared_ptr<Book>> get_borrowed_books() const;
};
```

#### 3. Library Class (Main Controller)
```cpp
class Library {
private:
    std::string name_;
    std::map<std::string, std::unique_ptr<Book>> books_;          // ISBN -> Book
    std::map<int, std::unique_ptr<LibraryMember>> members_;       // ID -> Member
    std::vector<Transaction> transaction_history_;
    std::unique_ptr<FileManager> file_manager_;
    
public:
    explicit Library(const std::string& name);
    
    // Book management
    void add_book(std::unique_ptr<Book> book);
    bool remove_book(const std::string& isbn);
    std::shared_ptr<Book> find_book(const std::string& isbn);
    std::vector<std::shared_ptr<Book>> search_books(const std::string& query);
    
    // Member management
    void register_member(std::unique_ptr<LibraryMember> member);
    bool remove_member(int member_id);
    std::shared_ptr<LibraryMember> find_member(int id);
    
    // Transactions
    bool issue_book(int member_id, const std::string& isbn);
    bool return_book(int member_id, const std::string& isbn);
    
    // Reports
    std::vector<Book> get_overdue_books() const;
    std::map<std::string, int> get_popular_books() const;
    
    // Persistence
    void save_to_file(const std::string& filename);
    void load_from_file(const std::string& filename);
};
```

## 🎮 Implementation Phases

### Phase 1: Basic Structure (Tuần 1)
- [ ] Implement core classes (Book, Member)
- [ ] Basic constructor, getters, setters
- [ ] Proper encapsulation
- [ ] Unit tests for basic functionality

### Phase 2: Advanced Features (Tuần 2)
- [ ] Implement Library class
- [ ] Add/remove operations
- [ ] Search functionality
- [ ] Exception handling

### Phase 3: Modern C++ Integration (Tuần 3)
- [ ] Smart pointers implementation
- [ ] Move semantics
- [ ] Lambda expressions for search
- [ ] Template functions for generic operations

### Phase 4: Persistence & UI (Tuần 4)
- [ ] File I/O operations
- [ ] Data serialization
- [ ] Console-based UI
- [ ] Error recovery mechanisms

### Phase 5: Polish & Testing (Tuần 5)
- [ ] Comprehensive unit tests
- [ ] Performance optimization
- [ ] Code documentation
- [ ] Final review and refactoring

## 🧪 Testing Strategy

### Unit Tests Structure
```cpp
// test_book.cpp
#include <gtest/gtest.h>
#include "book.h"

class BookTest : public ::testing::Test {
protected:
    void SetUp() override {
        valid_book = std::make_unique<Book>("978-0134685991", 
            "Effective Modern C++", "Scott Meyers");
    }
    
    std::unique_ptr<Book> valid_book;
};

TEST_F(BookTest, ConstructorValidatesISBN) {
    EXPECT_THROW(Book("invalid-isbn", "Title", "Author"), 
                 std::invalid_argument);
}

TEST_F(BookTest, NewBookIsAvailable) {
    EXPECT_TRUE(valid_book->is_available());
    EXPECT_EQ(Book::Status::Available, valid_book->get_status());
}

TEST_F(BookTest, BookComparison) {
    Book other_book("978-0134685991", "Same ISBN", "Different Author");
    EXPECT_EQ(*valid_book, other_book);  // Compare by ISBN
}
```

### Integration Tests
```cpp
// test_library.cpp
TEST(LibraryTest, IssueAndReturnBook) {
    Library lib("Test Library");
    
    auto book = std::make_unique<Book>("123", "Test Book", "Test Author");
    auto member = std::make_unique<LibraryMember>(1, "John Doe", "john@email.com");
    
    lib.add_book(std::move(book));
    lib.register_member(std::move(member));
    
    EXPECT_TRUE(lib.issue_book(1, "123"));
    EXPECT_FALSE(lib.issue_book(2, "123"));  // Book not available
    EXPECT_TRUE(lib.return_book(1, "123"));
}
```

## 🎯 Evaluation Criteria

### Code Quality (40%)
- **Class Design**: Well-structured, single responsibility
- **Encapsulation**: Proper data hiding
- **Error Handling**: Comprehensive exception management
- **Memory Management**: Smart pointer usage, no leaks

### Modern C++ Usage (30%)
- **Smart Pointers**: Proper unique_ptr, shared_ptr usage
- **Move Semantics**: Efficient resource transfer
- **STL Integration**: Appropriate container choices
- **Lambda Expressions**: Functional programming style

### Functionality (20%)
- **Feature Completeness**: All requirements implemented
- **Edge Case Handling**: Robust input validation
- **Performance**: Efficient algorithms and data structures

### Best Practices (10%)
- **Coding Standards**: Consistent naming, formatting
- **Documentation**: Clear comments, API documentation
- **Testing**: Comprehensive unit test coverage
- **Build System**: Proper CMake configuration

## 🚀 Getting Started

1. **Fork this repository**
2. **Read through all class designs**
3. **Start with Phase 1 implementation**
4. **Write tests as you go**
5. **Commit frequently with descriptive messages**

## 💡 Tips từ Kinh Nghiệm

### Design Decisions
```cpp
// ✅ GOOD: Use composition over inheritance
class Library {
private:
    std::unique_ptr<FileManager> file_manager_;    // Composition
    std::unique_ptr<SearchEngine> search_engine_;  // Composition
};

// ❌ AVOID: Deep inheritance hierarchies for this project

// ✅ EXCELLENT: Factory pattern for object creation
class BookFactory {
public:
    static std::unique_ptr<Book> create_from_input();
    static std::unique_ptr<Book> create_from_file_data(const std::string& data);
};
```

### Performance Considerations
```cpp
// ✅ GOOD: Use references for large objects
void Library::add_books(const std::vector<Book>& books);  // const reference

// ✅ EXCELLENT: Move for transfer operations
void Library::add_book(std::unique_ptr<Book> book) {
    books_[book->get_isbn()] = std::move(book);  // Transfer ownership
}

// ✅ GOOD: Reserve capacity when size is known
std::vector<Book> search_results;
search_results.reserve(estimated_result_size);
```

### Error Handling Strategy
```cpp
// Custom exception hierarchy
class LibraryException : public std::exception {};
class BookNotFoundException : public LibraryException {};
class MemberNotFoundException : public LibraryException {};
class InvalidOperationException : public LibraryException {};

// Usage in Library methods
std::shared_ptr<Book> Library::find_book(const std::string& isbn) {
    auto it = books_.find(isbn);
    if (it == books_.end()) {
        throw BookNotFoundException("Book with ISBN " + isbn + " not found");
    }
    return it->second;
}
```

## 📚 Resources

- **C++ Reference**: https://cppreference.com/
- **Google Test Framework**: For unit testing
- **CMake Documentation**: For build system
- **clang-format**: For code formatting

---

**Good luck! Đây là cơ hội để bạn demonstrate tất cả những gì đã học! 🚀**

*"The best way to learn C++ is by building real projects that solve real problems."*