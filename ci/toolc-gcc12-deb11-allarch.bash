
sudo apt-get install -y --quiet buildah podman 

# skip building if already built
buildah ps | grep $CTN_PACK_TOOLC
[[ $? -eq 0 ]] && exit 1

buildah from --name $CTN_BUILD_TOOLC $PICK_BASEIMG
[[ $? -eq 0 ]] || exit 2

USE_APT_MIRROR="deb.debian.org"
[[ -n $PICK_APT_MIRROR ]] && USE_APT_MIRROR=$PICK_APT_MIRROR
buildah run $CTN_BUILD_TOOLC -- \
	sed -i "s/deb.debian.org/${USE_APT_MIRROR}/" /etc/apt/sources.list
[[ $? -eq 0 ]] || exit 3

buildah run $CTN_BUILD_TOOLC -- \
	apt-get -o Acquire::ForceIPv4=true update
[[ $? -eq 0 ]] || exit 4

buildah run $CTN_BUILD_TOOLC -- \
	apt-get -o Acquire::ForceIPv4=true install g++ make wget curl bzip2 autogen dejagnu -y --quiet
[[ $? -eq 0 ]] || exit 5

USE_GNU_MIRROR="ftp.gnu.org"
[[ -n $PICK_GNU_MIRROR ]] && USE_GNU_MIRROR=$PICK_GNU_MIRROR
$PXY_FRONTEND buildah run $CTN_BUILD_TOOLC -- \
	bash -c "wget http://${USE_GNU_MIRROR}/gnu/gcc/gcc-${PICK_GCC_REL}/gcc-${PICK_GCC_REL}.tar.gz"
[[ $? -eq 0 ]] || exit 6

buildah run $CTN_BUILD_TOOLC -- \
	bash -c "tar xf gcc-${PICK_GCC_REL}.tar.gz"
[[ $? -eq 0 ]] || exit 7

$PXY_FRONTEND buildah run $CTN_BUILD_TOOLC -- \
	bash -c "cd gcc-${PICK_GCC_REL} && ./contrib/download_prerequisites"
[[ $? -eq 0 ]] || exit 8

buildah run $CTN_BUILD_TOOLC -- \
	bash -c "mkdir objdir && cd objdir && ../gcc-${PICK_GCC_REL}/configure --program-prefix='alt-' --disable-multilib --disable-libsanitizer --enable-languages=c,c++ --enable-shared=libgcc,libstdc++"
[[ $? -eq 0 ]] || exit 9

# build 100min+ on 4C, 1h20m on GH
buildah run $CTN_BUILD_TOOLC -- \
	bash -c 'cd objdir && make -j$(nproc)'
[[ $? -eq 0 ]] || exit 10

# test 6h+ on GH
# buildah run $CTN_BUILD_TOOLC -- \
	# bash -c 'cd objdir && make -j1 -k check-c++'

[[ $? -eq 0 ]] || exit 11

buildah run $CTN_BUILD_TOOLC -- \
	bash -c 'cd objdir && make install'
[[ $? -eq 0 ]] || exit 12

# package

buildah from --name $CTN_PACK_TOOLC $PICK_BASEIMG
[[ $? -eq 0 ]] || exit 13
buildah copy --from $CTN_BUILD_TOOLC $CTN_PACK_TOOLC '/usr/local' '/usr/local'
[[ $? -eq 0 ]] || exit 14

alt_ver_str=$(buildah run $CTN_BUILD_TOOLC -- bash -c "alt-gcc --version | head -1")
[[ $? -eq 0 ]] || exit 141
[[ $alt_ver_str =~ $PICK_GCC_REL ]] || exit 141

buildah commit $CTN_PACK_TOOLC $PICK_TOOLC_IMGFULLNAME
[[ $? -eq 0 ]] || exit 15

# save tar
podman save --output $PICK_TOOLC_ARTIFACTNAME.tar --format oci-archive $PICK_TOOLC_IMGFULLNAME
[[ $? -eq 0 ]] || exit 16

# save json
podman inspect $PICK_TOOLC_IMGFULLNAME >$PICK_TOOLC_ARTIFACTNAME.json
[[ $? -eq 0 ]] || exit 17

# save checksums
sha512sum $PICK_TOOLC_ARTIFACTNAME.tar >> CHECKSUMS
[[ $? -eq 0 ]] || exit 18
sha512sum $PICK_TOOLC_ARTIFACTNAME.json >> CHECKSUMS
[[ $? -eq 0 ]] || exit 19
