#if defined HEADERS
// All headers that are needed
#include "../decafc.c"
#elif defined TESTS
TEST("read_word reads one word"){
    FILE *fp1 = fopen("tests/english.decaf", "r"); // Macro is called in test_decafc.c
    ASSERT(fp1 != NULL);
    ASSERT(strcmp(read_word(fp1), "Hello")==0);
    fclose(fp1);
}

TEST("read_word reads one word"){
    ASSERT(true);
}
#endif
