
buildah rm tgfocus-builder-container
buildah from --name tgfocus-builder-container debian:bullseye-slim

# install dependencies
buildah copy --from tdlib-builder-container tgfocus-builder-container '/usr/local/include' '/usr/local/include'
buildah copy --from tdlib-builder-container tgfocus-builder-container '/usr/local/lib' '/usr/local/lib'
# buildah run tgfocus-builder-container -- \
	# apt-get --quiet update
# buildah run tgfocus-builder-container -- \
	# apt-get --quiet install git g++ cmake -y

# build tg-focus
if [[ ! -z $HTTPS_PROXY ]]
then
    echo Use proxy $HTTPS_PROXY!
fi
# buildah run tgfocus-builder-container -- \
# 	bash -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"
# buildah run tgfocus-builder-container -- \
# 	bash -c "source /root/.cargo/env && cd tg-focus && LD_LIBRARY_PATH=/usr/local/lib API_ID=$API_ID API_HASH=$API_HASH cargo build --release"



