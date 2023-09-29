CTN_TGFOCUS="build-tgfocus-container"
CTN_PACK="package-tgfocus-container"

flag=$(buildah ps | grep $CTN_PACK | wc -l)
test $flag -eq 0 || buildah rm $CTN_PACK

buildah from --name $CTN_PACK alpine:3.18

buildah copy --from $CTN_TGFOCUS $CTN_PACK \
	'/tg-focus/build/tf-conf' '/usr/local/bin'
buildah copy --from $CTN_TGFOCUS $CTN_PACK \
	'/tg-focus/build/tf-focusd' '/usr/local/bin'

buildah run $CTN_PACK -- sh -c "apk add --no-cache nano"

buildah config --cmd "/bin/sh -c tf-focusd" $CTN_PACK

buildah commit $CTN_PACK tg-focus

test $? -eq 0 || exit 1

podman images | grep tg-focus

curr_date=$(date +%Y%m%d)

TAR_NAME="tg-focus-ociapp-$curr_date"

podman save --output $TAR_NAME.tar --format oci-archive localhost/tg-focus:latest

podman inspect localhost/tg-focus:latest >$TAR_NAME.json

