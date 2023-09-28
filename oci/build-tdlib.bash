CTN_TDLIB="build-tdlib-container"

flag=$(buildah ps | grep $CTN_TDLIB | wc -l)
test $flag -eq 0 || exit 0

buildah from --name $CTN_TDLIB debian:bookworm-slim

buildah run build-tgfocus-container -- \
	sed -i 's/deb\.debian\.org/ftp\.us\.debian\.org/' /etc/apt/sources.list.d/debian.sources

buildah run $CTN_TDLIB -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_TDLIB -- \
	apt-get -o Acquire::ForceIPv4=true install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet

test $? -eq 0 || exit 2

test -z $HTTPS_PROXY || echo use https proxy $HTTPS_PROXY

buildah run $CTN_TDLIB -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone https://github.com/tdlib/td"

test $? -eq 0 || exit 3

buildah run $CTN_TDLIB -- \
	bash -c "cd td && git reset --hard a0d026239439c76847c4f75f2fa32f3e56b9b789"

test $? -eq 0 || exit 4

buildah run $CTN_TDLIB -- \
	bash -c "cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release  -DOPENSSL_CRYPTO_LIBRARY='/lib/x86_64-linux-gnu/libcrypto.a' -DOPENSSL_SSL_LIBRARY='/lib/x86_64-linux-gnu/libssl.a' -DZLIB_LIBRARY_RELEASE='/lib/x86_64-linux-gnu/libz.a' .. && make -j8 && make install"

build commit $CTN_TDLIB tg-focus-tdlib-1817

TAR_NAME="tg-focus-tdlib-1817"

podman save --output $TAR_NAME.tar --format oci-archive localhost/tg-focus-tdlib-1817:latest

podman inspect localhost/tg-focus-tdlib-1817:latest >$TAR_NAME.json

