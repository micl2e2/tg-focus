#!/bin/sh

###
### ATTENTION:
###   This script is not for developers, but CI runners.
###

function dl_3rd_deps
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
}

pick_src_fmt=$(cat dev/pick-src-fmt)
test $? -eq 0 || exit 1
pick_src_toml11=$(cat dev/pick-src-toml11)
test $? -eq 0 || exit 1
dl_3rd_deps
