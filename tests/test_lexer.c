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

TEST("test all keywords"){
    // A list of all keywords in alphabetical order
    FILE *fp2 = fopen("tests/all_keywords.decaf", "r"); // Macro is called in test_decafc.c
    ASSERT(fp2 != NULL);

    ASSERT(read_token(fp2).type == BOOL_K);
    ASSERT(read_token(fp2).type == BREAK_K);
    ASSERT(read_token(fp2).type == CONTINUE_K);
    ASSERT(read_token(fp2).type == ELSE_K);
    ASSERT(read_token(fp2).type == FALSE_K);
    ASSERT(read_token(fp2).type == FOR_K);
    ASSERT(read_token(fp2).type == IF_K);
    ASSERT(read_token(fp2).type == IMPORT_K);
    ASSERT(read_token(fp2).type == INT_K);
    ASSERT(read_token(fp2).type == LEN_K);
    ASSERT(read_token(fp2).type == RETURN_K);
    ASSERT(read_token(fp2).type == TRUE_K);
    ASSERT(read_token(fp2).type == VOID_K);
    ASSERT(read_token(fp2).type == WHILE_K);
}
#endif
