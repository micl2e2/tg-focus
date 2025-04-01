# ------------------------------ deps ------------------------------

[[ -n $PICK_ARCH ]] || exit 201

PICK_TGF_VER=$(cat ../dev/pick-ver-tgfocus)
PICK_PLATFORM="linux-$PICK_ARCH" # accurate one is "linux-gnu-$PICK_ARCH"
PACK_DIRNAME="tg-focus-${PICK_TGF_VER}-${PICK_PLATFORM}"
CURR_DATE=$(date +'%Y%m%d')

# --------------------------- toolchain ---------------------------

PICK_GCC_REL="$(cat ../dev/pick-ver-gcc)"

# ----------------------------- tdlib -----------------------------

PICK_TDLIB_SRC="$(cat ../dev/pick-src-tdlib)"

# ----------------------------- toml -----------------------------

PICK_LIBTOML_SRC="$(cat ../dev/pick-src-toml11)"

# ----------------------------- others -----------------------------

PXY_FRONTEND=""

if [[ -v USE_PROXYCHAINS ]]
then
    PXY_FRONTEND="proxychains"
    echo "[INFO] using proxy frontend '$PXY_FRONTEND'"
fi
