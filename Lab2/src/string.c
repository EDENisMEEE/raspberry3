unsigned int strlen(char* str) {
    int len = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        len = i;
    }
    return len;
}

int strcmp(char* str1, char* str2) {
    if (strlen(str1) != strlen(str2)) return 1;
    for (int i = 0; (str1[i] != '\0') && (str2[i] != '\0'); i++) {
        if (str1[i] != str2[i]) return 1;
    }
    return 0;
}

/**
 * @brief 反轉一個字串
 * * @param str 要反轉的字串
 * @param length 字串的長度 (不包含 null terminator)
 */
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        // 交換 str[start] 和 str[end]
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/**
 * @brief 將一個帶正負號的整數轉換為字串 (itoa)
 * 這個版本是為了 bare-metal 環境設計，不使用任何標準函式庫。
 * * @param value 要轉換的整數
 * @param str   一個足夠大的字元陣列，用來存放轉換後的字串。
 * 對於 32-bit int，最大值為 -2,147,483,648 (11個字元) + null，
 * 所以至少需要 12 個位元組的空間。
 * @return      回傳一個指向字串開頭的指標 (與傳入的 str 相同)。
 */
char* itoa_baremetal(int value, char *str) {
    int i = 0;
    int is_negative = 0;

    // 處理特殊情況：0
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // 處理負數
    // C 語言中，對負數取餘數的結果是負數或 0，例如 -123 % 10 = -3
    // 我們可以利用這個特性來處理 INT_MIN，避免 -value 造成溢位
    if (value < 0) {
        is_negative = 1;
    }

    // 從數字的最後一位開始產生字元，字串會是反的
    while (value != 0) {
        // `value % 10` 的結果範圍是 -9 到 9
        // `abs(value % 10)` -> 得到 0 到 9
        int remainder = value % 10;
        str[i++] = (remainder > 0 ? remainder : -remainder) + '0';
        value = value / 10;
    }

    // 如果是負數，在字串尾端加上負號
    if (is_negative) {
        str[i++] = '-';
    }

    // 加上字串的結束符號
    str[i] = '\0';

    // 將整個字串反轉以得到正確的順序
    reverse(str, i);

    return str;
}