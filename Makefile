OBJS = server.o ncad_task.o arp.o mac_table.o server_detect.o
.SUFFIXES: .c .o

ncad_task: ${OBJS}
	gcc -g `pkg-config --cflags --libs glib-2.0` ${OBJS} -o ncad_task

.c.o:
	gcc -c $<

mac_test: test.o mac_table_test.o
	gcc -DTEST `pkg-config --cflags --libs glib-2.0` mac_table_test.o test.o -o mac_test

detect_test: test.o server_detect_test.o
	gcc -DTEST server_detect_test.o test.o -o detect_test

test.o: test.c
	gcc -DTEST -c test.c

mac_table_test.o: mac_table.c
	gcc -DTEST -c mac_table.c -o mac_table_test.o

server_detect_test.o: server_detect.c
	gcc -DTEST -c server_detect.c -o server_detect_test.o

clean:
	rm -rf *.o
	rm -rf ncad_task mac_test detect_test
