Lillian Parker - CS250 
Taken: Sophomore Fall Semester 2023
Computer structure, assembly language, instruction execution, addressing techniques, and digital representation of data. 
Computer system organization, logic design, microprogramming, cache and memory systems, and input/output interfaces.

Descriptions of different assignments:

HW1: C Programming (September 2023)
recurse.c
- simple c program that recurses on function 'f', amount of recursive calls depends on user input

byseven.c
- simple c program that counts by seven, amount of iterations depends on user input

buildeff.c
- this program reads through text file that includes string of a building name, kilowatts per hour, and square footage
- program creates a linked list which is sorted by the efficiency of each building (this depends on the kilowatts per hour / square footage)
- finally prints out the buildings in order of least effective to most effective depending on efficiency
- if two buildings have equal efficiency ties are broken alphabetically by building name

homework-2-assembly: MIPS Assembly Programming (October/November 2023)
recurse.s
- MIPS assembly program that recurses on function 'f', amount of recursive calls depends on user input
- follows calling conventions and saves registers on stack for correct memory allocation

byseven.s
- MIPS assembly program that counts by seven, amount of iterations depends on user input

buildeff.s
- this program reads through text file that includes string of a building name, kilowatts per hour, and square footage
- program creates a linked list which is sorted by the efficiency of each building (this depends on the kilowatts per hour / square footage)
- linked list is created by malloc, creating space in memory for where the pointer to next building will live
- traverses through memory to do insert sort
- finally prints out the buildings in order of least effective to most effective depending on efficiency
- if two buildings have equal efficiency ties are broken alphabetically by building name

homework-5-cache-virtual-memory: C Programming, Cache simulator (November/December 2023)
cachesim.c
- C Program that creates a virtual cache structure, takes in data and uses Least Recently Used (LRU) replacement to flush cache when necessary
- helper functions for loading and storing data in the cache


