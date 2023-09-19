.PHONY: build clean test

build:
	@echo "--------------- MAKE BUILD BEG ---------------"
	mkdir -p build
	cmake -DCMAKE_BUILD_TYPE=Debug -B build
	cmake --build build
	@echo "--------------- MAKE BUILD END ---------------"

clean:
	@echo "--------------- MAKE CLEAN BEG ---------------"
	rm -rf ./build
	@echo "--------------- MAKE CLEAN END ---------------"

test: build
	@echo "--------------- MAKE TEST BEG ---------------"
	cd build && ctest
	@echo "--------------- MAKE TEST END ---------------"
