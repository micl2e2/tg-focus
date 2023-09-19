buildah rm build-tgfocus-container
buildah from --name build-tgfocus-container debian:bullseye-slim

# install dependencies
buildah copy --from tdlib-builder-container build-tgfocus-container '/usr/local/include' '/usr/local/include'
buildah copy --from tdlib-builder-container build-tgfocus-container '/usr/local/lib' '/usr/local/lib'
buildah run build-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true update
	# apt-get --quiet update
buildah run build-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true install git g++ cmake -y
	# apt-get --quiet install git g++ cmake -y

# build tg-focus
if [[ ! -z $HTTPS_PROXY ]]
then
    echo Use proxy $HTTPS_PROXY!
fi
buildah run build-tgfocus-container -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"
buildah run build-tgfocus-container -- \
	bash -c "https_proxy=$HTTPS_PROXY cd tg-focus && bash dl-deps.bash"
buildah run build-tgfocus-container -- \
	bash -c "cd tg-focus && cmake -B build && cmake --build build"



