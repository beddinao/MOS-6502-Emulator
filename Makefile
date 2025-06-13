all: 
	cmake -B build
	cmake --build build

re:
	cmake --build build

clean:
	rm -rf build

