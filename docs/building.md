
# Table of Contents

-   [Linux](#org745b526)



<a id="org745b526"></a>

# Linux

    # 1. Prepare OpenSSL 
    bash 3rd/setup-openssl.bash
    
    # 2. Build
    cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
    
    # 3. Check
    build/tgfocus version

