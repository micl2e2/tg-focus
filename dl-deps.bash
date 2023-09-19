mkdir -p deps


git clone --quiet https://github.com/fmtlib/fmt deps/fmt
cd deps/fmt && \
    git checkout --quiet f5e54359df4c26b6230fc61d38aa294581393084 # 10.1.1

git clone --quiet https://github.com/ToruNiina/toml11 deps/toml11
cd deps/toml11 && \
    git checkout --quiet dcfe39a783a94e8d52c885e5883a6fbb21529019 # v3.7.1

if [[ $? -eq 0 ]]
then
    echo "all dependencies downloaded!"
fi

