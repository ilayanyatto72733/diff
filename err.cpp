

#include <SDKDDKVer.h>
#define _WINSOCKAPI_
#include <windows.h>

#include<vector>
#include<string>


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <io.h>

#include "err.h"

#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))
typedef SSIZE_T ssize_t;

void* xmalloc(size_t size) {
	void* ptr;

	if (size == 0)
		errx(2, "xmalloc: zero size");
	ptr = malloc(size);
	if (ptr == NULL)
		err(2, "xmalloc: allocating %zu bytes", size);
	return ptr;
}

char* xstrdup(const char* str) {
	char* cp;

	if ((cp = _strdup(str)) == NULL)
		err(2, "xstrdup");
	return cp;
}

void* xcalloc(size_t nmemb, size_t size) {
	void* ptr;

	ptr = calloc(nmemb, size);
	if (ptr == NULL)
		err(2, "xcalloc: allocating %zu * %zu bytes", nmemb, size);
	return ptr;
}

#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))
void* reallocarray(void* optr, size_t nmemb, size_t size) {

	if ((nmemb >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) &&
		nmemb > 0 && SIZE_MAX / nmemb < size) {
		errno = ENOMEM;
		return (NULL);
	}
	return (realloc(optr, size * nmemb));
}

void* xreallocarray(void* ptr, size_t nmemb, size_t size) {
	void* new_ptr;

	new_ptr = reallocarray(ptr, nmemb, size);
	if (new_ptr == NULL)
		err(2, "xreallocarray: allocating %zu * %zu bytes",
			nmemb, size);
	return new_ptr;
}

int vasprintf(char** strp, const char* fmt, va_list ap) {
	// _vscprintf tells you how big the buffer needs to be
	int len = _vscprintf(fmt, ap);
	if (len == -1) {
		return -1;
	}
	size_t size = (size_t)len + 1;
	char* str = (char*)malloc(size);
	if (!str) {
		return -1;
	}
	// _vsprintf_s is the "secure" version of vsprintf
	//int r = _vsprintf_s(str, len + 1, fmt, ap);
	int r = vsprintf_s(str, len + 1, fmt, ap);
	if (r == -1) {
		free(str);
		return -1;
	}
	*strp = str;
	return r;
}

int asprintf(char** strp, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int r = vasprintf(strp, fmt, ap);
	va_end(ap);
	return r;
}

int xasprintf(char** ret, const char* fmt, ...)
{
	va_list ap;
	int i;

	va_start(ap, fmt);
	i = vasprintf(ret, fmt, ap);
	va_end(ap);

	if (i == -1)
		err(2, "xasprintf");

	return i;
}

int pread(int fd, void* buf, size_t nbytes, off_t offset) {
	off_t oldofs;
	//int  fh;
	ssize_t num;

	oldofs = _tell(fd);
	_lseek(fd, offset, SEEK_SET);

	num = _read(fd, buf, nbytes);

	_lseek(fd, oldofs, SEEK_SET);

	return(num);
}