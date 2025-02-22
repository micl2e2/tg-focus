source vars-x64-linux-gnu.bash 
###

# skip building if already built
buildah ps | grep $CTN_PACK_TOOLC
test $? -eq 0 && exit 1

buildah from --name $CTN_BUILD_TOOLC $PICK_BASEIMG
test $? -eq 0 || exit 2

if [[ -n $APT_COUNTRY_CODE ]]
then
    buildah run $CTN_BUILD_TOOLC -- \
	    sed -i \
	    "s/deb\.debian\.org/ftp\.$APT_COUNTRY_CODE\.debian\.org/" \
	    /etc/apt/sources.list
    test $? -eq 0 || exit 3
fi

buildah run $CTN_BUILD_TOOLC -- \
	apt-get -o Acquire::ForceIPv4=true update
test $? -eq 0 || exit 4

buildah run $CTN_BUILD_TOOLC -- \
	apt-get -o Acquire::ForceIPv4=true install g++ make wget curl bzip2 autogen dejagnu -y --quiet
test $? -eq 0 || exit 5

$PXY_FRONTEND buildah run $CTN_BUILD_TOOLC -- \
	bash -c "wget http://ftp.gnu.org/gnu/gcc/gcc-${PICK_GCC_REL}/gcc-${PICK_GCC_REL}.tar.gz"
test $? -eq 0 || exit 6

buildah run $CTN_BUILD_TOOLC -- \
	bash -c "tar xf gcc-${PICK_GCC_REL}.tar.gz"
test $? -eq 0 || exit 7

$PXY_FRONTEND buildah run $CTN_BUILD_TOOLC -- \
	bash -c "cd gcc-${PICK_GCC_REL} && ./contrib/download_prerequisites"
test $? -eq 0 || exit 8

buildah run $CTN_BUILD_TOOLC -- \
	bash -c "mkdir objdir && cd objdir && ../gcc-${PICK_GCC_REL}/configure --program-prefix='alt-' --disable-multilib --disable-libsanitizer --enable-languages=c,c++ --enable-shared=libgcc,libstdc++"
test $? -eq 0 || exit 9

# build 100min+ on 4C, 1h20m on GH
buildah run $CTN_BUILD_TOOLC -- \
	bash -c 'cd objdir && make -j$(nproc)'
test $? -eq 0 || exit 10

# test 6h+ on GH
# buildah run $CTN_BUILD_TOOLC -- \
	# bash -c 'cd objdir && make -j1 -k check-c++'

test $? -eq 0 || exit 11

buildah run $CTN_BUILD_TOOLC -- \
	bash -c 'cd objdir && make install'
test $? -eq 0 || exit 12

# package

buildah from --name $CTN_PACK_TOOLC $PICK_BASEIMG
test $? -eq 0 || exit 13
buildah copy --from $CTN_BUILD_TOOLC $CTN_PACK_TOOLC '/usr/local' '/usr/local'
test $? -eq 0 || exit 14
buildah commit $CTN_PACK_TOOLC $PICK_TOOLC_IMGFULLNAME
test $? -eq 0 || exit 15

# save tar
podman save --output $PICK_TOOLC_ARTIFACTNAME.tar --format oci-archive $PICK_TOOLC_IMGFULLNAME
test $? -eq 0 || exit 16

# save json
podman inspect $PICK_TOOLC_IMGFULLNAME >$PICK_TOOLC_ARTIFACTNAME.json
test $? -eq 0 || exit 17

# save checksums
sha512sum $PICK_TOOLC_ARTIFACTNAME.tar >> CHECKSUMS
test $? -eq 0 || exit 18
sha512sum $PICK_TOOLC_ARTIFACTNAME.json >> CHECKSUMS
test $? -eq 0 || exit 19
