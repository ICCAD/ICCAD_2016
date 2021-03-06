CC = g++
CFLAGS = -c
OPTFLAGS = -O3

bin/ICCAD_2016:main.o gds_read.o Bron_Kerbosch.o
	$(CC) $(OPTFLAGS) -g -o bin/ICCAD_2016 main.o gds_read.o Bron_Kerbosch.o
main.o:src/main.cpp src/gds_read.h
	$(CC) -g $(CFLAGS) $(OPTFLAGS) src/main.cpp src/gds_read.h src/data_structure.h
gds_read.o:src/gds_read.cpp src/gds_read.h 
	$(CC) -g $(CFLAGS) $(OPTFLAGS) src/gds_read.cpp src/gds_read.h src/data_structure.h
Bron_Kerbosch.o:src/Bron_Kerbosch.cpp src/Bron_Kerbosch.h
	$(CC) -g $(CFLAGS) $(OPTFLAGS) src/Bron_Kerbosch.cpp src/Bron_Kerbosch.h
clean:
	rm -rf *.o bin/*