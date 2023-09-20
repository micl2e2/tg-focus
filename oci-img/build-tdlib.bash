buildah rm build-tdlib-container
buildah from --name build-tdlib-container debian:bookworm-slim

buildah run build-tdlib-container -- \
	sed -i 's/deb\.debian\.org/ftp\.cn\.debian\.org/' /etc/apt/sources.list.d/debian.sources

buildah run build-tdlib-container -- \
	apt update
buildah run build-tdlib-container -- \
	apt install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet

test $? -eq 0 || exit 1

# build tdlib
buildah run build-tdlib-container -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone https://github.com/tdlib/td"
# lock src, last commit of 1.8.17
buildah run build-tdlib-container -- \
	bash -c "cd td && git reset --hard a0d026239439c76847c4f75f2fa32f3e56b9b789"
buildah run build-tdlib-container -- \
	bash -c "cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release  -DOPENSSL_CRYPTO_LIBRARY='/lib/x86_64-linux-gnu/libcrypto.a' -DOPENSSL_SSL_LIBRARY='/lib/x86_64-linux-gnu/libssl.a' -DZLIB_LIBRARY_RELEASE='/lib/x86_64-linux-gnu/libz.a' .. && make -j8 && make install"

