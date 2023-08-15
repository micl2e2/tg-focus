
buildah from --name app-tgfocus-container debian:bullseye-slim

buildah copy --from tdlib-builder-container app-tgfocus-container '/usr/local/lib' '/usr/local/lib'

buildah copy --from tgfocus-builder-container app-tgfocus-container '/tg-focus/target/release/tg-focus' '/'
