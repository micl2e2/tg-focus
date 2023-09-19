buildah rm package-tgfocus-container
buildah from --name package-tgfocus-container debian:bookworm-slim

# install dependencies
buildah copy --from build-tgfocus-container package-tgfocus-container \
	'/tg-focus/build/tf-conf' '/usr/local/bin'
buildah copy --from build-tgfocus-container package-tgfocus-container \
	'/tg-focus/build/tf-focusd' '/usr/local/bin'

buildah run package-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run package-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true install libstdc++6 libssl3 zlib1g nano -y

buildah config --cmd "/bin/bash -c tf-focusd" package-tgfocus-container

buildah commit package-tgfocus-container tg-focus

