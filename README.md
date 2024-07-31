# CppUTF8

> Simple UTF-8 library for C++

## Content
- CppUTF8.hpp `library header file`
- CppUTF8.cpp `library compilation unit file`
- test.cpp `example code`

## Docs
This library consists of only two functions:
- **UTF8ToUnicode**
    
    > Reads one character from a utf-8 encoded string and moves
    > the pointer to the next character.
    
    - Parameter (`const uint8_t*&`): utf-8 encoded string
    - Returns (`uint32_t`): decoded unicode character
- **UnicodeToUTF8**

    > Reads one unicode character and returns it as a
    > utf-8 encoded character.

    - Parameter (`uint32_t`): unicode character
    - Returns (`const char*`): utf-8 encoded string