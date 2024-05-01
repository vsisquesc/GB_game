@echo off

@REM you should pass --debug to frontend (Wf--debug)
@REM you should pass -y to the linker to make it create a CDB file used for debugging (-Wl-y )
@REM and also pass -m -w to the linker to produce "wide maps" that are useful for emulicious (-Wl-m)
@REM add -Wl-j if you want to create NoICE symbol file .NOI for use in BGB emulator
rd /S /Q build
md build


d:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o build/main.o  build/main.c
d:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -o  build/main.gb  build/main.o