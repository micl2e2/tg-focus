
# ATTENTION: time consuming

CTN_TDLIB="build-tdlib-container"
CTN_PACK="package-tdlib-container"

flag=$(buildah ps | grep $CTN_TDLIB | wc -l)
test $flag -eq 0 || buildah rm $CTN_TDLIB

buildah from --name $CTN_TDLIB alpine:3.18

buildah run $CTN_TDLIB -- \
	sed -i 's/https/http/' /etc/apk/repositories
buildah run $CTN_TDLIB -- \
	sh -c "https_proxy=$HTTPS_PROXY apk update"
buildah run $CTN_TDLIB -- \
        sh -c "https_proxy=$HTTPS_PROXY apk add cmake gperf g++ openssl-dev git make linux-headers zlib-dev zlib-static openssl-libs-static"

test $? -eq 0 || exit 2

test -z $HTTPS_PROXY || echo use https proxy $HTTPS_PROXY

buildah run $CTN_TDLIB -- \
	sh -c "https_proxy=$HTTPS_PROXY git clone https://github.com/tdlib/td"

test $? -eq 0 || exit 3

PICK_SRC="a0d026239439c76847c4f75f2fa32f3e56b9b789"

buildah run $CTN_TDLIB -- \
	sh -c "cd td && git reset --hard $PICK_SRC"

test $? -eq 0 || exit 4

buildah run $CTN_TDLIB -- \
	sh -c "cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release  .. && make -j$(nproc) && make install"

buildah from --name $CTN_PACK alpine:3.18
buildah copy --from $CTN_TDLIB $CTN_PACK '/usr/local/include' '/usr/local/include'
buildah copy --from $CTN_TDLIB $CTN_PACK '/usr/local/lib' '/usr/local/lib'

TAR_NAME="tdlib-$PICK_SRC"

buildah commit $CTN_PACK $TAR_NAME

podman save --output $TAR_NAME.tar --format oci-archive localhost/$TAR_NAME:latest

podman inspect localhost/$TAR_NAME:latest >$TAR_NAME.json

