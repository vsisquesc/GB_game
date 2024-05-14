REM Automatically generated from Makefile
mkdir obj
lcc  -debug  -v      -c -o obj\main.o src\main.c
lcc  -debug  -v      -c -o obj\bg_map.o src\maps\bg_map.c
lcc  -debug  -v      -c -o obj\bg_tiles.o src\tiles\bg_tiles.c
lcc  -debug  -v      -c -o obj\bird_tiles.o src\tiles\bird_tiles.c
lcc  -debug  -v      -c -o obj\gate_tiles.o src\tiles\gate_tiles.c
lcc  -debug  -v      -c -o obj\utils.o src\libs\utils.c
lcc  -debug  -v      -c -o obj\audio.o src\libs\audio.c
lcc  -debug  -v      -c -o obj\playable_agent.o src\libs\playable_agent.c
lcc  -debug  -v      -c -o obj\obstacle_agent.o src\libs\obstacle_agent.c
lcc  -debug  -v      -o obj\main.gb obj\main.o obj\bg_map.o obj\bg_tiles.o obj\bird_tiles.o  obj\utils.o obj\audio.o obj\playable_agent.o obj\gate_tiles.o obj\obstacle_agent.o 
