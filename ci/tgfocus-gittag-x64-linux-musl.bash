source vars-x64-linux-musl.bash

###

# prepare tdlib
buildah ps | grep $CTN_PACK_TDLIB
if [[ $? -ne 0 ]]
then
    buildah from --name $CTN_PACK_TDLIB "docker.io/micl2e2/$PICK_TDLIB_IMGFULLNAME"
    test $? -eq 0 || exit 1
    podman inspect "docker.io/micl2e2/$PICK_TDLIB_IMGFULLNAME" | grep Id
    test $? -eq 0 || exit 1
fi

# remove build and pack
buildah ps | grep $CTN_BUILD_TGFOCUS
test $? -eq 0 && buildah rm $CTN_BUILD_TGFOCUS || \
	echo "[INFO] $CTN_PACK_TGFOCUS not found"
test $? -eq 0 || exit 2

buildah ps | grep $CTN_PACK_TGFOCUS
test $? -eq 0 && buildah rm $CTN_PACK_TGFOCUS || \
	echo "[INFO] $CTN_PACK_TGFOCUS not found"
test $? -eq 0 || exit 2

buildah from --name $CTN_BUILD_TGFOCUS $PICK_BASEIMG
test $? -eq 0 || exit 2

buildah copy --from $CTN_PACK_TDLIB $CTN_BUILD_TGFOCUS '/usr/local' '/usr/local'
test $? -eq 0 || exit 3

buildah run $CTN_BUILD_TGFOCUS -- \
       sed -i "s/dl-cdn.alpinelinux.org/$PICK_APK_MIRROR/" /etc/apk/repositories
test $? -eq 0 || exit 4

buildah run $CTN_BUILD_TGFOCUS -- \
        sh -c "apk update"
test $? -eq 0 || exit 5

buildah run $CTN_BUILD_TGFOCUS -- \
        sh -c "apk add pkgconfig gcc g++ git cmake make"
test $? -eq 0 || exit 6

# better use *-dev
buildah run $CTN_BUILD_TGFOCUS -- \
        sh -c "ln -s /usr/lib/libssl.so.3 /usr/lib/libssl.so"
test $? -eq 0 || exit 7
buildah run $CTN_BUILD_TGFOCUS -- \
        sh -c "ln -s /usr/lib/libcrypto.so.3 /usr/lib/libcrypto.so"
test $? -eq 0 || exit 8
buildah run $CTN_BUILD_TGFOCUS -- \
        sh -c "ln -s /lib/libz.so.1 /lib/libz.so"
test $? -eq 0 || exit 9

$PXY_FRONTEND buildah run $CTN_BUILD_TGFOCUS -- \
	sh -c "git clone https://github.com/micl2e2/tg-focus"
test $? -eq 0 || exit 10

$PXY_FRONTEND buildah run $CTN_BUILD_TGFOCUS -- \
        sh -c "cd tg-focus && git checkout $PICK_TGFOCUS_GITTAG"
test $? -eq 0 || exit 11

$PXY_FRONTEND buildah run $CTN_BUILD_TGFOCUS -- \
	sh -c "cd tg-focus && sh dev/install-deps-oci-only.bash"
test $? -eq 0 || exit 12

buildah run $CTN_BUILD_TGFOCUS -- \
	sh -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -B build"
test $? -eq 0 || exit 13

buildah run $CTN_BUILD_TGFOCUS -- \
	sh -c 'cd tg-focus/build && make -j$(nproc)'
test $? -eq 0 || exit 14

buildah run $CTN_BUILD_TGFOCUS -- \
	sh -c "cd tg-focus/build && make test" # always UTC for clean alpine
test $? -eq 0 || exit 15

buildah run $CTN_BUILD_TGFOCUS -- \
	sh -c "cd tg-focus && strip /tg-focus/build/tgf-conf && strip /tg-focus/build/tgf-focusd"
test $? -eq 0 || exit 16


# package

buildah from --name $CTN_PACK_TGFOCUS $PICK_BASEIMG
test $? -eq 0 || exit 17

buildah copy --from $CTN_BUILD_TGFOCUS $CTN_PACK_TGFOCUS \
	'/tg-focus/build/tgf-conf' '/usr/local/bin'
test $? -eq 0 || exit 18

buildah copy --from $CTN_BUILD_TGFOCUS $CTN_PACK_TGFOCUS \
	'/tg-focus/build/tgf-focusd' '/usr/local/bin'
test $? -eq 0 || exit 18

buildah copy --from $CTN_BUILD_TGFOCUS $CTN_PACK_TGFOCUS \
	'/tg-focus/README.md' '/usr/local/bin'
test $? -eq 0 || exit 18

buildah copy --from $CTN_BUILD_TGFOCUS $CTN_PACK_TGFOCUS \
	'/tg-focus/ChangeLog' '/usr/local/bin'
test $? -eq 0 || exit 18

buildah copy --from $CTN_BUILD_TGFOCUS $CTN_PACK_TGFOCUS \
	'/tg-focus/LICENSE-GPL' '/usr/local/bin'
test $? -eq 0 || exit 18

buildah copy --from $CTN_BUILD_TGFOCUS $CTN_PACK_TGFOCUS \
	'/tg-focus/3rd/toml11/LICENSE' '/usr/local/bin/LICENSE-3rd-toml11'
test $? -eq 0 || exit 18

buildah run $CTN_PACK_TGFOCUS -- sh -c "apk add --no-cache nano"
buildah config --cmd "/bin/sh -c tgf-focusd" $CTN_PACK_TGFOCUS

# oci img artifact
buildah commit $CTN_PACK_TGFOCUS $PICK_TGFOCUS_IMGFULLNAME
test $? -eq 0 || exit 19

# save tar
podman save --output $PICK_TGFOCUS_ARTIFACTNAME.tar \
       --format oci-archive $PICK_TGFOCUS_IMGFULLNAME
test $? -eq 0 || exit 20
# save json
podman inspect $PICK_TGFOCUS_IMGFULLNAME >$PICK_TGFOCUS_ARTIFACTNAME.json
test $? -eq 0 || exit 21

# save checksums
sha512sum $PICK_TGFOCUS_ARTIFACTNAME.tar >> CHECKSUMS
test $? -eq 0 || exit 22
sha512sum $PICK_TGFOCUS_ARTIFACTNAME.json >> CHECKSUMS
test $? -eq 0 || exit 23

# Delete only when necessary
buildah rm $CTN_BUILD_TGFOCUS
buildah rm $CTN_PACK_TGFOCUS
buildah rm $CTN_PACK_TDLIB
podman rmi $PICK_TGFOCUS_IMGFULLNAME

