mkdir -p deps

cd deps

git clone https://github.com/fmtlib/fmt fmt
cd fmt && \
    git checkout --quiet f5e54359df4c26b6230fc61d38aa294581393084 # 10.1.1

if [[ $? -eq 0 ]]
then
    printf "%s dependencies downloaded\n" "fmt"
fi

cd ..

git clone https://github.com/ToruNiina/toml11 toml11
cd toml11 && \
    git checkout --quiet dcfe39a783a94e8d52c885e5883a6fbb21529019 # v3.7.1

if [[ $? -eq 0 ]]
then
    printf "%s dependencies downloaded\n" "toml11"
fi

