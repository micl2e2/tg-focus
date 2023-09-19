
mkdir -p deps

git clone --quiet https://github.com/ToruNiina/toml11 deps/toml11

cd deps/toml11 && \
    git checkout --quiet dcfe39a783a94e8d52c885e5883a6fbb21529019 # v3.7.1

if [[ $? -eq 0 ]]
then
    echo "all dependencies downloaded!"
fi

