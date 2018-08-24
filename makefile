CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = FileUtils.h Events.h 
OBJ = FileUtils.o Events.o FileEx.o 
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

FileEx: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

