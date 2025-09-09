/**
 * @file exercise1_personal_info.cpp
 * @brief Solution for Exercise 1 - Personal Information Display
 * @author C++ Training Course
 * 
 * This solution demonstrates:
 * - Appropriate data type selection
 * - Const usage for unchanging data
 * - Proper variable naming
 * - Formatted output
 */

#include <iostream>
#include <string>
#include <iomanip>

int main() {
    // Personal information variables with appropriate data types
    
    // std::string for name - can handle variable length text
    const std::string NAME{"John Doe"};
    
    // unsigned int for age - age cannot be negative
    const unsigned int AGE{22};
    
    // std::string for student ID - might contain letters and numbers
    const std::string STUDENT_ID{"CS2024001"};
    
    // double for GPA - needs decimal precision, typical range 0.0-4.0
    const double GPA{3.85};
    
    // const std::string for major - won't change during program execution
    const std::string MAJOR{"Computer Science"};
    
    // Display personal information with proper formatting
    std::cout << "=== Personal Information ===" << std::endl;
    std::cout << "Name: " << NAME << std::endl;
    std::cout << "Age: " << AGE << std::endl;
    std::cout << "Student ID: " << STUDENT_ID << std::endl;
    
    // Set precision for GPA display
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "GPA: " << GPA << std::endl;
    std::cout << "Major: " << MAJOR << std::endl;
    
    // Additional demonstration: Show data type sizes
    std::cout << "\n=== Data Type Analysis ===" << std::endl;
    std::cout << "Size of name (std::string): " << sizeof(NAME) << " bytes" << std::endl;
    std::cout << "Size of age (unsigned int): " << sizeof(AGE) << " bytes" << std::endl;
    std::cout << "Size of GPA (double): " << sizeof(GPA) << " bytes" << std::endl;
    
    return 0;
}

/*
 * DATA TYPE SELECTION RATIONALE:
 * 
 * NAME - std::string:
 *   - Variable length text
 *   - Handles Unicode characters
 *   - Memory managed automatically
 * 
 * AGE - unsigned int:
 *   - Age cannot be negative
 *   - Typical range 0-120 fits easily in unsigned int
 *   - Could use unsigned char for memory efficiency
 * 
 * STUDENT_ID - std::string:
 *   - Often contains both letters and numbers
 *   - Length varies by institution
 *   - Leading zeros need to be preserved
 * 
 * GPA - double:
 *   - Needs decimal precision
 *   - double provides sufficient precision for GPA calculations
 *   - float would be adequate but double is standard for calculations
 * 
 * All variables are const because they don't change during execution
 */