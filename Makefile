.PHONY: all test clean clean-all clean-local editor client common server build valgrind-client valgrind-server run-server run-client run-tests

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


# Compilación específica de componentes
compile-client:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build build/ --target taller_client $(EXTRA_COMPILE)

compile-server:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build build/ --target taller_server $(EXTRA_COMPILE)

compile-editor:
	mkdir -p build/
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug $(EXTRA_GENERATE)
	cmake --build build/ --target taller_editor $(EXTRA_COMPILE)


# 🔍 Ejecución con Valgrind para detección de memory leaks
valgrind-client: compile-debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./valgrind-suppressions.txt ./build/taller_client

valgrind-server: compile-debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./valgrind-suppressions.txt ./build/taller_server 8080


# 🚀 Comandos para ejecutar los componentes
run-server: compile-server
	@echo "Iniciando servidor en puerto 8080..."
	@./build/taller_server 8080

run-client: compile-client
	@echo "Iniciando cliente..."
	@./build/taller_client

run-editor: compile-editor
	@echo "Iniciando editor de mapas..."
	@./build/taller_editor