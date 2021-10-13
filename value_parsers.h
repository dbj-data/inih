#pragma once
/*
* mainly nicked from github and/or refactored to standard C, aka C11
*/

#include <stdbool.h> // dbj refactoring starts here ;)
#include <assert.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include <wctype.h> 

// assert if precondtion is not met
#ifdef _DEBUG
#define DBJ_PRECONDITION_( S_ ) \
do { \
	char * zero_ = strchar(S_,0 ); \
	assert(S_ != 0); \
	assert(false == iswspace(S_[0])); \
	assert(false == iswspace(--zero_)); \
} while (0)
#else
#define DBJ_PRECONDITION_( S_ )((void)0)
#endif // ! _DEBUG

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a boolean
  @param    ret_val a bool value found
  @param    val is string to parse
  @return   0 or EINVAL

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.

  A true boolean is found if one of the following is matched:

  - A string starting with 'y'
  - A string starting with 'Y'
  - A string starting with 't'
  - A string starting with 'T'
  - A string starting with '1'

  A false boolean is found if one of the following is matched:

  - A string starting with 'n'
  - A string starting with 'N'
  - A string starting with 'f'
  - A string starting with 'F'
  - A string starting with '0'

  The notfound value returned if no boolean is identified, does not
  necessarily have to be 0 or 1.
 */
 /*--------------------------------------------------------------------------*/
static inline int dbj_getboolean(bool* retval, const char* val)
{
	int          ret = 0;
	const char* c = val;

	// not necessary DBJ_PRECONDITION_(key);

	if (c[0] == 'y' || c[0] == 'Y' || c[0] == '1' || c[0] == 't' || c[0] == 'T') {
		*retval = true;
		return 0;
	}
	else if (c[0] == 'n' || c[0] == 'N' || c[0] == '0' || c[0] == 'f' || c[0] == 'F') {
		*retval = false;
		return 0;
	}

	return EINVAL;
}

#undef DBJ_PRECONDITION_



