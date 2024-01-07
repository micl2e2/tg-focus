###
### ATTENTION: each step must sync with cicd workflow
###

mkdir -p 3rd

# !!!
cd 3rd

PICK_GCC_REL="12.3.0"

echo "[INFO] installing essential packages..."
su -c "apt-get -o Acquire::ForceIPv4=true update && apt-get -o Acquire::ForceIPv4=true install g++ make wget curl bzip2 autogen dejagnu ccache -y --quiet"

test $? -eq 0 || { echo "failed"; exit 1; }

test -e gcc-${PICK_GCC_REL}.tar.gz || \
    wget https://ftp.gnu.org/gnu/gcc/gcc-${PICK_GCC_REL}/gcc-${PICK_GCC_REL}.tar.gz

test $? -eq 0 || { echo "failed"; exit 3; }

test -d gcc-${PICK_GCC_REL} || \
    tar xf gcc-${PICK_GCC_REL}.tar.gz

test $? -eq 0 || { echo "failed"; exit 4; }

cd gcc-${PICK_GCC_REL} && ./contrib/download_prerequisites && cd ..

test $? -eq 0 || { echo "failed"; exit 5; }

grep 'configure: exit 0' gcc-build/config.log

if [[ $? -ne 0 ]]
then
    mkdir -p gcc-build && \
	cd gcc-build \
	&& ../gcc-${PICK_GCC_REL}/configure \
		  --program-prefix='alt-' \
		  --disable-multilib \
		  --disable-libsanitizer \
		  --enable-languages=c,c++ \
		  --enable-shared=libgcc,libstdc++ \
	&& cd ..
 
fi

test $? -eq 0 || { echo "failed"; exit 6; }

cd gcc-build && make -j$(nproc) && cd ..
#cd gcc-build && make -j1 && cd ..

test $? -eq 0 || { echo "failed"; exit 7; }

echo 'finished successfully'

echo "Installcd GCC by running: cd $(pwd)/gcc-build && sudo make install"

cd ..


