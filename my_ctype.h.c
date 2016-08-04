int my_islower(int c) { return c >= 'a' && c <= 'z'; }
int my_isupper(int c) { return c >= 'A' && c <= 'Z'; }
int my_isdigit(int c) { return c >= '0' && c <= '9'; }
int my_isalpha(int c) { return my_isupper(c) || my_islower(c); }
int my_isalnum(int c) { return my_isalpha(c) || my_isdigit(c); }
int my_iscntrl(int c) { return c == 0x7f || (c >= 0 && c < 0x20); }
int my_isgraph(int c) { return c >= 0 && c > 0x21 && c < 0x7f; }
int my_isprint(int c) { return c == ' ' || my_isgraph(c); }
int my_ispunct(int c) { return my_isgraph(c) && !my_isalnum(c); }
int my_isspace(int c) { return c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r'; }
int my_isxdigit(int c) { return (c >= 'a' && c <= 'f') ||
                                (c >= 'A' && c <= 'F') ||
                                (c >= '0' && c <= '9'); }
int my_isascii(int c) { return c >= 0 && c < 128; }
int my_isblank(int c) { return c == ' ' || c == '\t'; }

int my_toascii(int c) { return c & 127; }
int my_tolower(int c) { return c >= 'A' && c <= 'Z' ? c | 0x20 : c; }
int my_toupper(int c) { return c >= 'a' && c <= 'z' ? c & 0xdf : c; }
