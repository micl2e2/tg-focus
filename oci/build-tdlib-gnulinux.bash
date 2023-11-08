### 

CTN_TDLIB="build-tdlib-gnulinux-container"
CTN_PACK="package-tdlib-gnulinux-container"
PICK_SRC="030e440757478ef5f5fdb01f60c3c6d885330c72"
PICK_PLATFORM="gnulinux"
PICK_BASEIMG="quay.io/centos/centos:stream9"
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
flag=$(buildah ps | grep $CTN_TDLIB | wc -l)
test $flag -eq 0 || exit 0

buildah from --name $CTN_TDLIB $PICK_BASEIMG

buildah run $CTN_TDLIB -- \
	sed -i \
	"s/deb\.debian\.org/ftp\.$APT_COUNTRY_CODE\.debian\.org/" \
	/etc/apt/sources.list

buildah run $CTN_TDLIB -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_TDLIB -- \
	apt-get -o Acquire::ForceIPv4=true install gperf cmake g++ git zlib1g-dev libssl-dev wget -y --quiet

test $? -eq 0 || exit 2

$PXY_FRONTEND buildah run $CTN_TDLIB -- \
	bash -c "git clone https://github.com/tdlib/td"

test $? -eq 0 || exit 3

buildah run $CTN_TDLIB -- \
	bash -c "cd td && git reset --hard $PICK_SRC"

test $? -eq 0 || exit 4

# FIXEM: handle rpath
buildah run $CTN_TDLIB -- \
	bash -c "cd td && mkdir -p build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DZLIB_USE_STATIC_LIBS=ON -DOPENSSL_USE_STATIC_LIBS=TRUE .. && make -j$(nproc) && make install"

# package

buildah from --name $CTN_PACK $PICK_BASEIMG
buildah copy --from $CTN_TDLIB $CTN_PACK '/usr/local/include' '/usr/local/include'
buildah copy --from $CTN_TDLIB $CTN_PACK '/usr/local/lib' '/usr/local/lib'

TAR_NAME="tdlib-$PICK_SRC-$PICK_PLATFORM"

buildah commit $CTN_PACK $TAR_NAME

podman save --output $TAR_NAME.tar --format oci-archive localhost/$TAR_NAME:latest

podman inspect localhost/$TAR_NAME:latest >$TAR_NAME.json
