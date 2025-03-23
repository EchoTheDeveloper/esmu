CC = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2
SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = esmu
INSTALL_DIR = /usr/local/bin

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

install: $(EXEC)
	sudo cp $(EXEC) $(INSTALL_DIR)

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all install clean

