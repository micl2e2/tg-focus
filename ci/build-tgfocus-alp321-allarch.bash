set -x

cmake -S .. -B ../build -DCMAKE_BUILD_TYPE=Release
[[ $? -eq 0 ]] || exit 255

cmake --build ../build -j$(nproc)
[[ $? -eq 0 ]] || exit 255

TZ=UTC ctest --test-dir ../build
[[ $? -eq 0 ]] || exit 255


