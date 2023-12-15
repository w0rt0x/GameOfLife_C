# Game of Life in C
Project to get familiar with C by using dynamic memory allocation and also checking for memory leaks by using Valgrind.

## How to use
#### Compile on linux
```
gcc main.c -o GoL
```

#### Run on linux
```
./GoL
```


## Using Valgrind to check for memory leaks
Install this tool with:
```
sudo apt install valgrind
```

Run it on the executable:
```
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./GoL
```

It will show 0 memory leaks:
```
==7504== HEAP SUMMARY:
==7504==     in use at exit: 0 bytes in 0 blocks
==7504==   total heap usage: 613 allocs, 613 frees, 245,824 bytes allocated
==7504== 
==7504== All heap blocks were freed -- no leaks are possible
==7504== 
==7504== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## To Dos
- [ ] Learn CMake and split up the main into different modules
- [ ] make x and y dimension variable via command line when calling the executable