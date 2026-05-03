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

TEST("test all symbols"){
    // A list of all keywords in alphabetical order
    FILE *fp3 = fopen("tests/all_symbols.decaf", "r"); // Macro is called in test_decafc.c
    ASSERT(fp3 != NULL);

    // Order is dictated by the testing file
    ASSERT(read_token(fp3).type == DIV);
    ASSERT(read_token(fp3).type == DOT);
    ASSERT(read_token(fp3).type == EQUALS);
    ASSERT(read_token(fp3).type == MINUS);
    ASSERT(read_token(fp3).type == PLUS);
    ASSERT(read_token(fp3).type == TIMES);
    ASSERT(read_token(fp3).type == MOD);
    ASSERT(read_token(fp3).type == GT);
    ASSERT(read_token(fp3).type == LT);
    ASSERT(read_token(fp3).type == L_PAREN);
    ASSERT(read_token(fp3).type == R_PAREN);
    ASSERT(read_token(fp3).type == L_BRACE);
    ASSERT(read_token(fp3).type == R_BRACE);
    ASSERT(read_token(fp3).type == Q_MARK);
    ASSERT(read_token(fp3).type == BANG);
    ASSERT(read_token(fp3).type == COL);
    ASSERT(read_token(fp3).type == SEMI_COL);
    ASSERT(read_token(fp3).type == QUOTE);
    ASSERT(read_token(fp3).type == QUOTE_D);

    ASSERT(read_token(fp3).type == GT_E);
    ASSERT(read_token(fp3).type == LT_E);
    ASSERT(read_token(fp3).type == EE);
    ASSERT(read_token(fp3).type == NE);
    ASSERT(read_token(fp3).type == AND);
    ASSERT(read_token(fp3).type == OR);
}
#endif
