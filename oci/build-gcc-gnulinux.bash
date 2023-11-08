### 

CTN_BUILD="build-gcc-gnulinux-container"
CTN_PACK="package-gcc-gnulinux-container"
PICK_PLATFORM="x86_64-linux-gnu"
PICK_BASEIMG="debian:buster-slim"
PXY_FRONTEND=""
APT_COUNTRY_CODE="us"

if [[ -v USE_PROXYCHAINS ]]
then
    PXY_FRONTEND="proxychains"
    echo "[INFO] using proxy frontend '$PXY_FRONTEND'"
fi

if [[ -v CURR_LOC ]]
then
    APT_COUNTRY_CODE=$CURR_LOC
    echo "[INFO] using country code '$APT_COUNTRY_CODE'"
fi


###

# skip building if already built
flag=$(buildah ps | grep $CTN_BUILD | wc -l)
test $flag -eq 0 || exit 0

buildah from --name $CTN_BUILD $PICK_BASEIMG

buildah run $CTN_BUILD -- \
	sed -i \
	"s/deb\.debian\.org/ftp\.$APT_COUNTRY_CODE\.debian\.org/" \
	/etc/apt/sources.list

buildah run $CTN_BUILD -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_BUILD -- \
	apt-get -o Acquire::ForceIPv4=true install g++ make wget curl bzip2 autogen dejagnu -y --quiet

test $? -eq 0 || exit 2

$PXY_FRONTEND buildah run $CTN_BUILD -- \
	bash -c "wget https://ftp.gnu.org/gnu/gcc/gcc-12.3.0/gcc-12.3.0.tar.gz"

test $? -eq 0 || exit 3

buildah run $CTN_BUILD -- \
	bash -c "tar xf gcc-12.3.0.tar.gz"

test $? -eq 0 || exit 4

buildah run $CTN_BUILD -- \
	bash -c "cd gcc-12.3.0 && ./contrib/download_prerequisites"

test $? -eq 0 || exit 4

buildah run $CTN_BUILD -- \
	bash -c "mkdir objdir && cd objdir && ../gcc-12.3.0/configure --prefix=/mygcc12 --program-prefix=my --disable-multilib --disable-libsanitizer --enable-languages=c,c++"

test $? -eq 0 || exit 4

# build 100min+ on 4C
buildah run $CTN_BUILD -- \
	bash -c 'cd objdir && make -j$(nproc)'

test $? -eq 0 || exit 4

# test 200min+ on 4C
# buildah run $CTN_BUILD -- \
#	bash -c 'cd objdir && make -j$(nproc) -k check'

test $? -eq 0 || exit 4

buildah run $CTN_BUILD -- \
	bash -c 'cd objdir && make install'

test $? -eq 0 || exit 4


# package

buildah from --name $CTN_PACK $PICK_BASEIMG
buildah copy --from $CTN_BUILD $CTN_PACK '/mygcc12' '/mygcc12'

TAR_NAME="gcc-$PICK_PLATFORM"

buildah commit $CTN_PACK $TAR_NAME

podman save --output $TAR_NAME.tar --format oci-archive localhost/$TAR_NAME:latest

podman inspect localhost/$TAR_NAME:latest >$TAR_NAME.json
