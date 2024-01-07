PROXY_FRONTEND=

if [[ ! -z $USE_PROXYCHAINS ]]
then
    PROXY_FRONTEND=proxychains
fi

if [[ ! $(pwd) =~ tg-focus$ ]]
then
    echo '[ERROR] current directory is NOT tg-focus source root'
    exit 10
fi

if [[ -f /etc/os-release ]]
then
    source /etc/os-release
fi

os_id= # e.g. deb12 deb11 fc38

######################################################################
###  Check distros
######################################################################

if [[ $VERSION == "12 (bookworm)" ]]
then
    os_id=deb12
elif [[ $VERSION == "11 (bullseye)" ]]
then
    os_id=deb11
elif [[ $VERSION == "10 (buster)" ]]
then
    os_id=deb10
elif [[ $VERSION == "alpine" ]]
then
    os_id=alp318
elif [[ $VERSION == "Fedora" ]]
then
    os_id=fc38
elif [[ $VERSION == "CYGWIN" ]]
then
    os_id=cygwin
else
    echo '[ERROR] this platform is not supported'
    exit 1
fi

echo "OS ID: $os_id"

######################################################################
###  Install Packages
######################################################################

echo "[INFO] Installing essential packages, administractive privileges required."
# Install packages
if [[ $os_id == deb10 || $os_id == deb11 || $os_id == deb12 ]]
then
    su -c 'apt install build-essential cmake gperf zlib1g-dev libssl-dev -y --quiet'
elif [[ $os_id == fc38 ]]
then    
    # tdlib
    sudo dnf install \
	 -y --quiet \
	 gperf zlib-devel openssl-devel cmake
    
    test $? -eq 0 || exit 1    

    # others
    sudo dnf install \
	 -y --quiet \
	 libstdc++-static
    test $? -eq 0 || exit 1    
elif [[ $os_id -eq 3 ]]
then
    # tdlib
    apk add \
	--quiet \
	cmake gperf g++ openssl-dev \
	git make linux-headers zlib-dev
    test $? -eq 0 || exit 1
    
    # others
    # apk add --quiet fmt-dev
    # test $? -eq 0 || exit 1
fi

test $? -eq 0 || exit 2

function dl-3rd-deps
{
    mkdir -p 3rd

    # fmt
    if [[ ! -d 3rd/fmt ]]
    then
	echo "[INFO] Fetching 'fmt'..."
	$PROXY_FRONTEND git clone https://github.com/fmtlib/fmt 3rd/fmt
	test $? -eq 0 || return 2
	echo "[INFO] Fetching 'fmt'...ok"
    fi
    git -C 3rd/fmt checkout --quiet $pick_src_fmt # 10.1.1
    test $? -eq 0 || return 2
    echo "[INFO] 'fmt' source picked"

    # toml11
    if [[ ! -d 3rd/toml11 ]]
    then
	echo "[INFO] Fetching 'toml11'..."
	$PROXY_FRONTEND git clone https://github.com/ToruNiina/toml11 3rd/toml11
	test $? -eq 0 || return 2
	echo "[INFO] Fetching 'toml11'...ok"
    fi
    git -C 3rd/toml11 checkout --quiet $pick_src_toml11 # v3.7.1
    test $? -eq 0 || return 2
    echo "[INFO] 'toml11' source picked"
    
    if [[ ! -d 3rd/td ]]
    then
	# tdlib
	echo "[INFO] Fetching 'tdlib'..."
	$PROXY_FRONTEND git clone https://github.com/tdlib/td 3rd/td
	test $? -eq 0 || return 2
	echo "[INFO] Fetching 'tdlib'...ok"
    fi
    git -C 3rd/td checkout --quiet $pick_src_tdlib
    test $? -eq 0 || return 3
    echo "[INFO] 'tdlib' source picked"

    return 0
}



######################################################################
###  Build
######################################################################

function build-tdlib
{
    ls /usr/local/lib | grep "libtdjson.so.$(cat dev/pick-ver-tdlib)"
    if [[ $? -eq 0 ]]
    then
	echo '[INFO] TDLib built, skip'
	return 0
    fi

    if [[ $os_id == deb10 ]]
    then
	(cd 3rd/td && cmake -DCMAKE_BUILD_TYPE=Debug -DZLIB_LIBRARY=/usr/lib/x86_64-linux-gnu/libz.a -DOPENSSL_USE_STATIC_LIBS=TRUE -DCMAKE_C_COMPILER=alt-gcc -DCMAKE_CXX_COMPILER=alt-g++ -B build)
	test $? -eq 0 || exit 3
	export LD_LIBRARY_PATH=/usr/local/lib64
	(cd 3rd/td/build && make -j$(nproc) && su -c 'LD_LIBRARY_PATH=/usr/local/lib64 make install')
	test $? -eq 0 || exit 3
	(cd 3rd/td/build && make test)
	test $? -eq 0 || exit 3
    else
	(cd 3rd/td && cmake -DCMAKE_BUILD_TYPE=Debug -B build)
	test $? -eq 0 || exit 3    
	(cd 3rd/td/build && make -j$(nproc) && make install)
	test $? -eq 0 || exit 3
	(cd 3rd/td/build && make test)
	test $? -eq 0 || exit 3
    fi
    
    return 0
}

function build-tdlib-win
{
    echo 'WIP'
}



######################################################################
### Main
######################################################################

pick_src_fmt=$(cat dev/pick-src-fmt)
pick_src_toml11=$(cat dev/pick-src-toml11)
pick_src_tdlib=$(cat dev/pick-src-tdlib)
dl-3rd-deps

if [[ $os_id == cygwin ]]
then
    echo build-tdlib-win
else
     build-tdlib
fi

echo "[INFO] Done, all dependencies installed!"
