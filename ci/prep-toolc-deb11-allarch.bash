set -x

sudo apt-get -o Acquire::ForceIPv4=true update
[[ $? -eq 0 ]] || exit 4

sudo apt-get -o Acquire::ForceIPv4=true install g++ make wget curl bzip2 autogen dejagnu ccache -y --quiet
[[ $? -eq 0 ]] || exit 5


if [[ ! -d ../3rd/gcc-${PICK_GCC_REL} ]]
then
    USE_GNU_MIRROR="ftp.gnu.org"
    [[ -n $PICK_GNU_MIRROR ]] && USE_GNU_MIRROR=$PICK_GNU_MIRROR

    $PXY_FRONTEND wget -O ../3rd/gcc-${PICK_GCC_REL}.tar.gz http://${USE_GNU_MIRROR}/gnu/gcc/gcc-${PICK_GCC_REL}/gcc-${PICK_GCC_REL}.tar.gz
    [[ $? -eq 0 ]] || exit 6

    bash -c "cd ../3rd && tar xf gcc-${PICK_GCC_REL}.tar.gz"
    [[ $? -eq 0 ]] || exit 7
fi


$PXY_FRONTEND bash -c "cd ../3rd/gcc-${PICK_GCC_REL} && ./contrib/download_prerequisites"
[[ $? -eq 0 ]] || exit 8

bash -c "cd ../3rd/gcc-${PICK_GCC_REL} && mkdir -p objdir && cd objdir && ../configure --program-prefix='tgf-' --disable-multilib --disable-libsanitizer --enable-languages=c,c++ --enable-shared=libgcc,libstdc++"
[[ $? -eq 0 ]] || exit 9

bash -c "cd ../3rd/gcc-${PICK_GCC_REL} && cd objdir && make -j$(nproc)"
[[ $? -eq 0 ]] || exit 10

sudo bash -c "cd ../3rd/gcc-${PICK_GCC_REL}/objdir && make install"
[[ $? -eq 0 ]] || exit 12

alt_ver_str=$(bash -c "tgf-gcc --version | head -1")
[[ $? -eq 0 ]] || exit 141
[[ $alt_ver_str =~ $PICK_GCC_REL ]] || exit 141
