cd build
del .\yaya.exe
g++ -c ..\src\yaya.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\camera.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\controls.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\debugger.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
g++ -c ..\src\duck.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
g++ -c ..\src\entity.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\events.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
g++ -c ..\src\game.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\graphics.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\neighborsfinder.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\player.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\egg.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\serialization.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\timer.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\utilities.cpp -I..\SFML-2.5.1\include -DSFML_STATIC

g++ ..\src\yaya.cpp^
 ..\src\camera.cpp^
 ..\src\controls.cpp^
 ..\src\debugger.cpp^
 ..\src\duck.cpp^
 ..\src\entity.cpp^
 ..\src\events.cpp^
 ..\src\game.cpp^
 ..\src\graphics.cpp^
 ..\src\neighborsfinder.cpp^
 ..\src\player.cpp^
 ..\src\egg.cpp^
 ..\src\timer.cpp^
 ..\src\utilities.cpp^
 -o yaya.exe -I..\SFML-2.5.1\include -DSFML_STATIC -L..\SFML-2.5.1\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32

@REM g++ yaya.o^
@REM  camera.o^
@REM  controls.o^
@REM  debugger.o^
@REM  duck.o^
@REM  entity.o^
@REM  events.o^
@REM  game.o^
@REM  graphics.o^
@REM  neighborsfinder.o^
@REM  player.o^
@REM  egg.o^
@REM  serialization.o^
@REM  timer.o^
@REM  utilities.o^
@REM  -o yaya.exe -I..\src -I..\SFML-2.5.1\include -DSFML_STATIC -L..\SFML-2.5.1\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32

.\yaya.exe
cd ..
