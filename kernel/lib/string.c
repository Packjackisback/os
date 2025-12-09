#include "string.h"

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len]) {
    len++;
  }
  return len;
}

int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while (n && *s1 && (*s1 == *s2)) {
    s1++;
    s2++;
    n--;
  }
  if (n == 0) {
    return 0;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

char *strcpy(char *dest, const char *src) {
  char *original = dest;
  while ((*dest++ = *src++))
    ;
  return original;
}

char *strncpy(char *dest, const char *src, size_t n) {
  char *original = dest;
  while (n && (*dest++ = *src++)) {
    n--;
  }
  while (n--) {
    *dest++ = '\0';
  }
  return original;
}

char *strcat(char *dest, const char *src) {
  char *original = dest;
  while (*dest) {
    dest++;
  }
  while ((*dest++ = *src++))
    ;
  return original;
}

char *strncat(char *dest, const char *src, size_t n) {
  char *original = dest;
  while (*dest) {
    dest++;
  }
  while (n-- && (*dest++ = *src++))
    ;
  *dest = '\0';
  return original;
}

char *strchr(const char *str, int c) {
  while (*str) {
    if (*str == (char)c) {
      return (char *)str;
    }
    str++;
  }
  return (*str == (char)c) ? (char *)str : 0;
}

char *strrchr(const char *str, int c) {
  const char *last = 0;
  while (*str) {
    if (*str == (char)c) {
      last = str;
    }
    str++;
  }
  if (*str == (char)c) {
    return (char *)str;
  }
  return (char *)last;
}

void *memset(void *ptr, int value, size_t num) {
  unsigned char *p = ptr;
  while (num--) {
    *p++ = (unsigned char)value;
  }
  return ptr;
}

void *memcpy(void *dest, const void *src, size_t n) {
  unsigned char *d = dest;
  const unsigned char *s = src;
  while (n--) {
    *d++ = *s++;
  }
  return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
  unsigned char *d = dest;
  const unsigned char *s = src;

  if (d < s) {
    while (n--) {
      *d++ = *s++;
    }
  } else {
    d += n;
    s += n;
    while (n--) {
      *--d = *--s;
    }
  }
  return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *p1 = s1;
  const unsigned char *p2 = s2;
  while (n--) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

int atoi(const char *str) {
  int result = 0;
  int sign = 1;

  while (*str == ' ' || *str == '\t' || *str == '\n') {
    str++;
  }

  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }

  while (*str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    str++;
  }

  return sign * result;
}

char *itoa(int value, char *str, int base) {
  char *ptr = str;
  char *ptr1 = str;
  char tmp_char;
  int tmp_value;

  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }

  if (value < 0 && base == 10) {
    *ptr++ = '-';
    ptr1++;
    value = -value;
  }

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
  } while (value);

  *ptr-- = '\0';

  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }

  return str;
}

char *utoa(uint32_t value, char *str, int base) {
  char *ptr = str;
  char *ptr1 = str;
  char tmp_char;
  uint32_t tmp_value;

  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }

  do {
    tmp_value = value;
    value /= base;
    *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
  } while (value);

  *ptr-- = '\0';

  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }

  return str;
}
