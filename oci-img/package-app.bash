CTN_TGFOCUS="build-tgfocus-container"
CTN_APP="package-app-container"

flag=$(buildah ps | grep $CTN_APP | wc -l)
test $flag -eq 0 || buildah rm $CTN_APP

buildah from --name $CTN_APP debian:bookworm-slim

buildah copy --from $CTN_TGFOCUS $CTN_APP \
	'/tg-focus/build/tf-conf' '/usr/local/bin'
buildah copy --from $CTN_TGFOCUS $CTN_APP \
	'/tg-focus/build/tf-focusd' '/usr/local/bin'

buildah run $CTN_APP -- \
	apt-get -o Acquire::ForceIPv4=true update
buildah run $CTN_APP -- \
	apt-get -o Acquire::ForceIPv4=true install nano -y
buildah run $CTN_APP -- \
	apt-get -o Acquire::ForceIPv4=true install locales -y

buildah run $CTN_APP -- \
	sed -i 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen
buildah run $CTN_APP -- \
        locale-gen
buildah config --env LANG="en_US.UTF-8" $CTN_APP

buildah config --cmd "/bin/bash -c tf-focusd" $CTN_APP

buildah commit $CTN_APP tg-focus

