CC = g++ 
APPS = datastructures

$APPS : main.o doubly_linked_list.o hash_table.o single_linked_list.o sorted_array.o queue.o binary_tree.o
		$(CC) -o datastructures main.o doubly_linked_list.o hash_table.o single_linked_list.o sorted_array.o queue.o binary_tree.o

main.o: main.c hash_table.h single_linked_list.h doubly_linked_list.h global_vars.h
		$(CC) -c main.c
doubly_linked_list.o: doubly_linked_list.c doubly_linked_list.h global_vars.h
		 $(CC) -c doubly_linked_list.c
hash_table.o: hash_table.c hash_table.h global_vars.h
		$(CC) -c hash_table.c
single_linked_list.o: single_linked_list.c single_linked_list.h global_vars.h
		$(CC) -c single_linked_list.c
queue.o: queue.c queue.h global_vars.h
		$(CC) -c queue.c
binary_tree.o: binary_tree.c binary_tree.h global_vars.h
		$(CC) -c binary_tree.c
clean:
	rm -f $(APPS)
	rm -f *.o
	find . -type f -name '*.o' -delete
