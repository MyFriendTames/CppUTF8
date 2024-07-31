#ifndef MYFRIEND_TAMES_CPPUTF8
#define MYFRIEND_TAMES_CPPUTF8

#include <cstdint>

uint32_t UTF8ToUnicode( const uint8_t*& );
const char* UnicodeToUTF8( uint32_t );

#endif