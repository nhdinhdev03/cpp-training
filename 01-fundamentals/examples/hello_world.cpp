/**
 * @file hello_world.cpp
 * @brief Basic C++ program demonstrating program structure
 * @author C++ Training Course
 * 
 * This program demonstrates:
 * - Basic program structure
 * - Include directives
 * - Main function
 * - Output operations
 * - Return values
 */

#include <iostream>

/**
 * @brief Main entry point of the program
 * @return 0 on successful execution
 */
int main() {
    // Output greeting message
    std::cout << "=== Welcome to C++ Programming! ===" << std::endl;
    std::cout << "Today is a great day to code." << std::endl;
    std::cout << "Author: C++ Training Course" << std::endl;
    
    // Demonstrate different output methods
    std::cout << "\nDifferent ways to output:" << std::endl;
    std::cout << "Method 1: Using std::cout with <<" << std::endl;
    std::cout << "Method 2: " << "Chaining multiple values " << 42 << std::endl;
    
    // Using std::endl vs \n
    std::cout << "Line with std::endl" << std::endl;  // Flushes buffer
    std::cout << "Line with \\n\n";                   // Just newline
    
    return 0;  // Indicate successful execution
}