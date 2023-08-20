# ArrayStats
Get a bunch of stats about an array fast.

## How to build the demo

`make`

## How to run the demo

`./output` or `make run`

## How to use in your project

`cp ArrayStats.h ArrayStats.cpp [DESTINATION_PATH]`

Add `#include "ArrayStats.h"` to the file you want to use it in

g++ compile with ArrayStats.cpp

## Nuance

If you don't specify anything, ArrayStats::getStats will get all the stats. If you don't need all the stats, you can specify which ones you need using the flags. Just bitwise or the flags you want, and the program will do a little less work. You can also specify if you've already sorted your array, so you can skip that bit.

A good compiler will optimize the hell out of a function like this. A good enough compiler would even optimize out the whole program and just hard code the output because everything in the test main.cpp is constant.


## Closing

This can be done faster, because you can get Median in O(n) time with quickSelect, but the idea is if you have a bunch of stats that need the array to be sorted, like mode (ik you can do it with maps), then you can share the performance benefit between them.
