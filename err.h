#ifndef _ERR_H_
#define _ERR_H_

#include<vector>
#include<string>


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <io.h>

typedef SSIZE_T ssize_t;

#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFDIR) == S_IFDIR)

template<typename... Targs>
void oo_omegaprntc(const std::string& format, Targs... Fargs) {
	long long size = _scprintf(format.c_str(), Fargs ...); // long long can return -1, not count EOL
	//size_t size = _snwprintf_s(nullptr, null, 0, format.c_str(), Fargs ...) + 1; // Extra space for '\0' // ?double EOL, anyway this fn() call is bad
	if (size <= 0) { printf("oo_omegaprntc() critical error: %s", format.c_str()); exit(666); } // add log
	std::vector<char> buf((size + 4), '\0'); // wchar_t(0) 
	_snprintf_s(buf.data(), buf.size(), size, format.c_str(), Fargs ...); // <stdio.h>
	printf("%s", buf.data());
}

template<typename... Targs>
void err(int eval, const std::string& format, Targs... Fargs) {
	printf("err: %d\n", eval);
	oo_omegaprntc(format.c_str(), Fargs ...);
	exit(eval);
}

template<typename... Targs>
void errx(int eval, const std::string& format, Targs... Fargs) {
	oo_omegaprntc(format.c_str(), Fargs ...);
	exit(eval);
}

template<typename... Targs>
void warn(const std::string& format, Targs... Fargs) {
	oo_omegaprntc(format.c_str(), Fargs ...);
}

template<typename... Targs>
void warnx(const std::string& format, Targs... Fargs) {
	oo_omegaprntc(format.c_str(), Fargs ...);
}

void* xmalloc(size_t size);

char* xstrdup(const char* str);

void* xcalloc(size_t nmemb, size_t size);

void* reallocarray(void* optr, size_t nmemb, size_t size);

void* xreallocarray(void* ptr, size_t nmemb, size_t size);

int vasprintf(char** strp, const char* fmt, va_list ap);

int asprintf(char** strp, const char* fmt, ...);

int xasprintf(char** ret, const char* fmt, ...);

int pread(int fd, void* buf, size_t nbytes, off_t offset);

#endif