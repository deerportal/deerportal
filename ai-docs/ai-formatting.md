# DeerPortal C++ Code Formatting Standard

## 📋 Overview

This document defines the consistent code formatting standard for the DeerPortal SFML project. All C++ source files (.cpp and .h) should follow these conventions to maintain code readability and consistency.

## 🎯 Core Principles

1. **Consistency First**: All code should follow the same patterns
2. **Readability**: Code should be easy to read and understand
3. **Modern C++**: Follow C++17 best practices
4. **SFML Integration**: Respect SFML coding conventions

## 📐 Indentation & Spacing

### Indentation
- **Standard**: 4 spaces (no tabs)
- **Consistency**: All nested blocks use 4-space indentation
- **Namespace**: Content inside namespaces is NOT indented

```cpp
namespace DP {
// NO indentation for namespace content
class Game {
    // 4 spaces for class content
    void function() {
        // 4 spaces for function content
        if (condition) {
            // 4 spaces for nested blocks
        }
    }
};
}
```

### Line Spacing
- **Function Separation**: 1 blank line between functions
- **Logical Sections**: 1 blank line between logical code sections
- **Class Sections**: 1 blank line between public/private/protected sections
- **No Excessive Spacing**: Avoid multiple consecutive blank lines

## 🔧 Braces & Brackets

### Brace Style (Allman/BSD Style)
- **Opening Brace**: Always on new line, same indentation as statement
- **Closing Brace**: Always on new line, same indentation as opening statement
- **Single Statements**: Use braces even for single-line if/for/while statements

```cpp
// ✅ Correct
if (condition)
{
    statement;
}

// ✅ Correct for single statement
if (condition)
{
    singleStatement;
}

// ❌ Incorrect
if (condition) {
    statement;
}

// ❌ Incorrect (missing braces)
if (condition)
    statement;
```

### Constructor Initializer Lists
```cpp
// ✅ Correct
Constructor(int param1, int param2)
    : member1(param1)
    , member2(param2)
{
    // Constructor body
}
```

## 🏷️ Naming Conventions

### Variables & Functions
- **camelCase**: `playerHealth`, `updatePosition()`
- **Descriptive**: Use clear, meaningful names
- **No Hungarian notation**: Avoid prefixes like `int iCount`

### Classes & Structs
- **PascalCase**: `GameBoard`, `PlayerCharacter`
- **Descriptive**: Clear purpose indication

### Constants & Enums
- **SCREAMING_SNAKE_CASE**: `MAX_PLAYERS`, `TILE_SIZE`
- **Enum values**: Use scoped enums with descriptive names

```cpp
enum class PlayerState
{
    WAITING,
    ROLLING_DICE,
    MOVING,
    FINISHED
};
```

## 🔗 Operators & Expressions

### Spacing Around Operators
```cpp
// ✅ Correct
int result = a + b * c;
if (x == y && z != w)
{
    // statements
}

// ❌ Incorrect
int result=a+b*c;
if(x==y&&z!=w)
{
    // statements
}
```

### Function Calls
```cpp
// ✅ Correct
function(param1, param2, param3);
object.method(argument);

// ❌ Incorrect
function( param1,param2 ,param3 );
object.method(argument);
```

## 📝 Comments & Documentation

### Doxygen Comments
```cpp
/*!
 * \brief Brief description of the function
 * \param paramName Description of parameter
 * \return Description of return value
 */
int function(int paramName);
```

### Inline Comments
- **Single Line**: Use `//` with space after
- **End of Line**: Align comments when multiple lines have them
- **Explanatory**: Focus on WHY, not WHAT

```cpp
// Initialize the game board
initBoard();

int playerCount = 4;    // Maximum supported players
int currentTurn = 0;    // Zero-based turn index
```

## 🗂️ File Organization

### Header Files (.h)
```cpp
#ifndef FILENAME_H
#define FILENAME_H

// System includes
#include <memory>
#include <vector>

// Third-party includes
#include <SFML/Graphics.hpp>

// Project includes
#include "otherfile.h"

namespace DP {

class ClassName
{
public:
    // Public members

private:
    // Private members
};

} // namespace DP

#endif // FILENAME_H
```

### Source Files (.cpp)
```cpp
#include "header.h"

// System includes (if needed)
#include <algorithm>
#include <stdexcept>

// Third-party includes (if needed)

// Other project includes
#include "dependency.h"

namespace DP {

// Implementation

} // namespace DP
```

## 🎮 SFML-Specific Conventions

### Smart Pointers
```cpp
// ✅ Correct
std::unique_ptr<sf::Text> textPtr = std::make_unique<sf::Text>();

// ✅ Correct initialization
textPtr->setFont(font);
textPtr->setString("Hello");
```

### Vector Operations
```cpp
// ✅ Correct
sf::Vector2f position(100.0f, 200.0f);
sprite->setPosition(position);

// ✅ Correct with direct construction
sprite->setPosition(sf::Vector2f(x, y));
```

## 🚨 Error Handling

### Exception Handling
```cpp
// ✅ Correct
if (!texture.resize(sf::Vector2u(width, height)))
{
    throw std::runtime_error("Failed to resize texture");
}
```

### Return Value Checking
```cpp
// ✅ Correct for SFML 3.0 nodiscard functions
if (!resource.loadFromFile(filename))
{
    // Handle error appropriately
    return false;
}
```

## 📦 Class Structure

### Member Order
1. Public constructors/destructor
2. Public methods
3. Public static methods
4. Public members (if any)
5. Protected methods
6. Protected members
7. Private methods
8. Private members

### Access Specifiers
```cpp
class Example
{
public:
    Example();
    ~Example();

    void publicMethod();

protected:
    void protectedMethod();

private:
    void privateMethod();
    int privateMember;
};
```

## ⚡ Performance Guidelines

### Pass by Reference
```cpp
// ✅ Correct for large objects
void function(const sf::Texture& texture);
void function(const std::string& text);

// ✅ Correct for modification
void function(sf::Vector2f& position);
```

### Move Semantics
```cpp
// ✅ Correct
players[0] = std::move(newPlayer);
textVector.push_back(std::move(textObject));
```

## 🔧 Tools & Enforcement

### Recommended Settings
- **Editor**: 4-space indentation, no tabs
- **Line Endings**: Unix (LF) for cross-platform compatibility
- **Encoding**: UTF-8

### Code Validation
- All code should compile without warnings
- Follow const-correctness principles
- Use modern C++17 features appropriately

## 📋 Examples

### Before Formatting
```cpp
namespace DP{
class Game{
public:
Game();
void update( sf::Time frameTime );
private:
int turn;bool gameEnded;
std::vector<Player>players;
};}
```

### After Formatting
```cpp
namespace DP {

class Game
{
public:
    Game();
    void update(sf::Time frameTime);

private:
    int turn;
    bool gameEnded;
    std::vector<Player> players;
};

} // namespace DP
```

## ✅ Checklist for Code Review

- [ ] 4-space indentation consistently applied
- [ ] Braces on new lines (Allman style)
- [ ] Proper spacing around operators
- [ ] Consistent naming conventions
- [ ] No trailing whitespace
- [ ] Single blank lines for separation
- [ ] Proper include organization
- [ ] SFML objects using smart pointers
- [ ] Error handling for nodiscard functions
- [ ] Documentation for public interfaces

---

**Note**: This standard prioritizes readability and consistency. When in doubt, follow the principle of making code as clear and maintainable as possible. 