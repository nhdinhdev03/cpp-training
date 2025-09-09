# Templates - Generic Programming Mastery

## 🎯 Tại Sao Templates Là Game Changer?

Sau 15 năm lập trình C++, tôi có thể nói rằng **Templates là heart của Modern C++**. STL, Boost, và hầu hết mọi C++ library đều dựa trên templates. Đây là điều phân biệt C++ với các ngôn ngữ khác.

### 💡 Templates vs. Alternatives

```cpp
// ❌ BAD: Code duplication without templates
int max_int(int a, int b) {
    return (a > b) ? a : b;
}

double max_double(double a, double b) {
    return (a > b) ? a : b;
}

std::string max_string(const std::string& a, const std::string& b) {
    return (a > b) ? a : b;
}

// ✅ EXCELLENT: Single template function
template<typename T>
T max_value(const T& a, const T& b) {
    return (a > b) ? a : b;
}

// Usage - compiler generates specific functions
int main() {
    std::cout << max_value(10, 20) << std::endl;           // max_value<int>
    std::cout << max_value(3.14, 2.71) << std::endl;      // max_value<double>
    std::cout << max_value(std::string("hello"), std::string("world")) << std::endl;
    
    return 0;
}
```

## 🔧 Function Templates - Cơ Bản

### Basic Syntax

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

// Simple function template
template<typename T>
T add(const T& a, const T& b) {
    return a + b;
}

// Multiple template parameters
template<typename T, typename U>
auto multiply(const T& a, const U& b) -> decltype(a * b) {
    return a * b;
}

// Template with non-type parameters
template<typename T, size_t N>
T sum_array(const T (&arr)[N]) {
    T result{};
    for (size_t i = 0; i < N; ++i) {
        result += arr[i];
    }
    return result;
}

void demonstrate_basic_templates() {
    std::cout << "=== Basic Function Templates ===" << std::endl;
    
    // Type deduction
    std::cout << "add(5, 10) = " << add(5, 10) << std::endl;
    std::cout << "add(3.14, 2.86) = " << add(3.14, 2.86) << std::endl;
    
    // Mixed types
    std::cout << "multiply(3, 2.5) = " << multiply(3, 2.5) << std::endl;
    
    // Array template
    int numbers[] = {1, 2, 3, 4, 5};
    std::cout << "sum_array = " << sum_array(numbers) << std::endl;
}
```

### 🎯 Template Specialization

```cpp
#include <iostream>
#include <string>
#include <cstring>

// Primary template
template<typename T>
bool are_equal(const T& a, const T& b) {
    return a == b;
}

// Full specialization for C-style strings
template<>
bool are_equal<const char*>(const char* const& a, const char* const& b) {
    return std::strcmp(a, b) == 0;
}

// Partial specialization (only for class templates, but here's the concept)
template<typename T>
struct TypeTraits {
    static constexpr bool is_pointer = false;
    static constexpr bool is_integral = false;
};

// Specialization for pointers
template<typename T>
struct TypeTraits<T*> {
    static constexpr bool is_pointer = true;
    static constexpr bool is_integral = false;
};

// Specialization for integers
template<>
struct TypeTraits<int> {
    static constexpr bool is_pointer = false;
    static constexpr bool is_integral = true;
};

void demonstrate_specialization() {
    std::cout << "=== Template Specialization ===" << std::endl;
    
    // Regular comparison
    std::cout << "are_equal(10, 10) = " << are_equal(10, 10) << std::endl;
    std::cout << "are_equal(\"hello\", \"hello\") = " << are_equal(std::string("hello"), std::string("hello")) << std::endl;
    
    // Specialized for C-strings
    const char* str1 = "hello";
    const char* str2 = "hello";
    std::cout << "are_equal(C-string) = " << are_equal(str1, str2) << std::endl;
    
    // Type traits
    std::cout << "int is_integral: " << TypeTraits<int>::is_integral << std::endl;
    std::cout << "int* is_pointer: " << TypeTraits<int*>::is_pointer << std::endl;
}
```

## 🚀 Advanced Template Techniques

### SFINAE (Substitution Failure Is Not An Error)

```cpp
#include <iostream>
#include <type_traits>
#include <vector>
#include <string>

// Enable function only for integral types
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safe_divide(T a, T b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

// Enable function only for floating-point types  
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
safe_divide(T a, T b) {
    if (std::abs(b) < std::numeric_limits<T>::epsilon()) {
        throw std::invalid_argument("Division by near-zero");
    }
    return a / b;
}

// Modern C++14/17 approach with constexpr if
template<typename T>
T modern_safe_divide(T a, T b) {
    if constexpr (std::is_integral_v<T>) {
        if (b == 0) {
            throw std::invalid_argument("Division by zero");
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        if (std::abs(b) < std::numeric_limits<T>::epsilon()) {
            throw std::invalid_argument("Division by near-zero");
        }
    }
    return a / b;
}
```

### Variadic Templates

```cpp
#include <iostream>
#include <sstream>

// C++11 variadic templates - recursive approach
template<typename T>
void print_values(const T& value) {
    std::cout << value << std::endl;
}

template<typename T, typename... Args>
void print_values(const T& first, const Args&... args) {
    std::cout << first << " ";
    print_values(args...);  // Recursive call
}

// C++17 fold expressions - much cleaner!
template<typename... Args>
void modern_print(const Args&... args) {
    ((std::cout << args << " "), ...);  // Fold expression
    std::cout << std::endl;
}

// Variadic template for sum calculation
template<typename... Args>
auto sum_all(Args... args) {
    return (args + ...);  // C++17 fold expression
}

// Type-safe printf replacement
template<typename... Args>
std::string format_string(const std::string& format, Args... args) {
    std::ostringstream oss;
    
    // This is simplified - real implementation would parse format string
    ((oss << args << " "), ...);
    
    return oss.str();
}

void demonstrate_variadic_templates() {
    std::cout << "=== Variadic Templates ===" << std::endl;
    
    // Print multiple values
    print_values(1, 2.5, "hello", 'c');
    modern_print("Modern:", 42, 3.14, "world");
    
    // Sum calculation
    std::cout << "sum_all(1, 2, 3, 4, 5) = " << sum_all(1, 2, 3, 4, 5) << std::endl;
    
    // Format string
    std::string formatted = format_string("Values:", 10, 20.5, "test");
    std::cout << "Formatted: " << formatted << std::endl;
}
```

## 🔍 Template Metaprogramming

### Compile-time Calculations

```cpp
#include <iostream>
#include <type_traits>

// Compile-time factorial calculation
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N-1>::value;
};

// Template specialization for base case
template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

// Modern C++11+ constexpr function (preferred)
constexpr int factorial(int n) {
    return (n == 0) ? 1 : n * factorial(n - 1);
}

// Compile-time type checking
template<typename T>
struct TypeInfo {
    static constexpr bool is_arithmetic = std::is_arithmetic_v<T>;
    static constexpr bool is_pointer = std::is_pointer_v<T>;
    static constexpr size_t size = sizeof(T);
    
    static void print_info() {
        std::cout << "Type size: " << size << " bytes" << std::endl;
        std::cout << "Is arithmetic: " << is_arithmetic << std::endl;
        std::cout << "Is pointer: " << is_pointer << std::endl;
    }
};

// SFINAE for container detection
template<typename T, typename = void>
struct has_begin : std::false_type {};

template<typename T>
struct has_begin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type {};

template<typename Container>
typename std::enable_if<has_begin<Container>::value>::type
print_container(const Container& container) {
    std::cout << "Container contents: ";
    for (const auto& item : container) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void demonstrate_metaprogramming() {
    std::cout << "=== Template Metaprogramming ===" << std::endl;
    
    // Compile-time calculations
    constexpr int fact5 = Factorial<5>::value;  // Computed at compile time
    constexpr int modern_fact5 = factorial(5);   // Also computed at compile time
    
    std::cout << "5! = " << fact5 << std::endl;
    std::cout << "5! (modern) = " << modern_fact5 << std::endl;
    
    // Type information
    TypeInfo<double>::print_info();
    TypeInfo<int*>::print_info();
    
    // Container detection
    std::vector<int> vec{1, 2, 3, 4, 5};
    print_container(vec);
    
    std::cout << "std::vector has begin(): " << has_begin<std::vector<int>>::value << std::endl;
    std::cout << "int has begin(): " << has_begin<int>::value << std::endl;
}
```

## 🏗️ Generic Algorithm Design

### Iterator-based Algorithms

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

// Generic find algorithm
template<typename Iterator, typename T>
Iterator generic_find(Iterator first, Iterator last, const T& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}

// Generic transform algorithm
template<typename InputIt, typename OutputIt, typename UnaryOperation>
OutputIt generic_transform(InputIt first, InputIt last, OutputIt result, UnaryOperation op) {
    for (; first != last; ++first, ++result) {
        *result = op(*first);
    }
    return result;
}

// Predicate-based algorithms
template<typename Iterator, typename Predicate>
Iterator find_if_generic(Iterator first, Iterator last, Predicate pred) {
    for (; first != last; ++first) {
        if (pred(*first)) {
            return first;
        }
    }
    return last;
}

// Count algorithm with predicate
template<typename Iterator, typename Predicate>
size_t count_if_generic(Iterator first, Iterator last, Predicate pred) {
    size_t count = 0;
    for (; first != last; ++first) {
        if (pred(*first)) {
            ++count;
        }
    }
    return count;
}

void demonstrate_generic_algorithms() {
    std::cout << "=== Generic Algorithms ===" << std::endl;
    
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::list<int> lst{10, 20, 30, 40, 50};
    
    // Generic find works with different containers
    auto it1 = generic_find(vec.begin(), vec.end(), 5);
    auto it2 = generic_find(lst.begin(), lst.end(), 30);
    
    if (it1 != vec.end()) {
        std::cout << "Found in vector: " << *it1 << std::endl;
    }
    
    if (it2 != lst.end()) {
        std::cout << "Found in list: " << *it2 << std::endl;
    }
    
    // Generic transform
    std::vector<int> squares(vec.size());
    generic_transform(vec.begin(), vec.end(), squares.begin(), 
        [](int x) { return x * x; });
    
    std::cout << "Squares: ";
    for (int square : squares) {
        std::cout << square << " ";
    }
    std::cout << std::endl;
    
    // Predicate-based algorithms
    auto even_it = find_if_generic(vec.begin(), vec.end(), 
        [](int x) { return x % 2 == 0; });
    
    if (even_it != vec.end()) {
        std::cout << "First even number: " << *even_it << std::endl;
    }
    
    size_t even_count = count_if_generic(vec.begin(), vec.end(),
        [](int x) { return x % 2 == 0; });
    
    std::cout << "Even numbers count: " << even_count << std::endl;
}
```

## 🎯 Template Best Practices

### Type Constraints (C++20 Concepts Preview)

```cpp
#include <iostream>
#include <type_traits>
#include <concepts>  // C++20

// Pre-C++20 approach with SFINAE
template<typename T>
typename std::enable_if<std::is_arithmetic<T>::value, T>::type
constrained_add(const T& a, const T& b) {
    return a + b;
}

// C++20 concepts approach (if available)
#ifdef __cpp_concepts
template<std::integral T>  // Requires T to be integral
T safe_multiply(T a, T b) {
    // Check for overflow
    if (a != 0 && b > std::numeric_limits<T>::max() / a) {
        throw std::overflow_error("Integer overflow");
    }
    return a * b;
}

template<std::floating_point T>  // Requires T to be floating point
T safe_divide(T a, T b) {
    if (std::abs(b) < std::numeric_limits<T>::epsilon()) {
        throw std::invalid_argument("Division by near-zero");
    }
    return a / b;
}
#endif

// Generic container interface
template<typename Container>
class ContainerWrapper {
private:
    Container container_;
    
public:
    using value_type = typename Container::value_type;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    
    template<typename... Args>
    explicit ContainerWrapper(Args&&... args) : container_(std::forward<Args>(args)...) {}
    
    // Generic interface
    void push_back(const value_type& value) {
        if constexpr (requires { container_.push_back(value); }) {
            container_.push_back(value);
        } else {
            container_.insert(container_.end(), value);
        }
    }
    
    size_t size() const { return container_.size(); }
    bool empty() const { return container_.empty(); }
    
    iterator begin() { return container_.begin(); }
    iterator end() { return container_.end(); }
    const_iterator begin() const { return container_.begin(); }
    const_iterator end() const { return container_.end(); }
};
```

### Template Error Messages

```cpp
// ❌ BAD: Cryptic error messages
template<typename T>
void bad_function(T value) {
    value.non_existent_method();  // Compiler error is confusing
}

// ✅ GOOD: Clear error messages with static_assert
template<typename T>
void good_function(T value) {
    static_assert(std::is_arithmetic_v<T>, 
                  "T must be an arithmetic type (int, float, etc.)");
    
    static_assert(sizeof(T) >= 4, 
                  "T must be at least 4 bytes");
    
    // Function implementation
}

// ✅ EXCELLENT: Comprehensive type checking
template<typename Iterator>
void process_iterator(Iterator first, Iterator last) {
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    
    static_assert(std::is_arithmetic_v<value_type>,
                  "Iterator must point to arithmetic type");
    
    static_assert(!std::is_same_v<Iterator, void>,
                  "Iterator cannot be void");
    
    // Implementation
    for (; first != last; ++first) {
        // Process *first
    }
}
```

## 🎓 Practical Example: Generic Data Structure

```cpp
#include <iostream>
#include <memory>
#include <functional>
#include <stdexcept>

/**
 * @brief Generic stack implementation demonstrating template best practices
 */
template<typename T>
class Stack {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> next;
        
        template<typename... Args>
        explicit Node(Args&&... args) : data(std::forward<Args>(args)...) {}
    };
    
    std::unique_ptr<Node> top_;
    size_t size_;
    
public:
    Stack() : top_(nullptr), size_(0) {}
    
    // Rule of 5 with move semantics
    ~Stack() = default;
    Stack(const Stack& other) = delete;  // Disable copying for simplicity
    Stack& operator=(const Stack& other) = delete;
    
    Stack(Stack&& other) noexcept 
        : top_(std::move(other.top_)), size_(other.size_) {
        other.size_ = 0;
    }
    
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            top_ = std::move(other.top_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Perfect forwarding for push
    template<typename... Args>
    void emplace(Args&&... args) {
        auto new_node = std::make_unique<Node>(std::forward<Args>(args)...);
        new_node->next = std::move(top_);
        top_ = std::move(new_node);
        ++size_;
    }
    
    void push(const T& value) {
        emplace(value);
    }
    
    void push(T&& value) {
        emplace(std::move(value));
    }
    
    T pop() {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        
        T result = std::move(top_->data);
        top_ = std::move(top_->next);
        --size_;
        return result;
    }
    
    const T& top() const {
        if (empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return top_->data;
    }
    
    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }
    
    // Generic operations
    template<typename Predicate>
    bool contains_if(Predicate pred) const {
        for (Node* current = top_.get(); current; current = current->next.get()) {
            if (pred(current->data)) {
                return true;
            }
        }
        return false;
    }
    
    template<typename Func>
    void for_each(Func func) const {
        for (Node* current = top_.get(); current; current = current->next.get()) {
            func(current->data);
        }
    }
};

void demonstrate_generic_stack() {
    std::cout << "=== Generic Stack Implementation ===" << std::endl;
    
    Stack<int> int_stack;
    Stack<std::string> string_stack;
    
    // Test with integers
    int_stack.push(10);
    int_stack.push(20);
    int_stack.emplace(30);  // Direct construction
    
    std::cout << "Integer stack size: " << int_stack.size() << std::endl;
    std::cout << "Top element: " << int_stack.top() << std::endl;
    
    // Test generic operations
    bool has_even = int_stack.contains_if([](int x) { return x % 2 == 0; });
    std::cout << "Contains even number: " << has_even << std::endl;
    
    std::cout << "Stack contents: ";
    int_stack.for_each([](int x) { std::cout << x << " "; });
    std::cout << std::endl;
    
    // Test with strings
    string_stack.emplace("Hello");
    string_stack.emplace("World");
    
    std::cout << "String stack top: " << string_stack.top() << std::endl;
    
    // Pop elements
    while (!int_stack.empty()) {
        std::cout << "Popped: " << int_stack.pop() << std::endl;
    }
}
```

## 🎓 Key Takeaways

### ✅ Template Best Practices

1. **Use meaningful names**: `template<typename Iterator>` not `template<typename T>`
2. **Const-correctness**: Always consider const versions
3. **Perfect forwarding**: Use `std::forward` for parameter passing
4. **SFINAE/Concepts**: Constrain templates appropriately
5. **Static assertions**: Provide clear error messages
6. **Move semantics**: Support efficient resource transfer

### 🚨 Common Template Pitfalls

```cpp
// ❌ Pitfall 1: Template bloat
template<typename T>
class BadContainer {
    std::vector<T> data_;
    void complex_algorithm() { /* Large implementation */ }
};

// ✅ Better: Move non-template code to base class
class ContainerBase {
protected:
    void complex_algorithm() { /* Implementation */ }
};

template<typename T>
class GoodContainer : private ContainerBase {
    std::vector<T> data_;
public:
    void do_work() { complex_algorithm(); }  // Uses base implementation
};

// ❌ Pitfall 2: Unintended type deduction
template<typename T>
void dangerous_function(T value) {
    // What if T is deduced as reference?
}

dangerous_function(some_variable);  // Might deduce T as int& instead of int

// ✅ Better: Use std::decay or explicit types
template<typename T>
void safe_function(std::decay_t<T> value) {
    // T is always a value type
}
```

## ➡️ Advanced Topics

Trong các chương tiếp theo:
- **Class Templates**
- **Template Template Parameters**
- **CRTP (Curiously Recurring Template Pattern)**
- **Expression Templates**

---

*"Templates are not just about code reuse - they're about expressing algorithms at the right level of abstraction." - Template Philosophy*