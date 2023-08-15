buildah from --name tdlib-builder-container2 alpine

buildah run tdlib-builder-container2 -- \
	apk add --no-cache cmake gperf g++ openssl-dev git wget make linux-headers

buildah run tdlib-builder-container2 -- \
	bash -c "cd td && git reset --hard 53888437cf11aca258aae7e76552a38c1750d6e7"

buildah run tdlib-builder-container2 -- \
	bash -c 'cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 && make install'
