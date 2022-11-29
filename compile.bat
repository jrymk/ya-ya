cd build
g++ -c ..\src\yaya.cpp -I..\SFML-2.5.1\include -DSFML_STATIC
g++ yaya.o -o yaya.exe -L..\SFML-2.5.1\lib -lsfml-graphics-s -lfreetype -lsfml-window-s -lsfml-system-s -lopengl32 -lwinmm -lgdi32
.\yaya.exe
cd ..
