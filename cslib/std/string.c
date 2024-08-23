#include "cslib/std/string.h"


/*
This function removes the prefix (beginning of the string) if the prefix is in it.
Make sure to pass a modifiable string! char *literal = ".." does not count.
Returns true if removed the prefix, else returns false.

NOTE: To just check if the prefix exists use the cslib_isprefix function.
*/
bool cslib_remove_prefix(char *string, const char *prefix)
{
    const size_t psize = strlen(prefix);
    const size_t ssize = strlen(string);

    // if the string size is 
    if ( ssize < psize )
    {
        return false;
    }

    // checks if the string has that prefix
    if ( memcmp(prefix, string, psize) != 0 )
    {
        return false;
    }

    // removes that prefix
    memmove(string, string + psize,  ssize - psize + 1);
   
    return true;
}


/*
Checks if it is the prefix of string

Returns true if yes, else false.
*/
bool cslib_check_prefix(char *string, const char *prefix)
{
    return memcmp(prefix, string, strlen(prefix)) == 0;
}

/*
Allocates memory for a string from a literal
*/
char* cslib_allocate_string(char *data, size_t min_size)
{
    size_t datasize = strlen(data);
    size_t alloc_size = min_size > datasize ? min_size : datasize;

    // +1 for the null terminator
    char *s = (char*)malloc(sizeof(char) * alloc_size + 1);
    strncpy(s, data, datasize + 1);

    return s;
}
