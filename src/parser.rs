// Recursive decent parser implementation
use crate::lexer::Lexer;
use crate::lexer::Token;
use std::mem::take;

// Parser cannot outlive the Lexer

pub struct Parser<'a>{
    l : Lexer<'a>,
    current_token : Token, 
    peek_token : Token
}

impl<'a> Parser<'a> {
    fn build_parser<'b>(input: &'b String) -> Parser<'b>{
        let l = Lexer::build_lexer(input);

        let mut p = Parser {
            l,
            current_token: Token::Eof,
            peek_token: Token::Eof 
        };

        p.current_token = p.l.next_token();
        p.peek_token = p.l.next_token();
        return p
    }

    fn next_token<'b>(&mut self) {
        let new_peek_token = self.l.next_token();

        self.current_token = self.peek_token.clone(); // peek_token is a mutable borrow 
        self.peek_token = self.l.next_token();
    }
}

