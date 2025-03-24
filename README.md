# ShiftArray
shift_array is a template class implements FPGA-Like Shift-Register which guarantees continuous memory and constant push and pop complexity O(1)

#Implementation
In this data structure, the entire data is placed in a memory segment and consecutively, and push and pop are done both at the front and at the back with the time complexity of O(1).
Also, there is no need for any reallocation in successive push and pops, and there is no need for any data shift, only here, each set or insert is copied twice.
While in similar data structures like devector in boost, if one side of the data reaches the boundaries of the allocated space, the entire data is shifted to be in the center of the space.
