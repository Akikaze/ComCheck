<<<<<<< HEAD
# DEFINITION DES ACRONYMES 
CXXFLAGS = -std=c++11
LDFLAGS =  -g
LDLIBS  =  -ldl
 
SRC     =   file.cpp \
=======
# DEFINITION DES ACRONYMES
CC 	= 	g++
CFLAGS 	= 	-std=c++11
LDFLAGS =	-ldl -g
SRC 	= 	file.cpp \
>>>>>>> adce5fa7ea4ff9469ad1406937c7a885b9dd5e01
			iplugin.cpp \
			plugin.cpp \
			core.cpp \
			main.cpp
<<<<<<< HEAD
  
OBJ     =   $(SRC:.cpp=.o)
EXEC    =   prog
  
=======

OBJ 	= 	$(SRC:.cpp=.o)
EXEC	=	prog

>>>>>>> adce5fa7ea4ff9469ad1406937c7a885b9dd5e01
# EXECUTION
all: $(EXEC)
  
# FABRICATION DES OBJETS (.o)
$(EXEC): $(OBJ)
	$(LINK.cc)  $(LOADLIBES) $(LDLIBS) -o $@ $^
  
# FABRICATION DES (.o) A PARTIR DES (.cpp)
%.o: src/%.cpp
<<<<<<< HEAD
	$(COMPILE.cc) $<
=======
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

>>>>>>> adce5fa7ea4ff9469ad1406937c7a885b9dd5e01
