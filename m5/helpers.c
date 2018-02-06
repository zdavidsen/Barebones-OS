
int strnCmp(char *str1, char *str2, int length) {
  int i, ret;

  for (i = 0; i < length; i++) {
    ret = str1[i] - str2[i];
    if (ret != 0)
      return ret;

    if (str1[i] == 0)
      return 0;
  }
  return 0;
}

int strnCpy(char *src, char *dest, int length) {
  int i;
  for (i = 0; i < length; i++) {
    dest[i] = src[i];
    if (src[i] == 0) {
      i++;
      break;
    }
  }
  return i;
}

int strLen(char *str) {
  int i;
  i = 0;
  while (str[i] != 0) {
    i++;
  }
  return i;
}

int asciiToInt(char *num) {
  if (num[0] > 0x29 && num[0] < 0x40) {
    return num[0] - 0x30;
  }
  if (num[0] > 0x40 && num[0] < 0x47) {
    return num[0] - 0x41 + 10;
  }
  if (num[0] > 0x60 && num[0] < 0x67) {
    return num[0] - 0x61 + 10;
  }
  return 0;
}

int div(int a, int b) {
  int count;
  count = 0;
  while (a >= b) {
    a = a-b;
    count++;
  }
  return count;
}

int mod(int a, int b) {
  while (a >= b) {
    a = a-b;
  }
  return a;
}