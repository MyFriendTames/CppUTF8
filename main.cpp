#include <clocale>
#include <iostream>
#include <cstdint>
#include <string>
#include <bitset>

uint32_t UTF8ToUnicode( const uint8_t*& chr ){
    static uint8_t continuation = 0b10000000;
    static uint8_t singleByte = 0b00000000;
    static uint8_t twoBytes = 0b11000000;
    static uint8_t threeBytes = 0b11100000;
    static uint8_t fourBytes = 0b11110000;
    static uint32_t replacement = 0xFFFD;
    uint32_t code = replacement;
    if ( *chr < twoBytes ){
        code = *( chr++ );
    }else if ( *chr < threeBytes ){
        code = *( chr++ ) ^ twoBytes;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) ^ continuation;
        }else return replacement;
    }else if ( *chr < fourBytes ){
        code = *( chr++ ) ^ threeBytes;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) ^ continuation;
        }else return replacement;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) ^ continuation;
        }else return replacement;
    }else if ( *chr <= fourBytes | 0b111 ){
        code = *( chr++ ) ^ fourBytes;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) ^ continuation;
        }else return replacement;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) ^ continuation;
        }else return replacement;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) ^ continuation;
        }else return replacement;
    }
    return code;
}

const char* UnicodeToUTF8( uint32_t code ){
    static uint8_t continuation = 0b10000000;
    static uint8_t continuationWildcard = 0b00111111;
    static const char* replacement = "�";
    static char chr [] = { 0, 0, 0, 0, 0 };
    if ( code <= 0x007F ){
        chr[ 0 ] = code;
        chr[ 1 ] = 0;
    }else if ( code <= 0x07FF ){
        chr[ 1 ] = code & continuationWildcard | continuation;
        code >>= 6;
        chr[ 0 ] = code & 0b00011111 | 0b11000000;
        chr[ 2 ] = 0;
    }else if ( code <= 0xFFFF ){
        chr[ 2 ] = code & continuationWildcard | continuation;
        chr[ 1 ] = ( code >>= 6 ) & continuationWildcard | continuation;
        chr[ 0 ] = ( code >>= 6 ) & 0b00001111 | 0b11100000;
        chr[ 3 ] = 0;
    }else if ( code <= 0x10FFFF ){
        chr[ 3 ] = code & continuationWildcard & continuation;
        chr[ 2 ] = ( code >>= 6 ) & continuationWildcard | continuation;
        chr[ 1 ] = ( code >>= 6 ) & continuationWildcard | continuation;
        chr[ 0 ] = ( code >>= 6 ) & 0b00000111 | 0b11110000;
        chr[ 4 ] = 0;
    }else{
        return replacement;
    }
    return chr;
}

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
    
    const char* txt = "abcÁÉÍÓÚáéíóú€";
    const uint8_t* chr = reinterpret_cast< const uint8_t* >( txt );
    
    std::cout << "Input: " << txt << std::endl;
    
    while ( *chr ){
        uint32_t code = UTF8ToUnicode( chr );
        std::cout << code << " ) -> " << UnicodeToUTF8( code ) << std::endl;
    }
    
    std::cout << UnicodeToUTF8( 12320 ) << std::endl;
    
    return 0;
}
