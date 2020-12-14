#include <Arduino.h>

typedef struct message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} message;

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
