
mac_test: test.o mac_table.o
	gcc -pedantic -DTEST `pkg-config --cflags --libs glib-2.0` mac_table.o test.o -o mac_test
	
test.o: test.c
	gcc -DTEST -c test.c

mac_table.o: mac_table.c
	gcc -DTEST -c mac_table.c
