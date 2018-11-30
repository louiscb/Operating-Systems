# Buddy

The buddy algorithm is a method of splitting and combining blocks of memory to
return to a system memory request. When receiving a request, the algorithm will
split a memory block into halves until it achieves a suitable size for the
specified request. The algorithm will also coalesce free blocks of memory into
larger blocks when it can in order to avoid a the memory being a large set of
tiny blocks of memory. Each block is subdivided into smaller blocks with all
blocks following a predefined range of block sizes. The level within this range
of block sizes relates to the amount of bytes that the corresponding block takes
up in memory as: 2^n where n is the level of the block. One of the buddy algorithm's benefits is that the total cost of the allocate and free operations are constant.

# Report

For more information read the Memory-Managment.pdf
