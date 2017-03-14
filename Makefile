ifeq ($(IsTravis),)
	# leave CXX blank for travis, set it otherwise
	CXX=g++
else
	# turn on coverage for travis
	CoverageFlags=--coverage
endif

CFlags=-c -Wall -O3 -pedantic -MMD -std=c++11 -Werror
GSLFlags=-lgsl -lgslcblas
Sources=$(wildcard src/*.cpp)
IncludeDir=-I./include
AllObjects=$(addprefix obj/,$(notdir $(Sources:.cpp=.o)))
Executables=main Test
Objects=$(filter-out $(addprefix obj/,$(Executables:=.o)),$(AllObjects))

all: $(Sources) $(Executables)

$(Executables): $(AllObjects)
	@mkdir -p data/test obj
	$(CXX) $(CoverageFlags) $(Objects) $(addprefix obj/,$@.o) $(GSLFlags) -o $@

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFlags) $(CoverageFlags) $(IncludeDir) $< -o $@

-include $(AllObjects:.o=.d)

test: $(Executables)
	./Test

clean:
	rm -rf obj/*.o obj/*.d $(Executables)
