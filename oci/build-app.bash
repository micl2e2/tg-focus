CTN_TDLIB="build-tdlib-container"
CTN_TGFOCUS="build-tgfocus-container"

flag=$(buildah ps | grep $CTN_TGFOCUS | wc -l)
test $flag -eq 0 || buildah rm $CTN_TGFOCUS

buildah from --name $CTN_TGFOCUS debian:bookworm-slim

buildah copy --from $CTN_TDLIB $CTN_TGFOCUS '/usr/local/include' '/usr/local/include'
buildah copy --from $CTN_TDLIB $CTN_TGFOCUS '/usr/local/lib' '/usr/local/lib'

buildah run build-tgfocus-container -- \
	sed -i 's/deb\.debian\.org/ftp\.us\.debian\.org/' /etc/apt/sources.list.d/debian.sources

buildah run $CTN_TGFOCUS -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_TGFOCUS -- \
	apt-get -o Acquire::ForceIPv4=true install git g++ libssl-dev zlib1g-dev cmake -y

test $? -eq 0 || exit 1

test -z $HTTPS_PROXY || echo use https proxy $HTTPS_PROXY

buildah run $CTN_TGFOCUS -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"

test $? -eq 0 || exit 2

buildah run $CTN_TGFOCUS -- \
	bash -c "https_proxy=$HTTPS_PROXY cd tg-focus && bash dl-3rd.bash"

test $? -eq 0 || exit 3

buildah run $CTN_TGFOCUS -- \
	bash -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -B build && cmake --build build"

test $? -eq 0 || exit 4

buildah run $CTN_TGFOCUS -- \
	bash -c "cd tg-focus && strip /tg-focus/build/tf-conf && strip /tg-focus/build/tf-focusd"



