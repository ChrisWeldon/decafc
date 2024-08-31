/**
 DECAF Lexer.
 This is a lexer for the MIT's Decaf language implemented by Chris Evans
*/
use std::num::ParseIntError;
use std::fmt;
// TODO all these to_string()'s are making it so strings are copied everywhere
// TODO two character tokens: !=, ==, >=, etc
// TODO make a REPL

#[derive(Debug)]
#[derive(PartialEq, Eq)]
pub enum Token {
    // File
    Illegal,
    Eof,

    // Literal
    Ident(String),
    Bool(bool),
    Int(u64), // Decaf defines ints as 64bit signed, but we are lexing as Token::Minus, Token:Int(<>)

    // Operators
    Assign,
    Plus,
    Bang,
    Minus,
    Times,
    Mod,
    PlusEquals,
    MinusEquals,
    And,
    Or,
    
    // Comparioson Operators
    Equals,
    NotEquals,
    GtEquals,
    LtEquals,
    LessThan,
    GreaterThan,
    
    // Delimiters
    Comma,
    Semicolon,
    Lparen,
    Rparen,
    Lbrace,
    Rbrace,
    Hex, // We are including 0x in the micro syntax
    Apost,
    Colon,
 
    // Keywords
    Function,
    Let,
    Return,
    If,
    For,
    Void,
    While,
    Break,
    Continue,
    Len
}

pub struct Lexer<'a>{
    input: &'a String,
    position: usize,
    read_position: usize,
    pub ch: Option<char>
}

impl Lexer<'_> {

    #[allow(dead_code)]
    pub fn build_lexer(input: & String) -> Lexer{
        // Lexer is initiated, then reads in first character 

        let mut l = Lexer {
            input,
            position: 0,
            read_position: 0,
            ch: Some(' ')
        };
        l.read_char();
        l
    }

    pub fn read_char(&mut self){
        // should maintain the len of the string somewhere cause this is O(n)
        if self.read_position > self.input.len() { 
            self.ch = Some(' ')
        }else{
            self.ch = self.input.chars().nth(self.read_position);
        }
        self.position = self.read_position;
        self.read_position += 1;
    }

    pub fn peek_char(&self) -> Option<char>{
        return self.input.chars().nth(self.read_position);
    }

    pub fn read_integer(&mut self) -> Result<u32, ParseIntError>{
        // consume all numeric characters in a row
        // TODO test this, I think it messes with the input ownership
        let position = self.position;
        while self.ch.is_some() && self.ch.unwrap().is_numeric() {
            self.read_char();
        }

        self.input[position .. self.position].parse::<u32>()
    }

    pub fn read_literal(&mut self) -> String{
        // consume all alphabetic letters in a row
        // TODO test this, I think it messes with the input ownership
        let position = self.position;
        while self.ch.is_some() && self.ch.unwrap().is_alphabetic() {
            self.read_char();
        }
        self.input[position .. self.position].to_string()
    }
    
    fn on_whitespace(& self) -> bool{
        match self.ch{
            Some(c)=> {
                c == ' ' || c == '\t' || c == '\r' || c == '\n' 
            },
            _ => { false }
        }
    }

    fn skip_whitespace(&mut self) {
        // This is a do while loop, since all the logic is in the expression
        while self.on_whitespace() {
            self.read_char();
        }
    }


    #[allow(dead_code)]
    pub fn next_token(&mut self) -> Token{
        // This function basically converts Lexer.ch to a token

        self.skip_whitespace();

        let tok;
        match self.ch{
            None => tok = Token::Eof,
            Some('+') => tok = Token::Plus,
            Some('{') => tok = Token::Lbrace,
            Some('}') => tok = Token::Rbrace,
            Some('(') => tok = Token::Lparen,
            Some(')') => tok = Token::Rparen,
            Some(',') => tok = Token::Comma,
            Some(';') => tok = Token::Semicolon,
            Some('>') => {
                if let Some('=') = self.peek_char(){
                    self.read_char();
                    tok = Token::GtEquals;
                }else{
                    tok = Token::GreaterThan;
                }
            },
            Some('<') => {
                if let Some('=') = self.peek_char(){
                    self.read_char();
                    tok = Token::LtEquals;
                }else{
                    tok = Token::LessThan;
                }
            },
            Some('!') => {
                //println!("!{:?}", self.peek_char());
                if let Some('=') = self.peek_char(){
                    self.read_char();
                    tok = Token::NotEquals;
                }else{
                    tok = Token::Bang;
                }
            },
            Some('=') => {
                //println!("={:?}", self.peek_char().unwrap());
                if let '=' = self.peek_char().unwrap(){
                    self.read_char();
                    tok = Token::Equals;
                }else{
                    tok = Token::Assign;
                }
            },
            _=> { // default is reading multi char tokens
                let c = self.ch.expect("Option<self.ch> is None in match statement");
                if c.is_alphabetic() {
                    let literal = self.read_literal();
                    if let Some(token) = lookup_keyword_token(literal.as_str()){
                        tok = token
                    }else{
                        tok = Token::Ident(literal);
                    }
                } else if c.is_numeric(){
                    // TODO change from unwrap
                    let integer: u32 = self.read_integer().unwrap();
                    tok = Token::Int(integer);
                }else{
                    tok = Token::Illegal
                }
            }
        };

        self.read_char();
        tok
    }

}

#[allow(dead_code)]
pub fn lookup_keyword_token(key: &str) -> Option<Token> {
    match key{ 
        "let" => Some(Token::Let),
        "fn" => Some(Token::Function),
        "return" => Some(Token::Return)
        _ => None
    }
}
