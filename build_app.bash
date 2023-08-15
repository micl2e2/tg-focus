
buildah from --name app-tgfocus-container debian:bullseye-slim

buildah copy --from tdlib-builder-container app-tgfocus-container '/usr/local/lib' '/usr/local/lib'

# cwd is /
buildah copy --from tgfocus-builder-container app-tgfocus-container '/tg-focus/target/release/tg-focus' '/'

# buildah run app-tgfocus-container -- bash -c 'mkdir /tmp/tg-focus'

buildah copy app-tgfocus-container 'tgfocus-white.jpg' '/'

buildah config --cmd "/bin/bash -c ' LD_LIBRARY_PATH=/usr/local/lib /tg-focus >/tmp/tg-focus/stdout 2>/tmp/tg-focus/stderr'" app-tgfocus-container

buildah commit app-tgfocus-container tg-focus
