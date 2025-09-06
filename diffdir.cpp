/*	$OpenBSD: diffdir.c,v 1.45 2015/10/05 20:15:00 millert Exp $	*/

/*
 * Copyright (c) 2003, 2010 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Sponsored in part by the Defense Advanced Research Projects
 * Agency (DARPA) and Air Force Research Laboratory, Air Force
 * Materiel Command, USAF, under agreement number F39502-99-1-0512.
 */

#include <SDKDDKVer.h>
#define _WINSOCKAPI_
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <locale.h>
#include <time.h>
#include <filesystem>
#include <string.h>
#include <sys/types.h> // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-170

#include <sys/stat.h>
#define __unused [[maybe_unused]]
typedef uintptr_t __uintptr_t;
#define __typeof decltype
//#include <__msvc_int128.hpp>
//#include <charconv>
#include "sys_tree_c.h" // contrib/libevent/WIN32-Code/tree.h  or  contrib/ntp/sntp/libevent/WIN32-Code/tree.h
//#include "sys_tree_o.h" // newer crypto/openssh/openbsd-compat/sys-tree.h // ain't gonna miss __attribute__((__unused__)) static in msvc
//#include "sys_tree_f.h" //#include <sys/tree.h> // newest sys/sys/tree.h
//						// https://www.geeksforgeeks.org/cpp/red-black-tree-in-cpp/
						// https://seneca-ictoer.github.io/data-structures-and-algorithms/K-Augmented-Data-Structures/red-black
						// https://www.geeksforgeeks.org/dsa/introduction-to-red-black-tree/
						// https://lh3lh3.users.sourceforge.net/udb.shtml

//#include <dirent.h>
#include "err.h" //#include <err.h>
#include <errno.h>
#include <fcntl.h>
//#include <fnmatch.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>

#include "diff.h"

static int selectfile(const struct dirent *);
static void diffit(struct dirent *, char *, size_t, struct dirent *,
	char *, size_t, int);
static void print_only(const char *, size_t, const char *);

#define d_status	d_type		/* we need to store status for -l */

struct inode {
	uint64_t dev; //dev_t dev;
	uint64_t ino; //ino_t ino; // typedef unsigned short _ino_t; // inode number (unused on Windows) // types.h
	// https://stackoverflow.com/questions/7162164/does-windows-have-inode-numbers-like-linux
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-file_id_info
	// 128-bit file identifier for the file. The file identifier and the volume serial number uniquely identify a file on a single computer.
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-file_id_info
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getfileinformationbyhandleex
	// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileinformationbyhandle?redirectedfrom=MSDN
	// or https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileinformationbyhandle?redirectedfrom=MSDN
	// dwVolumeSerialNumber + file id 2 parts https://learn.microsoft.com/en-us/windows/win32/api/fileapi/ns-fileapi-by_handle_file_information
	//
	// https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-file_id_128
	// https://learn.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-file_id_info
	// via
	// https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ne-minwinbase-file_info_by_handle_class
	// https://stackoverflow.com/questions/7162164/does-windows-have-inode-numbers-like-linux
	//
	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/standard-types?view=msvc-170
	// https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-170
	// _open() https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/open-wopen?view=msvc-170

	RB_ENTRY(inode) entry;
	/*
	struct {
		struct inode* rbe_link[3];
	} entry;
	//*/
};

static int
inodecmp(struct inode *a, struct inode *b)
{
	return (a->dev < b->dev ? -1 : a->dev > b->dev ? 1 :
	    a->ino < b->ino ? -1 : a->ino > b->ino ? 1 : 0);
}

///*/
//struct inodetree {
//	struct inode* rbh_root; // * root of the tree * //
//};//*/
RB_HEAD(inodetree, inode);
/*struct inodetree {
	struct inode* rbh_root;
};//*/
static struct inodetree v1 = RB_INITIALIZER(&v1);
static struct inodetree v2 = RB_INITIALIZER(&v2);
/*static struct inodetree v1 = {0};
static struct inodetree v2 = {0};//*/
//RB_GENERATE_STATIC(inodetree, inode, entry, inodecmp); // Error C2102 '&' requires l-value // ? decltype(inode*) fail
RB_PROTOTYPE(inodetree, inode, entry, inodecmp); // RB_PROTOTYPE(event_tree, event, ev_timeout_node, compare);
RB_GENERATE(inodetree, inode, entry, inodecmp);  // RB_GENERATE(event_tree, event, ev_timeout_node, compare);
/* two last C6237 sys/sys/tree.h
[[maybe_unused]] static struct inode* inodetree_RB_INSERT_COLOR(struct inodetree* head, struct inode* parent, struct inode* elm) {
	struct inode* child, * child_up, * gpar;
	child = nullptr;
	uintptr_t elmdir, sibdir;
	do {
		gpar = (parent)->entry.rbe_link[0];
		elmdir = (parent)->entry.rbe_link[((uintptr_t)2)] == elm ? ((uintptr_t)2) : ((uintptr_t)1);
		if ((*(uintptr_t*)&gpar) & elmdir) {
			(*(uintptr_t*)&(parent)->entry.rbe_link[0]) ^= elmdir;
			return (0);
		}
		sibdir = elmdir ^ ((uintptr_t)3);
		(*(uintptr_t*)&(parent)->entry.rbe_link[0]) ^= sibdir;
		if (((*(uintptr_t*)&gpar) & ((uintptr_t)3)) == 0) {
			child = elm;
			elm = parent;
			continue;
		}
		(parent)->entry.rbe_link[0] = gpar = (decltype(gpar))((uintptr_t)gpar & ~((uintptr_t)3)); // _RB_UP(parent, field) = gpar = _RB_PTR(gpar);
		if ((nullptr != child) && ((*(uintptr_t*)&(elm)->entry.rbe_link[0]) & elmdir)) { // if (_RB_BITSUP(elm, field) & elmdir) {
			do {
				// RB_ROTATE(elm, child, elmdir, field);

				// #define RB_ROTATE(elm, tmp, dir, field) do {				\
				// if ((_RB_LINK(elm, dir ^ _RB_LR, field) = _RB_LINK(tmp, dir, field)) != NULL)				\
				//		RB_SET_PARENT(_RB_LINK(tmp, dir, field), elm, field);	\
				// _RB_LINK(tmp, dir, field) = (elm);				\
				// RB_SET_PARENT(elm, tmp, field);					\
				// } while (/*CONSTCOND* / 0)
// !
				if (((elm)->entry.rbe_link[elmdir ^ ((uintptr_t)3)] = (child)->entry.rbe_link[elmdir]) != 0)
					do {	// RB_SET_PARENT()
					(*(uintptr_t*)&((child)->entry.rbe_link[elmdir])->entry.rbe_link[0]) = (uintptr_t)elm | ((*(uintptr_t*)&((child)->entry.rbe_link[elmdir])->entry.rbe_link[0]) & ((uintptr_t)3));
				} while (0); // RB_SET_PARENT() end
				(child)->entry.rbe_link[elmdir] = (elm); // _RB_LINK(tmp, dir, field) = (elm);
				do {		// RB_SET_PARENT()
					(*(uintptr_t*)&(elm)->entry.rbe_link[0]) = (uintptr_t)child | ((*(uintptr_t*)&(elm)->entry.rbe_link[0]) & ((uintptr_t)3));
				} while (0); // RB_SET_PARENT() end
			} while (0); child_up = (child)->entry.rbe_link[0]; if ((*(uintptr_t*)&child_up) & sibdir) (*(uintptr_t*)&(parent)->entry.rbe_link[0]) ^= elmdir; if ((*(uintptr_t*)&child_up) & elmdir) (*(uintptr_t*)&(elm)->entry.rbe_link[0]) ^= ((uintptr_t)3); else (*(uintptr_t*)&(elm)->entry.rbe_link[0]) ^= elmdir; if (((*(uintptr_t*)&child_up) & ((uintptr_t)3)) == 0) elm = child;
		} else child = elm; do {
			if (((parent)->entry.rbe_link[sibdir ^ ((uintptr_t)3)] = (child)->entry.rbe_link[sibdir]) != 0) do {
				(*(uintptr_t*)&((child)->entry.rbe_link[sibdir])->entry.rbe_link[0]) = (uintptr_t)parent | ((*(uintptr_t*)&((child)->entry.rbe_link[sibdir])->entry.rbe_link[0]) & ((uintptr_t)3));
			} while (0); (child)->entry.rbe_link[sibdir] = (parent); do {
				(*(uintptr_t*)&(parent)->entry.rbe_link[0]) = (uintptr_t)child | ((*(uintptr_t*)&(parent)->entry.rbe_link[0]) & ((uintptr_t)3));
			} while (0);
		} while (0); (child)->entry.rbe_link[0] = gpar; do {
			if (gpar == 0) (head)->rbh_root = (child); else if ((parent) == (gpar)->entry.rbe_link[((uintptr_t)1)]) (gpar)->entry.rbe_link[((uintptr_t)1)] = (child); else (gpar)->entry.rbe_link[((uintptr_t)2)] = (child);
		} while (0); if (elm != child) (void)0; (void)0; return (child);
	} while ((parent = gpar) != 0);
	return (0);
};

[[maybe_unused]] static struct inode* inodetree_RB_REMOVE_COLOR(struct inodetree* head, struct inode* parent, struct inode* elm) {
	struct inode* gpar, * sib, * up;
	uintptr_t elmdir, sibdir;
	if ((parent)->entry.rbe_link[((uintptr_t)2)] == elm && (parent)->entry.rbe_link[((uintptr_t)1)] == elm) {
		//(parent)->entry.rbe_link[0] = (decltype((parent)->entry.rbe_link[0]))	((uintptr_t)(parent)->entry.rbe_link[0] & (~((uintptr_t)3)));
		//(parent)->entry.rbe_link[0] = (inode*) ((uintptr_t)((parent)->entry.rbe_link[0]) & ~(uintptr_t)3);
		(parent)->entry.rbe_link[0] = (inode*) ((uintptr_t)((parent)->entry.rbe_link[0]) & ~(uintptr_t)3);
		elm = parent;
		if ((parent = (elm)->entry.rbe_link[0]) == 0) return (0);
	} do {
		gpar = (parent)->entry.rbe_link[0]; elmdir = (parent)->entry.rbe_link[((uintptr_t)2)] == elm ? ((uintptr_t)2) : ((uintptr_t)1); (*(uintptr_t*)&gpar) ^= elmdir; if ((*(uintptr_t*)&gpar) & elmdir) {
			(parent)->entry.rbe_link[0] = gpar; return (0);
		} if ((*(uintptr_t*)&gpar) & ((uintptr_t)3)) {
			(*(uintptr_t*)&gpar) ^= ((uintptr_t)3); (parent)->entry.rbe_link[0] = gpar; gpar = (decltype(gpar))((uintptr_t)gpar & ~((uintptr_t)3)); continue;
		} sibdir = elmdir ^ ((uintptr_t)3); sib = (parent)->entry.rbe_link[sibdir]; up = (sib)->entry.rbe_link[0]; (*(uintptr_t*)&up) ^= ((uintptr_t)3); if (((*(uintptr_t*)&up) & ((uintptr_t)3)) == 0) {
			(sib)->entry.rbe_link[0] = up; continue;
		} if (((*(uintptr_t*)&up) & sibdir) == 0) {
			elm = (sib)->entry.rbe_link[elmdir]; do {
				if (((sib)->entry.rbe_link[sibdir ^ ((uintptr_t)3)] = (elm)->entry.rbe_link[sibdir]) != 0) do {
					(*(uintptr_t*)&((elm)->entry.rbe_link[sibdir])->entry.rbe_link[0]) = (uintptr_t)sib | ((*(uintptr_t*)&((elm)->entry.rbe_link[sibdir])->entry.rbe_link[0]) & ((uintptr_t)3));
				} while (0); (elm)->entry.rbe_link[sibdir] = (sib); do {
					(*(uintptr_t*)&(sib)->entry.rbe_link[0]) = (uintptr_t)elm | ((*(uintptr_t*)&(sib)->entry.rbe_link[0]) & ((uintptr_t)3));
				} while (0);
			} while (0); up = (elm)->entry.rbe_link[0]; (*(uintptr_t*)&(parent)->entry.rbe_link[0]) ^= ((*(uintptr_t*)&up) & elmdir) ? ((uintptr_t)3) : elmdir; (*(uintptr_t*)&(sib)->entry.rbe_link[0]) ^= ((*(uintptr_t*)&up) & sibdir) ? ((uintptr_t)3) : sibdir; (*(uintptr_t*)&(elm)->entry.rbe_link[0]) |= ((uintptr_t)3);
		} else {
			if (((*(uintptr_t*)&up) & elmdir) == 0 && 0 && elm != 0) {
				(*(uintptr_t*)&(parent)->entry.rbe_link[0]) ^= sibdir; (*(uintptr_t*)&(sib)->entry.rbe_link[0]) ^= ((uintptr_t)3);
			} else if (((*(uintptr_t*)&up) & elmdir) == 0) {
				(*(uintptr_t*)&(parent)->entry.rbe_link[0]) ^= elmdir; (*(uintptr_t*)&(sib)->entry.rbe_link[0]) ^= sibdir;
			} else (*(uintptr_t*)&(sib)->entry.rbe_link[0]) ^= sibdir; elm = sib;
		} do {
			if (((parent)->entry.rbe_link[elmdir ^ ((uintptr_t)3)] = (elm)->entry.rbe_link[elmdir]) != 0) do {
				(*(uintptr_t*)&((elm)->entry.rbe_link[elmdir])->entry.rbe_link[0]) = (uintptr_t)parent | ((*(uintptr_t*)&((elm)->entry.rbe_link[elmdir])->entry.rbe_link[0]) & ((uintptr_t)3));
			} while (0); (elm)->entry.rbe_link[elmdir] = (parent); do {
				(*(uintptr_t*)&(parent)->entry.rbe_link[0]) = (uintptr_t)elm | ((*(uintptr_t*)&(parent)->entry.rbe_link[0]) & ((uintptr_t)3));
			} while (0);
		} while (0); do {
			(*(uintptr_t*)&(elm)->entry.rbe_link[0]) = (uintptr_t)gpar | ((*(uintptr_t*)&(elm)->entry.rbe_link[0]) & ((uintptr_t)3));
		} while (0); do {
			if (gpar == 0) (head)->rbh_root = (elm); else if ((parent) == (gpar)->entry.rbe_link[((uintptr_t)1)]) (gpar)->entry.rbe_link[((uintptr_t)1)] = (elm); else (gpar)->entry.rbe_link[((uintptr_t)2)] = (elm);
		} while (0); if (sib != elm) (void)0; return (parent);
	} while (elm = parent, (parent = gpar) != 0); return (0);
};

[[maybe_unused]] static struct inode* inodetree_RB_INSERT_FINISH(struct inodetree* head, struct inode* parent, struct inode** pptr, struct inode* elm) {
	struct inode* tmp = 0; do {
		(elm)->entry.rbe_link[0] = parent;
		(elm)->entry.rbe_link[((uintptr_t)1)] = (elm)->entry.rbe_link[((uintptr_t)2)] = 0;
	} while (0);
	*pptr = elm;
	if (parent != 0) tmp = inodetree_RB_INSERT_COLOR(head, parent, elm);
	do {
		if (tmp == elm) tmp = 0;
	//} while (0 && (elm = (decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3))) != 0); if (tmp != 0) (void)0; return (0);
	//} while (0 && (elm = (inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3))) != 0); // C6237: (<zero> && <expression>) is always zero // !
	} while (0 && (elm = (inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3))) != 0);
	if (tmp != 0)
		(void)0; // (void)RB_AUGMENT_CHECK(tmp); // !
	return (0);
};

[[maybe_unused]] static struct inode* inodetree_RB_INSERT(struct inodetree* head, struct inode* elm) {
	struct inode* tmp;
	struct inode** tmpp = &(head)->rbh_root;
	struct inode* parent = 0;
	while ((tmp = *tmpp) != 0) {
		parent = tmp;
		decltype(inodecmp(0, 0)) comp = (inodecmp)(elm, parent);
		if (comp < 0)
			tmpp = &(parent)->entry.rbe_link[((uintptr_t)1)];
		else if (comp > 0)
			tmpp = &(parent)->entry.rbe_link[((uintptr_t)2)]; else return (parent);
	} return (inodetree_RB_INSERT_FINISH(head, parent, tmpp, elm));
};

[[maybe_unused]] static struct inode* inodetree_RB_REMOVE(struct inodetree* head, struct inode* out) {
	struct inode* child, * in, * opar, * parent; child = (out)->entry.rbe_link[((uintptr_t)1)]; in = (out)->entry.rbe_link[((uintptr_t)2)]; opar = (out)->entry.rbe_link[0]; if (in == 0 || child == 0) {
		in = child = (in == 0 ? child : in); parent = opar = (decltype(opar))((uintptr_t)opar & ~((uintptr_t)3));
	} else {
		parent = in; while ((in)->entry.rbe_link[((uintptr_t)1)]) in = (in)->entry.rbe_link[((uintptr_t)1)]; do {
			(*(uintptr_t*)&(child)->entry.rbe_link[0]) = (uintptr_t)in | ((*(uintptr_t*)&(child)->entry.rbe_link[0]) & ((uintptr_t)3));
		} while (0); (in)->entry.rbe_link[((uintptr_t)1)] = child; child = (in)->entry.rbe_link[((uintptr_t)2)]; if (parent != in) {
			do {
				(*(uintptr_t*)&(parent)->entry.rbe_link[0]) = (uintptr_t)in | ((*(uintptr_t*)&(parent)->entry.rbe_link[0]) & ((uintptr_t)3));
			//} while (0); (in)->entry.rbe_link[((uintptr_t)2)] = parent; parent = (decltype((in)->entry.rbe_link[0]))((uintptr_t)(in)->entry.rbe_link[0] & ~((uintptr_t)3)); (parent)->entry.rbe_link[((uintptr_t)1)] = child;
			} while (0); (in)->entry.rbe_link[((uintptr_t)2)] = parent; parent = (inode*)((uintptr_t)(in)->entry.rbe_link[0] & ~((uintptr_t)3)); (parent)->entry.rbe_link[((uintptr_t)1)] = child;
		} (in)->entry.rbe_link[0] = opar; opar = (decltype(opar))((uintptr_t)opar & ~((uintptr_t)3));
	} do {
		if (opar == 0) (head)->rbh_root = (in); else if ((out) == (opar)->entry.rbe_link[((uintptr_t)1)]) (opar)->entry.rbe_link[((uintptr_t)1)] = (in); else (opar)->entry.rbe_link[((uintptr_t)2)] = (in);
	} while (0); if (child != 0) (child)->entry.rbe_link[0] = parent; if (parent != 0) {
		opar = inodetree_RB_REMOVE_COLOR(head, parent, child); if (parent == in && (parent)->entry.rbe_link[((uintptr_t)1)] == 0) {
			//opar = 0; parent = (decltype((parent)->entry.rbe_link[0]))((uintptr_t)(parent)->entry.rbe_link[0] & ~((uintptr_t)3));
			opar = 0; parent = (inode*)((uintptr_t)(parent)->entry.rbe_link[0] & ~((uintptr_t)3));
		} do {
			if (opar == parent) opar = 0;
		//} while (0 && (parent = (decltype((parent)->entry.rbe_link[0]))((uintptr_t)(parent)->entry.rbe_link[0] & ~((uintptr_t)3))) != 0);
		//} while (0 && (parent = (inode*)((uintptr_t)(parent)->entry.rbe_link[0] & ~((uintptr_t)3))) != 0); { // C6237: (<zero> && <expression>) is always zero // !
		} while (0 && (parent = (inode*)((uintptr_t)(parent)->entry.rbe_link[0] & ~((uintptr_t)3))) != 0);
		if (opar != 0) {
			(void)0; (void)0;
		}
	} return (out);
};

[[maybe_unused]] static struct inode* inodetree_RB_FIND(struct inodetree* head, struct inode* elm) {
	struct inode* tmp = (head)->rbh_root;
	decltype(inodecmp(0, 0)) comp;
	while (tmp) {
		comp = inodecmp(elm, tmp);
		if (comp < 0)
			tmp = (tmp)->entry.rbe_link[((uintptr_t)1)];
		else if (comp > 0)
			tmp = (tmp)->entry.rbe_link[((uintptr_t)2)];
		else
			return (tmp);
	} return (0);
};

[[maybe_unused]] static struct inode* inodetree_RB_NFIND(struct inodetree* head, struct inode* elm) {
	struct inode* tmp = (head)->rbh_root; struct inode* res = 0; decltype(inodecmp(0, 0)) comp; while (tmp) {
		comp = inodecmp(elm, tmp); if (comp < 0) {
			res = tmp; tmp = (tmp)->entry.rbe_link[((uintptr_t)1)];
		} else if (comp > 0) tmp = (tmp)->entry.rbe_link[((uintptr_t)2)]; else return (tmp);
	} return (res);
};

[[maybe_unused]] static struct inode* inodetree_RB_NEXT(struct inode* elm) {
	if ((elm)->entry.rbe_link[((uintptr_t)2)]) {
		elm = (elm)->entry.rbe_link[((uintptr_t)2)]; while ((elm)->entry.rbe_link[((uintptr_t)1)]) elm = (elm)->entry.rbe_link[((uintptr_t)1)];
	} else {
		//while ((decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)) && (elm == ((decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)))->entry.rbe_link[((uintptr_t)2)])) elm = (decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)); elm = (decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3));
		while ((inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)) && (elm == ((inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)))->entry.rbe_link[((uintptr_t)2)])) elm = (inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)); elm = (inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3));
	} return (elm);
};

[[maybe_unused]] static struct inode* inodetree_RB_INSERT_NEXT(struct inodetree* head, struct inode* elm, struct inode* next) {
	struct inode* tmp; struct inode** tmpp = &(elm)->entry.rbe_link[((uintptr_t)2)]; do {
	} while (0); if (inodetree_RB_NEXT(elm) != 0) do {
	} while (0); while ((tmp = *tmpp) != 0) {
		elm = tmp; tmpp = &(elm)->entry.rbe_link[((uintptr_t)1)];
	} return (inodetree_RB_INSERT_FINISH(head, elm, tmpp, next));
};

[[maybe_unused]] static struct inode* inodetree_RB_PREV(struct inode* elm) {
	if ((elm)->entry.rbe_link[((uintptr_t)1)]) {
		elm = (elm)->entry.rbe_link[((uintptr_t)1)]; while ((elm)->entry.rbe_link[((uintptr_t)2)]) elm = (elm)->entry.rbe_link[((uintptr_t)2)];
	} else {
		//while ((decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)) && (elm == ((decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)))->entry.rbe_link[((uintptr_t)1)])) elm = (decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)); elm = (decltype((elm)->entry.rbe_link[0]))((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3));
		while ((inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)) && (elm == ((inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)))->entry.rbe_link[((uintptr_t)1)])) elm = (inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3)); elm = (inode*)((uintptr_t)(elm)->entry.rbe_link[0] & ~((uintptr_t)3));
	} return (elm);
};

[[maybe_unused]] static struct inode* inodetree_RB_INSERT_PREV(struct inodetree* head, struct inode* elm, struct inode* prev) {
	struct inode* tmp; struct inode** tmpp = &(elm)->entry.rbe_link[((uintptr_t)1)]; do {
	} while (0); if (inodetree_RB_PREV(elm) != 0) do {
	} while (0); while ((tmp = *tmpp) != 0) {
		elm = tmp; tmpp = &(elm)->entry.rbe_link[((uintptr_t)2)];
	} return (inodetree_RB_INSERT_FINISH(head, elm, tmpp, prev));
};

[[maybe_unused]] static struct inode* inodetree_RB_MINMAX(struct inodetree* head, int val) {
	struct inode* tmp = (head)->rbh_root; struct inode* parent = 0; while (tmp) {
		parent = tmp; if (val < 0) tmp = (tmp)->entry.rbe_link[((uintptr_t)1)]; else tmp = (tmp)->entry.rbe_link[((uintptr_t)2)];
	} return (parent);
};

[[maybe_unused]] static struct inode* inodetree_RB_REINSERT(struct inodetree* head, struct inode* elm) {
	struct inode* cmpelm; if (((cmpelm = inodetree_RB_PREV(elm)) != 0 && inodecmp(cmpelm, elm) >= 0) || ((cmpelm = inodetree_RB_NEXT(elm)) != 0 && inodecmp(elm, cmpelm) >= 0)) {
		inodetree_RB_REMOVE(head, elm); return (inodetree_RB_INSERT(head, elm));
	} return (0);
};
//*/

static int
vscandir(struct inodetree* tree, const char* path, struct dirent*** dirp,
		 int (*selectf)(const struct dirent*),
		 int (*comparf)(const struct dirent**, const struct dirent**)) {
	//struct stat sb;		// inode, and therefore st_ino, has no meaning in the FAT, HPFS, or NTFS file systems
	//struct _stat64i32 sb; // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/stat-functions?view=msvc-170
	struct inode* ino = NULL;
	int fd = -1, ret, serrno;
	HANDLE fh = INVALID_HANDLE_VALUE;
	// HANDLE = _get_osfhandle(fd); // <io.h> // Never call CloseHandle on the return value of this function.
	// fd = _open_osfhandle(HANDLE, _O_RDONLY); // _close() Don't call the Win32 function CloseHandle on the original handle.
	// DuplicateHandle()
	// FILE * // fd = fileno(FILE *stream);

	FILE_ID_INFO fi;
	uint64_t ih = 0, il = 0;
	BYTE* ihp, * ilp;
	std::filesystem::directory_options::skip_permission_denied;
	std::filesystem::perm_options::nofollow;
	// std::filesystem no volume_serial no inode_file_id https://en.cppreference.com/w/cpp/filesystem/directory_entry/status.html
	std::filesystem::path l_path = std::filesystem::current_path() / path;

	if (std::filesystem::is_directory(l_path)) {
		//wprintf(L"_open(\"%s\", O_DIRECTORY) + fstat()...\n", l_path.c_str());
		ino = (inode*)calloc(1, sizeof(*ino)); // 40 40 printf("calloc %zd %zd\n", sizeof(*ino), sizeof(inode));
		if (NULL == ino)
			goto fail;
		fh = CreateFileW(l_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS), NULL);
		if (INVALID_HANDLE_VALUE == fh) {
			printf("open handle err 0x08%lx\n", GetLastError());
			goto fail;
		}
		GetFileInformationByHandleEx(fh, FILE_INFO_BY_HANDLE_CLASS{FileIdInfo}, &fi, sizeof(fi));
		//CloseHandle(fh);
		fd = _open_osfhandle(reinterpret_cast<intptr_t>(fh), _O_RDONLY); // (intptr_t) // windows driver samples
		if (fd == -1) goto fail;

		//for (size_t idx = 0; idx < 8; idx++) {
		for (size_t idx = 7; (idx < 8) && (idx >= 0); idx--) {
			ilp = (BYTE*) &il+idx;
			*ilp = fi.FileId.Identifier[idx];

			ihp = (BYTE*) &ih+idx;
			*ihp = fi.FileId.Identifier[idx+8];
		}
		//printf("volume_dev 0x%016zx, id 0x%016zx %016zx %zx\n", fi.VolumeSerialNumber, ih, il, i); // ok == fsutil file queryFileID dd
	} else {
		goto fail;
	}
	/*if ((fd = _open(path, O_DIRECTORY | O_RDONLY)) < 0 ||
	    (ino = (inode *)calloc(1, sizeof(*ino))) == NULL ||
	    fstat(fd, &sb) != 0)
		goto fail;//*/
	ino->dev = fi.VolumeSerialNumber;
	ino->ino = il; // !
	if (RB_FIND(inodetree, tree, ino)) {
	//if (inodetree_RB_FIND(tree, ino)) {
		free(ino);
		_close(fd);
		warnx("%s: Directory loop detected", path);
		*dirp = NULL;
		return (0);
	}
	if ((ret = fdscandir(l_path.c_str(), dirp, selectf, comparf)) < 0) //if ((ret = fdscandir(fd, dirp, selectf, comparf)) < 0)
		goto fail;

	//printf("fdscandir_end\n");

	RB_INSERT(inodetree, tree, ino);
	//inodetree_RB_INSERT(tree, ino);
	/*if (RB_FIND(inodetree, tree, ino)) {
	//if (inodetree_RB_FIND(tree, ino)) {
		printf("insert\n");
	}//*/
	if (fd >= 0) _close(fd);
	//printf("_end, %d\n", ret);
	return (ret);

fail:
	serrno = errno;
	if (ino != NULL)
		free(ino);
	if (fd >= 0)
		_close(fd);
	errno = serrno;
	return (-1);
}

/*
 * Diff directory traversal. Will be called recursively if -r was specified.
 */
void
diffdir(char *p1, char *p2, int flags)
{
	struct dirent *dent1, **dp1, **edp1, **dirp1 = NULL;
	struct dirent *dent2, **dp2, **edp2, **dirp2 = NULL;
	size_t dirlen1, dirlen2;
	char path1[PATH_MAX], path2[PATH_MAX];
	int pos;

	edp1 = edp2 = NULL;

	dent1 = dent2 = nullptr;
	dp1 = edp1 = dirp1 = dp2 = edp2 = dirp2 = nullptr;
	dirlen1 = dirlen2 = 0;
	memset(path1, 0, PATH_MAX);
	memset(path2, 0, PATH_MAX);

	dirlen1 = strlcpy(path1, *p1 ? p1 : ".", sizeof(path1));
	if (dirlen1 >= sizeof(path1) - 1) {
		warnc(ENAMETOOLONG, "%s", p1);
		status |= 2;
		return;
	}
	while (dirlen1 > 1 && path1[dirlen1 - 1] == '/')
		path1[--dirlen1] = '\0';
	dirlen2 = strlcpy(path2, *p2 ? p2 : ".", sizeof(path2));
	if (dirlen2 >= sizeof(path2) - 1) {
		warnc(ENAMETOOLONG, "%s", p2);
		status |= 2;
		return;
	}
	while (dirlen2 > 1 && path2[dirlen2 - 1] == '/')
		path2[--dirlen2] = '\0';

	/*
	 * Get a list of entries in each directory, skipping "excluded" files
	 * and sorting alphabetically.
	 */
	pos = vscandir(&v1, path1, &dirp1, selectfile, alphasort);
	if (pos == -1) {
		if (errno == ENOENT && (Nflag || Pflag)) {
			pos = 0;
		} else {
			warn("%s", path1);
			goto closem;
		}
	}
	dp1 = dirp1;
	edp1 = dirp1 + pos;

	pos = vscandir(&v2, path2, &dirp2, selectfile, alphasort);
	if (pos == -1) {
		if (errno == ENOENT && Nflag) {
			pos = 0;
		} else {
			warn("%s", path2);
			goto closem;
		}
	}
	dp2 = dirp2;
	edp2 = dirp2 + pos;

	/*
	 * If we were given a starting point, find it.
	 */
	if (start != NULL) {
		while (dp1 != edp1 && strcmp((*dp1)->d_name, start) < 0)
			dp1++;
		while (dp2 != edp2 && strcmp((*dp2)->d_name, start) < 0)
			dp2++;
	}

	/*
	 * Append separator so children's names can be appended directly.
	 */
	if (path1[dirlen1 - 1] != '/') {
		path1[dirlen1++] = '\\'; //'/';
		path1[dirlen1] = '\0';
	}
	if (path2[dirlen2 - 1] != '/') {
		path2[dirlen2++] = '\\'; // ' / ';
		path2[dirlen2] = '\0';
	}

	/*
	 * Iterate through the two directory lists, diffing as we go.
	 */
	while (dp1 != edp1 || dp2 != edp2) {
		dent1 = dp1 != edp1 ? *dp1 : NULL;
		dent2 = dp2 != edp2 ? *dp2 : NULL;

		pos = dent1 == NULL ? 1 : dent2 == NULL ? -1 :
		    ignore_file_case ? strcasecmp(dent1->d_name, dent2->d_name) :
		    strcmp(dent1->d_name, dent2->d_name) ;
		if (pos == 0) { // no nullptrs && equal names
			/* file exists in both dirs, diff it */
			diffit(dent1, path1, dirlen1, dent2, path2, dirlen2, flags);
			dp1++;
			dp2++;
		} else if (pos < 0) { // dent2==nullptr or value of strcasecmp() strcmp()
			if (dent1 == nullptr) // assert (never)
				err(660, "dent1 == nullptr\n");
			/* file only in first dir, only diff if -N */
			if (Nflag) {
				diffit(dent1, path1, dirlen1, dent2, path2,
					dirlen2, flags);
			} else {
				print_only(path1, dirlen1, dent1->d_name);
				status |= 1;
			}
			dp1++;
		} else { // pos==1, when dent1==nullptr or value of strcasecmp() strcmp()
			if (dent2 == nullptr) // assert (never)
				err(661,"dent2 == nullptr\n");
			/* file only in second dir, only diff if -N or -P */
			if (Nflag || Pflag)
				diffit(dent2, path1, dirlen1, dent1, path2,
					dirlen2, flags);
			else {
				print_only(path2, dirlen2, dent2->d_name);
				status |= 1;
			}
			dp2++;
		}
	}

closem:
	if (dirp1 != NULL) {
		for (dp1 = dirp1; dp1 < edp1; dp1++)
			free(*dp1);
		free(dirp1);
	}
	if (dirp2 != NULL) {
		for (dp2 = dirp2; dp2 < edp2; dp2++)
			free(*dp2);
		free(dirp2);
	}
}

/*
 * Do the actual diff by calling either diffreg() or diffdir().
 */
static void
diffit(struct dirent *dp, char *path1, size_t plen1, struct dirent *dp2,
	char *path2, size_t plen2, int flags)
{
	flags |= D_HEADER;

	if ((dp == nullptr) && (dp2 == nullptr))
		err(662, "diffit() both dp nullptr -> no filename"); // assert never

#pragma warning(disable : 6011) // dp ain't nullptr, diffdir()
	// called with nullptr from diffdir() due fdscandir()
	// ??? supposed to return nullptr when folder empty (numitems == 0)
	if (dp2 == nullptr) {
		strlcpy(path1 + plen1, dp->d_name, PATH_MAX - plen1); // no C6011
		strlcpy(path2 + plen2, dp->d_name, PATH_MAX - plen2);
	} else if (dp == nullptr) { // never, diffdir()
		strlcpy(path1 + plen1, dp2->d_name, PATH_MAX - plen1);
		strlcpy(path2 + plen2, dp2->d_name, PATH_MAX - plen2);
	} else {
		strlcpy(path1 + plen1, dp->d_name, PATH_MAX - plen1);
		// f() should never compare files with different names from diffdir()
		// dp always correct name via diffdir()
		// not sure what happen in diffdir() when ignore_file_case==true
		strlcpy(path2 + plen2, dp->d_name, PATH_MAX - plen2);
		// but ??? strcasecmp() copy-paste error/ai code or
#ifdef DEBUG
		if (ignore_file_case ? (strcasecmp(dp->d_name, dp2->d_name) != 0) : (strcmp(dp->d_name, dp2->d_name) != 0))
			warn("diffit() case != 0: different filenames to compare inside diffdir() automation: %s, %s\n", dp->d_name, dp2->d_name);
#endif
	}

	/*
	when dp2==nullptr strcasecmp() still compare dp2->d_name
	compare if {false=0, true} && {-1, 0, 1} == 0 where is freebsd such code style, when caller already compared names? ai code? copy-paste?
	                  0        && equal=0
					            1            == 0
                                             false
                      0        && different=1
					            0            == 0
								             true, so possible crashed inside strlcpy(,nullptr->d_name), different filenames
											          but most likely strcasecmp

	/*
	 * If we are ignoring file case, use dent2s name here if both names are
	 * the same apart from case.
	 * /
	if (ignore_file_case && strcasecmp(dp2->d_name, dp2->d_name) == 0)
		strlcpy(path2 + plen2, dp2->d_name, PATH_MAX - plen2);
	else // always else or else when not crash, combined with perfect if() above
		strlcpy(path2 + plen2, dp->d_name, PATH_MAX - plen2);

	// strcasecmp(dp2->d_name, dp2->d_name) compare nothing == 0 ---> if ((ignore_file_case && 0) == 0) ---> if (ignore_file_case) ??? ((false && false) == 0) === !(false && false)
	// now if case ignored there no strcmp(dp2->d_name, dp2->d_name)
	// https://github.com/freebsd/freebsd-src/commit/3931c072c63d5a51d83157009e7f7510e08da80c#r164580724

	//*/
	if (noderef) { // !
		if (lstat(path1, &stb1) != 0) {
			if (!(Nflag || Pflag) || errno != ENOENT) {
				warn("%s", path1);
				return;
			}
			flags |= D_EMPTY1;
			memset(&stb1, 0, sizeof(stb1));
		}

		if (lstat(path2, &stb2) != 0) {
			if (!Nflag || errno != ENOENT) {
				warn("%s", path2);
				return;
			}
			flags |= D_EMPTY2;
			memset(&stb2, 0, sizeof(stb2));
			stb2.st_mode = stb1.st_mode;
		}
		if (stb1.st_mode == 0)
			stb1.st_mode = stb2.st_mode;
		if (S_ISLNK(stb1.st_mode) || S_ISLNK(stb2.st_mode)) {
			if  (S_ISLNK(stb1.st_mode) && S_ISLNK(stb2.st_mode)) {
				char buf1[PATH_MAX];
				char buf2[PATH_MAX];
				ssize_t len1 = 0;
				ssize_t len2 = 0;

				len1 = readlink(path1, buf1, sizeof(buf1));
				len2 = readlink(path2, buf2, sizeof(buf2));

				if (len1 < 0 || len2 < 0) {
					perror("reading links");
					return;
				}
				buf1[len1] = '\0';
				buf2[len2] = '\0';

				if (len1 != len2 || strncmp(buf1, buf2, len1) != 0) {
					printf("Symbolic links %s and %s differ\n",
						path1, path2);
					status |= 1;
				}

				return;
			}

			printf("File %s is a %s while file %s is a %s\n",
				path1, S_ISLNK(stb1.st_mode) ? "symbolic link" :
					(S_ISDIR(stb1.st_mode) ? "directory" :
					(S_ISREG(stb1.st_mode) ? "file" : "error")),
				path2, S_ISLNK(stb2.st_mode) ? "symbolic link" :
					(S_ISDIR(stb2.st_mode) ? "directory" :
					(S_ISREG(stb2.st_mode) ? "file" : "error")));
			status |= 1;
			return;
		}
	} else {
		if (stat(path1, &stb1) != 0) {
			if (!(Nflag || Pflag) || errno != ENOENT) {
				warn("%s", path1);
				return;
			}
			flags |= D_EMPTY1;
			memset(&stb1, 0, sizeof(stb1));
		}

		if (stat(path2, &stb2) != 0) {
			if (!Nflag || errno != ENOENT) {
				warn("%s", path2);
				return;
			}
			flags |= D_EMPTY2;
			memset(&stb2, 0, sizeof(stb2));
			stb2.st_mode = stb1.st_mode;
		}
		if (stb1.st_mode == 0)
			stb1.st_mode = stb2.st_mode;
	}
	if (S_ISDIR(stb1.st_mode) && S_ISDIR(stb2.st_mode)) {
		if (rflag)
			diffdir(path1, path2, flags);
		else
			printf("Common subdirectories: %s and %s\n",
			    path1, path2);
		return;
	}
	if (!S_ISREG(stb1.st_mode) && !S_ISDIR(stb1.st_mode))
		dp->d_status = D_SKIPPED1;
	else if (!S_ISREG(stb2.st_mode) && !S_ISDIR(stb2.st_mode))
		dp->d_status = D_SKIPPED2;
	else
		dp->d_status = diffreg(path1, path2, flags, 0);
	print_status(dp->d_status, path1, path2, "");
#pragma warning(default : 6011)
}

/*
 * Returns 1 if the directory entry should be included in the
 * diff, else 0.  Checks the excludes list.
 */
static int
selectfile(const struct dirent *dp)
{
	struct excludes *excl;

	if (dp->d_fileno == 0)
		return (0);

	/* always skip "." and ".." */
	if (dp->d_name[0] == '.' && (dp->d_name[1] == '\0' ||
	    (dp->d_name[1] == '.' && dp->d_name[2] == '\0')))
		return (0);

	/* check excludes list */
	for (excl = excludes_list; excl != NULL; excl = excl->next)
		if (fnmatch(excl->pattern, dp->d_name, FNM_PATHNAME) == 0)
			return (0);

	return (1);
}

void
print_only(const char *path, size_t dirlen, const char *entry)
{
	if (dirlen > 1)
		dirlen--;
	printf("Only in %.*s: %s\n", (int)dirlen, path, entry);
}
