mod lexer; 

fn main() {
    // Simple Repl implementation
    use std::io::{self, stdout, Write};
    use lexer::Lexer;
    let mut buffer_in = String::new();

    let lines = std::io::stdin().lines();
    for line in lines{
        print!("C> ");
        io::stdout().flush().unwrap();
        let s = "hello world".to_string();
        let mut lex = Lexer::build_lexer(& s);
        let ltoken = lex.next_token();
        println!("{:?}", ltoken);
    }

}
