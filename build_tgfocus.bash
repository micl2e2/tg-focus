
buildah from --name tgfocus-builder-container debian:bullseye-slim

# install dependencies
buildah copy --from tdlib-builder-container tgfocus-builder-container '/usr/local/lib' '/usr/local/lib'
buildah run tgfocus-builder-container -- \
	bash -c 'sed "s/main/main contrib/" /etc/apt/sources.list -i'
buildah run tgfocus-builder-container -- \
	bash -c 'sed "s/deb\.debian\.org/ftp\.cn\.debian\.org/" /etc/apt/sources.list -i'
buildah run tgfocus-builder-container -- \
	apt-get --quiet update
buildah run tgfocus-builder-container -- \
	apt-get --quiet install pkg-config wget git g++ -y

# install rust 
buildah run tgfocus-builder-container -- \
	wget --quiet https://sh.rustup.rs -O install.sh
buildah run tgfocus-builder-container -- \
	bash install.sh --profile minimal -y
# remember source /root/.cargo/env

# build tg-focus
if [[ ! -z $HTTPS_PROXY ]]
then
    echo Use proxy $HTTPS_PROXY!
fi
buildah run tgfocus-builder-container -- \
	bash -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"
buildah run tgfocus-builder-container -- \
	bash -c "source /root/.cargo/env && cd tg-focus && LD_LIBRARY_PATH=/usr/local/lib API_ID=$API_ID API_HASH=$API_HASH cargo build --release"



