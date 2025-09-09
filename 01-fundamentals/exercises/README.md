# Bài Tập Thực Hành - Module 1: Fundamentals

## 📚 Hướng Dẫn Làm Bài

1. **Đọc kỹ đề bài** và yêu cầu
2. **Viết code** trong thư mục này
3. **Compile và test** chương trình của bạn
4. **So sánh** với solution trong thư mục `/solutions`
5. **Không nhìn solution** trước khi hoàn thành!

## 🎯 Bài Tập

### Bài 1: Personal Information Display (Cơ bản)
**File:** `exercise1_personal_info.cpp`

Viết chương trình hiển thị thông tin cá nhân với format sau:
```
=== Personal Information ===
Name: [Your Name]
Age: [Your Age]
Student ID: [Your ID]
GPA: [Your GPA]
Major: Computer Science
```

**Yêu cầu:**
- Sử dụng appropriate data types cho mỗi thông tin
- Comment giải thích tại sao chọn data type đó
- Sử dụng `const` cho thông tin không đổi

### Bài 2: Data Type Explorer (Trung bình)
**File:** `exercise2_type_explorer.cpp`

Viết chương trình:
1. Khai báo variables với tất cả fundamental data types
2. In ra size của mỗi type
3. In ra min/max values của integer types
4. Demonstate type conversion (cả implicit và explicit)

**Output mẫu:**
```
Type        Size    Min Value       Max Value
char        1       -128            127
int         4       -2147483648     2147483647
...
```

### Bài 3: Calculator Basics (Trung bình)
**File:** `exercise3_calculator.cpp`

Tạo basic calculator với:
- Input: 2 numbers và 1 operator (+, -, *, /)
- Output: Kết quả phép tính
- Handle division by zero
- Sử dụng appropriate data types

**Bonus:** Thêm support cho modulus (%) operator

### Bài 4: Temperature Converter (Trung bình)
**File:** `exercise4_temperature.cpp`

Viết chương trình convert temperature giữa:
- Celsius ↔ Fahrenheit
- Celsius ↔ Kelvin
- Fahrenheit ↔ Kelvin

**Công thức:**
- F = C × 9/5 + 32
- K = C + 273.15
- C = (F - 32) × 5/9

### Bài 5: Variable Scope Investigation (Khó)
**File:** `exercise5_scope.cpp`

Tạo chương trình demonstrate:
- Global variables
- Local variables trong function
- Block scope variables
- Variable shadowing
- Constant variables ở different scopes

In ra values của variables ở different points để show scope effects.

### Bài 6: Floating-Point Precision Test (Khó)
**File:** `exercise6_precision.cpp`

Viết chương trình:
1. Demonstrate floating-point precision issues
2. Implement safe floating-point comparison function
3. Test với different epsilon values
4. Compare `float` vs `double` precision

**Test cases:**
- 0.1 + 0.2 == 0.3
- 1.0 / 3.0 * 3.0 == 1.0
- Large number + small number precision loss

### Bài 7: Memory Layout Analyzer (Rất khó)
**File:** `exercise7_memory.cpp`

Advanced exercise:
1. Khai báo variables của different types
2. In ra addresses của variables (sử dụng `&` operator)
3. Calculate và verify memory layout
4. Demonstrate how different compilers might arrange memory

**Bonus:** Research và explain memory alignment.

## 🔧 Compilation Instructions

```bash
# Basic compilation
g++ -o output_name source_file.cpp

# With warnings và debug info (RECOMMENDED)
g++ -std=c++17 -Wall -Wextra -g -o output_name source_file.cpp

# With optimization
g++ -std=c++17 -Wall -Wextra -O2 -o output_name source_file.cpp
```

## ✅ Checklist Hoàn Thành

- [ ] Bài 1: Personal Information Display
- [ ] Bài 2: Data Type Explorer  
- [ ] Bài 3: Calculator Basics
- [ ] Bài 4: Temperature Converter
- [ ] Bài 5: Variable Scope Investigation
- [ ] Bài 6: Floating-Point Precision Test
- [ ] Bài 7: Memory Layout Analyzer

## 💡 Tips Từ Giảng Viên

1. **Luôn compile với warnings** - Chúng sẽ catch nhiều lỗi cơ bản
2. **Test edge cases** - Zero, negative numbers, maximum values
3. **Use meaningful variable names** - Code phải self-documenting
4. **Comment your reasoning** - Giải thích WHY, không phải WHAT
5. **Experiment!** - Thay đổi values và xem điều gì xảy ra

## 🚨 Common Mistakes Cần Tránh

1. **Uninitialized variables** - Luôn initialize variables
2. **Integer overflow** - Check bounds trước khi arithmetic
3. **Floating-point equality** - Sử dụng epsilon comparison
4. **Wrong data type choice** - `int` vs `unsigned int`, `float` vs `double`
5. **Variable shadowing** - Tránh trùng tên trong nested scopes

---

**Good luck và happy coding! 🚀**