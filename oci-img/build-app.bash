buildah rm build-tgfocus-container
buildah from --name build-tgfocus-container debian:bookworm-slim

# install dependencies
buildah copy --from build-tdlib-container build-tgfocus-container '/usr/local/include' '/usr/local/include'
buildah copy --from build-tdlib-container build-tgfocus-container '/usr/local/lib' '/usr/local/lib'

buildah run build-tgfocus-container -- \
	sed -i 's/deb\.debian\.org/ftp\.cn\.debian\.org/' /etc/apt/sources.list.d/debian.sources

buildah run build-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true update
	# apt-get --quiet update
buildah run build-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true install git g++ libssl-dev zlib1g-dev cmake -y
	# apt-get --quiet install git g++ cmake -y

test $? -eq 0 || exit 1

# build tg-focus
if [[ ! -z $HTTPS_PROXY ]]
then
    echo Use proxy $HTTPS_PROXY!
fi

buildah run build-tgfocus-container -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"
buildah run build-tgfocus-container -- \
	bash -c "https_proxy=$HTTPS_PROXY cd tg-focus && bash dl-deps.bash"

test $? -eq 0 || exit 2

buildah run build-tgfocus-container -- \
	bash -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -B build && cmake --build build"

# buildah run build-tgfocus-container -- \
	# bash -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_FIND_LIBRARY_SUFFIXES='.a' -B build && cmake --build build"



