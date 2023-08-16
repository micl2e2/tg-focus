
buildah rm tdlib-builder-container
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
	bash -c "https_proxy=$HTTPS_PROXY git clone https://github.com/tdlib/td"
# lock src
buildah run tdlib-builder-container -- \
	bash -c "cd td && git reset --hard 53888437cf11aca258aae7e76552a38c1750d6e7"
# 138 commits from 2e5319ff360cd2d6dab638a7e0370fe959e4201b^1, 1.8.15

buildah run tdlib-builder-container -- \
	bash -c 'cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8 && make install'
