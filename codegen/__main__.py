from codegen.expr.generate_all import generate_all as generate_all_expr
from codegen.mdm.generate_all import generate_all as generate_all_mdm

def main():
    generate_all_expr()
    generate_all_mdm()

if __name__ == "__main__":
    main()
