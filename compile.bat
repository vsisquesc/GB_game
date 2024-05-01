REM Automatically generated from Makefile
mkdir obj
lcc  -debug  -v      -c -o obj\main.o src\main.c
lcc  -debug  -v      -c -o obj\bg_map.o src\libs\bg_map.c
lcc  -debug  -v      -c -o obj\bg_tiles.o src\libs\bg_tiles.c
lcc  -debug  -v      -c -o obj\bird_tiles.o src\libs\bird_tiles.c
lcc  -debug  -v      -o obj\main.gb obj\main.o obj\bg_map.o obj\bg_tiles.o obj\bird_tiles.o 
