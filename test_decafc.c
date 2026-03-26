/*
 * This was pretty much pulled off of reddit cause I don't fully understand macros yet.
 *  I would like to explore embedding tests into the source code of each file.
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TESTBUILD

#define HEADERS
#include "tests/test_all.c"
#undef HEADERS

// TODO add a compiletime VERBOSE mode, Total tests run, did they pass etc
#define TEST(name) test = name;
#define ASSERT(ast)\
    do{ \
        assertion = #ast; \
        file = __FILE__; \
        line = __LINE__; \
        if(ast) putchar('.'); else goto fail; \
    } while(0)

int main(){
    const char *test = "";
    const char *assertion = "";
    const char *file = "";
    int line = 0;

    #define TESTS
    #include "tests/test_all.c"
    #undef TESTS
    printf("\nAll tests pass\n");
    return 0;

fail:
    printf("\n%s - failed \t (%s) is false \n \t file: %s, line: %i\n", test, assertion, file, line);
    return 1;

}
