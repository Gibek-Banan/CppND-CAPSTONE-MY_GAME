.PHONY: prepare configure build test run callgrind all

prepare:
	rm -rf build
	mkdir build
configure: prepare
	cd build && cmake ..
build: configure
	cd build && cmake --build .
test: build
	# cd build && make test
run:
	cd build && ./exe
callgrind:
	cd build && valgrind --tool=callgrind ./exe && kcachegrin callgrind.out.*
all: prepare configure build test run
