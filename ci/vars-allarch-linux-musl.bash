# ------------------------------ deps ------------------------------

[[ -n $PICK_ARCH ]] || exit 201

PICK_PLATFORM="$PICK_ARCH-linux-musl"
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
