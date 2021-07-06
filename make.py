import os
import sys


os.system("g++ -c main.cpp")
os.system("g++ -c game.cpp")
os.system("g++ main.o game.o -o app -lsfml-graphics -lsfml-window -lsfml-system")