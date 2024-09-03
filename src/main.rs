mod lexer; 
mod parser; 

fn main() {
    // Simple Repl implementation
    use std::io::{self, stdout, Write};
    use lexer::Lexer;
    use lexer::Token;

    loop{
        print!("C> ");
        let mut buffer_in = String::new();
        io::stdout().flush().unwrap();
        std::io::stdin().read_line(&mut buffer_in);
        let mut lex = Lexer::build_lexer(& buffer_in);

        let mut ltoken = lex.next_token();
        while ltoken != Token::Eof {
            println!("{:?}", ltoken);
            if ltoken == Token::Ident("exit".to_string()){
                return;
            }
            ltoken = lex.next_token();

        }
    }

}
