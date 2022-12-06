cd build
del .\yaya.exe
@REM g++ -c ..\src\yaya.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\camera.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\controls.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\debugger.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\entity.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\events.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\game.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\graphics.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\neighborsfinder.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\objects.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\serialization.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\timer.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\utilities.cpp -I..\SFML-2.5.1\include -DSFML_STATIC

@REM g++ ..\src\yaya.cpp^
@REM  ..\src\camera.cpp^
@REM  ..\src\controls.cpp^
@REM  ..\src\debugger.cpp^
@REM  ..\src\entity.cpp^
@REM  ..\src\events.cpp^
@REM  ..\src\game.cpp^
@REM  ..\src\graphics.cpp^
@REM  ..\src\neighborsfinder.cpp^
@REM  ..\src\objects.cpp^
@REM  ..\src\serialization.cpp^
@REM  ..\src\timer.cpp^
@REM  ..\src\utilities.cpp^
@REM  -o yaya.exe -I..\SFML-2.5.1\include -DSFML_STATIC -L..\SFML-2.5.1\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32

g++ yaya.o^
 camera.o^
 controls.o^
 debugger.o^
 entity.o^
 events.o^
 game.o^
 graphics.o^
 neighborsfinder.o^
 objects.o^
 serialization.o^
 timer.o^
 utilities.o^
 -o yaya.exe -I..\src -I..\SFML-2.5.1\include -DSFML_STATIC -L..\SFML-2.5.1\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
.\yaya.exe
cd ..
