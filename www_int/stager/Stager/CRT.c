#pragma warning(disable:4244)
#include <Windows.h>
int _fltused = 0;
int _ftol2_sse = 0;
int _allmul = 0;
int _allshl = 0;
int _allshr = 0;

PVOID ZeroBuffer(_Out_writes_bytes_all_(Size) PVOID Pointer, _In_ SIZE_T Size)
{
	volatile char* vptr = (volatile char*)Pointer;

	char * endptr = (char *)vptr + Size;
	while (vptr < endptr) {
		*vptr = 0; vptr++;

	}
	return Pointer;
}

size_t _strlen(LPCSTR String)
{
	UINT Length = 0;
	while (*String)
	{
		Length++;
		String++;
	}
	return (Length);
}

size_t _wcslen(LPCWSTR String)
{
	UINT Length = 0;
	while (*String)
	{
		Length++;
		String++;
	}
	return (Length);
}
INT _strncmp(LPCSTR s1, LPCSTR s2, SIZE_T n) {
	CONST UCHAR* a = (CONST UCHAR*)s1;
	CONST UCHAR* b = (CONST UCHAR*)s2;
	const unsigned char* fini = a + n;
	while (a != fini) {
		int res = *a - *b;
		if (res) return res;
		if (!*a) return 0;
		++a; ++b;
	}
	return 0;
}

char *_strchr(const char *s, int c)
{
	while (*s != c && *s != '\0') {
		s++;
	}
	if (*s == c) {
		return (PCHAR)s;
	}
	else {
		return NULL;
	}
}

// taken from online
double _strtod(const char *s, char **sret)
{
	 long double r;        
	int e;            
	long double d;    
	int sign;         
	int esign;
	int i;
	int flags = 0;
	
	r = 0.0;
	sign = 1;
	e = 0;
	esign = 1;
	
	if (s == NULL)
		return r;
	
	while ((*s == ' ') || (*s == '\t'))
		s++;
	
	if (*s == '+')
		s++;
	else if (*s == '-')
	{
		sign = -1;
		s++;
	}
	while ((*s >= '0') && (*s <= '9'))
	{
		flags |= 1;
		r *= 10.0;
		r += *s - '0';
		s++;
	}
	
	if (*s == '.')
	{
		d = 0.1L;
		s++;
		while ((*s >= '0') && (*s <= '9'))
		{
			flags |= 2;
			r += d * (*s - '0');
			s++;
			d *= 0.1L;
		}
	}
	if (flags == 0)
	{
		if (sret)
			*sret = (char *)s;
		return 0;
	}
	if ((*s == 'e') || (*s == 'E'))
	{
		s++;
		if (*s == '+')
			s++;
		else if (*s == '-')
		{
			s++;
			esign = -1;
		}
		if ((*s < '0') || (*s > '9'))
		{
		if (sret)
			*sret = (char *)s;
		return r;
		}
		while ((*s >= '0') && (*s <= '9'))
		{
			e *= 10;
			e += *s - '0';
			s++;
		}
	}
	if (esign < 0)
		for (i = 1; i <= e; i++)
			r *= 0.1L;
	else
		for (i = 1; i <= e; i++)
			r *= 10.0;
	
	if (sret)
		*sret = (char *)s;
	return r * sign;
}



INT _strcmp(LPCSTR s1, LPCSTR s2) {
	while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
		s1++;
		s2++;
	}
	INT i = 0;
	if (*s1 < *s2) i = -1;
	else if (*s1 > *s2) i = 1;
	else i = 0;
	return i;
}

INT _wcscmp(LPCWSTR s1, LPCWSTR s2) {
	while (*s1 != L'\0' && *s2 != L'\0' && *s1 == *s2) {
		s1++;
		s2++;
	}
	INT i = 0;
	if (*s1 < *s2) i = -1;
	else if (*s1 > *s2) i = 1;
	else i = 0;
	return i;
}

long _strtol(const char *nptr, char **endptr, int base)
{
	const char *s;
	long acc, cutoff;
	int c;
	int neg, any, cutlim;

	s = nptr;
	do {
		c = (unsigned char)*s++;
	} while (c <= ' ' || c >= 0x7f);
	if (c == '-') {
		neg = 1;
		c = *s++;
	}
	else {
		neg = 0;
		if (c == '+')
			c = *s++;
	}
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	cutoff = neg ? LONG_MIN : LONG_MAX;
	cutlim = cutoff % base;
	cutoff /= base;
	if (neg) {
		if (cutlim > 0) {
			cutlim -= base;
			cutoff += 1;
		}
		cutlim = -cutlim;
	}
	for (acc = 0, any = 0;; c = (unsigned char)*s++) {
		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 10;
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0)
			continue;
		if (neg) {
			if (acc < cutoff || (acc == cutoff && c > cutlim)) {
				any = -1;
				acc = LONG_MIN;
			}
			else {
				any = 1;
				acc *= base;
				acc -= c;
			}
		}
		else {
			if (acc > cutoff || (acc == cutoff && c > cutlim)) {
				any = -1;
				acc = LONG_MAX;
			}
			else {
				any = 1;
				acc *= base;
				acc += c;
			}
		}
	}
	if (endptr != 0)
		*endptr = (char *)(any ? s - 1 : nptr);
	return (acc);
}
