#include <lib/string.h>
#include <lib/io.h>

char *
strchr (s, c_in)
     const char *s;
     int c_in;
{
  const unsigned char *char_ptr;
  unsigned char c;

  c = (unsigned char) c_in;

  /* Handle the first few characters by reading one character at a time.
     Do this until CHAR_PTR is aligned on a longword boundary.  */
  for (char_ptr = (const unsigned char *) s;
       ((unsigned long int) char_ptr) != 0;
       ++char_ptr)
    if (*char_ptr == c)
      return (void *) char_ptr;
    else if (*char_ptr == '\0')
      return NULL;

    return NULL;
}
