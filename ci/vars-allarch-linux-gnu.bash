# ------------------------------ deps ------------------------------

[[ -n $PICK_ARCH ]] || exit 201

PICK_PLATFORM="$PICK_ARCH-linux-gnu"
CURR_DATE=$(date +'%Y%m%d')

# --------------------------- toolchain ---------------------------

PICK_GCC_REL="$(cat ../dev/pick-ver-gcc)"
PICK_TOOLC_FULLVER="gcc12"
PICK_TOOLC_OS="deb11"
PICK_TOOLC_ARCH="$PICK_ARCH"
PICK_TOOLC_FULLTAG="$PICK_TOOLC_FULLVER-$PICK_TOOLC_OS-$PICK_TOOLC_ARCH" # no-buildstamp tag
PICK_TOOLC_IMGNAME="tg-focus-cicd-toolc" # much more formal, hence hyphen
PICK_TOOLC_IMGFULLNAME="$PICK_TOOLC_IMGNAME:$PICK_TOOLC_FULLTAG"
PICK_TOOLC_ARTIFACTNAME="$PICK_TOOLC_IMGNAME-$PICK_TOOLC_FULLTAG"
CTN_BUILD_TOOLC="build-toolc-tmpcontainer"
CTN_PACK_TOOLC="package-toolc-tmpcontainer"
## ENV: PICK_APT_MIRROR
## ENV: PICK_GNU_MIRROR

# ----------------------------- tdlib -----------------------------

PICK_TDLIB_IMGNAME="tg-focus-cicd-tdlib" # much more formal, hence hyphen
PICK_TDLIB_SRC="$(cat ../dev/pick-src-tdlib)"
PICK_TDLIB_TAG="$PICK_TDLIB_SRC-$PICK_TOOLC_FULLTAG" # tag without buildstamp
PICK_TDLIB_IMGFULLNAME="$PICK_TDLIB_IMGNAME:$PICK_TDLIB_TAG"
PICK_TDLIB_ARTIFACTNAME="$PICK_TDLIB_IMGNAME-$PICK_TDLIB_TAG"
CTN_BUILD_TDLIB="build-tdlib-tmpcontainer"
CTN_PACK_TDLIB="package-tdlib-tmpcontainer"

# ---------------------------- tgfocus ----------------------------

CTN_BUILD_TGFOCUS="build-tgfocus-tmpcontainer"
CTN_PACK_TGFOCUS="package-tgfocus-tmpcontainer"
PICK_TGFOCUS_GITTAG="$(cat ../dev/pick-ver-tgfocus)"
PICK_TGFOCUS_IMGNAME="tg-focus" # much more formal, hence hyphen
PICK_TGFOCUS_IMGTAG="$PICK_TGFOCUS_GITTAG-$PICK_PLATFORM"
PICK_TGFOCUS_IMGFULLNAME="$PICK_TGFOCUS_IMGNAME:$PICK_TGFOCUS_IMGTAG"
PICK_TGFOCUS_ARTIFACTNAME="$PICK_TGFOCUS_IMGNAME-$PICK_TGFOCUS_IMGTAG"
PICK_BASEIMG="debian:bullseye-slim"

# ----------------------------- others -----------------------------

PXY_FRONTEND=""
APT_COUNTRY_CODE=""

if [[ -v USE_PROXYCHAINS ]]
then
    PXY_FRONTEND="proxychains"
    echo "[INFO] using proxy frontend '$PXY_FRONTEND'"
fi
