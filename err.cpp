/* $OpenBSD: xmalloc.c,v 1.10 2019/06/28 05:44:09 deraadt Exp $ */
/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Versions of malloc and friends that check their results, and never return
 * failure (they call fatal if they encounter an error).
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */
/*-
 * Copyright (c) 2016 Mariusz Zaborski <oshogbo@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2008-2010, 2015 Robert N. M. Watson
 * Copyright (c) 2012 FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed at the University of Cambridge Computer
 * Laboratory with support from a grant from Google, Inc.
 *
 * Portions of this software were developed by Pawel Jakub Dawidek under
 * sponsorship from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2013 FreeBSD Foundation
 *
 * This software was developed by Pawel Jakub Dawidek under sponsorship from
 * the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/* $OpenBSD: xmalloc.c,v 1.10 2019/06/28 05:44:09 deraadt Exp $ */
/*
 * Author: Tatu Ylonen <ylo@cs.hut.fi>
 * Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
 *                    All rights reserved
 * Versions of malloc and friends that check their results, and never return
 * failure (they call fatal if they encounter an error).
 *
 * As far as I am concerned, the code I have written for this software
 * can be used freely for any purpose.  Any derived versions of this
 * software must be clearly marked as such, and if the derived work is
 * incompatible with the protocol description in the RFC file, it must be
 * called by a name other than "ssh" or "Secure Shell".
 */
/**
 * @file mkstemp.c
 * Copyright (C) 2013 MinGW.org project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, this permission notice, and the following
 * disclaimer shall be included in all copies or substantial portions of
 * the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OF OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2007-2014 QLogic Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1992, 1993, 1994 Henry Spencer.
 * Copyright (c) 1992, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * Copyright (c) 2011 The FreeBSD Foundation
 *
 * Portions of this software were developed by David Chisnall
 * under sponsorship from the FreeBSD Foundation.
 *
 * This code is derived from software contributed to Berkeley by
 * Henry Spencer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2009 David Schultz <das@FreeBSD.org>
 * Copyright (c) 2021 Dell EMC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include <SDKDDKVer.h>
#define _WINSOCKAPI_
#include <windows.h>

#include<vector>
#include<string>
#include<filesystem>
#include <fstream>
//#include <sstream>
#include <iostream> // std::cin

#include <fcntl.h>
#include <wincrypt.h>

#include<string.h> // memcpy() memcpy_s


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <io.h>

#include "err.h"

//#define MUL_NO_OVERFLOW	((size_t)1 << (sizeof(size_t) * 4))
//typedef SSIZE_T ssize_t;

//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable : 4996)

//typedef	long		off_t; // windows
//typedef	long long	__off_t;	/* file offset */ // __int64_t sys/_types.h
//typedef	__off_t		off_t; /* file offset */      // sys/types.h

// getdelim(), getline() {getdelim(..., '\n');}
size_t
p2roundup(size_t n) {

	if (!powerof2(n)) {
		n--;
		n |= n >> 1;
		n |= n >> 2;
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
//#if SIZE_T_MAX > 0xffffffffU
#if SIZE_MAX > 0xffffffffU
		n |= n >> 32;
#endif
		n++;
	}
	return (n);
}

/*ssize_t getline(char** linep, size_t* linecapp, FILE* stream) { // read_excludes_file()
	return -1;

	// HANDLE = _get_osfhandle(fd); // <io.h> // Never call CloseHandle on the return value of this function.
	// fd = _open_osfhandle(HANDLE, _O_RDONLY); // _close() Don't call the Win32 function CloseHandle on the original handle.
	// DuplicateHandle()
	// FILE * // fd = fileno(FILE *stream);
}//*/

// return the total length of the string they tried to create. For strlcpy() that means the length of src.
inline size_t _strlcpy(char* dst, const char* src, size_t dstsize) {
	size_t l = strlen(src);
	memset(dst, 0, dstsize);
	if (l == 0) // __strlcpy, scalar // freebsd-src-main/lib/libc/amd64/string/strlcpy.S
		return 0;
	if (l < dstsize) { // strlcpy() copies up to dstsize - 1 characters from the string src to dst, NUL-terminating the result if dstsize is not 0.
		memcpy(dst, src, l);
		return l;
	} else {
		memcpy(dst, src, (dstsize - 1)); // src string was longer anyway. partially copied null-terminated instead.
		return (dstsize - 1);
	}
}
size_t strlcpy(char* dst, const char* src, size_t dstsize) {
	return _strlcpy(dst, src, dstsize);
}
size_t strlcpy(char* dst, unsigned char* src, size_t dstsize) {
	return _strlcpy(dst, (const char*)src, dstsize);
}

// freebsd-src-main/lib/libc/amd64/string/strlcat.c
size_t strlcat(char* dst, const char* src, size_t dstsize) {
	char* loc = (char*)memchr(dst, '\0', dstsize);

	if (loc != NULL) {
		size_t dstlen = (size_t)(loc - dst);

		return (dstlen + strlcpy(loc, src, dstsize - dstlen)); //__strlcpy(loc, src, dstsize - dstlen)); // err strcpy_s ain't return size_t
	} else
		return (dstsize + strlen(src));
}

// other freebsd-src-main/lib/libc/amd64/string/strsep.c
char* strsep(char** stringp, const char* delim) {
	char* s;
	const char* spanp;
	int c, sc;
	char* tok;

	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */
}


// https://stackoverflow.com/questions/6036227/mkstemp-implementation-for-win32

//#define __MKSTEMP_INLINE  static __inline__ __attribute__((__always_inline__))
//int __cdecl __mingw_mkstemp(char*);
//#define CRYPTO_INLINE  static __inline__ __attribute__((__always_inline__))
//CRYPTO_INLINE
//#define RSA_MODE( OPT, FLAG )  PROV_RSA_##OPT, CRYPT_##FLAG

inline HCRYPTPROV crypto_provider(void) {
	/* Helper to establish a cryptographic provider context for the
	 * cryptographically secure random number generator.
	 *
	 * At the outset, this provider requires initialization.
	 */
	static HCRYPTPROV id = (HCRYPTPROV)(0);

	/* On second, and subsequent calls, it should already have been
	 * initialized...
	 */
	if (id != (HCRYPTPROV)(0))
		/*
		 * ...so, simply return the saved context handle...
		 */
		return id;

	/* If we're still here, this must be the first call, (or any
	 * preceding call failed to initialize the context); initialize
	 * it now, and if successful...
	 */
	 //if (CryptAcquireContext(&id, NULL, NULL, RSA_MODE(FULL, VERIFYCONTEXT)))
	if (CryptAcquireContextW(&id, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
		/*
		 * ...return the now-initialized context handle.
		 */
		return id;

	/* And finally, if we ever get to here, the context remains
	 * uninitialized; ensure that it remains marked as such, and
	 * return the uninitialized context handle.
	 */
	return id = (HCRYPTPROV)(0);
}

inline void* crypto_randomize(void* buf, DWORD buflen) {
	/* Helper to fill a specified buffer, of specified length,
	 * with cryptographically secure random bytes...
	 */
	if (CryptGenRandom(crypto_provider(), buflen, (unsigned char*)buf))
		/*
		 * ...returning a pointer to the buffer, when successful...
		 */
		return (void*)buf;

	/* ...or nothing, otherwise.
	 */
	return NULL;
}

inline unsigned char* crypto_random_filename_char(unsigned char* caret) {
	/* Helper to generate a random sequence of characters, suitable for
	 * use in file names; although there are other valid possibilities, we
	 * restrict this to the set of lower case ASCII alpha-numerics, giving
	 * us 36 degrees of freedom for each character; (note that we cannot
	 * gain additional degrees of freedom by using mixed case, because
	 * the MS-Windows file system is case-insensitive).
	 */
	const unsigned char span = 'z' - 'a' + 1 + '9' - '0' + 1;

	/* We also wish to ensure that each of the possible 36 characters has
	 * an equal probability of selection; thus, of the UCHAR_MAX possible
	 * raw byte selections, we want to consider at most the largest even
	 * multiple of the 36 character span, which lies below the UCHAR_MAX
	 * limit, (which, since zero is a valid choice, is one less than the
	 * result of discounting the remainder from modulo division).
	 */
	const unsigned char max = UCHAR_MAX - (UCHAR_MAX % span) - 1;

	/* Deposit randomly selected characters at the "caret" location...
	 */
	do {
		if (crypto_randomize(caret, sizeof(unsigned char)) == NULL)
			/*
			 * ...bailing out, on any failure of the sequence generator...
			 */
			return NULL;

		/* ...until we get one which is within the largest possible
	 * subset which yields equal probabilty to each outcome, when
	 * reduced modulo the 36 available degrees of freedom.
	 */
	} while (*caret > max);

	/* Perform the modulo 36 reduction, and offset the result into the
	 * alpha-numeric character range...
	 */
	*caret = '0' + (*caret % span);
	/*
	 * ...while discounting those unsuitable characters which lie within
	 * the range, between '9' and 'a' exclusively.
	 */
	if (*caret > '9') *caret += 'a' - '9' - 1;

	/* Finally, return the "caret" location, indicating the successful
	 * transformation of the character in that position.
	 */
	return caret;
}


char* crypto_tmpname(char* tmplt) {
	//char* crypto_tmpname(char* template) {
		/* Helper function, based on Microsoft's wincrypt API, to construct
		 * the candidate names for temporary files, both in a less predictable
		 * manner than Microsoft's _mktemp() function, and without suffering
		 * its inherent limitation of allowing no more than 26 file names
		 * per template per process thread.
		 *
		 * We begin by locating the position, within the given template,
		 * where the string of six replaceable 'XXXXXX's should begin.
		 */
	char* tail = tmplt + strlen(tmplt) - 6;

	/* Provided this appears sane -- i.e. it at least doesn't place the
	 * six character "tail" before the start of the template itself...
	 */
	if (tail >= tmplt) {
		/* ...then, walk over each of the six bytes of the "tail", until
		 * we reach the NUL terminator...
		 */
		while (*tail) {
			/* ...checking that each byte is initially ASCII 'X', as POSIX
			 * requires them to be; (note that we don't consider that these
			 * may be MBCS trail bytes, since the required 'X' is a single
			 * byte in an MBCS representation anyway)...
			 */
			if ((*tail != 'X') || (crypto_random_filename_char((unsigned char*)tail++) == NULL))
				/*
				 * ...bailing out, and returning nothing, if not.
				 */
				return NULL;
		}
	}
	/* Finally, when we have successfully replaced all six 'XXXXXX's,
	 * we return the modified template, in place.
	 */
	return tmplt;
}

inline int __mingw_mkstemp(char* tmplt) {
	//int __mingw_mkstemp(int setmode, char* template) {
		/* Implementation of the low-level functional support for mkstemp(3);
		 * this provides the formal function implementation, including support
		 * for adjustment of its behaviour w.r.t. temporary file persistence.
		 *
		 * By default, temporary files will persist until explicitly deleted;
		 * POSIX prescribes that temporary files are to be created with the
		 * following attributes, (with O_BINARY added, to ensure there is
		 * no undue influence from "helpful" text mode transformations):
		 */

	static int omode = O_CREAT | O_EXCL | O_RDWR | O_BINARY;

	/* Formal MinGW implementation of the mkstemp(3) function; to begin,
		* we assume that it may fail, and record an invalid file descriptor
		* for return in such eventuality.
		*/
	int fd = -1;

	/* Check that the caller gave us a viable template...
	 */
	if (tmplt == NULL) {
		/* ...bailing out, if nothing at all...
		 */
		errno = EINVAL;
	} else { /* ...but assume that anything at all is potentially viable;
		   * set up a retry limit, and estimate the storage requirement
		   * for a working scratch buffer.
		   */
		int retry = TMP_MAX;
		size_t bufsiz = 1 + strlen(tmplt);

		/* Until we either get a valid file descriptor, or we exhaust
		 * the retry limit while attempting to get one...
		 */
		while ((fd < 0) && (retry-- > 0)) {
			/* ...set up the scratch buffer, copy the template into it,
			 * then transform to get a cryptographically secure candidate
			 * file name for the temporary file; (each retry cycle will
			 * generate a randomly differing candidate file name)...
			 */
			std::vector<char>filename(strlen(tmplt) + 1, '\0'); //char filename[bufsiz];
			strcpy_s(filename.data(), filename.size(), tmplt);
			if (crypto_tmpname(filename.data()) == NULL) {
				/* ...bailing out, on any unsuccessful attempt to generate
				 * the candidate name; (this is most likely to occur during
				 * the first cycle, due to a malformed template; if we can
				 * successfully generate the first candidate, successive
				 * attempts are unlikely to fail).
				 */
				errno = EINVAL;
				retry = 0;
			} else { /* We got a usable candidate file name; attempt to open it
				   * as a new file...
				   */
#pragma warning(disable : 4996)	// _CRT_SECURE_NO_WARNINGS
				if ((fd = _open(filename.data(), omode, S_IREAD | S_IWRITE)) >= 0)
					/*
					 * ...and, on success, update the template to reflect the
					 * name of the file we've opened, and we are done...
					 */
					strcpy_s(tmplt, strlen(tmplt), filename.data());
#pragma warning(default : 4996)

				/* ...but, if we failed for any reason other than that a file
				 * with the candidate name already exists...
				 */
				else if (errno != EEXIST)
					/*
					 * ...then, any retry will most likely also fail, so we may
					 * as well just give up now.
					 */
					retry = 0;
			}
		}
	}
	/* Finally, whether we succeeded in opening any temporary file, or we
	 * ultimately gave up in disgust, we return the prevailing state of the
	 * file descriptor we attempted to assign.
	 */
	return fd;
}
int mkstemp(char* __name_template) {
	return __mingw_mkstemp(__name_template);
}


struct pr* start_pr(char* file1, char* file2) {
	return (pr*)NULL;
};
void stop_pr(struct pr*) {
};


cap_rights_t* __cap_rights_init(int version, cap_rights_t* rights, ...) {
	return rights;
};

#define	cap_rights_set(...)						\
	__cap_rights_set(__VA_ARGS__, 0ULL)
cap_rights_t* __cap_rights_set(cap_rights_t* rights, ...) {
	return rights;
};

#define	cap_rights_clear(...)						\
	__cap_rights_clear(__VA_ARGS__, 0ULL)
cap_rights_t* __cap_rights_clear(cap_rights_t* rights, ...) {
	return rights;
};

#define	cap_rights_is_set(...)						\
	__cap_rights_is_set(__VA_ARGS__, 0ULL)
bool __cap_rights_is_set(const cap_rights_t* rights, ...) {
	return false;
};

bool cap_rights_is_empty(const cap_rights_t* rights) {
	return true;
};

bool cap_rights_is_valid(const cap_rights_t* rights) {
	return false;
};
cap_rights_t* cap_rights_merge(cap_rights_t* dst, const cap_rights_t* src) {
	return dst;
};
cap_rights_t* cap_rights_remove(cap_rights_t* dst, const cap_rights_t* src) {
	return dst;
};

const cap_rights_t* cap_rights_fde(const struct filedescent* fde) {
	return (cap_rights_t*)NULL;
};
const cap_rights_t* cap_rights(struct filedesc* fdp, int fd) {
	return (cap_rights_t *)NULL;
};



bool can_libdiff(int flags) { // diffreg_new()
	return false;
}
int		 diffreg_new(char*, char*, int, int) {
#define	D_ERROR		7	/* A file access error occurred */ // diff.h
	return D_ERROR;
}

/*
 * Alphabetic order comparison routine for those who want it.
 * POSIX 2008 requires that alphasort() uses strcoll().
 */
int
alphasort(const struct dirent** d1, const struct dirent** d2) {

	return (strcoll((*d1)->d_name, (*d2)->d_name));
}

/*// strverscmp.c
int
versionsort(const struct dirent** d1, const struct dirent** d2) {

	return (strverscmp((*d1)->d_name, (*d2)->d_name));
}//*/

static int
scandir_thunk_cmp(const void* p1, const void* p2, void* thunk) {
	int (*dc)(const struct dirent**, const struct dirent**);

	dc = *(int (**)(const struct dirent**, const struct dirent**))thunk;
	return (dc((const struct dirent**)p1, (const struct dirent**)p2));
}


static inline void
swapfunc(char* a, char* b, size_t es) {
	char t;

	do {
		t = *a;
		*a++ = *b;
		*b++ = t;
	} while (--es > 0);
}
#define	vecswap(a, b, n)				\
	if ((n) > 0) swapfunc(a, b, n)

#define local_qsort local_qsort_r_compat
//#define	CMP(t, x, y) (cmp((t), (x), (y)))
//#define	CMP(t, x, y) (cmp((x), (y)))
//#define	CMP(t, x, y) (alphasort((const dirent**)(x), (const dirent**)(y)))
//#define	CMP(t, x, y) (cmp(alphasort((const dirent**)(x), (const dirent**)(y))))
//#define	CMP(t, x, y) (cmp((x), (y)))
// cmp() is cmp_t* cmp from caller fn()
#define	CMP(t, x, y) (cmp((const dirent**)(x), (const dirent**)(y)))

//static inline char*
//med3(char* a, char* b, char* c, cmp_t* cmp, void* thunk
//#if !defined(I_AM_QSORT_R) && !defined(I_AM_QSORT_R_COMPAT) && !defined(I_AM_QSORT_S)
//	 __unused
//#endif
static inline char*
med3(char* a, char* b, char* c, cmp_t* cmp, void* thunk) { // thunk __unused
	return CMP(thunk, a, b) < 0 ?
			(CMP(thunk, b, c) < 0 ? b : (CMP(thunk, a, c) < 0 ? c : a))
		:	(CMP(thunk, b, c) > 0 ? b : (CMP(thunk, a, c) < 0 ? a : c));
}

static void
local_qsort(void* a, size_t n, size_t es, cmp_t* cmp, void* thunk) {

	char* pa, * pb, * pc, * pd, * pl, * pm, * pn;
	size_t d1, d2;
	int cmp_result;
	int swap_cnt;

	pa = pb = pc = pd = pl = pm = pn = nullptr;
	d1 = d2 = 0;
	cmp_result = swap_cnt = 0;

	/* if there are less than 2 elements, then sorting is not needed */
	if (__predict_false(n < 2))
		return;
loop:
	swap_cnt = 0;
	if (n < 7) {
		for (pm = (char*)a + es; pm < (char*)a + n * es; pm += es)
			for (pl = pm;
				 pl > (char*)a && CMP(thunk, pl - es, pl) > 0;
				 pl -= es)
				swapfunc(pl, pl - es, es);
		return;
	}
	pm = (char*)a + (n / 2) * es;
	if (n > 7) {
		pl = (char*)a;
		pn = (char*)a + (n - 1) * es;
		if (n > 40) {
			size_t d = (n / 8) * es;

			pl = med3(pl, pl + d, pl + 2 * d, cmp, thunk);
			pm = med3(pm - d, pm, pm + d, cmp, thunk);
			pn = med3(pn - 2 * d, pn - d, pn, cmp, thunk);
		}
		pm = med3(pl, pm, pn, cmp, thunk);
	}
	swapfunc((char*)a, pm, es);
	pa = pb = (char*)a + es;

	pc = pd = (char*)a + (n - 1) * es;
	for (;;) {
		while (pb <= pc && (cmp_result = CMP(thunk, pb, a)) <= 0) {
			if (cmp_result == 0) {
				swap_cnt = 1;
				swapfunc(pa, pb, es);
				pa += es;
			}
			pb += es;
		}
		while (pb <= pc && (cmp_result = CMP(thunk, pc, a)) >= 0) {
			if (cmp_result == 0) {
				swap_cnt = 1;
				swapfunc(pc, pd, es);
				pd -= es;
			}
			pc -= es;
		}
		if (pb > pc)
			break;
		swapfunc(pb, pc, es);
		swap_cnt = 1;
		pb += es;
		pc -= es;
	}
	if (swap_cnt == 0) {  /* Switch to insertion sort */
		for (pm = (char*)a + es; pm < (char*)a + n * es; pm += es)
			for (pl = pm;
				 pl > (char*)a && CMP(thunk, pl - es, pl) > 0;
				 pl -= es)
				swapfunc(pl, pl - es, es);
		return;
	}

	pn = (char*)a + n * es;
	d1 = MIN(pa - (char*)a, pb - pa);
	vecswap((char*)a, pb - d1, d1);
	/*
	 * Cast es to preserve signedness of right-hand side of MIN()
	 * expression, to avoid sign ambiguity in the implied comparison.  es
	 * is safely within [0, SSIZE_MAX].
	 */
	d1 = MIN(pd - pc, pn - pd - (ssize_t)es);
	vecswap(pb, pn - d1, d1);

	d1 = pb - pa;
	d2 = pd - pc;
	if (d1 <= d2) {
		/* Recurse on left partition, then iterate on right partition */
		if (d1 > es) {
			local_qsort(a, d1 / es, es, cmp, thunk);
		}
		if (d2 > es) {
			/* Iterate rather than recurse to save stack space */
			/* qsort(pn - d2, d2 / es, es, cmp); */
			a = pn - d2;
			n = d2 / es;
			goto loop;
		}
	} else {
		/* Recurse on right partition, then iterate on left partition */
		if (d2 > es) {
			local_qsort(pn - d2, d2 / es, es, cmp, thunk);
		}
		if (d1 > es) {
			/* Iterate rather than recurse to save stack space */
			/* qsort(a, d1 / es, es, cmp); */
			n = d1 / es;
			goto loop;
		}
	}
}

static inline void __qsort_r_compat(void* a, size_t n, size_t es, void* thunk, cmp_t* cmp) {
	local_qsort_r_compat(a, n, es, cmp, thunk); // 4<->5
}
//static inline void qsort_r__(void* base, size_t nmemb, size_t size,
//						   void* thunk, int (*compar)(void*, const void*, const void*)) {
//	__qsort_r_compat(base, nmemb, size, thunk, compar);
//}
inline
void qsort_r(void* base, size_t nmemb, size_t size,
							void* thunk,
							int (*compar)(const struct dirent**, const struct dirent**)) {
						   //void* thunk, int (*compar)(void*, const void*, const void*)) {

	__qsort_r_compat(base, nmemb, size, thunk, compar);
}


/*
 * After readdir() returns the last entry in a block, a call to telldir
 * returns a location that is after the end of that last entry.
 * However, that location doesn't refer to a valid directory entry.
 * Ideally, the call to telldir() would return a location that refers to
 * the first entry in the next block.  That location is not known
 * until the next block is read, so readdir() calls this function after
 * fetching a new block to fix any such telldir() locations.
 */
void
_fixtelldir(DIR* dirp, off_t oldseek, size_t oldloc) {
	struct ddloc_mem* lp;

	lp = LIST_FIRST(&dirp->dd_td->td_locq);
	if (lp != NULL) {
		if (lp->loc_loc == oldloc && lp->loc_seek == oldseek) {
			lp->loc_seek = dirp->dd_seek;
			lp->loc_loc = dirp->dd_loc;
		}
	}
}

static ssize_t _getdirentries(int fd, _Out_writes_bytes_(count) char* buf, size_t count, _Out_opt_ off_t* basep) {
	basep = nullptr;
	return -1;
}

bool _filldir(DIR* dirp, bool use_current_pos) {
	return (false);
}

/*
 * get next entry in a directory.
 */
struct dirent*
_readdir_unlocked(DIR* dirp, int flags) {
	struct dirent* dp;
	off_t initial_seek;
	size_t initial_loc = 0;
	ssize_t ret;

	for (;;) {
		if (dirp->dd_loc >= dirp->dd_size) {
			if (dirp->dd_flags & __DTF_READALL)
				return (NULL);
			initial_loc = dirp->dd_loc;
			dirp->dd_flags &= ~__DTF_SKIPREAD;
			dirp->dd_loc = 0;
		}
		return (NULL);
		// below
		// if (dp->d_ino == 0 && (flags & RDU_SKIP) != 0)
		// struct dirent* dp;
		// d_ino part of struct	direct {uint32_t d_ino;, ...}
		if (dirp->dd_loc == 0 && !(dirp->dd_flags & (__DTF_READALL | __DTF_SKIPREAD))) {
			dirp->dd_size = 0;
			initial_seek = dirp->dd_seek;
			// _getdirentries SYSCALL 554 SYS_getdirentries // get directory entries in a file system independent format
			// int     getdirentries(int fd, _Out_writes_bytes_(count) char* buf, u_int count, _Out_opt_ long* basep);   // 156 is old getdirentries
			// ssize_t getdirentries(int fd, _Out_writes_bytes_(count) char* buf, size_t count, _Out_opt_ off_t* basep); // 554
			// sys_getdirentries(struct thread *td, struct getdirentries_args *uap)  ->
			//   error = kern_getdirentries(td, uap->fd, uap->buf, uap->count, &base, NULL, UIO_USERSPACE);
			//   error = getvnode(td, fd, &cap_read_rights, &fp); // struct file *fp; // vfs_syscalls.c
			//       Convert a user file descriptor to a kernel file entry and check
			//       that, if it is a capability, the correct rights are present.
			//       A reference on the file entry is held upon returning.
			//   struct vnode *vp;  = fp->f_vnode;
			//   error = VOP_READDIR(vp, &auio, fp->f_cred, &eofflag, NULL, NULL); // ufs_readdir etc
			// https://man.freebsd.org/cgi/man.cgi?query=getdirentries&apropos=0&sektion=0&manpath=FreeBSD+14.3-RELEASE+and+Ports&arch=default&format=html
			// The current position pointer associated with fd is set to point to the next block of entries.
			// A value of zero is returned when the end of the directory has been reached.
			ret = _getdirentries(dirp->dd_fd, dirp->dd_buf, dirp->dd_len, &dirp->dd_seek);
			if (ret <= 0) // _getdirentries(): A value of zero is returned when the end of the directory has been reached.
				return (NULL);
			dirp->dd_size = (size_t)ret;
			_fixtelldir(dirp, initial_seek, initial_loc);
		}
		dirp->dd_flags &= ~__DTF_SKIPREAD;
		dp = (struct dirent*)(dirp->dd_buf + dirp->dd_loc);
		if ((long)dp & 03L)	/* bogus pointer check */
			return (NULL);
/*
		if (dp->d_reclen <= 0 ||
			dp->d_reclen > dirp->dd_len + 1 - dirp->dd_loc)
			return (NULL);
		dirp->dd_loc += dp->d_reclen;
		if (dp->d_ino == 0 && (flags & RDU_SKIP) != 0)
			continue;
		if (dp->d_type == DT_WHT && (dirp->dd_flags & DTF_HIDEW))
			continue;
		if (dp->d_namlen >= sizeof(dp->d_name) &&
			(flags & RDU_SHORT) != 0)
			continue;
//*/
		return (dp);
	}
}

struct dirent*
readdir(DIR* dirp) {
	struct dirent* dp;

	dp = _readdir_unlocked(dirp, RDU_SKIP);
	return (dp);
}

static int
scandir_dirp(DIR* dirp, struct dirent*** namelist,
			 int (*select)(const struct dirent*),
			 int (*dcomp)(const struct dirent**, const struct dirent**))
{
	struct dirent* d, * p = NULL, ** names = NULL, ** names2;
	size_t arraysz = 32, numitems = 0;
	int serrno;

	names = (dirent **)malloc(arraysz * sizeof(*names));
	if (names == NULL)
		return (-1);

	while (errno = 0, (d = readdir(dirp)) != NULL) {
		if (select != NULL && !SELECT(d))	// select() {return(0) if excluding} // selectfile() diffdir.cpp
			continue;	/* just selected names */
		/*
		 * Make a minimum size copy of the data
		 */
		p = (dirent *)malloc(_GENERIC_DIRSIZ(d));
		if (p == NULL)
			goto fail;
		p->d_fileno = d->d_fileno;
		p->d_type = d->d_type;
		p->d_reclen = d->d_reclen;
		p->d_namlen = d->d_namlen;
		memcpy(p->d_name, d->d_name, p->d_namlen + 1);
		/*
		 * Check to make sure the array has space left and
		 * realloc the maximum size.
		 */
		if (numitems >= arraysz) {
			arraysz = arraysz * 2;
			names2 = (dirent **)reallocarray(names, arraysz, sizeof(*names));
			if (names2 == NULL)
				goto fail;
			names = names2;
		}
		names[numitems++] = p;
	}
	/*
	 * Since we can't simultaneously return both -1 and a count, we
	 * must either suppress the error or discard the partial result.
	 * The latter seems the lesser of two evils.
	 */
	if (errno != 0)
		goto fail;
	if (numitems > 0 && dcomp != NULL) {
		//qsort_r(names, numitems, sizeof(struct dirent*), scandir_thunk_cmp, &dcomp);
		qsort_r(names, numitems, sizeof(struct dirent*), scandir_thunk_cmp, dcomp);
	}
	*namelist = names;
	return (numitems);

fail:
	serrno = errno;
	if (numitems == 0 || names[numitems - 1] != p)
		free(p);
	while (numitems > 0)
		free(names[--numitems]);
	free(names);
	errno = serrno;
	return (-1);
}


/*
 * Common routine for opendir(3), __opendir2(3) and fdopendir(3).
 */
DIR*
__opendir_common(int fd, int flags, bool use_current_pos) {
	DIR* dirp;
	ssize_t ret;
	int incr;
	int saved_errno;
	bool unionstack;
	SYSTEM_INFO si;

	if ((dirp = (DIR *)malloc(sizeof(DIR) + sizeof(struct _telldir))) == NULL)
		return (NULL);

	dirp->dd_buf = NULL;
	dirp->dd_fd = fd;
	dirp->dd_flags = flags;
	dirp->dd_loc = 0;
	dirp->dd_lock = NULL;
	dirp->dd_td = (struct _telldir*)((char*)dirp + sizeof(DIR));
	LIST_INIT(&dirp->dd_td->td_locq);
	dirp->dd_td->td_loccnt = 0;
	dirp->dd_compat_de = NULL;

	/*
	 * Use the system page size if that is a multiple of DIRBLKSIZ.
	 * Hopefully this can be a big win someday by allowing page
	 * trades to user space to be done by _getdirentries().
	 */
	//incr = getpagesize();
	GetSystemInfo(&si);
	incr = (size_t)si.dwPageSize; //incr = 4096; // popular number
	if ((incr % DIRBLKSIZ) != 0)
		incr = DIRBLKSIZ;

	/*
	 * Determine whether this directory is the top of a union stack. // "union" flag from mount(2) or unionfs
	 */
	unionstack = false;
	if (flags & DTF_NODUP) {
//		unionstack = is_unionstack(fd);
	}

	if (unionstack) {
		if (!_filldir(dirp, use_current_pos))
			goto fail;
		dirp->dd_flags |= __DTF_READALL;
	} else {
		dirp->dd_len = incr;
		dirp->dd_buf = (char *)malloc(dirp->dd_len);
		if (dirp->dd_buf == NULL)
			goto fail;
		if (use_current_pos) {
			/*
			 * Read the first batch of directory entries
			 * to prime dd_seek.  This also checks if the
			 * fd passed to fdopendir() is a directory.
			 */
			ret = _getdirentries(dirp->dd_fd, dirp->dd_buf, dirp->dd_len, &dirp->dd_seek);
			if (ret < 0)
				goto fail;
			dirp->dd_size = (size_t)ret;
			dirp->dd_flags |= __DTF_SKIPREAD;
		} else {
			dirp->dd_size = 0;
			dirp->dd_seek = 0;
		}
	}

	return (dirp);

fail:
	saved_errno = errno;
	free(dirp->dd_buf);
	free(dirp);
	errno = saved_errno;
	return (NULL);
}

DIR*
fdopendir(int fd) {
	/*int flags, rc;

	flags = _fcntl(fd, F_GETFD, 0);
	if (flags == -1)
		return (NULL);

	if ((flags & FD_CLOEXEC) == 0) {
		rc = _fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
		if (rc == -1)
			return (NULL);
	}//*/
	return (__opendir_common(fd, DTF_HIDEW | DTF_NODUP, true));
}

/*
 * Reclaim memory for telldir cookies which weren't used.
 */
void
_reclaim_telldir(DIR* dirp) {
	struct ddloc_mem* lp;
	struct ddloc_mem* templp;

	lp = LIST_FIRST(&dirp->dd_td->td_locq);
	while (lp != NULL) {
		templp = lp;
		lp = LIST_NEXT(lp, loc_lqe);
		free(templp);
	}
	LIST_INIT(&dirp->dd_td->td_locq);
}

int
fdclosedir(DIR* dirp) {
	int fd;

	fd = dirp->dd_fd;
	dirp->dd_fd = -1;
	dirp->dd_loc = 0;
	free((void*)dirp->dd_buf);
	free(dirp->dd_compat_de);
	_reclaim_telldir(dirp);
	free((void*)dirp);
	return (fd);
}

/*int
fdscandir_orig(int dirfd, struct dirent*** namelist,
		  int (*select)(const struct dirent*),
		  int (*dcomp)(const struct dirent**, const struct dirent**))
{
	DIR* dirp;
	int ret, serrno;

	dirp = fdopendir(dirfd);
	if (dirp == NULL)
		return (-1);
	ret = scandir_dirp(dirp, namelist, select, dcomp);
	serrno = errno;
	fdclosedir(dirp);
	errno = serrno;
	return (ret);
}//*/

int
fdscandir(const std::wstring& in_str, struct dirent*** namelist,
			   int (*select)(const struct dirent*),
			   int (*dcomp)(const struct dirent**, const struct dirent**)) {

	FILE_ID_INFO fi;
	uint64_t ih = 0, il = 0;
	BYTE* ihp, * ilp;

	//scandir_;
	struct dirent* scandir_p = NULL, ** names = NULL, ** names2;
	struct dirent scandir_d = {NULL};
	size_t scandir_arraysz = 32, scandir_numitems = 0;
	HANDLE scandir_fh = INVALID_HANDLE_VALUE;
	std::filesystem::path scandir_path;
	std::string scandir_str;
	std::wstring scandir_wstr;
	size_t scandir_psize;

	scandir_d.d_fileno = 1; // 0 to skip in selectfile()
	std::filesystem::path l_path = in_str.c_str();
	std::filesystem::directory_options::skip_permission_denied;
	std::filesystem::perm_options::nofollow;

	/*
	* dirent* filled in scandir_dirp()
	* typedef struct _dirdesc DIR; // trash
	* so this vscandir(..., reffer to dirp as struct dirent ***dirp, ...)
	* while called fdscandir() {dirp is DIR*} // struct dirent*** namelist
	*
	* fdscandir() {
	* __opendir_common() {
	*   DIR* dirp;
	*
	*   dirp = (DIR *)malloc(sizeof(DIR) + sizeof(struct _telldir)) // DIR* required for readdir()
	*   ret = _getdirentries(); // first call to _getdirentries()
	*   dirp->dd_size = (size_t)ret;
	*   dirp->dd_flags |= __DTF_SKIPREAD;
	*
	*   return (dirp);
	* }
	*
	* res = scandir_dirp(DIR* dirp, struct dirent*** namelist, selfn(), comparefn()) {
	*   struct dirent* d, * p = NULL, ** names = NULL, ** names2;
	*   size_t arraysz = 32, numitems = 0;
	*
	*   names = (dirent **)malloc(arraysz * sizeof(*names));
	*
	*   // (dirent)readdir(DIR*)
	*   while (errno = 0, (d = readdir(dirp)) != NULL) { // call _getdirentries()
	*     if (select != NULL && !SELECT(d)) continue; // select is trash if (('.') || ('?.')) + excludes_list
	*
	*     p = (dirent *)malloc(_GENERIC_DIRSIZ(d)); // p by d
	*
	*     if (numitems >= arraysz) {
	*       names2 = (dirent **)reallocarray(names, arraysz, sizeof(*names));
	*       if (names2 == NULL)
	*         goto fail;
	*       names = names2;
	*     }
	*     names[numitems++] = p;
	*   }
	*   sortfn();
	*   *namelist = names;
	*   return (numitems);
	*
	* fail:
	*   serrno = errno;
	*   if (numitems == 0 || names[numitems - 1] != p)
	*     free(p);
	*   while (numitems > 0)
	*     free(names[--numitems]);
	*   free(names);
	*   errno = serrno;
	*   return (-1);
	* }
	*
	* return res;
	* }
	*/

	//*

	//wprintf(L"fdscandir \"%s\"\n", l_path.c_str());

	names = (dirent**)malloc(scandir_arraysz * sizeof(*names));
	if (names == NULL)
		goto scandir_fail;

	// or https://learn.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory
	for (auto it{std::filesystem::directory_iterator(l_path)}; it != std::filesystem::directory_iterator(); ++it) {

		scandir_path = it->path(); // fqp

		//scandir_psize = _GENERIC_DIRSIZ(&scandir_d);
		scandir_str = wstring2string(it->path().filename().c_str());
		scandir_psize = _GENERIC_DIRLEN(scandir_str.length());

		memset(scandir_d.d_name, 0, ARRAYSIZE(scandir_d.d_name));
		memcpy(scandir_d.d_name, scandir_str.data(), scandir_str.length());
		if (!SELECT(&scandir_d))
			continue;

		//wprintf(L"\"%s\", \"%s\", %zd\n", it->path().filename().c_str(), scandir_path.c_str(), scandir_psize);
		//scandir_p = (dirent*)malloc(_GENERIC_DIRSIZ(&scandir_d));
		scandir_p = (dirent*)malloc(scandir_psize);
		if (scandir_p == NULL)
			goto scandir_fail;
		memset(scandir_p, 0, scandir_psize);

		scandir_fh = CreateFileW(scandir_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS), NULL);
		if (INVALID_HANDLE_VALUE == scandir_fh) {
			printf("scandir_open handle err 0x08%lx\n", GetLastError());
			goto scandir_fail;
		}
		memset(&fi, 0, sizeof(fi));
		GetFileInformationByHandleEx(scandir_fh, FILE_INFO_BY_HANDLE_CLASS{FileIdInfo}, &fi, sizeof(fi));
		for (size_t idx = 7; (idx < 8) && (idx >= 0); idx--) {
			ilp = (BYTE*)&il + idx;
			*ilp = fi.FileId.Identifier[idx];

			ihp = (BYTE*)&ih + idx;
			*ihp = fi.FileId.Identifier[idx + 8];
		}
		CloseHandle(scandir_fh);

		scandir_p->d_fileno = il;
		//p->d_type = d->d_type;
		if (std::filesystem::is_directory(it->symlink_status())) {
			scandir_p->d_type = DT_DIR;
			//printf("\t\tdir\n");
		} else if (std::filesystem::is_regular_file(it->symlink_status())) {
			scandir_p->d_type = DT_REG; // + DT_LNK
			//printf("\t\tfile\n");
		} else {
			scandir_p->d_type = DT_UNKNOWN;
			//printf("\t\toth\n");
		} // fs::is_symlink(s), !fs::exists(s) // windows: Junction, symlink, hard link // !

		//printf("cvt1: 0x%016zx, %d\n", il, scandir_p->d_reclen);
		//scandir_p->d_reclen = sizeof(struct dirent); // d->d_reclen; // set in ufs_readdir() // !
		scandir_p->d_reclen = static_cast<uint16_t>(scandir_psize); // d->d_reclen; // set in ufs_readdir() // !
		//scandir_p->d_namlen = wcslen(it->path().c_str());// d->d_namlen;
		scandir_p->d_namlen = static_cast<uint16_t>(scandir_str.length());// d->d_namlen;
		//memcpy(p->d_name, d->d_name, p->d_namlen + 1);
		memcpy(scandir_p->d_name, scandir_str.data(), scandir_str.length());
		//printf("volume_dev 0x%016zx, id 0x%016zx %016zx %zx\n", fi.VolumeSerialNumber, ih, il, i);
		//printf("cvt2: 0x%016zx, %s, %s, %ld, %ld\n", scandir_p->d_fileno, scandir_str.c_str(), scandir_p->d_name, scandir_p->d_reclen, scandir_p->d_namlen);

		if (scandir_numitems >= scandir_arraysz) {
			scandir_arraysz = scandir_arraysz * 2;
			names2 = (dirent**)reallocarray(names, scandir_arraysz, sizeof(*names));
			if (names2 == NULL)
				goto scandir_fail;
			names = names2;
		}
		names[scandir_numitems++] = scandir_p;

	}

	if (scandir_numitems > 0 && dcomp != NULL) {
		qsort_r(names, scandir_numitems, sizeof(struct dirent*), select, dcomp);
	}
	//goto scandir_fail;

	//printf("ret dirp %zx, %zd\n", (uint64_t)names, scandir_numitems);
	//*dirp = names;
	*namelist = names;
	//return (numitems);
	//ret = static_cast<int>(scandir_numitems);
	return (static_cast<int>(scandir_numitems));

	//*/

scandir_fail:
	//printf("scandir_fail\n");
	//serrno = errno;
	if (scandir_numitems == 0 || names[scandir_numitems - 1] != scandir_p)
		free(scandir_p);
	while (scandir_numitems > 0)
		free(names[--scandir_numitems]);
	free(names);
	//errno = serrno;
	return (-1);

}

std::string wstring2string(const std::wstring& in_str) {
	return unicode::to_utf8(in_str);
}
std::wstring string2wstring(const std::string& in_str) {
	return unicode::to_wstring(in_str);
}

// uint32_t nanoseconds(const *char fname);
DWORD getnsec(const std::string& f) {
	HANDLE fh = INVALID_HANDLE_VALUE;
	struct _FILETIME ft = {};
	uint64_t res = 0;

	std::wstring fn = string2wstring(f);

	fh = CreateFileW(fn.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS), NULL);
	if (INVALID_HANDLE_VALUE == fh)
		return 0;
	if (!GetFileTime(fh, NULL, NULL, &ft)) {
		CloseHandle(fh);
		return 0;
	}
	CloseHandle(fh);
	res = (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
	//FileTimeToSystemTime(); -> RtlTimeToTimeFields();
#define TICKSPERSEC 10000000
#define nsec_max     999999999
#define NSECPERSEC  1000000000
#define TICKSPERMSEC      10000
	//DWORD Milliseconds = (CSHORT)((IntTime % TICKSPERSEC) / TICKSPERMSEC);

#define REMAINDER %
#define EINS_ZWEI_POLIZEI_KARPOVICH REMAINDER
#define MODULO EINS_ZWEI_POLIZEI_KARPOVICH
#define DIV0 MODULO
#define DIVO DIV0
#define AZERBAIJANIS_SINGOV DIVO
#define EPSTEIN_DIVING AZERBAIJANIS_SINGOV
#define YASHIN EPSTEIN_DIVING
#define ДАЛЬ YASHIN

	return ((res ДАЛЬ TICKSPERSEC) * (NSECPERSEC / TICKSPERSEC));

#define AZERBAIJANIS PDC_ARRIVAL
#define Maciuś AZERBAIJANIS
	// Fem King Maciuś {Casu martzu, Macho, Matcha_抹茶_Маття_Matt_Jailmate, MACH3.EXE_and_nearby_files -> supersonic ? Sonic_console -> dream diary_dream journal(book)-as-dream team}

	// dal' modulo
	// dal' mu figa dulya
	// dal' DIVO - div 0 delit' nelzya ??? vzyali SINGOV u nego net ruki toi - pedofiliya uznaem
	// dal' Epstein DIVING
}


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

	num = _read(fd, buf, (unsigned int)nbytes);

	_lseek(fd, oldofs, SEEK_SET);

	return((int)num);
}