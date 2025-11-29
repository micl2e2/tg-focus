# set -x

# SYS

IS_FEDORA=$(grep 'Fedora Linux' /etc/os-release --quiet && printf true || printf false)
IS_CENTOS=$(grep 'CentOS Stream' /etc/os-release --quiet && printf true || printf false)
IS_DEBIAN=$(grep 'Debian GNU/Linux' /etc/os-release --quiet && printf true || printf false)

if [[ $IS_CENTOS = true ]]
then
    sudo dnf config-manager --set-enabled crb && \
	sudo dnf install -y --quiet epel-release epel-next-release && \
	sudo dnf install -y --quiet gperf zlib-devel g++ libstdc++-static cmake
fi

if [[ $IS_FEDORA = true ]]
then
    sudo dnf install -y --quiet gperf zlib-devel g++ cmake openssl-devel
fi

if [[ $IS_DEBIAN = true ]]
then
    sudo apt update && \
	sudo apt install -y --quiet zlib1g-dev gperf cmake g++ libssl-dev
fi

# GIT TDLIB

if [[ ! -d tdlib/.git ]]
then
    git clone --depth=1000 https://github.com/tdlib/td tdlib || exit 255
    git --git-dir=tdlib/.git --work-tree=tdlib reset --hard $(cat ../dev/pick-src-tdlib) || exit 255
fi

# GIT TOML11

if [[ ! -d toml11/.git ]]
then
    git clone --depth=1000 https://github.com/ToruNiina/toml11 toml11
    git --git-dir=toml11/.git --work-tree=toml11 reset --hard $(cat ../dev/pick-src-toml11)
fi

# TARBALL OPENSSL

if [[ $IS_CENTOS = true ]]
then
    if [[ ! -f openssl/build/lib64/libssl.a || ! -f openssl/build/lib64/libcrypto.a ]]
    then
	OPENSSL_URL_PFIX="https://github.com/openssl/openssl/releases/download"
	OPENSSL_VER=$(cat ../dev/pick-ver-openssl)

	if [[ ! -d openssl ]]
	then
	    wget ${OPENSSL_URL_PFIX}/openssl-${OPENSSL_VER}/openssl-${OPENSSL_VER}.tar.gz -O openssl.tgz && \
		mkdir -p openssl && tar --extract -C openssl --strip-components=1 --file openssl.tgz || \
		    { rm -f openssl.tgz && exit 1; }
	fi

	cd openssl && ./Configure --prefix=$(pwd)/build && make install
    fi    
fi
