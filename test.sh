#!/bin/bash

# Renkler
GREEN="\033[0;32m"
RED="\033[0;31m"
BLUE="\033[0;34m"
YELLOW="\033[1;33m"
RESET="\033[0m"

echo -e "${BLUE}=== ft_ls Test Süiti Başlatılıyor ===${RESET}"

# Derleme
echo -e "${YELLOW}>>> make re çalıştırılıyor...${RESET}"
make re > /dev/null
if [ $? -ne 0 ]; then
    echo -e "${RED}Derleme hatası!${RESET}"
    exit 1
fi
echo -e "${GREEN}✓ Derleme başarılı!${RESET}\n"

TEST_COUNT=0
PASS_COUNT=0

run_test() {
    local desc="$1"
    local ft_cmd="$2"
    local sys_cmd="$3"
    local check_filter="$4"

    TEST_COUNT=$((TEST_COUNT + 1))
    echo -e "${BLUE}[Test $TEST_COUNT] $desc${RESET}"
    echo "  ft_ls: $ft_cmd"
    echo "  ls   : $sys_cmd"

    local out_ft="/tmp/ft_ls_out_$$"
    local out_sys="/tmp/sys_ls_out_$$"
    local err_ft="/tmp/ft_ls_err_$$"
    local err_sys="/tmp/sys_ls_err_$$"

    eval "$ft_cmd" > "$out_ft" 2> "$err_ft"
    local exit_ft=$?

    eval "$sys_cmd" > "$out_sys" 2> "$err_sys"
    local exit_sys=$?

    if [ "$check_filter" = "strip_xattr" ]; then
        # ACL / extended attributes (@, +) PDF'e göre zorunlu değildir.
        sed -i '' -e 's/@ /  /g' -e 's/+ /  /g' "$out_sys" 2>/dev/null || sed -i -e 's/@ /  /g' -e 's/+ /  /g' "$out_sys" 2>/dev/null
    fi

    local diff_out=$(diff -u "$out_ft" "$out_sys")
    local diff_err=$(diff -u <(sed 's/ft_ls/ls/g' "$err_ft") "$err_sys")

    if [ -z "$diff_out" ] && [ "$exit_ft" -eq "$exit_sys" ]; then
        echo -e "  ${GREEN}✓ BAŞARILI${RESET}"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "  ${RED}✗ FARK BULUNDU!${RESET}"
        if [ -n "$diff_out" ]; then
            echo "Stdout farkı:"
            echo "$diff_out"
        fi
        if [ "$exit_ft" -ne "$exit_sys" ]; then
            echo "Çıkış kodu farkı: ft_ls=$exit_ft vs ls=$exit_sys"
        fi
    fi

    rm -f "$out_ft" "$out_sys" "$err_ft" "$err_sys"
    echo ""
}

# 1. Temel Listeleme
run_test "Varsayılan listeleme" "./ft_ls" "LC_ALL=C ls -1"

# 2. -a Seçeneği (Gizli dosyalar)
run_test "-a seçeneği" "./ft_ls -a" "LC_ALL=C ls -1a"

# 3. -r Seçeneği (Ters sıralama)
run_test "-r seçeneği" "./ft_ls -r" "LC_ALL=C ls -1r"

# 4. -t Seçeneği (Zamana göre sıralama)
run_test "-t seçeneği" "./ft_ls -t" "LC_ALL=C ls -1t"

# 5. -rt Kombinasyonu
run_test "-rt kombinasyonu" "./ft_ls -rt" "LC_ALL=C ls -1rt"

# 6. -R Seçeneği (Özyinelemeli)
run_test "-R özyinelemeli listeleme" "./ft_ls -R" "LC_ALL=C ls -1R"

# 7. -l Uzun format
run_test "-l uzun format" "./ft_ls -l" "LC_ALL=C ls -l" "strip_xattr"

# 8. -la Kombinasyonu
run_test "-la kombinasyonu" "./ft_ls -la" "LC_ALL=C ls -la" "strip_xattr"

# 9. -lt Kombinasyonu
run_test "-lt kombinasyonu" "./ft_ls -lt" "LC_ALL=C ls -lt" "strip_xattr"

# 10. -lR Kombinasyonu
run_test "-lR özyinelemeli uzun format" "./ft_ls -lR" "LC_ALL=C ls -lR" "strip_xattr"

# 11. Dosya argümanları
run_test "Dosya argümanları listeleme" "./ft_ls Makefile test.sh" "LC_ALL=C ls -1 Makefile test.sh"

# 12. Dosya argümanları ile -l
run_test "Dosya argümanları -l formatı" "./ft_ls -l Makefile test.sh" "LC_ALL=C ls -l Makefile test.sh" "strip_xattr"

# 13. Karışık argümanlar (dosya + klasör)
run_test "Karışık argümanlar (dosya + klasör)" "./ft_ls Makefile includes" "LC_ALL=C ls -1 Makefile includes"

# 14. Var olmayan dosya hatası
run_test "Var olmayan dosya hata yönetimi" "./ft_ls non_existent_file Makefile" "LC_ALL=C ls -1 non_existent_file Makefile"

# 15. Geçersiz bayrak
echo -e "${BLUE}[Test 15] Geçersiz bayrak (-z)${RESET}"
TEST_COUNT=$((TEST_COUNT + 1))
./ft_ls -z > /dev/null 2>&1
if [ $? -eq 1 ]; then
    echo -e "  ${GREEN}✓ BAŞARILI (Hata kodu 1 döndü)${RESET}\n"
    PASS_COUNT=$((PASS_COUNT + 1))
else
    echo -e "  ${RED}✗ Hata kodu 1 bekleniyordu!${RESET}\n"
fi

# 16. Bellek sızıntısı testi (leaks)
if command -v leaks &> /dev/null; then
    echo -e "${BLUE}[Test 16] Bellek sızıntısı kontrolü (leaks)${RESET}"
    TEST_COUNT=$((TEST_COUNT + 1))
    leaks -atExit -- ./ft_ls -laR > /tmp/leaks_out 2>&1
    if grep -q "0 leaks for 0 total leaked bytes" /tmp/leaks_out; then
        echo -e "  ${GREEN}✓ BAŞARILI (0 leaks)${RESET}\n"
        PASS_COUNT=$((PASS_COUNT + 1))
    else
        echo -e "  ${RED}✗ Bellek sızıntısı tespit edildi!${RESET}\n"
        cat /tmp/leaks_out | tail -n 15
    fi
    rm -f /tmp/leaks_out
fi

echo -e "${BLUE}========================================${RESET}"
echo -e "Sonuç: ${GREEN}$PASS_COUNT / $TEST_COUNT test başarılı!${RESET}"
echo -e "${BLUE}========================================${RESET}"
