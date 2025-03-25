set -x

# ----------------------------- tdlib -----------------------------

sudo apk add gperf cmake g++ git zlib-dev zlib-static openssl-libs-static linux-headers wget ccache --quiet
[[ $? -eq 0 ]] || exit 255

[[ -d ../3rd/tdlib/.git ]] || $PXY_FRONTEND git clone --depth=1000 https://github.com/tdlib/td ../3rd/tdlib
[[ $? -eq 0 ]] || exit 255

git --git-dir=../3rd/tdlib/.git --work-tree=../3rd/tdlib reset --hard ${PICK_TDLIB_SRC}
[[ $? -eq 0 ]] || exit 255

# ------------------------------ toml ------------------------------

[[ -d ../3rd/toml11/.git ]] || $PXY_FRONTEND git clone --depth=1000 https://github.com/ToruNiina/toml11 ../3rd/toml11
[[ $? -eq 0 ]] || exit 255

git --git-dir=../3rd/toml11/.git --work-tree=../3rd/toml11 reset --hard ${PICK_LIBTOML_SRC}
[[ $? -eq 0 ]] || exit 255

# ---------------------------- tgfocus ----------------------------
