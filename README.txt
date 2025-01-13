Vashisht Borkar vb471
Saransh Batwe ssb209

Preliminary Testing:
While we were writing the code for the malloc function, we had multiple print statements to check what the 
payload size, the normalized address, the actual address and whether that payload was allocated or not

We used that information to make sure that the payloads were being allocated correctly and 
make sure that everything was function as intended

Correctness Testing:

We have basic functions that test the basic fuctionality of malloc and free and check through
all possible test cases and any possible errors.

Performance Testing:

We are trying to determine the efficiency of our implementation by running large workloads 50 times and
checking how long it takes to run the functions. Aside from the first 3 workloads, the 4th workload was 
one that progressively allocates increasing blocks of memory and then freeing it all. The 5th workload
is allocating a random number of bytes between 1 and 40, 50 times and then deallocating the memory. 

Design:

We created a struct called metadata that takes two ints, one int that determines if it is allocated 
and the other that determines the size of the payload. We also created a coalesce free memory method
to prevent the heap from becoming overly fragmented. The function begins by setting up two pointers, the
max address and the starting address which define the region of memory the allocater manages.

We then iterate through all the memory blocks using a basic metadata pointer and if a block is marked as free, 
we check if the previous block was also free using a "prev" metadata pointer. If that block was free, it merges 
the current block with the previous one by adding the sizes of the two blocks and the metadata header. If that
free block doesn't exist, it sets the current block as "prev".


