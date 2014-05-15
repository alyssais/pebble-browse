#pragma once

typedef struct {
  char *string;
  char *start;
  char *end;
} Substr;

uint32_t substrlen(Substr range);
char *substrcpy(char *destination, Substr range);
