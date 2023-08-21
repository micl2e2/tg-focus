buildah rm tdlib-builder-container2
buildah from --name tdlib-builder-container2 alpine:latest

# install deps
buildah run tdlib-builder-container2 -- \
	apk update
buildah run tdlib-builder-container2 -- \
	apk add --no-cache cmake gperf g++ openssl-dev git make linux-headers zlib-dev

# build tdlib
buildah run tdlib-builder-container2 -- \
	sh -c "https_proxy=$HTTPS_PROXY git clone https://github.com/tdlib/td"

buildah run tdlib-builder-container2 -- \
	sh -c "cd td && git reset --hard 53888437cf11aca258aae7e76552a38c1750d6e7"
# 138 commits from 2e5319ff360cd2d6dab638a7e0370fe959e4201b^1, 1.8.15

buildah run tdlib-builder-container2 -- \
	sh -c 'cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j2 && make install'
