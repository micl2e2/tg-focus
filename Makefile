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

test:
	@echo "--------------- MAKE TEST BEG ---------------"
	cmake --build build -j$(nproc) && ctest --test-dir build -j$(nproc)
	@echo "--------------- MAKE TEST END ---------------"
