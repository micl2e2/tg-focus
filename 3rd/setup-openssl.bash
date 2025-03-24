
[[ $(basename $(pwd)) == 3rd ]] || { exit 1; }

[[ -f /usr/lib/libssl.a ]] && exit 0
[[ -f /usr/lib64/libssl.a ]] && exit 0
[[ -f /usr/local/lib64/libssl.a ]] && exit 0

OPENSSL_URL_PFIX="https://github.com/openssl/openssl/releases/download"
OPENSSL_VER=$(cat ../dev/pick-ver-openssl)

[[ -d openssl ]] || \
    wget ${OPENSSL_URL_PFIX}/openssl-${OPENSSL_VER}/openssl-${OPENSSL_VER}.tar.gz -O openssl.tgz && \
	mkdir -p openssl && tar --extract -C openssl --strip-components=1 --file openssl.tgz || \
	    { rm -f openssl.tgz && exit 1; }

cd openssl && ./Configure && make && sudo make install
