#!/bin/bash
# COMPILE LIFISH FIRST!
${CXX:-g++} -ggdb -std=c++11 -I../src/{core{,/collisions,/components,/cutscenes,/entities,/debug},ui,lifish{,/entities,/components,/debug,/level},third_party} $1  ../CMakeFiles/lifish.dir/src/{core{,/collisions,/components,/cutscenes,/entities,/debug},lifish{,/entities,/components,/debug,/level,/conf},third_party,ui}/*o  ../lib/libnfd_linux_x86_64_pie.a -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio `pkg-config --libs gtk+-3.0` -o ${1%.cpp}
