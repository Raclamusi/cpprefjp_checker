#!/bin/zsh -e

stems=(
semicolon
link
paren
square
curly
extract
)

if [ "$1" = 'clean' ]; then
    echo "$0: cleaning..."
    rm -f "${stems[@]}"
    echo "$0: done."
    exit 0
fi

for stem in "${stems[@]}"; do
    src="$stem.cpp"
    exe="$stem"

    if [ ! -f "$exe" ] || [ "$src" -nt "$exe" ]; then
        echo "$0: compiling '$src'..."
        clang++ -std=c++26 -O2 -Wall -Wextra "$src" -o "$exe"
        if [ $? -ne 0 ]; then
            echo "$0: error: compilation failed for '$src'." >&2
            exit 1
        fi
    fi
done

for stem in "${stems[@]}"; do
    exe="$stem"
    txt="$stem.txt"

    echo -e "\x1b[K$0: checking $stem..."
    ./"$exe" > "$txt"
done
echo -ne "\x1b[K"

echo "$0: compiling examples..."
(
    make -i -j8 obj >& make.txt
    error_count=$(grep -c 'Error 1' make.txt || true)
    if [ $error_count -ne 0 ]; then
        echo "$error_count error$(if [ $error_count -ne 1 ]; then echo 's'; fi) generated." >&2
    fi
)

echo "$0: running examples..."
(
    make -i -j8 exe >& make_exe.txt
    make -i -j8 run >& make_run.txt
    ./output_check.sh > output_check.txt
)

echo "$0: running CI scripts..."
(
    cd site_generator/cpprefjp/site/
    source ~/venv/bin/activate
    python3 .github/workflows/script/check.py
)

# echo "$0: checking typos..."
# (
#     typos site_generator/cpprefjp/site/ > typos.txt || true
#     typo_count=$(grep -c 'error: ' typos.txt || true)
#     if [ $typo_count -ne 0 ]; then
#         echo "$typo_count typo$(if [ $typo_count -ne 1 ]; then echo 's'; fi) generated." >&2
#     fi
# )

echo "$0: done."
