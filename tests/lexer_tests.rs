use decafc::lexer::Lexer;
use decafc::lexer::Token;


#[test]
fn test_next_token_subset(){
    let input = "=+{}(),;".to_string();
    let tokens = [
        Token::Assign,
        Token::Plus,
        Token::Lbrace,
        Token::Rbrace,
        Token::Lparen,
        Token::Rparen,
        Token::Comma,
        Token::Semicolon,
        Token::Eof
    ];

    let mut lex = Lexer::build_lexer(& input);

    for token in tokens.iter(){
        let ltoken = lex.next_token();
        assert_eq!(*token, ltoken)
    }
}

#[test]
fn test_let_fn_lit(){
    let input = "fn let".to_string();

    let tokens = [
        Token::Function,
        Token::Let
    ];

    let mut lex = Lexer::build_lexer(& input);

    for token in tokens.iter(){
        let ltoken = lex.next_token();
        assert_eq!(*token, ltoken)
    }
}

#[test]
fn test_two_char_tok(){
    let input = "!= ==".to_string();

    let tokens = [
        Token::NotEquals,
        Token::Equals
    ];

    let mut lex = Lexer::build_lexer(& input);

    for token in tokens.iter(){
        let ltoken = lex.next_token();
        assert_eq!(*token, ltoken)
    }
}

#[test]
fn problem_tokens(){
    let input = " hello) let".to_string();
    let tokens = [
        Token::Ident("hello".to_string()),
        Token::Rparen,
        Token::Let
    ];

    let mut lex = Lexer::build_lexer(& input);

    for token in tokens.iter(){
        let ltoken = lex.next_token();
        println!("Should be {:?}, is {:?}", *token, ltoken);
        assert_eq!(*token, ltoken);
    }
}

#[test]
fn test_next_token(){
    let input = "
    let five = 5;
    let ten = 10;
    
    let add = fn(x, y) {
        return x + y;
    };

    let result = add(five, ten);
    if(result >= 10){
        return true;
    }
    ".to_string();

    let tokens = [
        Token::Let,
        Token::Ident("five".to_string()),
        Token::Assign,
        Token::Int(5),
        Token::Semicolon,
        Token::Let,
        Token::Ident("ten".to_string()),
        Token::Assign,
        Token::Int(10),
        Token::Semicolon,
        Token::Let,
        Token::Ident("add".to_string()),
        Token::Assign,
        Token::Function,
        Token::Lparen,
        Token::Ident("x".to_string()),
        Token::Comma,
        Token::Ident("y".to_string()),
        Token::Rparen,
        Token::Lbrace,
        Token::Return,
        Token::Ident("x".to_string()),
        Token::Plus,
        Token::Ident("y".to_string()),
        Token::Semicolon,
        Token::Rbrace,
        Token::Semicolon,
        Token::Let,
        Token::Ident("result".to_string()),
        Token::Assign,
        Token::Ident("add".to_string()),
        Token::Lparen,
        Token::Ident("five".to_string()),
        Token::Comma,
        Token::Ident("ten".to_string()),
        Token::Rparen,
        Token::Semicolon,
        Token::If,
        Token::Lparen,
        Token::Ident("result".to_string()),
        Token::GtEquals,
        Token::Int(10),
        Token::Rparen,
        Token::Lbrace,
        Token::Return,
        Token::Bool(true),
        Token::Semicolon,
        Token::Rbrace
    ];

    let mut lex = Lexer::build_lexer(& input);

    for token in tokens.iter(){
        let ltoken = lex.next_token();
        assert_eq!(*token, ltoken)
    }
}

