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