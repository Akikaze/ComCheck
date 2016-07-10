# DEFINITION DES ACRONYMES
CC 	= 	g++
CFLAGS 	= 	-std=c++11 -Wall
SRC 	= 	src/file.cpp \
			src/plugin.cpp \
			src/core.cpp \
			src/main.cpp
			
OBJ 	= 	$(SRC:.cpp=.o)
EXEC	=	prog.exe

# EXECUTION
all: $(EXEC)

# FABRICATION DES OBJETS (.o)
$(EXEC): $(OBJ)
	$(CC) -o $@ $^

# FABRICATION DES (.o) A PARTIR DES (.cpp)
src/%.o: %.cpp
	$(CC) -c $(CFLAGS) $<