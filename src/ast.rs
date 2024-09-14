// The abstract syntact tree nodes

/*
 * Keeping standard typing nomenclature
 * S for Statements
 * E for Expressions
 *
 */


use std::fmt;

pub struct Program {
    pub statements: Vec<Box<Statement>>
}

trait Node {
}

pub enum Statement {
    // <statement>
    Assign(AssignStatement), // <location> <assign_expr> ;
    MethodCall(), // <method_call> ;
    IfStatement(), // if ( <expr> ) <block> [Else <block>]
    Return(ReturnStatement), // return [<expr>]
    Break, // break ;
    Continue // continue ;
}

impl fmt::Display for Statement{
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let _ = match *self{
            Statement::Assign(ref assign_state) => write!(f, "Statement({})", assign_state),
            _ =>  write!(f, "(unimplemented Statement node!)")
        };
        Ok(())
    }
}

pub struct AssignStatement{
    // <location> <assign_expr> ;
    pub location : Box<Location>, 
    pub assign_expr: Box<Expression> // This is not the real grammer, should be AssignExpression
}

impl fmt::Display for AssignStatement{
    fn fmt(&self, f:&mut fmt::Formatter) -> fmt::Result {
        write!(f, "Assign({}, {})", self.location, self.assign_expr);
        Ok(())
    }
}

pub struct ReturnStatement{
    value: Option<Expression> // Not sure about this. I think optionals should maybe be unrolled to
                              // enums
}

pub enum Location {
    IdentLocation(String),
    //Array(Array)
}

impl fmt::Display for Location{
    fn fmt(&self, f:&mut fmt::Formatter) -> fmt::Result {
        let _ = match *self {
            Location::IdentLocation(ref id_loc)  => write!(f, "IdentLocation({})", id_loc),
            _ => write!(f, "(unimplemented Expression)")
        };
        Ok(())

    }
}

pub struct Array {
    pub id: String,
    pub expr: Box<Expression>
}

pub enum Expression {
    Literal(LiteralExpr),
    BinOperation(Box<BinaryOperationExpr>)
}

impl fmt::Display for Expression{
    fn fmt(&self, f:&mut fmt::Formatter) -> fmt::Result {
        // TODO
        let _ = match *self {
            Expression::Literal(ref literalexpr)  => write!(f, "Literal({})", literalexpr),
            _ => write!(f, "(unimplemented Expression)")
        };
        Ok(())
    }
}

pub struct BinaryOperationExpr{
    lexpr: Expression,
    //binop: BinaryOperator, always + for now 
    rexpr: Expression
}

pub enum LiteralExpr {
    IntLiteral(u64),
    BoolLiteral(bool),
    CharLiteral(char)
}

impl fmt::Display for LiteralExpr{
    fn fmt(&self, f:&mut fmt::Formatter) -> fmt::Result {
        // TODO
        let _ = match *self {
            LiteralExpr::IntLiteral(l)  => write!(f, "IntLiteral({})", l),
            LiteralExpr::BoolLiteral(l)  => write!(f, "{}", l),
            LiteralExpr::CharLiteral(l)  => write!(f, "{}", l)
        };
        Ok(())
    }
}



#[cfg(test)]
mod ast_tests {
    use super::*;

    #[test]
    fn test_to_string(){
        // x = 10;
        let program = Statement::Assign(AssignStatement {
            location: Box::new(Location::IdentLocation("x".to_string())),
            assign_expr: Box::new(Expression::Literal(
                    LiteralExpr::IntLiteral(10)
            ))
        });

        let program_str_model = "Statement(Assign(IdentLocation(x), Literal(IntLiteral(10))))";

        assert_eq!(format!("{}", program), program_str_model);
    }
}
