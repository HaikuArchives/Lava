CC = gcc
CCFLAGS = -O2
LDFLAGS = -lbe

TestBurnDevice: TestBurnDevice.cpp TestBurnDevice.h BurnDevice.o
	$(CC) $(CCFLAGS) $(LDFLAGS) TestBurnDevice.cpp BurnDevice.o -o TestBurnDevice

BurnDevice.o: ../BurnDevice.cpp ../BurnDevice.h
	$(CC) $(CCFLAGS) -c BurnDevice.cpp -o BurnDevice.o

clean:
	rm -f *.o TestBurnDevice
