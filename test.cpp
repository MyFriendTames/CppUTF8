#include <clocale>
#include <iostream>
#include <string>
#include <bitset>

#include "CppUTF8.hpp"

int main(){
    std::setlocale( LC_ALL, "en_US.UTF-8" );
    
    const char * text = "á";
    
    uint8_t a = text[ 0 ] ^ 0b11000000;
    uint8_t b = text[ 1 ] ^ 0b10000000;
    
    std::cout << "( á ) Unicode: 225 ( " << std::bitset< 16 >( 225 ) << " )" << std::endl;
    std::cout << " a: " << std::bitset< 8 >( a ).to_string() << " ( " << std::bitset< 8 >( text[ 0 ] ) << " )" << std::endl;
    std::cout << " b: " << std::bitset< 8 >( b ).to_string() << " ( " << std::bitset< 8 >( text[ 1 ] ) << " )" << std::endl;
    
    uint16_t c = a;
    c <<= 6;
    c |= b;
    
    std::cout << " c: " << ( int )c << " ( " << std::bitset< 16 >( c ) << " ) -> ";
    std::wcout << ( wchar_t )c << std::endl;
    
    std::cout << ( uint32_t )0x10FFFF << " - " << std::bitset< 32 >( 0x10FFFF ) << std::endl;
    
    const char* txt = "abcÁÉÍÓÚáéíóú€öô";
    const uint8_t* chr = reinterpret_cast< const uint8_t* >( txt );
    
    std::cout << "Input: " << txt << std::endl;
    
    while ( *chr ){
        uint32_t code = UTF8ToUnicode( chr );
        std::cout << code << " ) -> " << UnicodeToUTF8( code ) << std::endl;
    }
    
    std::cout << UnicodeToUTF8( 12320 ) << std::endl;
    
    return 0;
}
