.PHONY: all test clean clean-all clean-local editor client common server build

compile-debug:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build  build/ $(EXTRA_COMPILE)

run-tests: compile-debug
	./build/taller_tests

all: clean-local run-tests

# 🔸 Limpieza liviana: borra binarios y objetos, pero no las libs externas
clean: clean-local

clean-local:
	find build/ -type f -name '*.o' -delete
	find build/ -type f -executable -delete
	rm -f build/*.a
	rm -f build/*.so
	rm -f build/*.out

# 🔴 Limpieza total: borra TODO, incluyendo FetchContent (SDL, GoogleTest, etc)
clean-all:
	rm -rf build/
