source vars-x64-linux-gnu.bash 
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
test $? -eq 0 && buildah rm $CTN_BUILD_TGFOCUS  || \
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

if [[ -n $APT_COUNTRY_CODE ]]
then
    buildah run $CTN_BUILD_TGFOCUS -- \
	    sed -i \
	    "s/deb\.debian\.org/ftp\.$APT_COUNTRY_CODE\.debian\.org/" \
	    /etc/apt/sources.list
    test $? -eq 0 || exit 4
fi

buildah run $CTN_BUILD_TGFOCUS -- \
	apt-get -o Acquire::ForceIPv4=true update
    test $? -eq 0 || exit 4

buildah run $CTN_BUILD_TGFOCUS -- \
	apt-get -o Acquire::ForceIPv4=true \
	install \
	g++ git make cmake zlib1g-dev libssl-dev locales \
	-y --quiet
test $? -eq 0 || exit 4

# locales
buildah run $CTN_BUILD_TGFOCUS -- \
	sed -i 's/# en_HK.UTF-8/en_HK.UTF-8/' /etc/locale.gen
test $? -eq 0 || exit 6
buildah run $CTN_BUILD_TGFOCUS -- \
	sed -i 's/# en_US.UTF-8/en_US.UTF-8/' /etc/locale.gen
test $? -eq 0 || exit 7
buildah run $CTN_BUILD_TGFOCUS -- \
	sed -i 's/# en_ZW.UTF-8/en_ZW.UTF-8/' /etc/locale.gen
test $? -eq 0 || exit 8
buildah run $CTN_BUILD_TGFOCUS -- \
	sed -i 's/# zh_CN.UTF-8/zh_CN.UTF-8/' /etc/locale.gen
test $? -eq 0 || exit 81
buildah run $CTN_BUILD_TGFOCUS -- \
	sed -i 's/# zh_HK.UTF-8/zh_HK.UTF-8/' /etc/locale.gen
test $? -eq 0 || exit 82
buildah run $CTN_BUILD_TGFOCUS -- \
        locale-gen
test $? -eq 0 || exit 9

$PXY_FRONTEND buildah run $CTN_BUILD_TGFOCUS -- \
        git clone https://github.com/micl2e2/tg-focus
test $? -eq 0 || exit 11

$PXY_FRONTEND buildah run $CTN_BUILD_TGFOCUS -- \
        bash -c "cd tg-focus && git checkout $PICK_TGFOCUS_GITTAG"
test $? -eq 0 || exit 12

$PXY_FRONTEND buildah run $CTN_BUILD_TGFOCUS -- \
	bash -c "cd tg-focus && bash dev/install-deps-oci-only.bash"
test $? -eq 0 || exit 13

buildah run $CTN_BUILD_TGFOCUS -- \
	bash -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/local/bin/alt-gcc -DCMAKE_CXX_COMPILER=/usr/local/bin/alt-g++ -B build"
test $? -eq 0 || exit 14

buildah run $CTN_BUILD_TGFOCUS -- \
	bash -c 'cd tg-focus/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make -j$(nproc)'
test $? -eq 0 || exit 15

buildah run $CTN_BUILD_TGFOCUS -- \
	bash -c 'cd tg-focus/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH TZ=UTC make test'
test $? -eq 0 || exit 16

buildah run $CTN_BUILD_TGFOCUS -- \
	bash -c "cd tg-focus && strip /tg-focus/build/tgf-conf && strip /tg-focus/build/tgf-focusd"
test $? -eq 0 || exit 17


# package

buildah commit $CTN_BUILD_TGFOCUS $PICK_TGFOCUS_IMGFULLNAME
test $? -eq 0 || exit 13

tmpctn=$(podman create $PICK_TGFOCUS_IMGFULLNAME)
test $? -eq 0 || exit 14

rm -rf $PICK_TGFOCUS_ARTIFACTNAME
mkdir -p $PICK_TGFOCUS_ARTIFACTNAME/license

podman cp $tmpctn:/tg-focus/build/tgf-conf $PICK_TGFOCUS_ARTIFACTNAME/tgf-conf
test $? -eq 0 || exit 15

podman cp $tmpctn:/tg-focus/build/tgf-focusd $PICK_TGFOCUS_ARTIFACTNAME/tgf-focusd
test $? -eq 0 || exit 16

podman cp $tmpctn:/tg-focus/README.md $PICK_TGFOCUS_ARTIFACTNAME/README.md
test $? -eq 0 || exit 16

podman cp $tmpctn:/tg-focus/ChangeLog $PICK_TGFOCUS_ARTIFACTNAME/ChangeLog
test $? -eq 0 || exit 16

podman cp $tmpctn:/tg-focus/LICENSE-GPL $PICK_TGFOCUS_ARTIFACTNAME/license/LICENSE-GPL
test $? -eq 0 || exit 16

podman cp $tmpctn:/tg-focus/3rd/toml11/LICENSE $PICK_TGFOCUS_ARTIFACTNAME/license/LICENSE-toml11
test $? -eq 0 || exit 16

tar --create --gzip --file $PICK_TGFOCUS_ARTIFACTNAME.tar.gz $PICK_TGFOCUS_ARTIFACTNAME
test $? -eq 0 || exit 17

sha512sum $PICK_TGFOCUS_ARTIFACTNAME.tar.gz > CHECKSUMS
test $? -eq 0 || exit 18

podman rm $tmpctn
podman rm $CTN_BUILD_TGFOCUS
podman rm $CTN_PACK_TDLIB
podman rmi $PICK_TGFOCUS_IMGFULLNAME
test $? -eq 0 || exit 19



