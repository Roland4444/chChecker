g++ -w -fPIC -ldl -Isrc main.cpp -shared -o obj/libsch.so; g++ main.cpp -ldl -o obj/main;
