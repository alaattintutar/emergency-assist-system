#!/bin/bash
clear
g++ main.cpp QuakeAssistController.cpp Team.cpp MissionStack.cpp RequestQueue.cpp Request.cpp -o test
./test < example_input.txt