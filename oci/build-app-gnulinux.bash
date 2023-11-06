###

CTN_TDLIB="package-tdlib-gnulinux-container"
CTN_TGFOCUS="build-tgfocus-gnulinux-container"
PICK_PLATFORM="gnulinux"
PICK_BASEIMG="debian:bookworm-slim"
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

flag=$(buildah ps | grep $CTN_TGFOCUS | wc -l)
test $flag -eq 0 || buildah rm $CTN_TGFOCUS

buildah from --name $CTN_TGFOCUS $PICK_BASEIMG

buildah copy --from $CTN_TDLIB $CTN_TGFOCUS '/usr/local/include' '/usr/local/include'
buildah copy --from $CTN_TDLIB $CTN_TGFOCUS '/usr/local/lib' '/usr/local/lib'

test $? -eq 0 || exit 1

buildah run $CTN_TGFOCUS -- \
	sed -i \
	"s/deb\.debian\.org/ftp\.$APT_COUNTRY_CODE\.debian\.org/" \
	/etc/apt/sources.list
buildah run $CTN_TGFOCUS -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_TGFOCUS -- \
	apt-get -o Acquire::ForceIPv4=true \
	install \
	g++ git make cmake zlib1g-dev libssl-dev locales \
	-y --quiet

test $? -eq 0 || exit 2

test -z $HTTPS_PROXY || echo use https proxy $HTTPS_PROXY

$PXY_FRONTEND buildah run $CTN_TGFOCUS -- \
        git clone --depth=1 https://github.com/micl2e2/tg-focus

test $? -eq 0 || exit 3

$PXY_FRONTEND buildah run $CTN_TGFOCUS -- \
	bash -c "cd tg-focus && bash dev/install-deps-oci-only.bash"

test $? -eq 0 || exit 4

buildah run $CTN_TGFOCUS -- \
	bash -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -B build && cmake --build build"

test $? -eq 0 || exit 5

buildah run $CTN_TGFOCUS -- \
        bash -c 'sed -i "s/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/" /etc/locale.gen'

test $? -eq 0 || exit 6

buildah run $CTN_TGFOCUS -- \
        locale-gen

test $? -eq 0 || exit 7

buildah run $CTN_TGFOCUS -- \
	bash -c "cd tg-focus/build && ctest"

test $? -eq 0 || exit 8

buildah run $CTN_TGFOCUS -- \
	bash -c "cd tg-focus && strip /tg-focus/build/tf-conf && strip /tg-focus/build/tf-focusd"

test $? -eq 0 || exit 9



