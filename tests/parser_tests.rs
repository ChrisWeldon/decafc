use decafc::parser::Parser;
use decafc::ast;

#[test]
fn test_simple_program(){
    let input = "
        x = 10;
        y = x + 20;
        return y;
    ".to_string();
    panic!("test not implemented yet") 
}

#[test]
fn test_parse_assign_statement(){
    let input = "x = 10;".to_string();
    let program_model = "Assign(Ident(x), Literal(IntLiteral(10)))".to_string();

    let mut p = Parser::build_parser(&input);
    let program_actual = p.parse_assign_statement();
    
    assert_eq!(format!("{}", program_actual), program_model);

}

