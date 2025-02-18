#!/bin/bash

cd source
gcc main.c tui.c -fcommon shared.h -lncurses -o hexdump 

#gcc main.c -lncurses -o hexdump

