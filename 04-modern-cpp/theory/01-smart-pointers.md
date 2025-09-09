# Modern C++ - Smart Pointers: Giải Pháp Cho Memory Management

## 🎯 Tại Sao Smart Pointers Là Game Changer?

Trong 15 năm lập trình C++, tôi đã chứng kiến **95% memory leaks và segfaults** đều có thể tránh được bằng smart pointers. Đây là một trong những tính năng quan trọng nhất của Modern C++.

### ⚠️ Vấn Đề Với Raw Pointers

```cpp
#include <iostream>

// ❌ DANGEROUS: Raw pointer management
class BadResourceManager {
private:
    int* data_;
    
public:
    BadResourceManager(int size) {
        data_ = new int[size];  // Manual allocation
        std::cout << "Allocated " << size << " integers" << std::endl;
    }
    
    ~BadResourceManager() {
        delete[] data_;  // Easy to forget or do incorrectly!
        std::cout << "Memory freed" << std::endl;
    }
    
    // Problems:
    // 1. What if constructor throws after allocation?
    // 2. What about copy constructor/assignment?
    // 3. Exception safety?
    // 4. Double deletion?
};

void dangerous_function() {
    int* ptr = new int(42);
    
    // If exception happens here, memory leak!
    if (some_condition()) {
        throw std::runtime_error("Oops!");  // ptr never deleted!
    }
    
    delete ptr;  // Only executed if no exception
}
```

## 🧠 std::unique_ptr - Single Ownership

### Basic Usage

```cpp
#include <iostream>
#include <memory>
#include <vector>

class Resource {
private:
    std::string name_;
    
public:
    Resource(const std::string& name) : name_{name} {
        std::cout << "Resource created: " << name_ << std::endl;
    }
    
    ~Resource() {
        std::cout << "Resource destroyed: " << name_ << std::endl;
    }
    
    void use() const {
        std::cout << "Using resource: " << name_ << std::endl;
    }
    
    const std::string& get_name() const { return name_; }
};

void demonstrate_unique_ptr() {
    std::cout << "=== std::unique_ptr Demo ===" << std::endl;
    
    // ✅ GOOD: Modern way to create unique_ptr
    auto resource1 = std::make_unique<Resource>("Resource 1");
    
    // Alternative syntax (less preferred)
    std::unique_ptr<Resource> resource2(new Resource("Resource 2"));
    
    // Use like regular pointer
    resource1->use();
    (*resource1).use();  // Same as above
    
    if (resource1) {  // Check if not null
        std::cout << "Resource1 exists: " << resource1->get_name() << std::endl;
    }
    
    // Transfer ownership
    auto resource3 = std::move(resource1);  // resource1 becomes nullptr
    
    if (!resource1) {
        std::cout << "resource1 is now nullptr" << std::endl;
    }
    
    resource3->use();
    
    // Automatic cleanup when scope ends - no memory leaks!
}
```

### 💡 unique_ptr Best Practices

```cpp
#include <memory>
#include <vector>

class SafeResourceManager {
private:
    std::unique_ptr<int[]> data_;  // Array version
    size_t size_;
    
public:
    // ✅ EXCELLENT: Exception-safe constructor
    SafeResourceManager(size_t size) 
        : data_{std::make_unique<int[]>(size)}, size_{size} {
        std::cout << "Safely allocated " << size << " integers" << std::endl;
        
        // Even if exception happens here, data_ is automatically cleaned up
        if (size > 1000000) {
            throw std::invalid_argument("Size too large");
        }
    }
    
    // No need for destructor - unique_ptr handles it!
    
    // Move constructor (transfer ownership)
    SafeResourceManager(SafeResourceManager&& other) noexcept
        : data_{std::move(other.data_)}, size_{other.size_} {
        other.size_ = 0;
    }
    
    // Move assignment
    SafeResourceManager& operator=(SafeResourceManager&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = other.size_;
            other.size_ = 0;
        }
        return *this;
    }
    
    // Copy operations are deleted (unique ownership)
    SafeResourceManager(const SafeResourceManager&) = delete;
    SafeResourceManager& operator=(const SafeResourceManager&) = delete;
    
    // Access methods
    int& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }
    
    size_t size() const { return size_; }
    
    // Release ownership (rarely needed)
    int* release() {
        size_ = 0;
        return data_.release();
    }
    
    // Replace managed object
    void reset(size_t new_size) {
        data_ = std::make_unique<int[]>(new_size);
        size_ = new_size;
    }
};
```

## 🤝 std::shared_ptr - Shared Ownership

### Basic Shared Ownership

```cpp
#include <memory>
#include <iostream>

class ExpensiveResource {
private:
    std::string data_;
    
public:
    ExpensiveResource(const std::string& data) : data_{data} {
        std::cout << "Created expensive resource: " << data_ << std::endl;
    }
    
    ~ExpensiveResource() {
        std::cout << "Destroyed expensive resource: " << data_ << std::endl;
    }
    
    const std::string& get_data() const { return data_; }
};

void demonstrate_shared_ptr() {
    std::cout << "=== std::shared_ptr Demo ===" << std::endl;
    
    // Create shared resource
    auto resource = std::make_shared<ExpensiveResource>("Shared Data");
    std::cout << "Reference count: " << resource.use_count() << std::endl;
    
    {
        // Share ownership
        auto resource_copy = resource;
        std::cout << "Reference count after copy: " << resource.use_count() << std::endl;
        
        // Another copy
        std::shared_ptr<ExpensiveResource> resource_copy2 = resource;
        std::cout << "Reference count with 3 owners: " << resource.use_count() << std::endl;
        
        // All copies point to same object
        std::cout << "Same object? " << (resource.get() == resource_copy.get()) << std::endl;
        
    }  // resource_copy and resource_copy2 destroyed here
    
    std::cout << "Reference count after scope: " << resource.use_count() << std::endl;
    
    // Reset one owner
    resource.reset();
    std::cout << "After reset, object is destroyed" << std::endl;
}
```

### 🚨 Circular Reference Problem

```cpp
#include <memory>
#include <iostream>

// ❌ PROBLEM: Circular reference with shared_ptr
struct BadNode {
    std::shared_ptr<BadNode> next;
    std::shared_ptr<BadNode> parent;  // Creates circular reference!
    
    ~BadNode() {
        std::cout << "BadNode destroyed" << std::endl;  // Might never print!
    }
};

// ✅ SOLUTION: Use weak_ptr for one direction
struct GoodNode {
    std::shared_ptr<GoodNode> next;
    std::weak_ptr<GoodNode> parent;  // Weak reference - no ownership
    
    ~GoodNode() {
        std::cout << "GoodNode destroyed" << std::endl;
    }
};

void demonstrate_circular_reference() {
    {
        // Bad example - memory leak!
        auto bad1 = std::make_shared<BadNode>();
        auto bad2 = std::make_shared<BadNode>();
        bad1->next = bad2;
        bad2->parent = bad1;  // Circular reference - objects never destroyed!
    }
    
    {
        // Good example - proper cleanup
        auto good1 = std::make_shared<GoodNode>();
        auto good2 = std::make_shared<GoodNode>();
        good1->next = good2;
        good2->parent = good1;  // weak_ptr - no circular reference
        
        // Check if parent still exists
        if (auto parent = good2->parent.lock()) {
            std::cout << "Parent still exists" << std::endl;
        }
    }
}
```

## 🔗 std::weak_ptr - Observer Pattern

```cpp
#include <memory>
#include <vector>
#include <iostream>

class Subject;  // Forward declaration

class Observer {
private:
    std::weak_ptr<Subject> subject_;  // Don't keep subject alive
    
public:
    void observe(std::shared_ptr<Subject> subj) {
        subject_ = subj;
    }
    
    void check_subject() {
        if (auto subj = subject_.lock()) {  // Convert to shared_ptr
            std::cout << "Subject is still alive" << std::endl;
        } else {
            std::cout << "Subject has been destroyed" << std::endl;
        }
    }
};

class Subject {
public:
    ~Subject() {
        std::cout << "Subject destroyed" << std::endl;
    }
    
    void notify() {
        std::cout << "Subject notifying observers" << std::endl;
    }
};

void demonstrate_observer_pattern() {
    Observer obs1, obs2;
    
    {
        auto subject = std::make_shared<Subject>();
        
        obs1.observe(subject);
        obs2.observe(subject);
        
        obs1.check_subject();  // Subject exists
        obs2.check_subject();  // Subject exists
        
    }  // subject destroyed here
    
    obs1.check_subject();  // Subject destroyed
    obs2.check_subject();  // Subject destroyed
}
```

## 🏭 Factory Pattern với Smart Pointers

```cpp
#include <memory>
#include <string>
#include <iostream>

// Abstract base class
class Animal {
public:
    virtual ~Animal() = default;
    virtual void make_sound() const = 0;
    virtual std::string get_type() const = 0;
};

// Concrete implementations
class Dog : public Animal {
public:
    void make_sound() const override {
        std::cout << "Woof!" << std::endl;
    }
    
    std::string get_type() const override {
        return "Dog";
    }
};

class Cat : public Animal {
public:
    void make_sound() const override {
        std::cout << "Meow!" << std::endl;
    }
    
    std::string get_type() const override {
        return "Cat";
    }
};

// ✅ EXCELLENT: Factory with smart pointers
class AnimalFactory {
public:
    static std::unique_ptr<Animal> create_animal(const std::string& type) {
        if (type == "dog") {
            return std::make_unique<Dog>();
        } else if (type == "cat") {
            return std::make_unique<Cat>();
        }
        return nullptr;
    }
    
    // For shared ownership scenarios
    static std::shared_ptr<Animal> create_shared_animal(const std::string& type) {
        if (type == "dog") {
            return std::make_shared<Dog>();
        } else if (type == "cat") {
            return std::make_shared<Cat>();
        }
        return nullptr;
    }
};

void demonstrate_factory_pattern() {
    std::cout << "=== Smart Pointer Factory Pattern ===" << std::endl;
    
    // Create animals using factory
    auto dog = AnimalFactory::create_animal("dog");
    auto cat = AnimalFactory::create_animal("cat");
    auto unknown = AnimalFactory::create_animal("bird");
    
    if (dog) {
        std::cout << "Created: " << dog->get_type() << std::endl;
        dog->make_sound();
    }
    
    if (cat) {
        std::cout << "Created: " << cat->get_type() << std::endl;
        cat->make_sound();
    }
    
    if (!unknown) {
        std::cout << "Unknown animal type not supported" << std::endl;
    }
    
    // Automatic cleanup - no memory management needed!
}
```

## 🎯 Custom Deleters

```cpp
#include <memory>
#include <iostream>
#include <cstdio>

// Custom deleter for C resources
struct FileDeleter {
    void operator()(FILE* file) const {
        if (file) {
            std::cout << "Closing file" << std::endl;
            std::fclose(file);
        }
    }
};

// Type alias for convenience
using FilePtr = std::unique_ptr<FILE, FileDeleter>;

// Custom deleter for arrays (though prefer std::vector)
struct ArrayDeleter {
    void operator()(int* ptr) const {
        std::cout << "Deleting array with delete[]" << std::endl;
        delete[] ptr;
    }
};

void demonstrate_custom_deleters() {
    std::cout << "=== Custom Deleters Demo ===" << std::endl;
    
    // File management with custom deleter
    {
        FilePtr file(std::fopen("temp.txt", "w"));
        if (file) {
            std::fprintf(file.get(), "Hello, Smart Pointers!");
            std::cout << "File written successfully" << std::endl;
        }
        // File automatically closed when file goes out of scope
    }
    
    // Array with custom deleter
    {
        std::unique_ptr<int[], ArrayDeleter> arr(new int[10]);
        for (int i = 0; i < 10; ++i) {
            arr[i] = i * i;
        }
        // Array automatically deleted with delete[] when arr goes out of scope
    }
    
    // Lambda deleter
    {
        auto lambda_deleter = [](int* ptr) {
            std::cout << "Lambda deleter called" << std::endl;
            delete ptr;
        };
        
        std::unique_ptr<int, decltype(lambda_deleter)> ptr(new int(42), lambda_deleter);
        std::cout << "Value: " << *ptr << std::endl;
    }
}
```

## 🏆 Practical Example: RAII Resource Manager

```cpp
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/**
 * @brief A comprehensive example showing smart pointers in action
 * Demonstrates RAII, exception safety, and modern C++ best practices
 */
class DatabaseConnection {
private:
    std::string connection_string_;
    bool is_connected_;
    
public:
    DatabaseConnection(const std::string& conn_str) 
        : connection_string_{conn_str}, is_connected_{false} {
        // Simulate connection
        std::cout << "Connecting to: " << conn_str << std::endl;
        is_connected_ = true;
    }
    
    ~DatabaseConnection() {
        if (is_connected_) {
            std::cout << "Disconnecting from: " << connection_string_ << std::endl;
        }
    }
    
    void execute_query(const std::string& query) {
        if (!is_connected_) {
            throw std::runtime_error("Not connected to database");
        }
        std::cout << "Executing: " << query << std::endl;
    }
    
    bool is_connected() const { return is_connected_; }
};

class ResourceManager {
private:
    std::vector<std::shared_ptr<DatabaseConnection>> connections_;
    std::unique_ptr<std::ofstream> log_file_;
    
public:
    ResourceManager(const std::string& log_filename) 
        : log_file_{std::make_unique<std::ofstream>(log_filename)} {
        if (!log_file_->is_open()) {
            throw std::runtime_error("Failed to open log file");
        }
        log("ResourceManager initialized");
    }
    
    std::shared_ptr<DatabaseConnection> get_connection(const std::string& conn_str) {
        // Check if connection already exists
        for (auto& conn : connections_) {
            if (conn && conn->is_connected()) {
                log("Reusing existing connection");
                return conn;
            }
        }
        
        // Create new connection
        auto new_conn = std::make_shared<DatabaseConnection>(conn_str);
        connections_.push_back(new_conn);
        log("Created new connection");
        return new_conn;
    }
    
    void cleanup_dead_connections() {
        auto it = std::remove_if(connections_.begin(), connections_.end(),
            [](const std::weak_ptr<DatabaseConnection>& weak_conn) {
                return weak_conn.expired();
            });
        connections_.erase(it, connections_.end());
        log("Cleaned up dead connections");
    }
    
private:
    void log(const std::string& message) {
        if (log_file_ && log_file_->is_open()) {
            *log_file_ << message << std::endl;
            log_file_->flush();
        }
    }
};

void demonstrate_resource_manager() {
    try {
        auto manager = std::make_unique<ResourceManager>("app.log");
        
        {
            auto conn1 = manager->get_connection("database1");
            auto conn2 = manager->get_connection("database2");
            auto conn3 = manager->get_connection("database1");  // Should reuse conn1
            
            conn1->execute_query("SELECT * FROM users");
            conn2->execute_query("SELECT * FROM products");
            
            // Connections automatically managed by shared_ptr
        }
        
        manager->cleanup_dead_connections();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    // All resources automatically cleaned up
}
```

## 🎓 Key Takeaways

### ✅ Best Practices

1. **Prefer make_unique/make_shared** over `new`
2. **Use unique_ptr by default** - only use shared_ptr when needed
3. **Use weak_ptr** to break circular references
4. **Custom deleters** for non-standard resources
5. **RAII principle** - Resource Acquisition Is Initialization
6. **Exception safety** - Smart pointers provide automatic cleanup

### 🚨 Common Pitfalls

1. **Circular references** with shared_ptr
2. **Mixing raw and smart pointers**
3. **Using shared_ptr when unique_ptr is sufficient**
4. **Not using make_shared/make_unique**
5. **Storing weak_ptr without checking expiration**

### 📊 Performance Notes

- `unique_ptr`: Zero overhead compared to raw pointers
- `shared_ptr`: Small overhead for reference counting
- `weak_ptr`: Similar to shared_ptr but doesn't affect lifetime
- `make_shared`: More efficient than `shared_ptr(new T())`

## ➡️ Next Topics

Trong các chương tiếp theo:
- **Move Semantics & Perfect Forwarding**
- **Lambda Expressions**
- **Auto và Type Deduction**
- **Variadic Templates**

---

*"Smart pointers are not just about memory management - they're about expressing ownership semantics clearly in your code." - Modern C++ Philosophy*