# Chương 2: Variables và Data Types - Nền Tảng Vững Chắc

## 🎯 Tại Sao Phải Hiểu Rõ Data Types?

Sau 15 năm làm việc với C++, tôi thấy **90% bugs của junior developers** đều liên quan đến việc không hiểu rõ data types và memory layout. Chương này sẽ giúp bạn tránh những lỗi cơ bản nhất.

## 📊 Fundamental Data Types

### Integer Types

```cpp
#include <iostream>
#include <climits>  // For INT_MAX, INT_MIN, etc.

int main() {
    // Signed integers
    char c = 'A';           // 1 byte, -128 to 127
    short s = 1000;         // 2 bytes, -32,768 to 32,767
    int i = 100000;         // 4 bytes (usually), -2B to 2B
    long l = 1000000L;      // 4 or 8 bytes (platform dependent)
    long long ll = 1000000000LL; // 8 bytes minimum
    
    // Unsigned integers
    unsigned char uc = 255;        // 0 to 255
    unsigned int ui = 4000000000U; // 0 to 4B (approximately)
    
    // Size verification (IMPORTANT for debugging)
    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "INT_MAX: " << INT_MAX << std::endl;
    std::cout << "INT_MIN: " << INT_MIN << std::endl;
    
    return 0;
}
```

### 🚨 Kinh Nghiệm Thực Tế: Integer Overflow

```cpp
#include <iostream>

int main() {
    int max_int = 2147483647;  // INT_MAX on most systems
    std::cout << "max_int: " << max_int << std::endl;
    
    // Dangerous! Integer overflow
    max_int = max_int + 1;
    std::cout << "max_int + 1: " << max_int << std::endl;  // Will be negative!
    
    // Safe approach
    if (max_int < INT_MAX) {
        max_int++;
    } else {
        std::cout << "Cannot increment, would overflow!" << std::endl;
    }
    
    return 0;
}
```

### Floating-Point Types

```cpp
#include <iostream>
#include <iomanip>  // For precision control
#include <cfloat>   // For FLT_MAX, DBL_MAX, etc.

int main() {
    float f = 3.14159f;         // 4 bytes, ~7 decimal digits precision
    double d = 3.14159265359;   // 8 bytes, ~15 decimal digits precision
    long double ld = 3.14159265358979323846L; // Usually 8-16 bytes
    
    // Precision demonstration
    std::cout << std::setprecision(10);
    std::cout << "float:       " << f << std::endl;
    std::cout << "double:      " << d << std::endl;
    std::cout << "long double: " << ld << std::endl;
    
    return 0;
}
```

### 🚨 Floating-Point Gotchas (Từ Kinh Nghiệm Đau Thương)

```cpp
#include <iostream>

int main() {
    // NEVER do this for equality comparison!
    double a = 0.1 + 0.2;
    double b = 0.3;
    
    if (a == b) {
        std::cout << "Equal" << std::endl;
    } else {
        std::cout << "Not equal!" << std::endl;  // This will print!
    }
    
    // Correct way to compare floating-point numbers
    const double EPSILON = 1e-9;
    if (std::abs(a - b) < EPSILON) {
        std::cout << "Equal (with tolerance)" << std::endl;
    }
    
    return 0;
}
```

## 🏷️ Variables - Naming và Best Practices

### Variable Declaration và Initialization

```cpp
#include <iostream>

int main() {
    // Different ways to initialize variables
    
    // C-style initialization
    int a = 10;
    
    // Constructor-style initialization
    int b(20);
    
    // Modern C++ (C++11+) - RECOMMENDED!
    int c{30};              // Uniform initialization
    int d{};                // Zero initialization
    
    // Multiple variables
    int x = 1, y = 2, z = 3;  // OK but not recommended
    
    // Better approach
    int x_coord{1};
    int y_coord{2};
    int z_coord{3};
    
    return 0;
}
```

### 💡 Naming Conventions (Từ 15 Năm Kinh Nghiệm)

```cpp
// ✅ GOOD naming conventions
int student_count{25};       // snake_case for variables
const double PI{3.14159};    // ALL_CAPS for constants
bool is_valid{true};         // Boolean variables start with 'is', 'has', 'can'

class StudentManager {       // PascalCase for classes
private:
    int student_count_;      // Trailing underscore for private members
    
public:
    void calculateGrade();   // camelCase for methods
};

// ❌ BAD naming conventions
int sc;                      // Too short, unclear
int studentCount;            // Mixing conventions
int Student_Count;           // Mixed case styles
bool flag;                   // Non-descriptive
```

## 🔍 Auto Keyword - Modern C++ Best Practice

```cpp
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Before C++11
    std::vector<std::string> names;
    std::vector<std::string>::iterator it = names.begin();
    
    // With auto (C++11+) - MUCH BETTER!
    auto names_modern = std::vector<std::string>{};
    auto it_modern = names_modern.begin();
    
    // Auto với literals
    auto integer = 42;           // int
    auto floating = 42.0;        // double
    auto character = 'A';        // char
    auto text = "Hello";         // const char*
    auto modern_text = std::string{"Hello"}; // std::string
    
    // Type verification
    std::cout << typeid(integer).name() << std::endl;
    
    return 0;
}
```

### ⚠️ Auto Pitfalls (Từ Kinh Nghiệm)

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<bool> flags{true, false, true};
    
    // Dangerous! This is NOT bool&
    auto flag = flags[0];  // Returns proxy object, not bool!
    
    // Correct approaches
    bool flag_copy = flags[0];        // Explicit copy
    auto&& flag_ref = flags[0];       // Universal reference
    
    // Always specify reference when needed
    auto& explicit_ref = some_variable;
    const auto& const_ref = some_variable;
    
    return 0;
}
```

## 📏 Constants - Immutability là Sức Mạnh

### Different Types of Constants

```cpp
#include <iostream>

int main() {
    // Compile-time constants
    const int MAX_STUDENTS{100};        // Runtime or compile-time
    constexpr int BUFFER_SIZE{1024};    // MUST be compile-time
    
    // Difference demonstration
    int user_input;
    std::cin >> user_input;
    
    const int runtime_const{user_input};     // OK - runtime const
    // constexpr int compile_const{user_input}; // ERROR! Not compile-time
    
    // Const with pointers (CONFUSING for beginners)
    int value{42};
    int other{24};
    
    const int* ptr_to_const = &value;      // Can change pointer, not value
    int* const const_ptr = &value;         // Can change value, not pointer
    const int* const const_both = &value;  // Cannot change either
    
    // ptr_to_const = &other;  // OK
    // *ptr_to_const = 100;    // ERROR!
    
    *const_ptr = 100;       // OK
    // const_ptr = &other;  // ERROR!
    
    return 0;
}
```

## 🎯 Scope và Lifetime - Hiểu Để Tránh Bugs

```cpp
#include <iostream>

int global_variable{100};  // Global scope - avoid if possible!

int main() {
    int main_variable{200}; // Function scope
    
    {
        int block_variable{300}; // Block scope
        std::cout << global_variable << std::endl;    // OK
        std::cout << main_variable << std::endl;      // OK
        std::cout << block_variable << std::endl;     // OK
    }
    
    // std::cout << block_variable << std::endl;  // ERROR! Out of scope
    
    // Variable shadowing (DANGEROUS!)
    int x{10};
    {
        int x{20};  // Shadows outer x
        std::cout << x << std::endl;  // Prints 20, not 10!
    }
    std::cout << x << std::endl;  // Prints 10
    
    return 0;
}
```

## 🔧 Type Conversion - Nguy Hiểm Nếu Không Cẩn Thận

### Implicit Conversion

```cpp
#include <iostream>

int main() {
    // Implicit conversions (automatic)
    int i{42};
    double d = i;        // int to double - safe
    
    double pi{3.14159};
    int truncated = pi;  // double to int - LOSSY! Value becomes 3
    
    // Dangerous implicit conversions
    unsigned int ui{4000000000U};
    int signed_int = ui; // Might become negative!
    
    std::cout << "Original unsigned: " << ui << std::endl;
    std::cout << "Converted signed: " << signed_int << std::endl;
    
    return 0;
}
```

### Explicit Conversion (Casting)

```cpp
#include <iostream>

int main() {
    double pi{3.14159};
    
    // C-style casting (AVOID!)
    int old_style = (int)pi;
    
    // Modern C++ casting (PREFERRED!)
    int modern_style = static_cast<int>(pi);
    
    // When you KNOW what you're doing
    const int* const_ptr = &old_style;
    int* mutable_ptr = const_cast<int*>(const_ptr);  // Removes const
    
    // For polymorphic types (advanced)
    // dynamic_cast, reinterpret_cast (will cover later)
    
    return 0;
}
```

## 🧪 Practical Examples

### Example 1: Temperature Converter

```cpp
#include <iostream>
#include <iomanip>

int main() {
    std::cout << "Temperature Converter (Celsius to Fahrenheit)" << std::endl;
    
    double celsius{};
    std::cout << "Enter temperature in Celsius: ";
    std::cin >> celsius;
    
    // Formula: F = C * 9/5 + 32
    constexpr double CONVERSION_FACTOR{9.0/5.0};
    constexpr double OFFSET{32.0};
    
    double fahrenheit{celsius * CONVERSION_FACTOR + OFFSET};
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << celsius << "°C = " << fahrenheit << "°F" << std::endl;
    
    return 0;
}
```

### Example 2: Data Type Size Explorer

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << "=== C++ Data Type Sizes ===" << std::endl;
    
    std::cout << "Type\t\tSize (bytes)\tMin Value\t\tMax Value" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    
    std::cout << "char\t\t" << sizeof(char) << "\t\t" 
              << static_cast<int>(CHAR_MIN) << "\t\t\t" 
              << static_cast<int>(CHAR_MAX) << std::endl;
              
    std::cout << "short\t\t" << sizeof(short) << "\t\t" 
              << SHRT_MIN << "\t\t" << SHRT_MAX << std::endl;
              
    std::cout << "int\t\t" << sizeof(int) << "\t\t" 
              << INT_MIN << "\t" << INT_MAX << std::endl;
              
    std::cout << "float\t\t" << sizeof(float) << std::endl;
    std::cout << "double\t\t" << sizeof(double) << std::endl;
    
    return 0;
}
```

## 🎓 Key Takeaways

1. **Always initialize variables** - Uninitialized variables are bugs waiting to happen
2. **Use appropriate data types** - Don't use `int` for everything
3. **Be careful with floating-point comparisons** - Never use `==` directly
4. **Prefer `constexpr` over `const`** when possible
5. **Use `auto` wisely** - Great for complex types, be explicit when clarity matters
6. **Modern C++ casting** - Use `static_cast`, avoid C-style casts

## 🔥 Common Mistakes từ Kinh Nghiệm

1. **Integer overflow** - Always check bounds
2. **Floating-point precision** - Use epsilon for comparisons
3. **Uninitialized variables** - Always initialize
4. **Wrong data type choice** - `float` vs `double`, `int` vs `size_t`
5. **Variable shadowing** - Avoid same names in nested scopes

## ➡️ Next Chapter

Trong chương tiếp theo, chúng ta sẽ tìm hiểu về **Operators và Expressions** - cách thao tác với data một cách hiệu quả và an toàn.

---

*"The most important single aspect of software development is to be clear about what you are trying to build." - Bjarne Stroustrup*