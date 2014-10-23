OUTPUT   = wire_example
CC       = clang
CXXFLAGS = -std=c++14 -Wall -Wextra -pedantic -Wno-unused-variable -Wno-reorder
LDFLAGS  = -lstdc++

OPTIM    = -Ofast
SRCDIR   = src
OBJDIR   = obj
BUILDDIR = build

SOURCES  = main.cpp
OBJ      = $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))

.PHONY: all clean

all: $(BUILDDIR)/$(OUTPUT)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $< $(CXXFLAGS) $(OPTIM) -o $@
    
$(BUILDDIR)/$(OUTPUT): $(OBJ)
	@echo :: Compiling $(OUTPUT)...
	$(CC) $^ $(LDFLAGS) -o $@
	@echo :: done.

clean:
	rm $(OBJDIR)/*.o $(BUILDDIR)/$(OUTPUT)

