cc_build:
    cc -o comp1 -g cg.c expr.c gen.c main.c misc.c scan.c stmt.c tree.c decl.c sym.c

cc_buildn:
    cc -o compn -g cg.c expr.c gen.c main.c misc.c scan.c stmt.c tree.c decl.c sym.c

zig_build:
    zig build

buildall:
     cc_build
     cc_buildn

comp_ac:zig_build
    ./zig-out/bin/lacwj ./test/input002.ac

as:comp_ac
   cc -o out out.s

test:as
   ./out

clean:
	rm -f ./zig-out/bin/lacwj  *.o *.s out
