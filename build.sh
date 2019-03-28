g++  -w -fPIC -ldl -Isrc main.cpp -shared -o obj/libsimple.so; g++  -ldl main.cpp -o obj/main;
