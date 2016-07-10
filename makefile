# DEFINITION DES ACRONYMES
CC 	= 	g++
CFLAGS 	= 	-std=c++11 -Wall -Wextra -ansi -pedantic -g -pg
SRC 	= 	file.cpp \
			plugin.cpp \
			core.cpp \
			main.cpp

OBJ 	= 	$(SRC:.cpp=.o)
EXEC	=	prog.exe

# EXECUTION
all: $(EXEC)

# FABRICATION DES OBJETS (.o)
$(EXEC): $(OBJ)
	$(CC) -o $@ $^

# FABRICATION DES (.o) A PARTIR DES (.cpp)
%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $<

# CLEAN
clean :
	rm -rf $(OBJ)

# MRPROPER
mrproper : clean
	rm -rf $(EXEC)
