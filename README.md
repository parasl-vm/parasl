# parasl

Cmake build has not supported yet (not tested at least). Build and launch with
```
bison -d parser.y
flex lexer.l
g++ -std=c++17 parser/parser.tab.c lexer/lex.yy.c -o frontend
./frontend /parasl/test/example.pcl
```
