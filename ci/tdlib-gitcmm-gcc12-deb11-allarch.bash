
sudo apt-get install -y --quiet buildah podman

# remove tmpctn if exists
buildah ps | grep $CTN_BUILD_TDLIB
[[ $? -eq 0 ]] && buildah rm $CTN_BUILD_TDLIB || \
	echo "[INFO] $CTN_BUILD_TDLIB not found"
[[ $? -eq 0 ]] || exit 1

buildah ps | grep $CTN_PACK_TDLIB
[[ $? -eq 0 ]] && buildah rm $CTN_PACK_TDLIB || \
	echo "[INFO] $CTN_PACK_TDLIB not found"
[[ $? -eq 0 ]] || exit 2

buildah ps | grep $CTN_PACK_TOOLC
[[ $? -eq 0 ]] && buildah rm $CTN_PACK_TOOLC || \
	echo "[INFO] $CTN_PACK_TOOLC not found"
[[ $? -eq 0 ]] || exit 2

# prepare gcc img
buildah ps | grep $CTN_PACK_TDLIB
if [[ $? -ne 0 ]]
then
    buildah from --name $CTN_PACK_TOOLC "docker.io/micl2e2/$PICK_TOOLC_IMGFULLNAME"
    [[ $? -eq 0 ]] || exit 3
    buildah inspect "docker.io/micl2e2/$PICK_TOOLC_IMGFULLNAME" | grep FromImageID
    [[ $? -eq 0 ]] || exit 3
fi

# prepare build ctn
buildah from --name $CTN_BUILD_TDLIB $PICK_BASEIMG
[[ $? -eq 0 ]] || exit 4

USE_APT_MIRROR="deb.debian.org"
[[ -n $PICK_APT_MIRROR ]] && USE_APT_MIRROR=$PICK_APT_MIRROR
buildah run $CTN_BUILD_TDLIB -- \
	sed -i "s/deb.debian.org/${USE_APT_MIRROR}/" /etc/apt/sources.list
[[ $? -eq 0 ]] || exit 3

buildah run $CTN_BUILD_TDLIB -- \
	apt-get -o Acquire::ForceIPv4=true update
[[ $? -eq 0 ]] || exit 5

buildah run $CTN_BUILD_TDLIB -- \
	apt-get -o Acquire::ForceIPv4=true install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet
[[ $? -eq 0 ]] || exit 6

$PXY_FRONTEND buildah run $CTN_BUILD_TDLIB -- \
	bash -c "git clone --depth=1000 https://github.com/tdlib/td"
[[ $? -eq 0 ]] || exit 7

buildah run $CTN_BUILD_TDLIB -- \
	bash -c "cd td && git reset --hard $PICK_TDLIB_SRC"
[[ $? -eq 0 ]] || exit 8

# grab gcc bins
buildah copy --from $CTN_PACK_TOOLC $CTN_BUILD_TDLIB '/usr/local' '/usr/local'
[[ $? -eq 0 ]] || exit 9

buildah run $CTN_BUILD_TDLIB -- \
	bash -c "cd td && cmake -DCMAKE_BUILD_TYPE=Release -DOPENSSL_USE_STATIC_LIBS=TRUE -DZLIB_USE_STATIC_LIBS=ON -DCMAKE_C_COMPILER=/usr/local/bin/alt-gcc -DCMAKE_CXX_COMPILER=/usr/local/bin/alt-g++ -B build"
[[ $? -eq 0 ]] || exit 10

buildah run $CTN_BUILD_TDLIB -- \
	bash -c 'cd td/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make -j$(nproc)'
[[ $? -eq 0 ]] || exit 11

buildah run $CTN_BUILD_TDLIB -- \
	bash -c 'cd td/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make test'
[[ $? -eq 0 ]] || exit 12

buildah run $CTN_BUILD_TDLIB -- \
	bash -c 'cd td/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make install'
[[ $? -eq 0 ]] || exit 13

# package

buildah from --name $CTN_PACK_TDLIB $PICK_BASEIMG
[[ $? -eq 0 ]] || exit 14

buildah copy --from $CTN_BUILD_TDLIB $CTN_PACK_TDLIB '/usr/local' '/usr/local'
[[ $? -eq 0 ]] || exit 15

buildah commit $CTN_PACK_TDLIB $PICK_TDLIB_IMGFULLNAME
[[ $? -eq 0 ]] || exit 16

# save tar
podman save --output $PICK_TDLIB_ARTIFACTNAME.tar --format oci-archive $PICK_TDLIB_IMGFULLNAME
[[ $? -eq 0 ]] || exit 17

# save json
buildah inspect $PICK_TDLIB_IMGFULLNAME >$PICK_TDLIB_ARTIFACTNAME.json
[[ $? -eq 0 ]] || exit 18

# save checksums
sha512sum $PICK_TDLIB_ARTIFACTNAME.tar >> CHECKSUMS
[[ $? -eq 0 ]] || exit 19
sha512sum $PICK_TDLIB_ARTIFACTNAME.json >> CHECKSUMS
[[ $? -eq 0 ]] || exit 20

