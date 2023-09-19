buildah rm tgfocus-builder-container2
buildah from --name tgfocus-builder-container2 alpine:latest

# install deps
buildah copy --from tdlib-builder-container2 tgfocus-builder-container2 \
	'/usr/local/lib/' '/usr/local/lib' # needs all
buildah run tgfocus-builder-container2 -- \
	apk update
buildah run tgfocus-builder-container2 -- \
	apk add --no-cache wget pkgconfig gcc git g++

# install rust 
buildah run tgfocus-builder-container2 -- \
	wget --quiet https://sh.rustup.rs -O install.sh
buildah run tgfocus-builder-container2 -- \
	sh install.sh --profile minimal -y
# remember source /root/.cargo/env


# build tg-focus
if [[ ! -z $HTTPS_PROXY ]]
then
    echo Use proxy $HTTPS_PROXY!
fi
buildah run tgfocus-builder-container2 -- \
	sh -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tdlib"
buildah run tgfocus-builder-container2 -- \
	sh -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"
buildah run tgfocus-builder-container2 -- \
	sh -c "source /root/.cargo/env && cd tg-focus && LD_LIBRARY_PATH=/usr/local/lib API_ID=$API_ID API_HASH=$API_HASH RUSTFLAGS='-C target-feature=-crt-static' cargo build --release"
