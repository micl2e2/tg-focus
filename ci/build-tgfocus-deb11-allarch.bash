set -x

cmake -S .. -B ../build -DCMAKE_CXX_COMPILER=/usr/local/bin/tgf-g++ -DCMAKE_C_COMPILER=/usr/local/bin/tgf-gcc \
      -DCMAKE_BUILD_TYPE=Release
      # -DCMAKE_BUILD_TYPE=Debug
[[ $? -eq 0 ]] || exit 255

LD_LIBRARY_PATH=/usr/local/lib/gcc/${PICK_LIBDIRPREFIX}-linux-gnu/${PICK_GCC_REL}/:/usr/local/lib64/ cmake --build ../build -j$(nproc)
[[ $? -eq 0 ]] || exit 255

LD_LIBRARY_PATH=/usr/local/lib/gcc/${PICK_LIBDIRPREFIX}-linux-gnu/${PICK_GCC_REL}/:/usr/local/lib64/ TZ=UTC bash -c "cd ../build && ctest"
[[ $? -eq 0 ]] || exit 255


