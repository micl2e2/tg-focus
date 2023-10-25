if [[ ! $(pwd) =~ tg-focus$ ]]
then
    echo '[ERROR] current directory is NOT tg-focus source root'
    exit 10
fi


distro_id=$(cat /etc/os-release)

os_id=0 # 1:debian 2:fedora

# Check distros
if [[ $distro_id =~ "debian" ]]
then
    os_id=1
elif [[ $distro_id =~ "fedora" ]]
then
    os_id=2
elif [[ $distro_id =~ "alpine" ]]
then
    os_id=3
else
    echo '[ERROR] this platform is not supported'
    exit 1
fi

# Install packages
if [[ $os_id -eq 1 ]]
then
    su -c 'apt install gperf zlib-dev libssl-dev -y --quiet'
    # TODO
elif [[ $os_id -eq 2 ]]
then
    
    echo '[INFO] Installing dependencies for TDLib...'
    # tdlib
    sudo dnf install \
	 -y --quiet \
	 gperf zlib-devel openssl-devel cmake
    
    test $? -eq 0 || exit 1    
    echo '[INFO] Installing dependencies for TDLib...ok'

    echo '[INFO] Installing other dependencies...'
    # others
    sudo dnf install \
	 -y --quiet \
	 libstdc++-static
    test $? -eq 0 || exit 1    
    echo '[INFO] Installing other dependencies...ok'
    
elif [[ $os_id -eq 3 ]]
then
    echo '[INFO] Installing dependencies for TDLib...'
    # tdlib
    https_proxy=$HTTPS_PROXY apk add \
			     --quiet \
			     cmake gperf g++ openssl-dev \
			     git make linux-headers zlib-dev
    test $? -eq 0 || exit 1
    
    echo '[INFO] Installing other dependencies...'
    # others
    # apk add --quiet fmt-dev
    test $? -eq 0 || exit 1

    # fetch tdlib
fi

test $? -eq 0 || exit 2

function dl-3rd-deps
{
    if [[ -d 3rd ]]
    then
	echo '[INFO] third-party dependencies exist'
	return 0
    fi
    
    mkdir -p 3rd

    # !!!
    cd 3rd

    # fmt
    echo "[INFO] Fetching 'fmt'..."
    https_proxy=$HTTPS_PROXY git clone --quiet https://github.com/fmtlib/fmt fmt
    test $? -eq 0 || exit 2
    echo "[INFO] Fetching 'fmt'...ok"

    # !!!
    cd fmt && \
	git checkout --quiet $pick_src_fmt # 10.1.1
    test $? -eq 0 || exit 2
    echo "[INFO] 'fmt' source picked"
    cd .. #  back to 3rd

    # toml11
    echo "[INFO] Fetching 'toml11'..."
    https_proxy=$HTTPS_PROXY git clone --quiet https://github.com/ToruNiina/toml11 toml11
    test $? -eq 0 || exit 2
    echo "[INFO] Fetching 'toml11'...ok"

    # !!!
    cd toml11 && \
	git checkout --quiet $pick_src_toml11 # v3.7.1
    test $? -eq 0 || exit 2
    echo "[INFO] 'toml11' source picked"
    cd .. #  back to 3rd

    # tdlib
    echo "[INFO] Fetching 'tdlib'..."
    https_proxy=$HTTPS_PROXY git clone --quiet https://github.com/tdlib/td tdlib
    test $? -eq 0 || exit 2
    echo "[INFO] Fetching 'tdlib'...ok"

    # !!!
    cd tdlib && \
	git checkout --quiet $pick_src_tdlib
    test $? -eq 0 || exit 3
    echo "[INFO] 'tdlib' source picked"
    cd .. #  back to 3rd

    cd .. #  back to dev

    return 0
}


function build-tdlib
{
    if [[ -e /usr/local/lib/libtdclient.a ]]
    then
	echo '[INFO] TDLib built and installed, skip'
	return 0
    fi

    # !!!
    cd 3rd/tdlib
    
    if [[ ! -e build/libtdclient.a ]]
    then
	echo '[INFO] Building TDLib...'
	cmake -DCMAKE_BUILD_TYPE=Debug -B build
	test $? -eq 0 || exit 3
	cmake --build build
	test $? -eq 0 || exit 3
	echo '[INFO] Building TDLib...ok'
    fi

    echo '[INFO] Installing TDLib...'
    if [[ $os_id -eq 3 ]]
    then
	cmake --install build
    else
	sudo cmake --install build
    fi
    test $? -eq 0 || exit 3
    echo '[INFO] Installing TDLib...ok'

    cd ../..
    
    return 0
}

pick_src_fmt=$(cat dev/pick-src-fmt)
pick_src_toml11=$(cat dev/pick-src-toml11)
pick_src_tdlib=$(cat dev/pick-src-tdlib)
dl-3rd-deps

build-tdlib
if [[ ! $(pwd) =~ tg-focus$ ]]
then
    echo '[ERROR] current directory is NOT tg-focus source root'
    exit 10
fi

echo "[INFO] Done, all dependencies installed!"
