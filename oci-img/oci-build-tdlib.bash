buildah rm tdlib-builder-container
buildah from --name tdlib-builder-container debian:bookworm-slim

# install dependencies
# buildah run tdlib-builder-container -- \
# 	bash -c 'sed "s/main/main contrib/" /etc/apt/sources.list -i'
# buildah run tdlib-builder-container -- \
# 	bash -c 'sed "s/deb\.debian\.org/ftp\.cn\.debian\.org/" /etc/apt/sources.list -i'
buildah run tdlib-builder-container -- \
	apt update
buildah run tdlib-builder-container -- \
	apt install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet

# build tdlib
buildah run tdlib-builder-container -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone https://github.com/tdlib/td"
# lock src, last commit of 1.8.17
buildah run tdlib-builder-container -- \
	bash -c "cd td && git reset --hard a0d026239439c76847c4f75f2fa32f3e56b9b789"
buildah run tdlib-builder-container -- \
	bash -c 'cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 && make install'

