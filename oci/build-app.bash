CTN_TDLIB="package-tdlib-container"
CTN_TGFOCUS="build-tgfocus-container"

flag=$(buildah ps | grep $CTN_TGFOCUS | wc -l)
test $flag -eq 0 || buildah rm $CTN_TGFOCUS

buildah from --name $CTN_TGFOCUS alpine:3.18

buildah copy --from $CTN_TDLIB $CTN_TGFOCUS '/usr/local/include' '/usr/local/include'
buildah copy --from $CTN_TDLIB $CTN_TGFOCUS '/usr/local/lib' '/usr/local/lib'

test $? -eq 0 || exit 1

buildah run $CTN_TGFOCUS -- \
	sed -i 's/https/http/' /etc/apk/repositories
buildah run $CTN_TGFOCUS -- \
        sh -c "https_proxy=$HTTPS_PROXY apk update"
buildah run $CTN_TGFOCUS -- \
        sh -c "https_proxy=$HTTPS_PROXY apk add pkgconfig gcc g++ git cmake make"

test $? -eq 0 || exit 1

# better use *-dev
buildah run $CTN_TGFOCUS -- \
        sh -c "ln -s /usr/lib/libssl.so.3 /usr/lib/libssl.so"
buildah run $CTN_TGFOCUS -- \
        sh -c "ln -s /usr/lib/libcrypto.so.3 /usr/lib/libcrypto.so"
buildah run $CTN_TGFOCUS -- \
        sh -c "ln -s /lib/libz.so.1 /lib/libz.so"

test -z $HTTPS_PROXY || echo use https proxy $HTTPS_PROXY

buildah run $CTN_TGFOCUS -- \
	sh -c "https_proxy=$HTTPS_PROXY git clone --depth=1 https://github.com/micl2e2/tg-focus"

test $? -eq 0 || exit 2

buildah run $CTN_TGFOCUS -- \
	sh -c "https_proxy=$HTTPS_PROXY cd tg-focus && sh dl-3rd.bash"

test $? -eq 0 || exit 3

buildah run $CTN_TGFOCUS -- \
	sh -c "cd tg-focus && cmake -DCMAKE_BUILD_TYPE=Release -B build && cmake --build build"

test $? -eq 0 || exit 4

buildah run $CTN_TGFOCUS -- \
	sh -c "cd tg-focus/build && ctest"

test $? -eq 0 || exit 5

buildah run $CTN_TGFOCUS -- \
	sh -c "cd tg-focus && strip /tg-focus/build/tf-conf && strip /tg-focus/build/tf-focusd"

test $? -eq 0 || exit 6



