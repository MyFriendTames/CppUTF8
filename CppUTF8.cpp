#include "CppUTF8.hpp"

uint32_t UTF8ToUnicode( const uint8_t*& chr ){
    static uint8_t continuation = 0b00111111;
    static uint8_t singleByte = 0b00000000;
    static uint8_t twoBytes = 0b11000000;
    static uint8_t threeBytes = 0b11100000;
    static uint8_t fourBytes = 0b11110000;
    static uint32_t replacement = 0xFFFD;
    uint32_t code = replacement;
    if ( *chr < 0b11000000 ){ // ----------------- Single Byte
        code = *( chr++ );
    }else if ( *chr < 0b11100000 ){ // ----------- Two Bytes
        code = *( chr++ ) & 0b00011111;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) & continuation;
        }else return replacement;
    }else if ( *chr < 0b11110000 ){ // ----------- Three Bytes
        code = *( chr++ ) & 0b00001111;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) & continuation;
        }else return replacement;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) & continuation;
        }else return replacement;
    }else if ( *chr <= 0b11110111 ){ // ---------- Four Bytes
        code = *( chr++ ) & 0b00000111;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) & continuation;
        }else return replacement;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) & continuation;
        }else return replacement;
        if ( *chr && *chr >> 6 == 2 ){
            code <<= 6;
            code |= *( chr++ ) & continuation;
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