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
 */


/*
 * Reads in a word character by character, growing the bufsize as word expands
 *  TODO: Shrink the word as we go
 *
 * fp: A file pointer which points to the input text file to be compiled
 * 
 * returns: a (char *) read out. If it is EOF then returns an empty string.
*/

#ifdef DEBUG
#define printf_debug(fmt, ...) printf(fmt, __VA_ARGS__); 
#else
#define printf_debug(...)
#endif

#define COL_COUNT 80 // max number of characters read into scanning buffer


bool is_whitespace(int c){
    return c == ' ' || c == EOF || c == '\n' || c == '\t' || c == '\f' || c == '\r' || c == '\v';
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
    
    // Variadic
    IDENT,
    STRING,     // /\w+/    (basically any keyword)
    INT,        // /\d+/
    HEX,        // /0x(\d+)/
    
    // Single char
    DIV,        // /
    DOT,        // .
    EQUALS,     // =
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
    L_CURLY,    // {
    R_CURLY,    // }
    Q_MARK,     // ?
    BANG,       // !
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

char * get_token_type(token_type type){
    switch(type){
        case UNDET: return "UNDET" ;      // Not yet determined, Probably won't need this 
        case  ROLLBACK: return "ROLLBACK" ;   // Guessed at a keyword or something, but was wrong
        case  ERROR_TOK: return "ERROR_TOK" ;  // Microsyntax failure
        //
        case  BOOL_K: return "BOOL_K" ;     // bool
        case  BREAK_K: return "BREAK_K" ;    // break
        case  CONTINUE_K: return "CONTINUE_K" ; // continue
        case  ELSE_K: return "ELSE_K" ;     // else
        case  FALSE_K: return "FALSE_K" ;    // false
        case  FOR_K: return "FOR_K" ;      // for
        case  IF_K: return "IF_K" ;       // if   (not considering this "double char" cause i does not start any doubles)
        case  IMPORT_K: return "IMPORT_K" ;   // import
        case  INT_K: return "INT_K" ;      // int 
        case  LEN_K: return "LEN_K" ;      // len
        case  RETURN_K: return "RETURN_K" ;   // return
        case  TRUE_K: return "TRUE_K" ;     // true
        case  VOID_K: return "VOID_K" ;     // void
        case  WHILE_K: return "WHILE_K" ;    // while
        
        // Variadic
        case IDENT: return "IDENT" ;
        case STRING: return "STRING" ;     // /\w+/    (basically any keyword)
        case INT: return "INT" ;        // /\d+/
        case HEX: return "HEX" ;        // /0x(\d+)/
        
        // Single char
        case DIV: return "DIV" ;        // /
        case  DOT: return "DOT" ;        // .
        case  EQUALS: return "EQUALS" ;     // =
        case  MINUS: return "MINUS" ;      // -
        case  PLUS: return "PLUS" ;       // +
        case  TIMES: return "TIMES" ;      // *
        case  MOD: return "MOD" ;        // %
        case  GT: return "GT" ;         // >
        case  LT: return "LT" ;         // <
        case  L_PAREN: return "L_PAREN" ;    // (
        case  R_PAREN: return "R_PAREN" ;    // )
        case  L_BRACE: return "L_BRACE" ;    // [
        case  R_BRACE: return "R_BRACE" ;    // ]
        case  L_CURLY: return "L_CURLY" ;    // {
        case  R_CURLY: return "R_CURLY" ;    // }
        case  Q_MARK: return "Q_MARK" ;     // ?
        case  BANG: return "BANG" ;       // !
        case  COL: return "COL" ;        // :
        case  SEMI_COL: return "SEMI_COL" ;   // ;
        case  QUOTE: return "QUOTE" ;      // '
        case  QUOTE_D: return "QUOTE_D" ;    // "
        // Double char
        case GT_E: return "GT_E" ;       // >=
        case  LT_E: return "LT_E" ;       // <=
        case  EE: return "EE" ;         // ==
        case  NE: return "NE" ;         // !=
        case  AND: return "AND" ;        // &&
        case  OR: return "OR";          // ||
        default:
            return "UNDET";
    }
}

typedef struct token {
    token_type type; 
    int bufsize;
    int offset;
    char *lexeme;
} token;

typedef struct textbuf {
    char *text;
    int size;
    int loc;
} textbuf;

/*
 * Reads in all the whitespace
 */

void textbuf_ungetc(char c, textbuf *buf);
int textbuf_getc(textbuf *buf);
int textbuf_peekc(textbuf *buf);

void s0(textbuf *buffer, token *tok); // Start of token, could be anything
void ident(textbuf *buffer, token *tok); // Any letter, going to be an ident
void keyword(textbuf *buffer, token *tok, token_type word, char * rem);
void number(textbuf *buffer, token *tok);
void decimal(textbuf *buffer, token *tok);
int fpeek(FILE *fp);

void token_print(token *tok);
void token_addchar(char c, token *tok);
void token_removechar(token *tok);

void skip_whitespace(textbuf *buffer){
    // assumes we were at whitesace
    int c = textbuf_peekc(buffer);
    while(is_whitespace(c)){
        textbuf_getc(buffer); // Eat it
        c = textbuf_peekc(buffer);
    }
}

int textbuf_getc(textbuf *buf){
    if(buf->loc==buf->size-1){
        return '\0';
    }
    return buf->text[buf->loc++];
}

void textbuf_ungetc(char c, textbuf *buf){
    buf->loc--;
}

int textbuf_peekc(textbuf *buf){
    if(buf->loc>=buf->size){
        return '\0';
    }
    return buf->text[buf->loc];
}

token * read_token(textbuf *buffer, token *tok){
    int bufsize = 8; // Different from buffer non-dynamic for the time being
    int offset = 0;

    tok->type=UNDET;
    tok->bufsize=bufsize;
    tok->offset=0; // This is not a buffer offset, this is for copying from the buffer to the lexeme
    tok->lexeme=malloc(bufsize);
    
    skip_whitespace(buffer);
    s0(buffer, tok);
    return tok;
}

void free_token(token *tok){
    free(tok->lexeme);
    free(tok);
}

void token_addchar(char c, token *tok){
    // Tacks c onto the token lexeme
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
    // NOTE: This could be a huge performance problem
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}

void scan_fail(token *tok, int c){
    printf("Error when scanning token: %s\n", tok->lexeme);
    printf("Unexpected character: \'%c\'\n", current_char(tok));
    exit(1);
}

void expected(token *tok, char * expected, int got){
    printf("Error when scanning token: %s\n", tok->lexeme);
    printf("\t Expected: %s. Got: \'%c\'\n", expected, got);
    exit(1);
}

void s0(textbuf *buffer, token *tok){
    // TODO simplify this, the processors are tough to debug 
    // This is to help cleanup the all the switch statement branches
    
#define KEYWORD_BRANCH(word, type) \
    if(c==word[0] && textbuf_peekc(buffer)==word[1]){ \
        token_addchar(c, tok); \
        keyword(buffer, tok, type, word + sizeof(char)); \
        return; \
    }

#define CHAR_BRANCH(char_inv, type_inv) \
    if(c==char_inv){  \
        tok->type = type_inv; \
        token_addchar(c, tok); \
        skip_whitespace(buffer);  \
        return; \
    }

#define DOUBLE_CHAR_BRANCH(char_inv, type_inv, followup_char, type_res) \
    do{ \
        int peek = textbuf_peekc(buffer); \
        if(c==char_inv && peek!=followup_char){ \
            tok->type = type_inv; \
            token_addchar(c, tok); \
        } \
        else if(c==char_inv && peek==followup_char){ \
            tok->type = type_res; \
            token_addchar(c, tok); \
            token_addchar(peek, tok); \
            textbuf_getc(buffer); \
        } \
        skip_whitespace(buffer); \
    } while(0);

#define MAND_DOUBLE_CHAR_BRANCH(char_inv, followup_char, type_res) \
    do{ \
        int peek = textbuf_peekc(buffer); \
        if(c==char_inv && peek!=followup_char){ \
            char should[2] = {followup_char, '\0'}; \
            expected(tok, should, peek); \
        } \
        else if(c==char_inv && peek==followup_char){ \
            tok->type = type_res; \
            token_addchar(c, tok); \
            token_addchar(peek, tok); \
            textbuf_getc(buffer); \
        } \
        skip_whitespace(buffer); \
    } while(0);

    /*
     * S0: State-Zero of a DFA Graph
     *  The next state (S1, S2, S3) denoted by their definitive keyword
     *  is determined by the first and second letter of the keyword.
     *  This has the limitation that two keywords cannot share more than
     *  two starting characters (An easy ask). Rollbacks are not necessary 
     *  because we have said invariant. The time complexity remains at 0(n).
     */
    //token_addchar(textbuf_getc(buffer), tok);
    int c = textbuf_getc(buffer); // current_char
    
    KEYWORD_BRANCH("break", BREAK_K);
    KEYWORD_BRANCH("bool", BOOL_K);
    KEYWORD_BRANCH("continue", CONTINUE_K);
    KEYWORD_BRANCH("else", ELSE_K);
    KEYWORD_BRANCH("for", FOR_K);
    KEYWORD_BRANCH("false", FALSE_K);
    KEYWORD_BRANCH("if", IF_K);
    KEYWORD_BRANCH("import", IMPORT_K);
    KEYWORD_BRANCH("int", INT_K);
    KEYWORD_BRANCH("len", LEN_K);
    KEYWORD_BRANCH("return", RETURN_K);
    KEYWORD_BRANCH("true", TRUE_K);
    KEYWORD_BRANCH("void", VOID_K);
    KEYWORD_BRANCH("while", WHILE_K);

    CHAR_BRANCH('/', DIV);
    CHAR_BRANCH('.', DOT);
    DOUBLE_CHAR_BRANCH('=', EQUALS, '=', EE);
    CHAR_BRANCH('-', MINUS);
    CHAR_BRANCH('+', PLUS);
    CHAR_BRANCH('*', TIMES);
    CHAR_BRANCH('%', MOD);
    DOUBLE_CHAR_BRANCH('>', GT, '=', GT_E);  
    DOUBLE_CHAR_BRANCH('<', LT, '=', LT_E); 
    CHAR_BRANCH('(', L_PAREN);
    CHAR_BRANCH(')', R_PAREN);
    CHAR_BRANCH('[', L_BRACE);
    CHAR_BRANCH(']', R_BRACE);
    CHAR_BRANCH('{', L_CURLY);
    CHAR_BRANCH('}', R_CURLY);
    CHAR_BRANCH('?', Q_MARK);
    DOUBLE_CHAR_BRANCH('!', BANG, '=', NE);
    CHAR_BRANCH(':', COL);
    CHAR_BRANCH(';', SEMI_COL);
    CHAR_BRANCH('\'', QUOTE);
    CHAR_BRANCH('\"', QUOTE_D);
    MAND_DOUBLE_CHAR_BRANCH('&', '&', AND);
    MAND_DOUBLE_CHAR_BRANCH('|', '|', OR);

    if(c>='a' && c<='z' && tok->type==UNDET){
        tok->type == UNDET;
        token_addchar(c, tok);
        ident(buffer, tok);
    }

    if(c>='0' && c<='9' && tok->type==UNDET){
        tok->type = INT;
        token_addchar(c, tok);
        number(buffer, tok);
    }
    
    if(is_whitespace(c) && tok->type == UNDET){
        tok->type = IDENT;
        token_addchar(c, tok);
    }
    skip_whitespace(buffer);

    // TODO, characters that are not a part of any keyword need to be managed
    // scanfail() that

    return;
}

void ident(textbuf *buffer, token *tok){
    // This is working with identifier
    // TODO allow identifiers to not have spaces around them.
    while(1){
        int c = textbuf_peekc(buffer);
        tok->type = IDENT;
        skip_whitespace(buffer);
        if(c>='a' && c<='z'){
            //ident(fp, tok);
            token_addchar(c, tok);
            textbuf_getc(buffer);
            c = textbuf_peekc(buffer);
            continue;
        }
        skip_whitespace(buffer);
        return;
    }
}

void number(textbuf *buffer, token *tok){
    while(1){
        int c = textbuf_peekc(buffer);
        if(c>='0' && c<='9'){
            textbuf_getc(buffer);
            token_addchar(c, tok);
            continue;
        }
        return;
    }
}

void decimal(textbuf *buffer, token *tok){
    while(1){
        int c = textbuf_peekc(buffer);
        if(c>='0' && c<='9'){
            textbuf_getc(buffer);
            token_addchar(c, tok);
            continue;
        }
        return;
    }

}

void keyword(textbuf *buffer, token *tok, token_type word, char *rem){
    /*
     * keyword is DFA which is kicked off by first letter,
     * and each following letter is a state, which must end in whitespace
     */
    int c = textbuf_peekc(buffer);
    
    for(int i=0;i<strlen(rem);i++){
        if(c == rem[i]){
            // good
            token_addchar(c, tok);
            textbuf_getc(buffer);
            c = textbuf_peekc(buffer);
            continue;
        }
        if(c>='a' && c<='z' && tok->type==UNDET){
            token_addchar(c, tok);
            textbuf_getc(buffer);
            c = textbuf_peekc(buffer);
            ident(buffer, tok);
            return;
        }
        if(is_whitespace(c) && tok->type==UNDET){
            tok->type=IDENT;
            skip_whitespace(buffer);
            return;
        }
        scan_fail(tok, c);
    }

    // Last character is either a space returning a keyword or ident.
    //c = fgetc(fp);
    if(!is_whitespace(c)){
        ident(buffer, tok);
        return;
    }

    tok->type=word;
    skip_whitespace(buffer);
    return;
}

void tokenize(textbuf *buf) {
    int i = 0; 
    
    token *token = malloc(sizeof(token));
    while(read_token(buf, token), strcmp("", token->lexeme) != 0){
        token_print(token);
    }
    free_token(token);
}

void token_print(token *tok){
    printf("<token [%s] \t [%s]> \n", tok->lexeme, get_token_type(tok->type));
}

#ifndef TESTBUILD
int main(int argc, char **argv){
    FILE * doc = stdin;
    if(argc > 1){
        doc = fopen(argv[1], "r");
        printf_debug("Compiling: %s\n", argv[1]);

        if(doc == NULL){
            printf_debug("Cannot find or open file: %s\n", argv[1]);
            exit(1);
        }
    }else{
        printf("decafc tokenizer REPL\n");
    }

    char * text = malloc(COL_COUNT);
    textbuf buf = {
        .size=COL_COUNT,
        .text=text,
        .loc=0,
    };

    if(argc == 1) printf("decafc> ");
    while(fgets(buf.text, buf.size, doc)){
        buf.loc=0;
        if(strcmp(buf.text, "exit\n") == 0){
            printf("Terminating\n");
            return 0;
        }
        tokenize(&buf);
        if(argc == 1) printf("decafc> ");
    }

    free(text);
}
#endif

