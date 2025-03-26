set -x

PACK_DIRNAME=tgfocus-linux-musl-${PICK_ARCH}

mkdir -p ${PACK_DIRNAME}/LICENSE
[[ $? -eq 0 ]] || exit 255

cp ../build/tgfocus ${PACK_DIRNAME}/
[[ $? -eq 0 ]] || exit 255

cp ../README.md ${PACK_DIRNAME}/
[[ $? -eq 0 ]] || exit 255

cp ../LICENSE-GPL ${PACK_DIRNAME}/LICENSE/
[[ $? -eq 0 ]] || exit 255

cp ../3rd/tdlib/LICENSE_1_0.txt ${PACK_DIRNAME}/LICENSE/
[[ $? -eq 0 ]] || exit 255

cp ../3rd/toml11//LICENSE ${PACK_DIRNAME}/LICENSE/
[[ $? -eq 0 ]] || exit 255

tar --create --file ${PACK_DIRNAME}.tgz ${PACK_DIRNAME}
[[ $? -eq 0 ]] || exit 255

sha512sum ${PACK_DIRNAME}.tgz > CHECKSUM-${PACK_DIRNAME}.tgz 
[[ $? -eq 0 ]] || exit 255


