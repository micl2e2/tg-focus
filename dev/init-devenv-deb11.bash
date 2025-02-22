
PICK_GCC_REL=$(cat pick-ver-gcc)
PICK_TDLIB_SRC=$(cat pick-src-tdlib)

# -------------------------- Install deps --------------------------

sudo apt update

test $? -eq 0 || exit 11

sudo apt upgrade -y

test $? -eq 0 || exit 12

sudo apt-get install -y g++ make wget curl bzip2 autogen dejagnu 

test $? -eq 0 || exit 13

sudo apt-get install -y gperf cmake g++ git zlib1g-dev libssl-dev wget

test $? -eq 0 || exit 14

# ------------------------ Prepare alt-gcc ------------------------

if [[ ! -x /usr/local/bin/alt-gcc ]]
then
    
    cd ~

    wget http://ftp.gnu.org/gnu/gcc/gcc-${PICK_GCC_REL}/gcc-${PICK_GCC_REL}.tar.gz

    test $? -eq 0 || exit 15

    tar xf gcc-${PICK_GCC_REL}.tar.gz

    cd gcc-${PICK_GCC_REL} && ./contrib/download_prerequisites

    test $? -eq 0 || exit 16

    cd ~

    mkdir objdir

    cd objdir && ../gcc-${PICK_GCC_REL}/configure --program-prefix='alt-' --disable-multilib --disable-libsanitizer --enable-languages=c,c++ --enable-shared=libgcc,libstdc++

    test $? -eq 0 || exit 17

    cd ~

    cd objdir && make -j$(nproc)

    test $? -eq 0 || exit 18

    cd ~

    cd objdir && sudo make install

    test $? -eq 0 || exit 19

    cd ~
    
fi

# -------------------------- Prepare tdlib --------------------------

if [[ ! -f /usr/local/lib/libtdcore.a ]]
then

    cd ~

    test -d td/.git || git clone --depth=1000 https://github.com/tdlib/td
	
    test $? -eq 0 || exit 20

    cd td && git reset --hard ${PICK_TDLIB_SRC}

    test $? -eq 0 || exit 21

    cd ~

    cd td && cmake -DCMAKE_BUILD_TYPE=Release -DOPENSSL_USE_STATIC_LIBS=TRUE -DZLIB_USE_STATIC_LIBS=ON -DCMAKE_C_COMPILER=/usr/local/bin/alt-gcc -DCMAKE_CXX_COMPILER=/usr/local/bin/alt-g++ -B build

    test $? -eq 0 || exit 22

    cd ~

    cd td/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make -j$(nproc)

    test $? -eq 0 || exit 23

    cd ~

    cd td/build && LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make test

    test $? -eq 0 || exit 25

    cd ~

    cd td/build && sudo LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH make install

    test $? -eq 0 || exit 24
    
fi









