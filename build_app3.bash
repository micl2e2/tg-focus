
buildah rm app-tgfocus-container3
buildah from --name app-tgfocus-container3 debian:bookworm-slim

# cwd is /
buildah copy --from tgfocus-builder-container3 app-tgfocus-container3 '/tg-focus/target/release/tg-focus' '/'

buildah config --cmd "/bin/sh -c '/tg-focus >/tmp/tg-focus/stdout 2>/tmp/tg-focus/stderr'" app-tgfocus-container3

buildah commit app-tgfocus-container3 tg-focus-gramme
