# This code is free to use, copy, distribute, and modify.
# If you use this code or any modification of this code, we request that you reference both this code https://zenodo.org/record/x and the paper https://arxiv.org/abs/17xx.xxxxx.

ifeq ($(IsTravis),)
	# leave CXX blank for travis, set it otherwise
	CXX=g++
endif

CFlags=-c -Wall -O3 -pedantic -MMD -std=c++0x -Werror
GSLFlags=-lgsl -lgslcblas
Sources=$(wildcard src/*.cpp)
IncludeDir=-I./include
AllObjects=$(addprefix obj/,$(notdir $(Sources:.cpp=.o)))
Executables=main
Objects=$(filter-out $(addprefix obj/,$(Executables:=.o)),$(AllObjects))

all: $(Sources) $(Executables)

$(Executables): $(AllObjects)
	@mkdir -p data obj fig
	$(CXX) $(Objects) $(addprefix obj/,$@.o) $(GSLFlags) -o $@

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFlags) $(IncludeDir) $< -o $@

-include $(AllObjects:.o=.d)

clean:
	rm -rf obj/*.o obj/*.d $(Executables)

