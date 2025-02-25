Implementing C's malloc and free functions

Testing Strategy:
1. Use memtest file to fill memory and free the objects to test the basic functionality of mymalloc.
2. Use memtest file with -DLEAK in compilation to test leak checker functionality.
3. Create a new test program where we test if adjacent blocks coalesce when freed.
4. Add tests to the second test program designed to test each possible error, malloc with not enough space, free an address that has not been malloc’d, free an item twice, and free with an address that is not at the start of a chunk.
5. Add tests to the program that test a variety of uses of malloc and free that should return no errors or cause memory leaks that need to be tracked.
6. Allow the user to input numbers to test individual test cases.
