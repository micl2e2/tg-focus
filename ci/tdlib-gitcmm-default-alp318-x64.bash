source vars-x64-linux-musl.bash 

###

buildah ps | grep $CTN_BUILD_TDLIB
test $? -eq 0 && buildah rm $CTN_BUILD_TDLIB || \
	echo "[INFO] $CTN_BUILD_TDLIB not found"
test $? -eq 0 || exit 1

buildah ps | grep $CTN_PACK_TDLIB
test $? -eq 0 && buildah rm $CTN_PACK_TDLIB || \
	echo "[INFO] $CTN_PACK_TDLIB not found"
test $? -eq 0 || exit 2

buildah from --name $CTN_BUILD_TDLIB $PICK_BASEIMG
test $? -eq 0 || exit 3

buildah run $CTN_BUILD_TDLIB -- \
       sed -i "s/dl-cdn.alpinelinux.org/$PICK_APK_MIRROR/" /etc/apk/repositories
test $? -eq 0 || exit 4
 
buildah run $CTN_BUILD_TDLIB -- \
	sh -c "apk update"
test $? -eq 0 || exit 5

buildah run $CTN_BUILD_TDLIB -- \
        sh -c "apk add cmake gperf g++ openssl-dev git make linux-headers zlib-dev"
test $? -eq 0 || exit 6

$PXY_FRONTEND buildah run $CTN_BUILD_TDLIB -- \
	sh -c "git clone https://github.com/tdlib/td"
test $? -eq 0 || exit 7

buildah run $CTN_BUILD_TDLIB -- \
	sh -c "cd td && git reset --hard $PICK_TDLIB_SRC"
test $? -eq 0 || exit 8

buildah run $CTN_BUILD_TDLIB -- \
	sh -c "cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .."
test $? -eq 0 || exit 9

buildah run $CTN_BUILD_TDLIB -- \
	sh -c "cd td/build && make -j$(nproc)"
test $? -eq 0 || exit 10

buildah run $CTN_BUILD_TDLIB -- \
	sh -c "cd td/build && make test"
test $? -eq 0 || exit 11

buildah run $CTN_BUILD_TDLIB -- \
	sh -c "cd td/build && make install"
test $? -eq 0 || exit 11

# package

buildah from --name $CTN_PACK_TDLIB $PICK_BASEIMG
test $? -eq 0 || exit 12

buildah copy --from $CTN_BUILD_TDLIB $CTN_PACK_TDLIB '/usr/local' '/usr/local'
test $? -eq 0 || exit 13

buildah commit $CTN_PACK_TDLIB $PICK_TDLIB_IMGFULLNAME
test $? -eq 0 || exit 14

# save tar
podman save --output $PICK_TDLIB_ARTIFACTNAME.tar --format oci-archive $PICK_TDLIB_IMGFULLNAME
test $? -eq 0 || exit 15

# save json
podman inspect $PICK_TDLIB_IMGFULLNAME >$PICK_TDLIB_ARTIFACTNAME.json
test $? -eq 0 || exit 16

# save checksums
sha512sum $PICK_TDLIB_ARTIFACTNAME.tar >> CHECKSUMS
test $? -eq 0 || exit 17

sha512sum $PICK_TDLIB_ARTIFACTNAME.json >> CHECKSUMS
test $? -eq 0 || exit 18

# Delete only when necessary
# buildah rm $CTN_BUILD_TDLIB
# buildah rm $CTN_PACK_TDLIB
podman rmi $PICK_TDLIB_IMGFULLNAME
