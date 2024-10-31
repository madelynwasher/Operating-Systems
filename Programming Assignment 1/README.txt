In this assignment, I use shared memory and semaphores to synchronize a producer and a consumer. The producer generates items and places them on the table,
then the consumer picks up the items. The table can hold two items at once. When the table has two items, the producer waits. When there are no items on the table,
the consumer waits. Threads are used in both the producer program and the consumer program, and the table is implemented using shared memory. Mutual exclusion is considered in this program.

The code can be run by entering the following commands into a UNIX environment:
$ gcc producer.c -pthread -lrt -o producer
$ gcc consumer.c -pthread -lrt -o consumer
$ ./producer & ./consumer &
