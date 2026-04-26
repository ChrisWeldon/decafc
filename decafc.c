#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* 
 * Lexer/Scanner for decaf
 * 
 */

/*
 * What I am doing next:
 *  - Adding more symbols to the lexer
 *  - Figuring our a better TDD framework
 */


/*
 * Reads in a word character by character, growing the bufsize as word expands
 *  TODO: Shrink the word as we go
 *
 * fp: A file pointer which points to the input text file to be compiled
 * 
 * returns: a (char *) read out. If it is EOF then returns an empty string.
*/
bool is_whitespace(int c){
    return c == ' ' || c == EOF || c == '\n';
}

char * read_word(FILE *fp){
    
    int bufsize = 4; // 4 chars to start
    int offset = 0;
    int c;
    
    char* buf = malloc(bufsize);

    if(buf == NULL) // rare but checking anyways
        return NULL;
    
    while(c = fgetc(fp), !is_whitespace(c)){ // TODO make is_space function 
        if(offset == bufsize-1){
            bufsize = bufsize*2;
            buf = realloc(buf, bufsize);
        }
        buf[offset] = c;
        offset++;
    }

    buf[offset] = '\0'; 
    return buf;
}

/*
 * Reads in all the whitespace
 */
void skip_whitespace(FILE *fp){
    // assumes we were at whitesace
    int c;
    do{
        c = fgetc(fp);
    }while(c == ' ' || c == '\n');
    ungetc(c, fp);
}


// Tokens have an ID given to them by order, this id is then used to identify which token type they are
//  I think I should give them a more robust ID that is hard assigned so all my tests don't break often
typedef enum token_type {
    // Multichar Word (space surrounded)
    UNDET,      // Not yet determined, Probably won't need this 
    ROLLBACK,   // Guessed at a keyword or something, but was wrong
    ERROR_TOK,  // Microsyntax failure
    //
    BOOL_K,     // bool
    BREAK_K,    // break
    CONTINUE_K, // continue
    ELSE_K,     // else
    FALSE_K,    // false
    FOR_K,      // for
    IF_K,       // if   (not considering this "double char" cause i does not start any doubles)
    IMPORT_K,   // import
    INT_K,      // int 
    LEN_K,      // len
    RETURN_K,   // return
    TRUE_K,     // true
    VOID_K,     // void
    WHILE_K,    // while
    
    IDENT,
    STRING,     // /\w+/    (basically any keyword)
    INT,        // /\d+/
    
    // Single char
    HEX,        // /0x(\d+)/
    DIV,        // /
    DOT,        // .
    MINUS,      // -
    PLUS,       // +
    TIMES,      // *
    MOD,        // %
    GT,         // >
    LT,         // <
    L_PAREN,    // (
    R_PAREN,    // )
    L_BRACE,    // [
    R_BRACE,    // ]
    Q_MARK,     // ?
    BANG,       // !
    NEG,        // -
    COL,        // :
    SEMI_COL,   // ;
    QUOTE,      // '
    QUOTE_D,    // "
    // Double char
    GT_E,       // >=
    LT_E,       // <=
    EE,         // ==
    NE,         // !=
    AND,        // &&
    OR          // ||
} token_type;

typedef struct token {
    token_type type; 
    int bufsize;
    int offset;
    char *lexeme;
} token;


void s0(FILE *fp, token *tok); // Start of token, could be anything
void ident(FILE *fp, token *tok); // Any letter, going to be an ident
void keyword(FILE *fp, token *tok, token_type word, char * rem);
int fpeek(FILE *fp);

const char * remaining_string(int i, char * string);

token read_token(FILE *fp){
    int bufsize = 8; // non-dynamic for the time being
    int offset = 0;

    token tok = {
        .lexeme = malloc(bufsize),
        .bufsize=bufsize,
        .offset=0,
        .type=UNDET
    };
    s0(fp, &tok);
    return tok;
}

void token_addchar(char c, token *tok){
    tok->lexeme[tok->offset] = c;
    tok->offset++;
}

void token_removechar(token *tok){
    tok->offset--;
}

int current_char(token *tok){
    return tok->lexeme[tok->offset-1]; 
}

int fpeek(FILE *fp){
    /* This function is used in place of a one state rollback */
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}


void scan_fail(token *tok){
    printf("Error when scanning token: %s\n", tok->lexeme);
    printf("Unexpected character: \'%c\'\n", current_char(tok));
    exit(1);
}


const char * remaining_string(int s, char * string){
    int len = strlen(string); 
}


// This is to help cleanup the all the switch statement branches
#define KEYWORD_BRANCH(word, type) \
    if(c==word[0]){ \
        keyword(fp, tok, type, "ontinue"); \
    }


void s0(FILE *fp, token *tok){ // This is 
    //printf("-> s0 %s\n", tok->lexeme);
    /*
     * S0: State-Zero of a DFA Graph
     *  The next state (S1, S2, S3) denoted by their definitive keyword
     *  is determined by the first and second letter of the keyword.
     *  This has the limitation that two keywords cannot share more than
     *  two starting characters (An easy ask). Rollbacks are not necessary 
     *  because we have said invariant. The time complexity remains at 0(n).
     */
    token_addchar(fgetc(fp), tok);
    int c = current_char(tok); // current_char
    // This may be a good candidate for a macro
    if(c=='b' && fpeek(fp)=='r'){
        keyword(fp, tok, BREAK_K, "reak");
    }
    if(c=='b' && fpeek(fp)=='o'){
        keyword(fp, tok, BOOL_K, "ool");
    }
    KEYWORD_BRANCH("continue", CONTINUE_K);
    if(c=='e'){
        keyword(fp, tok, ELSE_K, "lse");
    }
    if(c=='f' && fpeek(fp)=='o'){
        keyword(fp, tok, FOR_K, "or"); 
    }
    if(c=='f' && fpeek(fp) == 'a'){
        keyword(fp, tok, FALSE_K, "alse");
    }
    if(c=='i' && fpeek(fp) == 'f'){
        keyword(fp, tok, IF_K, "f");
    }
    if(c=='i' && fpeek(fp) == 'n'){
        keyword(fp, tok, INT_K, "nt");
    }
    if(c=='i' && fpeek(fp) == 'm'){
        keyword(fp, tok, IMPORT_K, "mport");
    }
    if(c=='l'){
        keyword(fp, tok, LEN_K, "en");
    }
    if(c=='r'){
        keyword(fp, tok, RETURN_K, "eturn");
    }
    if(c=='t'){
        keyword(fp, tok, TRUE_K, "rue");
    }
    if(c=='v'){
        keyword(fp, tok, VOID_K, "oid");
    }

    if(c=='w'){
        keyword(fp, tok, WHILE_K, "hile");
    }

    if(c>='a' && c<='z' && tok->type==UNDET){
        tok->type == UNDET;
        ident(fp, tok);
    }
    
    if(is_whitespace(c) && tok->type == UNDET){
        //printf("-> <ident> %s\n", tok->lexeme);
        tok->type = IDENT;
        skip_whitespace(fp);
    }
    return;
}

void ident(FILE *fp, token *tok){
    // This is working with identifier
    //printf("-> ident %s\n", tok->lexeme);
    token_addchar(fgetc(fp), tok);
    int c = current_char(tok); // current_char
    while(1){
        if(c>='a' && c<='z'){
            //ident(fp, tok);
            token_addchar(fgetc(fp), tok);
            c = current_char(tok);
            //printf("-> ident %s\n", tok->lexeme);
            continue;
        }else if(is_whitespace(c)){
            printf("-> <ident> %s\n", tok->lexeme);
            tok->type = IDENT;
            skip_whitespace(fp);
            return;
        }
        scan_fail(tok);
    }
}

void keyword(FILE *fp, token *tok, token_type word, char *rem){
    /*
     * keyword is DFA which is kicked off by first letter,
     * and each following letter is a state, which must end in whitespace
     */
    // printf("-> %s %s\n", rem, tok->lexeme);
    token_addchar(fgetc(fp), tok);
    int c = current_char(tok); // current_char
    
    for(int i=0;i<strlen(rem);i++){
        if(c == rem[i]){
            // good
            //printf("-> %s %s\n", rem, tok->lexeme); // TODO Macro this
            token_addchar(fgetc(fp), tok);
            c = current_char(tok); // current_char
            continue;
        }
        if(c>='a' && c<='z' && tok->type==UNDET){
            ident(fp, tok);
            return;
        }
        if(is_whitespace(c) && tok->type==UNDET){
            printf("-> <ident> %s\n", tok->lexeme);
            tok->type=IDENT;
            skip_whitespace(fp);
            return;
        }
        scan_fail(tok);
    }

    // Last character is either a space returning a keyword or ident.
    //c = fgetc(fp);
    if(!is_whitespace(c)){
        ident(fp, tok);
        return;
    }

    tok->type=word;
    skip_whitespace(fp);
    printf("-> <%d> %s\n", word, tok->lexeme);
    return;
}

void tokenize(FILE *fp) {
    int i = 0; 
    
    char *word;
    while(word = read_word(fp), strcmp("", word) != 0){
        skip_whitespace(fp);
        printf("%s: %zu \n", word, sizeof(word));
    }
}


#ifndef TESTBUILD
int main(int argc, char **argv){
    
    if(argv[1] == NULL){
        printf("%i\n arguments provided", argc);
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    printf("Compiling: %s\n", argv[1]);

    if(fp == NULL){
        printf("Cannot find or open file: %s\n", argv[1]);
        exit(1);
    }

    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
    read_token(fp);
}
#endif

