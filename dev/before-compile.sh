sed -i -E "s/TDLIB_SRC = \"[a-z0-9]+\";/TDLIB_SRC = \"$(cat dev/pick-src-tdlib)\";/" shared/common.hh

sed -i -E "s/TDLIB_VER = \"[0-9.]+\";/TDLIB_VER = \"$(cat dev/pick-ver-tdlib)\";/" shared/common.hh

sed -i -E "s/TF_VER = \"[0-9.]+\";/TF_VER = \"$(cat dev/pick-ver-tgfocus)\";/" shared/common.hh
