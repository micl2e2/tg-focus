
buildah rm app-tgfocus-container2
buildah from --name app-tgfocus-container2 alpine:latest

# buildah copy --from tdlib-builder-container2 app-tgfocus-container2 \
	# '/usr/local/lib/libtdjson.so.1.8.15' '/usr/local/lib'
buildah copy --from tdlib-builder-container2 app-tgfocus-container2 \
	'/usr/local/lib' '/usr/local/lib'

# cwd is /
buildah copy --from tgfocus-builder-container2 app-tgfocus-container2 '/tg-focus/target/release/tg-focus' '/'

buildah copy app-tgfocus-container2 'tgfocus-white.jpg' '/'

buildah config --cmd "/bin/sh -c 'LD_LIBRARY_PATH=/usr/local/lib /tg-focus >/tmp/tg-focus/stdout 2>/tmp/tg-focus/stderr'" app-tgfocus-container2

buildah commit app-tgfocus-container2 tg-focus-small
