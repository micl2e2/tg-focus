buildah rm tgfocus-builder-container3
buildah from --name tgfocus-builder-container3 debian:bookworm-slim

# install deps
buildah run tgfocus-builder-container3 -- \
	bash -c 'sed "s/main/main contrib/" /etc/apt/sources.list.d/debian.sources -i'
buildah run tgfocus-builder-container3 -- \
	bash -c 'sed "s/deb\.debian\.org/ftp\.cn\.debian\.org/" /etc/apt/sources.list.d/debian.sources -i'
buildah run tgfocus-builder-container3 -- \
	apt-get --quiet update
buildah run tgfocus-builder-container3 -- \
	apt-get --quiet install wget git gcc -y

# buildah run tgfocus-builder-container3 -- \
# 	apk update
# buildah run tgfocus-builder-container3 -- \
# 	apk add wget git gcc

# install rust 
buildah run tgfocus-builder-container3 -- \
	wget --quiet https://sh.rustup.rs -O install.sh
buildah run tgfocus-builder-container3 -- \
	sh install.sh --profile minimal -y
# remember source /root/.cargo/env

# build tg-focus
if [[ ! -z $HTTPS_PROXY ]]
then
    echo Use proxy $HTTPS_PROXY!
fi
buildah run tgfocus-builder-container3 -- \
	sh -c "https_proxy=$HTTPS_PROXY git clone https://github.com/micl2e2/tg-focus"
buildah run tgfocus-builder-container3 -- \
	sh -c "cd tg-focus && git checkout try-gramme"
buildah run tgfocus-builder-container3 -- \
	bash -c "source /root/.cargo/env && cd tg-focus && RUSTFLAGS='-C target-feature=-crt-static' cargo build --release"
