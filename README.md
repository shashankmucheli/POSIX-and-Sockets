Part I:  Research and Program Producer-Consumer Problem
In this assignment you will design a programming solution to the bounded-buffer problem using the producer and consumer processes shown in the lecture notes.  The solution presented in the notes uses three semaphores: empty and full, which count the number of empty and full slots in the buffer, and mutex, which is a binary semaphore that protects the actual insertion or removal of items in the buffer.  For this assignment, standard counting semaphores will be used for empty and full, Use a mutex lock, rather than a binary semaphore,  to represent the mutex.  The producer and consumer—running as separate threads will move items to and from a buffer that is synchronized with these empty, full, and mutex structures.  
The buffer
Internally, the buffer will consist of a fixed-size array of type buffer_item (which will be defined using  typedef).  The array of buffer_item objects will be manipulated as a circular queue.  
The buffer will be manipulated with two functions, insert_item() and remove_item(), which are called by the producer and consumer threads, respectively.  
The insert_item() and remove_item) functions will synchronize the producer and consumer using the algorithms outlined in the class lecture.  The buffer will also require an initialization function that initializes the mutual exclusion object mutex along with the empty and full semaphores.
The main() function will initialize the buffer and create the separate producer and consumer threads.  Once it has created the producer and consumer threads, the main() function will sleep for a period of time and upon awakening, will terminate the application.  The main() function will be passed three parameters on the command line:
a.   How long to sleep before terminating
b. The number of producer threads
c. The number of consumer threads
Producer and Consumer Threads
The producer thread will alternate between sleeping for a random period of time and inserting a random integer into the buffer.  Random numbers will be produced using the rand() function, which produces random integers between 0 and RAND_MAX.  The consumer will also sleep for a random period of time and, upon wakening, will attempt to remove an item from the buffer.  
Mutex Locks
The code should have commands to create, acquire, and release mutex locks.  
Assumption:  You will use the book and lecture notes for base info to complete the program.  You will conduct your own research to fill in any knowledge holes. 
Use POSIX Pthreads.  
