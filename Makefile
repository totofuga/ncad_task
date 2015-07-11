OBJS = server.o ncad_task.o
.SUFFIXES: .c .o

ncad_task: ${OBJS}
	gcc `pkg-config --cflags --libs glib-2.0` server.o ncad_task.o -o ncad_task

.c.o:
	gcc -c $<

mac_test: test.o mac_table.o
	gcc -DTEST `pkg-config --cflags --libs glib-2.0` mac_table.o test.o -o mac_test
	
test.o: test.c
	gcc -DTEST -c test.c

mac_table.o: mac_table.c
	gcc -DTEST -c mac_table.c
