# DEFINITION DES ACRONYMES 
CXXFLAGS =	-std=c++11
LDFLAGS	=	-g
LDLIBS =	-ldl

SRC		=	file.cpp \
			iplugin.cpp \
			plugin.cpp \
			core.cpp \
			main.cpp

OBJ		=	$(SRC:.cpp=.o)
EXEC	=	prog
  
# EXECUTION
all: $(EXEC)

# FABRICATION DES OBJETS (.o)
$(EXEC): $(OBJ)
	$(LINK.cc)  $(LOADLIBES) $(LDLIBS) -o $@ $^

# FABRICATION DES (.o) A PARTIR DES (.cpp)
%.o: src/%.cpp
	$(COMPILE.cc) $< 
