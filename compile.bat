cd build
del .\yaya.exe
@REM g++ -c ..\src\yaya.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\camera.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\entity.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\events.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\game.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\graphics.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\neighborsfinder.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\timer.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
@REM g++ -c ..\src\utilities.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
g++ ..\src\yaya.cpp^
 ..\src\camera.cpp^
 ..\src\controls.cpp^
 ..\src\debugger.cpp^
 ..\src\entity.cpp^
 ..\src\events.cpp^
 ..\src\game.cpp^
 ..\src\graphics.cpp^
 ..\src\neighborsfinder.cpp^
 ..\src\objects.cpp^
 ..\src\serialization.cpp^
 ..\src\timer.cpp^
 ..\src\utilities.cpp^
 -o yaya.exe -I..\SFML-2.5.1\include -DSFML_STATIC -L..\SFML-2.5.1\lib -mwindows -lsfml-graphics-s -lfreetype -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
@REM .\yaya.exe
cd ..
