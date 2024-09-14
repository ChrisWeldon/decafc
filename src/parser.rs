// Recursive decent parser implementation of decaf
use crate::lexer::Lexer;
use crate::lexer::Token;
use crate::ast;

// Parser cannot outlive the Lexer

pub struct Parser<'a>{
    l : Lexer<'a>,
    current_token : Token, 
    peek_token : Token
}

impl<'a> Parser<'a> {
    pub fn build_parser(input: & String) -> Parser{
        let l = Lexer::build_lexer(input);

        let mut p = Parser {
            l,
            current_token: Token::Eof,
            peek_token: Token::Eof 
        };

        p.current_token = p.l.next_token();
        p.peek_token = p.l.next_token();
        p
    }

    fn advance_tokens(&mut self) {
        self.current_token = self.peek_token.clone(); // peek_token is a mutable borrow 
        self.peek_token = self.l.next_token();
    }

    fn current_token_is(& self, t: Token) -> bool{
        self.current_token == t
    }

    fn peek_token_is(& self, t: Token) -> bool{
        self.peek_token == t
    }

    fn expect_peek_token(&mut self, t: Token ) -> bool {
        if self.peek_token_is(t) {
            self.advance_tokens();
            true
        }else{
            false
        }
    }

    pub fn parse_program(&mut self) -> ast::Program{
        // TODO parse program business 
        
        ast::Program {
            statements: Vec::new()
        }
    }

    pub fn parse_assign_statement(&mut self) -> ast::AssignStatement{
        let location = Box::new(self.parse_location());
        self.expect_peek_token(Token::Assign);
        let assign_expr = Box::new(self.parse_expr());

        ast::AssignStatement {
            location,
            assign_expr
        }
    }

    fn parse_assign_expr(&mut self){
        // TODO
    }

    fn parse_location(&mut self) -> ast::Location{
        ast::Location::IdentLocation("blah".to_string())
    }

    fn parse_expr(&mut self) -> ast::Expression{
        /* <expr> -> <location>
         *        |  <method call>
         *        |  <literal>
         *        |  len (<id>)
         *        |  <expr> <bin op> <expr>
         *        |  - <expr>
         *        |  ! <expr>
         *        |  ( <expr> )
         *        |  <expr> ? <expr> : <expr>
         */

        ast::Expression::Literal(ast::LiteralExpr::BoolLiteral(false))
    }
}

#[cfg(test)]
mod tests{
    use crate::parser::Parser;
    use crate::lexer::Token;

    #[test]
    fn test_peek_token_is(){
        let input = "x = 10;".to_string();
        let mut p = Parser::build_parser(&input);
        
        assert!(p.peek_token_is(Token::Assign));
        assert!(!p.peek_token_is(Token::Semicolon));
        p.advance_tokens();
        assert!(p.peek_token_is(Token::Int(10)));
        assert!(!p.peek_token_is(Token::Semicolon));
    }

    #[test]
    fn test_current_token_is(){
        let input = "x = 10;".to_string();
        let mut p = Parser::build_parser(&input);
        
        assert!(p.current_token_is(Token::Ident("x".to_string())));
        assert!(!p.peek_token_is(Token::Semicolon));
        p.advance_tokens();
        assert!(p.current_token_is(Token::Assign));
        assert!(!p.peek_token_is(Token::Semicolon));
    }

    #[test]
    fn test_advance_token(){
        let input = "x = 10;".to_string();
        let mut p = Parser::build_parser(&input);

        assert!(matches!(p.current_token, Token::Ident(ref x) if x==&"x".to_string() ));
        assert!(matches!(p.peek_token, Token::Assign));

        p.advance_tokens();

        assert!(matches!(p.current_token, Token::Assign));
        assert!(matches!(p.peek_token, Token::Int(i) if i==10));

    }

}
