# C++ Best Practices - Kinh Nghiệm 15 Năm Trong Ngành

## 🎯 Tại Sao Best Practices Quan Trọng?

Trong 15 năm làm việc, tôi đã thấy quá nhiều projects thất bại không phải vì technical problems mà vì **lack of consistent coding standards**. Code không chỉ viết cho compiler mà còn cho đồng nghiệp và chính bạn trong tương lai.

## 📝 Naming Conventions - Nghệ Thuật Đặt Tên

### Variable Naming

```cpp
// ✅ EXCELLENT: Clear, descriptive names
int student_count{0};                    // Clear purpose
double average_temperature{25.5};       // Self-documenting
bool is_connection_active{true};         // Boolean naming convention
std::string user_input_filename;        // Compound names with underscores

// ✅ GOOD: Consistent abbreviations (if unavoidable)
int max_retry_attempts{3};              // "max" is widely understood
std::string db_connection_string;       // "db" is common abbreviation

// ❌ BAD: Unclear, abbreviated names
int sc;                                 // What is 'sc'?
double temp;                           // Temperature? Template? Temporary?
bool flag;                             // What kind of flag?
std::string str;                       // Generic, meaningless name

// ❌ TERRIBLE: Misleading names
int average_value{42};                 // Single value, not average!
bool is_ready{false};                  // Double negative confusion
std::string number_string{"hello"};    // Contains text, not number
```

### Function Naming

```cpp
class StudentManager {
public:
    // ✅ EXCELLENT: Verb phrases, clear actions
    void add_student(const Student& student);
    bool remove_student_by_id(int student_id);
    std::vector<Student> find_students_by_major(const std::string& major);
    double calculate_class_average() const;
    
    // ✅ GOOD: Getters and setters
    std::string get_manager_name() const;
    void set_max_capacity(int capacity);
    
    // ✅ EXCELLENT: Boolean functions with clear prefixes
    bool is_student_enrolled(int student_id) const;
    bool has_available_slots() const;
    bool can_enroll_student() const;
    
    // ❌ BAD: Unclear purpose
    void process();                     // Process what?
    bool check();                       // Check what?
    void handle(const Student& s);      // Handle how?
    
    // ❌ BAD: Misleading names
    void get_student();                 // Returns void, doesn't "get" anything
    bool validate();                    // Validate what? Returns what?
};
```

### Class and Type Naming

```cpp
// ✅ EXCELLENT: PascalCase for classes, clear purpose
class DatabaseConnection {              // Clear what it represents
private:
    ConnectionState state_;
    std::string connection_string_;
    
public:
    enum class ConnectionState {        // Scoped enum
        Disconnected,
        Connecting,
        Connected,
        Error
    };
};

class TemperatureSensor {               // Hardware abstraction
private:
    static constexpr double MIN_TEMP = -273.15;
    static constexpr double MAX_TEMP = 1000.0;
};

// ✅ GOOD: Template naming
template<typename DataType, typename CompareFunc>
class SortedContainer {
    // Implementation
};

// ❌ BAD: Unclear, generic names
class Manager {                         // Manager of what?
class Helper {                          // Helps with what?
class Util {                            // Too generic
class Data {                            // What kind of data?
```

## 🏗️ Code Organization - Structure Matters

### Header File Organization

```cpp
// student.h - ✅ EXCELLENT header organization

#pragma once  // Modern include guard (preferred over #ifndef)

// 1. System includes first
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// 2. Third-party library includes
// #include <boost/algorithm/string.hpp>

// 3. Project includes
#include "person.h"
#include "grade.h"

// 4. Forward declarations (when possible)
class Course;
class Transcript;

/**
 * @brief Represents a student in the academic system
 * 
 * The Student class encapsulates all student-related data and operations.
 * It maintains academic records, enrollment information, and provides
 * methods for grade management and transcript generation.
 * 
 * @author Your Name
 * @date 2024-01-01
 * @version 1.0
 */
class Student {
public:
    // 5. Public types first
    enum class AcademicStatus {
        Active,
        Probation,
        Suspended,
        Graduated
    };
    
    // 6. Static constants
    static constexpr double MIN_GPA = 0.0;
    static constexpr double MAX_GPA = 4.0;
    
    // 7. Constructors (default, parameterized, copy, move)
    Student() = default;
    Student(const std::string& name, int id);
    Student(const Student& other);
    Student(Student&& other) noexcept;
    
    // 8. Destructor
    ~Student() = default;
    
    // 9. Assignment operators
    Student& operator=(const Student& other);
    Student& operator=(Student&& other) noexcept;
    
    // 10. Public member functions (logical grouping)
    
    // Academic operations
    void enroll_in_course(std::shared_ptr<Course> course);
    void drop_course(const std::string& course_code);
    bool add_grade(const std::string& course_code, double grade);
    
    // Information retrieval
    std::string get_name() const;
    int get_student_id() const;
    double calculate_gpa() const;
    AcademicStatus get_academic_status() const;
    
    // Utility functions
    void print_transcript() const;
    bool is_eligible_for_graduation() const;

private:
    // 11. Private member variables (grouped logically)
    
    // Basic information
    std::string full_name_;
    int student_id_;
    
    // Academic data
    std::vector<Grade> grades_;
    std::vector<std::shared_ptr<Course>> enrolled_courses_;
    AcademicStatus status_{AcademicStatus::Active};
    
    // 12. Private helper functions
    void update_academic_status();
    bool is_valid_grade(double grade) const;
    double calculate_course_credits() const;
};
```

### Implementation File Organization

```cpp
// student.cpp - ✅ EXCELLENT implementation organization

#include "student.h"

// System includes needed for implementation
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <iomanip>

// Project includes
#include "course.h"
#include "grade.h"

// Anonymous namespace for file-local helpers
namespace {
    constexpr double PROBATION_GPA_THRESHOLD = 2.0;
    constexpr double GRADUATION_GPA_THRESHOLD = 2.0;
    constexpr int MIN_GRADUATION_CREDITS = 120;
    
    // Helper function only used in this file
    std::string format_gpa(double gpa) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << gpa;
        return oss.str();
    }
}

// Constructors
Student::Student(const std::string& name, int id) 
    : full_name_{name}, student_id_{id} {
    
    if (name.empty()) {
        throw std::invalid_argument("Student name cannot be empty");
    }
    
    if (id <= 0) {
        throw std::invalid_argument("Student ID must be positive");
    }
}

// Copy constructor with deep copy
Student::Student(const Student& other)
    : full_name_{other.full_name_}
    , student_id_{other.student_id_}
    , grades_{other.grades_}  // Deep copy of grades
    , enrolled_courses_{other.enrolled_courses_}  // Shared ownership of courses
    , status_{other.status_} {
}

// Move constructor
Student::Student(Student&& other) noexcept
    : full_name_{std::move(other.full_name_)}
    , student_id_{other.student_id_}
    , grades_{std::move(other.grades_)}
    , enrolled_courses_{std::move(other.enrolled_courses_)}
    , status_{other.status_} {
    
    // Reset moved-from object
    other.student_id_ = 0;
    other.status_ = AcademicStatus::Active;
}

// Public member functions implementation
void Student::enroll_in_course(std::shared_ptr<Course> course) {
    if (!course) {
        throw std::invalid_argument("Course cannot be null");
    }
    
    // Check if already enrolled
    auto it = std::find(enrolled_courses_.begin(), enrolled_courses_.end(), course);
    if (it != enrolled_courses_.end()) {
        throw std::runtime_error("Student already enrolled in this course");
    }
    
    enrolled_courses_.push_back(course);
}

double Student::calculate_gpa() const {
    if (grades_.empty()) {
        return 0.0;
    }
    
    double total_points = std::accumulate(grades_.begin(), grades_.end(), 0.0,
        [](double sum, const Grade& grade) {
            return sum + (grade.get_points() * grade.get_credits());
        });
    
    double total_credits = std::accumulate(grades_.begin(), grades_.end(), 0.0,
        [](double sum, const Grade& grade) {
            return sum + grade.get_credits();
        });
    
    return total_credits > 0 ? total_points / total_credits : 0.0;
}

// Private helper functions
void Student::update_academic_status() {
    double current_gpa = calculate_gpa();
    
    if (current_gpa < PROBATION_GPA_THRESHOLD) {
        status_ = AcademicStatus::Probation;
    } else if (is_eligible_for_graduation()) {
        status_ = AcademicStatus::Graduated;
    } else {
        status_ = AcademicStatus::Active;
    }
}

bool Student::is_valid_grade(double grade) const {
    return grade >= MIN_GPA && grade <= MAX_GPA;
}
```

## 🔍 Error Handling - Professional Approach

### Exception Handling Best Practices

```cpp
#include <stdexcept>
#include <system_error>
#include <fstream>
#include <iostream>

class FileProcessor {
public:
    // ✅ EXCELLENT: Specific exception types
    class FileProcessingError : public std::runtime_error {
    public:
        FileProcessingError(const std::string& message, const std::string& filename)
            : std::runtime_error(message + " (file: " + filename + ")") {}
    };
    
    void process_file(const std::string& filename) {
        std::ifstream file(filename);
        
        // ✅ GOOD: Check operation success
        if (!file.is_open()) {
            throw FileProcessingError("Cannot open file", filename);
        }
        
        try {
            // Process file content
            std::string line;
            while (std::getline(file, line)) {
                process_line(line);
            }
            
        } catch (const std::exception& e) {
            // ✅ EXCELLENT: Wrap and re-throw with context
            throw FileProcessingError(
                "Error processing file content: " + std::string(e.what()), 
                filename
            );
        }
        
        // File automatically closed by RAII
    }
    
private:
    void process_line(const std::string& line) {
        if (line.empty()) {
            return;  // ✅ GOOD: Handle empty lines gracefully
        }
        
        // Process line...
        if (line.length() > 1000) {
            throw std::invalid_argument("Line too long: " + std::to_string(line.length()));
        }
    }
};

// ✅ EXCELLENT: Proper exception handling at application boundary
int main() {
    try {
        FileProcessor processor;
        processor.process_file("data.txt");
        
    } catch (const FileProcessor::FileProcessingError& e) {
        std::cerr << "File processing failed: " << e.what() << std::endl;
        return 1;
        
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 2;
        
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 3;
    }
    
    return 0;
}
```

### Error Codes vs Exceptions

```cpp
#include <optional>
#include <string>
#include <iostream>

class Calculator {
public:
    // ✅ GOOD: Using std::optional for operations that might fail
    std::optional<double> safe_divide(double numerator, double denominator) const {
        if (denominator == 0.0) {
            return std::nullopt;  // No exception, just no result
        }
        return numerator / denominator;
    }
    
    // ✅ GOOD: Exceptions for programming errors
    double divide(double numerator, double denominator) const {
        if (denominator == 0.0) {
            throw std::invalid_argument("Division by zero");
        }
        return numerator / denominator;
    }
    
    // ✅ EXCELLENT: Result type pattern (C++23 std::expected style)
    struct DivisionResult {
        bool success;
        double value;
        std::string error_message;
        
        static DivisionResult ok(double val) {
            return {true, val, ""};
        }
        
        static DivisionResult error(const std::string& msg) {
            return {false, 0.0, msg};
        }
    };
    
    DivisionResult try_divide(double numerator, double denominator) const {
        if (denominator == 0.0) {
            return DivisionResult::error("Division by zero");
        }
        return DivisionResult::ok(numerator / denominator);
    }
};

void demonstrate_error_handling() {
    Calculator calc;
    
    // Using optional
    if (auto result = calc.safe_divide(10.0, 2.0)) {
        std::cout << "Result: " << *result << std::endl;
    } else {
        std::cout << "Division failed" << std::endl;
    }
    
    // Using result type
    auto result = calc.try_divide(10.0, 0.0);
    if (result.success) {
        std::cout << "Result: " << result.value << std::endl;
    } else {
        std::cout << "Error: " << result.error_message << std::endl;
    }
}
```

## 🚀 Performance Best Practices

### Pass by Reference vs Value

```cpp
#include <string>
#include <vector>

class DataProcessor {
public:
    // ✅ EXCELLENT: Different parameter passing strategies
    
    // Small, cheap-to-copy types: pass by value
    void set_id(int id) { id_ = id; }
    void set_score(double score) { score_ = score; }
    
    // Large objects: pass by const reference for read-only
    void process_data(const std::vector<std::string>& data) {
        for (const auto& item : data) {  // const auto& for range-based loops
            process_item(item);
        }
    }
    
    // Objects to be modified: pass by reference
    void modify_data(std::vector<std::string>& data) {
        for (auto& item : data) {  // auto& for modification
            item = transform_item(item);
        }
    }
    
    // Move semantics for transferring ownership
    void set_name(std::string name) {  // Pass by value for move
        name_ = std::move(name);
    }
    
    // Perfect forwarding for generic code
    template<typename T>
    void add_item(T&& item) {
        items_.emplace_back(std::forward<T>(item));
    }

private:
    int id_;
    double score_;
    std::string name_;
    std::vector<std::string> items_;
    
    void process_item(const std::string& item) {
        // Process single item
    }
    
    std::string transform_item(const std::string& item) {
        return "transformed_" + item;
    }
};
```

### Memory and Performance Optimization

```cpp
#include <vector>
#include <memory>
#include <algorithm>

class OptimizedContainer {
private:
    std::vector<int> data_;
    
public:
    // ✅ EXCELLENT: Reserve capacity when size is known
    explicit OptimizedContainer(size_t expected_size) {
        data_.reserve(expected_size);  // Avoid reallocations
    }
    
    // ✅ GOOD: Use emplace for in-place construction
    void add_item(int value) {
        data_.emplace_back(value);  // More efficient than push_back
    }
    
    // ✅ EXCELLENT: Return by const reference when possible
    const std::vector<int>& get_data() const {
        return data_;  // No copy
    }
    
    // ✅ GOOD: Move when transferring ownership
    std::vector<int> extract_data() {
        return std::move(data_);  // Transfer ownership
    }
    
    // ✅ EXCELLENT: Algorithms over manual loops
    void sort_data() {
        std::sort(data_.begin(), data_.end());  // Optimized algorithm
    }
    
    size_t count_even() const {
        return std::count_if(data_.begin(), data_.end(), 
            [](int n) { return n % 2 == 0; });  // Algorithm + lambda
    }
    
    // ✅ GOOD: Const-correctness
    bool contains(int value) const {
        return std::find(data_.cbegin(), data_.cend(), value) != data_.cend();
    }
};

// ✅ EXCELLENT: RAII for resource management
class ResourceManager {
private:
    std::unique_ptr<int[]> buffer_;
    size_t size_;
    
public:
    explicit ResourceManager(size_t size) 
        : buffer_{std::make_unique<int[]>(size)}, size_{size} {}
    
    // Automatic cleanup via smart pointer destructor
    
    int& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return buffer_[index];
    }
    
    const int& operator[](size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return buffer_[index];
    }
};
```

## 🎓 Key Principles Summary

### ✅ Golden Rules

1. **Code for Humans**: Write code that humans can read and understand
2. **Consistency**: Follow established patterns throughout the project
3. **KISS Principle**: Keep It Simple, Stupid - prefer simple solutions
4. **DRY Principle**: Don't Repeat Yourself - abstract common functionality
5. **RAII**: Resource Acquisition Is Initialization - use constructors/destructors
6. **Const Correctness**: Use const wherever possible
7. **Fail Fast**: Detect errors early and report them clearly

### 🚨 Common Anti-Patterns to Avoid

```cpp
// ❌ ANTI-PATTERN: Magic numbers
if (status == 3) {  // What does 3 mean?
    // ...
}

// ✅ BETTER: Named constants
enum class Status { Active = 1, Inactive = 2, Suspended = 3 };
if (status == Status::Suspended) {
    // ...
}

// ❌ ANTI-PATTERN: Long parameter lists
void create_user(std::string name, int age, std::string email, 
                std::string address, std::string phone, bool active);

// ✅ BETTER: Parameter object
struct UserInfo {
    std::string name;
    int age;
    std::string email;
    std::string address;
    std::string phone;
    bool active = true;
};
void create_user(const UserInfo& info);

// ❌ ANTI-PATTERN: Deeply nested code
if (condition1) {
    if (condition2) {
        if (condition3) {
            // Deep nesting
        }
    }
}

// ✅ BETTER: Early returns
if (!condition1) return;
if (!condition2) return;
if (!condition3) return;
// Main logic here
```

## 📚 Recommended Tools

### Static Analysis
- **clang-tidy**: Comprehensive linting
- **cppcheck**: Static analysis
- **PVS-Studio**: Commercial static analyzer

### Code Formatting
- **clang-format**: Automatic formatting
- **uncrustify**: Alternative formatter

### Documentation
- **Doxygen**: API documentation
- **Sphinx**: Project documentation

---

*"Any fool can write code that a computer can understand. Good programmers write code that humans can understand." - Martin Fowler*