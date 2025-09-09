# Chương 1: Classes và Objects - Nền Tảng OOP

## 🎯 Tại Sao Object-Oriented Programming?

Sau 15 năm làm việc với C++, tôi có thể khẳng định rằng **OOP không chỉ là cách tổ chức code** - nó là cách tư duy về vấn đề. Những project thành công nhất tôi từng tham gia đều có thiết kế OOP tốt.

### 🏗️ Lợi Ích Của OOP (Từ Kinh Nghiệm Thực Tế)

1. **Maintainability**: Code dễ maintain và debug
2. **Reusability**: Có thể reuse classes trong nhiều projects
3. **Scalability**: Dễ extend và modify functionality
4. **Team Collaboration**: Nhiều người có thể work trên different classes
5. **Real-world Modeling**: Map directly từ problem domain sang code

## 🏛️ Class Definition - Cách Thiết Kế Class Tốt

### Basic Class Structure

```cpp
#include <iostream>
#include <string>

/**
 * @brief Represents a bank account with basic operations
 * 
 * Design principles applied:
 * - Encapsulation: Private data members
 * - Single Responsibility: Only handles account operations
 * - Clear interface: Public methods are intuitive
 */
class BankAccount {
private:  // Data hiding - core of encapsulation
    std::string account_holder_;
    std::string account_number_;
    double balance_;
    static int next_account_id_;  // Shared across all objects

public:  // Interface to the outside world
    // Constructor - initialize object state
    BankAccount(const std::string& holder, double initial_balance);
    
    // Destructor - cleanup resources
    ~BankAccount();
    
    // Member functions - object behavior
    void deposit(double amount);
    bool withdraw(double amount);
    double get_balance() const;  // const - doesn't modify object
    
    // Static function - belongs to class, not object
    static int get_total_accounts();
    
    // Friend function - can access private members (use sparingly!)
    friend void print_account_details(const BankAccount& account);
};

// Static member definition (IMPORTANT!)
int BankAccount::next_account_id_ = 1000;
```

### 💡 Class Design Best Practices (15 Năm Kinh Nghiệm)

```cpp
class Student {
private:
    // ✅ GOOD: Clear, descriptive names with consistent naming convention
    std::string full_name_;
    unsigned int student_id_;
    std::vector<double> grades_;
    
    // ✅ GOOD: Helper functions are private
    double calculate_gpa() const;
    bool is_valid_grade(double grade) const;

public:
    // ✅ GOOD: Constructor with validation
    Student(const std::string& name, unsigned int id) 
        : full_name_{name}, student_id_{id} {
        if (name.empty()) {
            throw std::invalid_argument("Name cannot be empty");
        }
    }
    
    // ✅ GOOD: Const methods for getters
    std::string get_name() const { return full_name_; }
    unsigned int get_id() const { return student_id_; }
    
    // ✅ GOOD: Clear method names that explain what they do
    bool add_grade(double grade);
    double get_current_gpa() const;
    size_t get_total_courses() const { return grades_.size(); }
};

// ❌ BAD example for comparison
class BadStudent {
public:  // Everything public - no encapsulation!
    std::string n;      // Cryptic names
    int id;             // Wrong type (could be negative)
    std::vector<int> g; // Wrong type for grades
};
```

## 🏗️ Constructor và Destructor - Quản Lý Lifecycle

### Different Types of Constructors

```cpp
#include <iostream>
#include <string>
#include <memory>

class Rectangle {
private:
    double width_;
    double height_;
    std::string name_;

public:
    // 1. Default constructor
    Rectangle() : width_{1.0}, height_{1.0}, name_{"Default Rectangle"} {
        std::cout << "Default constructor called" << std::endl;
    }
    
    // 2. Parameterized constructor
    Rectangle(double w, double h, const std::string& n) 
        : width_{w}, height_{h}, name_{n} {
        // Input validation (ALWAYS DO THIS!)
        if (w <= 0 || h <= 0) {
            throw std::invalid_argument("Dimensions must be positive");
        }
        std::cout << "Parameterized constructor called for " << name_ << std::endl;
    }
    
    // 3. Copy constructor (Deep copy)
    Rectangle(const Rectangle& other) 
        : width_{other.width_}, height_{other.height_}, name_{other.name_ + "_copy"} {
        std::cout << "Copy constructor called" << std::endl;
    }
    
    // 4. Move constructor (C++11) - Transfer ownership
    Rectangle(Rectangle&& other) noexcept
        : width_{other.width_}, height_{other.height_}, name_{std::move(other.name_)} {
        // Reset moved-from object
        other.width_ = 0;
        other.height_ = 0;
        std::cout << "Move constructor called" << std::endl;
    }
    
    // Destructor - cleanup resources
    ~Rectangle() {
        std::cout << "Destructor called for " << name_ << std::endl;
    }
    
    // Member functions
    double area() const { return width_ * height_; }
    double perimeter() const { return 2 * (width_ + height_); }
    
    void display_info() const {
        std::cout << name_ << ": " << width_ << " x " << height_ 
                  << " (Area: " << area() << ")" << std::endl;
    }
};
```

### 🚨 Constructor Initialization List - QUAN TRỌNG!

```cpp
class ExpensiveObject {
private:
    std::string large_string_;
    std::vector<int> large_vector_;
    const int CONSTANT_VALUE_;  // Must be initialized in initialization list

public:
    // ✅ GOOD: Use initialization list
    ExpensiveObject(const std::string& str, size_t vec_size, int constant)
        : large_string_{str},                    // Direct initialization
          large_vector_(vec_size, 0),            // Construct with size
          CONSTANT_VALUE_{constant} {            // Initialize const member
        std::cout << "Efficient constructor" << std::endl;
    }
    
    // ❌ BAD: Assignment in constructor body
    // ExpensiveObject(const std::string& str, size_t vec_size, int constant) {
    //     large_string_ = str;        // Creates default string first, then assigns
    //     large_vector_ = std::vector<int>(vec_size, 0);  // Same issue
    //     // CONSTANT_VALUE_ = constant;  // ERROR! Can't assign to const
    // }
};
```

## 🔒 Encapsulation - Data Hiding Done Right

### Access Specifiers

```cpp
class BankAccount {
private:    // Only accessible within this class
    double balance_;
    std::string account_number_;
    
    // Private helper methods
    bool is_valid_amount(double amount) const {
        return amount > 0 && amount <= 1000000;  // Business rule
    }

protected:  // Accessible by this class and derived classes
    std::string account_type_;
    
    void log_transaction(const std::string& type, double amount) {
        std::cout << "Transaction: " << type << " $" << amount << std::endl;
    }

public:     // Accessible from anywhere
    // Constructor
    BankAccount(const std::string& number, double initial_balance)
        : account_number_{number}, balance_{initial_balance}, account_type_{"Basic"} {
        if (!is_valid_amount(initial_balance)) {
            throw std::invalid_argument("Invalid initial balance");
        }
    }
    
    // Public interface methods
    bool deposit(double amount) {
        if (!is_valid_amount(amount)) {
            return false;
        }
        balance_ += amount;
        log_transaction("Deposit", amount);
        return true;
    }
    
    bool withdraw(double amount) {
        if (!is_valid_amount(amount) || amount > balance_) {
            return false;
        }
        balance_ -= amount;
        log_transaction("Withdrawal", amount);
        return true;
    }
    
    // Getter methods (const!)
    double get_balance() const { return balance_; }
    std::string get_account_number() const { return account_number_; }
};
```

### 🎯 Getter/Setter Best Practices

```cpp
class Temperature {
private:
    double celsius_;

public:
    // ✅ GOOD: Simple getter (const)
    double get_celsius() const { return celsius_; }
    
    // ✅ GOOD: Setter with validation
    void set_celsius(double temp) {
        if (temp < -273.15) {  // Absolute zero
            throw std::out_of_range("Temperature below absolute zero");
        }
        celsius_ = temp;
    }
    
    // ✅ EXCELLENT: Computed properties
    double get_fahrenheit() const {
        return celsius_ * 9.0/5.0 + 32.0;
    }
    
    double get_kelvin() const {
        return celsius_ + 273.15;
    }
    
    // ❌ BAD: Exposing internal representation
    // double& get_celsius_ref() { return celsius_; }  // Breaks encapsulation!
};
```

## 🎭 Static Members - Class-level Data và Functions

```cpp
class Counter {
private:
    int value_;
    static int total_counters_;  // Shared by all objects
    static int max_value_;       // Configuration

public:
    // Constructor
    Counter(int initial_value = 0) : value_{initial_value} {
        ++total_counters_;  // Increment when object is created
    }
    
    // Destructor
    ~Counter() {
        --total_counters_;  // Decrement when object is destroyed
    }
    
    // Instance methods
    void increment() { 
        if (value_ < max_value_) {
            ++value_; 
        }
    }
    
    int get_value() const { return value_; }
    
    // Static methods - don't operate on specific object
    static int get_total_counters() { return total_counters_; }
    
    static void set_max_value(int max_val) { 
        max_value_ = max_val; 
    }
    
    static int get_max_value() { return max_value_; }
};

// Static member definitions (MUST be outside class!)
int Counter::total_counters_ = 0;
int Counter::max_value_ = 100;

// Usage example
void demonstrate_static_members() {
    std::cout << "Initial counter count: " << Counter::get_total_counters() << std::endl;
    
    {
        Counter c1(5);
        Counter c2(10);
        std::cout << "After creating 2 counters: " << Counter::get_total_counters() << std::endl;
        
        // Can call static methods on objects too (but not recommended)
        // std::cout << c1.get_total_counters() << std::endl;  // Works but confusing
    }  // c1 and c2 destroyed here
    
    std::cout << "After scope exit: " << Counter::get_total_counters() << std::endl;
}
```

## 👥 Friend Functions và Classes

```cpp
class Vector2D {
private:
    double x_, y_;

public:
    Vector2D(double x, double y) : x_{x}, y_{y} {}
    
    // Getters
    double get_x() const { return x_; }
    double get_y() const { return y_; }
    
    // Friend function - can access private members
    friend double dot_product(const Vector2D& v1, const Vector2D& v2);
    
    // Friend class - all methods can access private members
    friend class VectorMath;
    
    // Overloaded operator as friend (will cover in detail later)
    friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
};

// Friend function implementation
double dot_product(const Vector2D& v1, const Vector2D& v2) {
    return v1.x_ * v2.x_ + v1.y_ * v2.y_;  // Direct access to private members
}

// Friend class implementation
class VectorMath {
public:
    static double magnitude(const Vector2D& v) {
        // Direct access to private members
        return std::sqrt(v.x_ * v.x_ + v.y_ * v.y_);
    }
    
    static double angle(const Vector2D& v) {
        return std::atan2(v.y_, v.x_);
    }
};
```

### ⚠️ Friend Functions - Khi Nào Nên Dùng?

**✅ GOOD uses:**
- Operator overloading (particularly binary operators)
- Helper functions cần access multiple private members
- Testing classes cần access internal state

**❌ AVOID:**
- General accessor functions (use getters instead)
- Breaking encapsulation for convenience
- Making entire classes friends unnecessarily

## 🧪 Practical Example: Complete Class Design

```cpp
/**
 * @brief A well-designed Student class demonstrating OOP principles
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

class Student {
private:
    std::string name_;
    unsigned int student_id_;
    std::vector<double> grades_;
    std::string major_;
    
    static unsigned int next_id_;  // Auto-generate IDs
    
    // Private helper methods
    bool is_valid_grade(double grade) const {
        return grade >= 0.0 && grade <= 100.0;
    }

public:
    // Constructors
    Student(const std::string& name, const std::string& major)
        : name_{name}, student_id_{next_id_++}, major_{major} {
        if (name.empty()) {
            throw std::invalid_argument("Student name cannot be empty");
        }
    }
    
    // Copy constructor
    Student(const Student& other)
        : name_{other.name_}, student_id_{next_id_++}, 
          grades_{other.grades_}, major_{other.major_} {}
    
    // Destructor
    ~Student() = default;  // Compiler-generated is fine
    
    // Grade management
    bool add_grade(double grade) {
        if (!is_valid_grade(grade)) {
            return false;
        }
        grades_.push_back(grade);
        return true;
    }
    
    // Getters
    std::string get_name() const { return name_; }
    unsigned int get_id() const { return student_id_; }
    std::string get_major() const { return major_; }
    size_t get_grade_count() const { return grades_.size(); }
    
    // Computed properties
    double get_average_grade() const {
        if (grades_.empty()) {
            return 0.0;
        }
        double sum = std::accumulate(grades_.begin(), grades_.end(), 0.0);
        return sum / grades_.size();
    }
    
    char get_letter_grade() const {
        double avg = get_average_grade();
        if (avg >= 90) return 'A';
        if (avg >= 80) return 'B';
        if (avg >= 70) return 'C';
        if (avg >= 60) return 'D';
        return 'F';
    }
    
    // Display method
    void display_info() const {
        std::cout << "Student ID: " << student_id_ << std::endl;
        std::cout << "Name: " << name_ << std::endl;
        std::cout << "Major: " << major_ << std::endl;
        std::cout << "Grades: " << get_grade_count() << " courses" << std::endl;
        std::cout << "Average: " << get_average_grade() << " (" 
                  << get_letter_grade() << ")" << std::endl;
    }
    
    // Static method
    static unsigned int get_next_id() { return next_id_; }
};

// Static member definition
unsigned int Student::next_id_ = 1000;

// Usage demonstration
int main() {
    try {
        Student alice("Alice Johnson", "Computer Science");
        Student bob("Bob Smith", "Mathematics");
        
        alice.add_grade(85.5);
        alice.add_grade(92.0);
        alice.add_grade(78.5);
        
        bob.add_grade(95.0);
        bob.add_grade(87.5);
        
        std::cout << "=== Student Information ===" << std::endl;
        alice.display_info();
        std::cout << std::endl;
        bob.display_info();
        
        std::cout << "\nNext student ID will be: " << Student::get_next_id() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
```

## 🎓 Key Takeaways

1. **Encapsulation is key** - Keep data private, provide controlled access
2. **Initialize in constructor** - Use initialization lists for efficiency
3. **const-correctness** - Mark methods const if they don't modify state
4. **Validate inputs** - Always check parameters in constructors and methods
5. **RAII principle** - Constructor acquires resources, destructor releases them
6. **Static members** - Use sparingly, mainly for class-wide configuration
7. **Friends** - Use only when necessary, prefer public interface

## ➡️ Next Chapter

Trong chương tiếp theo, chúng ta sẽ tìm hiểu về **Inheritance và Polymorphism** - sức mạnh thật sự của OOP.

---

*"Object-oriented programming is not a silver bullet, but when used properly, it makes code more maintainable and understandable." - Practical Experience*