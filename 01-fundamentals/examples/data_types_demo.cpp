/**
 * @file data_types_demo.cpp
 * @brief Comprehensive demonstration of C++ data types
 * @author C++ Training Course
 * 
 * This program demonstrates:
 * - Fundamental data types
 * - Variable initialization
 * - Type sizes and limits
 * - Auto keyword usage
 * - Const and constexpr
 */

#include <iostream>
#include <iomanip>
#include <climits>
#include <cfloat>
#include <typeinfo>

int main() {
    std::cout << "=== C++ Data Types Demonstration ===" << std::endl;
    
    // Integer types demonstration
    std::cout << "\n1. Integer Types:" << std::endl;
    std::cout << std::setw(15) << "Type" << std::setw(10) << "Size" 
              << std::setw(15) << "Min Value" << std::setw(15) << "Max Value" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    std::cout << std::setw(15) << "char" << std::setw(10) << sizeof(char)
              << std::setw(15) << static_cast<int>(CHAR_MIN)
              << std::setw(15) << static_cast<int>(CHAR_MAX) << std::endl;
              
    std::cout << std::setw(15) << "short" << std::setw(10) << sizeof(short)
              << std::setw(15) << SHRT_MIN
              << std::setw(15) << SHRT_MAX << std::endl;
              
    std::cout << std::setw(15) << "int" << std::setw(10) << sizeof(int)
              << std::setw(15) << INT_MIN
              << std::setw(15) << INT_MAX << std::endl;
              
    std::cout << std::setw(15) << "long" << std::setw(10) << sizeof(long)
              << std::setw(15) << LONG_MIN
              << std::setw(15) << LONG_MAX << std::endl;
    
    // Variable initialization methods
    std::cout << "\n2. Variable Initialization Methods:" << std::endl;
    
    // C-style initialization
    int old_style = 42;
    std::cout << "C-style (int old_style = 42): " << old_style << std::endl;
    
    // Constructor-style initialization
    int constructor_style(42);
    std::cout << "Constructor-style (int constructor_style(42)): " 
              << constructor_style << std::endl;
    
    // Modern C++ uniform initialization (C++11+)
    int modern_style{42};
    std::cout << "Modern uniform (int modern_style{42}): " 
              << modern_style << std::endl;
    
    // Zero initialization
    int zero_init{};
    std::cout << "Zero initialization (int zero_init{}): " 
              << zero_init << std::endl;
    
    // Auto keyword demonstration
    std::cout << "\n3. Auto Keyword Usage:" << std::endl;
    
    auto auto_int = 42;
    auto auto_double = 42.5;
    auto auto_char = 'A';
    auto auto_string = "Hello World";
    
    std::cout << "auto auto_int = 42; Type: " << typeid(auto_int).name() 
              << ", Value: " << auto_int << std::endl;
    std::cout << "auto auto_double = 42.5; Type: " << typeid(auto_double).name() 
              << ", Value: " << auto_double << std::endl;
    std::cout << "auto auto_char = 'A'; Type: " << typeid(auto_char).name() 
              << ", Value: " << auto_char << std::endl;
    std::cout << "auto auto_string = \"Hello\"; Type: " << typeid(auto_string).name() 
              << ", Value: " << auto_string << std::endl;
    
    // Constants demonstration
    std::cout << "\n4. Constants (const vs constexpr):" << std::endl;
    
    const double PI = 3.14159265359;
    constexpr int BUFFER_SIZE = 1024;
    
    std::cout << "const double PI = " << std::setprecision(10) << PI << std::endl;
    std::cout << "constexpr int BUFFER_SIZE = " << BUFFER_SIZE << std::endl;
    
    // Floating-point precision demonstration
    std::cout << "\n5. Floating-Point Precision Issues:" << std::endl;
    
    float f = 0.1f + 0.2f;
    double d = 0.1 + 0.2;
    double expected = 0.3;
    
    std::cout << std::setprecision(20);
    std::cout << "float (0.1f + 0.2f): " << f << std::endl;
    std::cout << "double (0.1 + 0.2): " << d << std::endl;
    std::cout << "expected (0.3): " << expected << std::endl;
    
    std::cout << "\nEquality check:" << std::endl;
    std::cout << "d == expected: " << (d == expected ? "true" : "false") << std::endl;
    
    // Safe floating-point comparison
    const double EPSILON = 1e-9;
    bool safe_equal = std::abs(d - expected) < EPSILON;
    std::cout << "Safe comparison (with epsilon): " << (safe_equal ? "true" : "false") << std::endl;
    
    // Type conversion demonstration
    std::cout << "\n6. Type Conversions:" << std::endl;
    
    double pi_double = 3.14159;
    int pi_truncated = static_cast<int>(pi_double);  // Explicit conversion
    int pi_implicit = pi_double;                     // Implicit conversion (same result)
    
    std::cout << "Original double: " << pi_double << std::endl;
    std::cout << "static_cast<int>: " << pi_truncated << std::endl;
    std::cout << "Implicit conversion: " << pi_implicit << std::endl;
    
    // Overflow demonstration
    std::cout << "\n7. Integer Overflow Example:" << std::endl;
    
    int max_int = INT_MAX;
    std::cout << "INT_MAX: " << max_int << std::endl;
    std::cout << "INT_MAX + 1: " << (max_int + 1) << " (Overflow!)" << std::endl;
    
    return 0;
}