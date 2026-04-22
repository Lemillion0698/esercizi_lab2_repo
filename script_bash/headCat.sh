echo "questo script stampa le prime 3 righe"
echo " dei file in input "

for file in "$@"; do
        if test -e "$file" && ! test -d "$file"; then
                head -3 "$file"
        fi
done