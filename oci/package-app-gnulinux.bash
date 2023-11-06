###

CTN_TGFOCUS="build-tgfocus-gnulinux-container"
CTN_PACK="package-tgfocus-gnulinux-container"
PICK_PLATFORM="gnulinux"
FINAL_IMGNAME="tg-focus-$PICK_PLATFORM"
PICK_BASEIMG="debian:bookworm-slim"
APT_COUNTRY_CODE="us"

if [[ -v CURR_LOC ]]
then
    APT_COUNTRY_CODE=$CURR_LOC
    echo "[INFO] using country code '$APT_COUNTRY_CODE'"
fi

###

flag=$(buildah ps | grep $CTN_PACK | wc -l)
test $flag -eq 0 || buildah rm $CTN_PACK

buildah from --name $CTN_PACK $PICK_BASEIMG

test $? -eq 0 || exit 1

buildah copy --from $CTN_TGFOCUS $CTN_PACK \
	'/tg-focus/build/tf-conf' '/usr/local/bin'
buildah copy --from $CTN_TGFOCUS $CTN_PACK \
	'/tg-focus/build/tf-focusd' '/usr/local/bin'

test $? -eq 0 || exit 2


buildah run $CTN_PACK -- \
	sed -i \
	"s/deb\.debian\.org/ftp\.$APT_COUNTRY_CODE\.debian\.org/" \
	/etc/apt/sources.list.d/debian.sources
buildah run $CTN_PACK -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_PACK -- \
	apt-get -o Acquire::ForceIPv4=true \
	install \
	locales nano \
	-y --quiet

test $? -eq 0 || exit 3

buildah run $CTN_PACK -- \
        bash -c 'sed -i "s/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/" /etc/locale.gen'

test $? -eq 0 || exit 4

buildah run $CTN_PACK -- \
        locale-gen

test $? -eq 0 || exit 5

buildah config --cmd "/bin/sh -c tf-focusd" $CTN_PACK

buildah commit $CTN_PACK $FINAL_IMGNAME

test $? -eq 0 || exit 6

podman images | grep $FINAL_IMGNAME

test $? -eq 0 || exit 7

curr_date=$(date +%Y%m%d)

TAR_NAME="${FINAL_IMGNAME}-ociapp-$curr_date"

# skip for gnulinux 
#podman save --output $TAR_NAME.tar --format oci-archive localhost/$FINAL_IMGNAME:latest

test $? -eq 0 || exit 8

# skip for gnulinux 
#podman inspect localhost/$FINAL_IMGNAME:latest >$TAR_NAME.json

test $? -eq 0 || exit 9

tmpctnid=$(podman create localhost/$FINAL_IMGNAME:latest)
podman cp $tmpctnid:/usr/local/bin "${FINAL_IMGNAME}-bin"
podman rm $tmpctnid
