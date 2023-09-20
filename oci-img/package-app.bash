buildah rm package-tgfocus-container
buildah from --name package-tgfocus-container debian:bookworm-slim

buildah copy --from build-tgfocus-container package-tgfocus-container \
	'/tg-focus/build/tf-conf' '/usr/local/bin'
buildah copy --from build-tgfocus-container package-tgfocus-container \
	'/tg-focus/build/tf-focusd' '/usr/local/bin'

# install dependencies
buildah run package-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true --quiet update
buildah run package-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true install nano --quiet -y
	# apt-get -o Acquire::ForceIPv4=true install libstdc++6 libssl3 zlib1g nano --quiet -y
buildah run package-tgfocus-container -- \
	apt-get -o Acquire::ForceIPv4=true install locales -y

# generate locales, and setup env
buildah run package-tgfocus-container -- \
	sed -i 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen
buildah run package-tgfocus-container -- \
        locale-gen
buildah config --env LANG="en_US.UTF-8" package-tgfocus-container

buildah config --cmd "/bin/bash -c tf-focusd" package-tgfocus-container

buildah commit package-tgfocus-container tg-focus

