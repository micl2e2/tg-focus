
buildah from --name tdlib-builder-container debian:bullseye-slim

# install dependencies
buildah run tdlib-builder-container -- \
	bash -c 'sed "s/main/main contrib/" /etc/apt/sources.list -i'
buildah run tdlib-builder-container -- \
	bash -c 'sed "s/deb\.debian\.org/ftp\.cn\.debian\.org/" /etc/apt/sources.list -i'
buildah run tdlib-builder-container -- \
	apt update
buildah run tdlib-builder-container -- \
	apt install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet

# build tdlib
buildah run tdlib-builder-container -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/tdlib/td"
buildah run tdlib-builder-container -- \
	bash -c 'cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 && make install'
