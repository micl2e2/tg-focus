set -x

cmake -S .. -B ../build -DCMAKE_CXX_COMPILER=/usr/local/bin/tgf-g++ -DCMAKE_C_COMPILER=/usr/local/bin/tgf-gcc -DCMAKE_BUILD_TYPE=Release

LD_LIBRARY_PATH=/usr/local/lib/gcc/x86_64-pc-linux-gnu/${PICK_GCC_REL}/:/usr/local/lib64/ cmake --build ../build -j$(nproc)

LD_LIBRARY_PATH=/usr/local/lib/gcc/x86_64-pc-linux-gnu/${PICK_GCC_REL}/:/usr/local/lib64/ TZ=UTC bash -c "cd ../build && ctest"


