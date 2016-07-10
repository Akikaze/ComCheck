# DEFINITION DES ACRONYMES
CC 	= 	g++
CFLAGS 	= 	-std=c++11
LDFLAGS =	-ldl -g
SRC 	= 	file.cpp \
			iplugin.cpp \
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
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

# CLEAN
clean :
	rm -rf $(OBJ)

# MRPROPER
mrproper : clean
	rm -rf $(EXEC)
	
# WHY
why :
	g++ -std=c++11 src/file.cpp src/iplugin.cpp src/plugin.cpp src/core.cpp src/main.cpp -ldl -g -o prog.exe

