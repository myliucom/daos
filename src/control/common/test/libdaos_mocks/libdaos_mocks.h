/* Code generated by cmd/cgo; DO NOT EDIT. */

/* package github.com/daos-stack/daos/src/control/common/test/libdaos_mocks */


#line 1 "cgo-builtin-export-prolog"

#include <stddef.h> /* for ptrdiff_t below */

#ifndef GO_CGO_EXPORT_PROLOGUE_H
#define GO_CGO_EXPORT_PROLOGUE_H

#ifndef GO_CGO_GOSTRING_TYPEDEF
typedef struct { const char *p; ptrdiff_t n; } _GoString_;
#endif

#endif

/* Start of preamble from import "C" comments.  */


#line 20 "libdaos_mocks.go"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif




#include <daos.h>

#include "daos_hdlr.h"

// typedefs to work around cgo's limitations
typedef const char const_char;
typedef char const *const * char_const_in_arr;
typedef void *const * void_const_out_arr;
typedef void const *const * void_const_in_arr;
typedef size_t const * size_t_in_arr;
typedef size_t * size_t_out_arr;

#line 1 "cgo-generated-wrapper"


/* End of preamble from import "C" comments.  */


/* Start of boilerplate cgo prologue.  */
#line 1 "cgo-gcc-export-header-prolog"

#ifndef GO_CGO_PROLOGUE_H
#define GO_CGO_PROLOGUE_H

typedef signed char GoInt8;
typedef unsigned char GoUint8;
typedef short GoInt16;
typedef unsigned short GoUint16;
typedef int GoInt32;
typedef unsigned int GoUint32;
typedef long long GoInt64;
typedef unsigned long long GoUint64;
typedef GoInt64 GoInt;
typedef GoUint64 GoUint;
typedef __SIZE_TYPE__ GoUintptr;
typedef float GoFloat32;
typedef double GoFloat64;
typedef float _Complex GoComplex64;
typedef double _Complex GoComplex128;

/*
  static assertion to make sure the file is being used on architecture
  at least with matching size of GoInt.
*/
typedef char _check_for_64_bit_pointer_matching_GoInt[sizeof(void*)==64/8 ? 1:-1];

#ifndef GO_CGO_GOSTRING_TYPEDEF
typedef _GoString_ GoString;
#endif
typedef void *GoMap;
typedef void *GoChan;
typedef struct { void *t; void *v; } GoInterface;
typedef struct { void *data; GoInt len; GoInt cap; } GoSlice;

#endif

/* End of boilerplate cgo prologue.  */

#ifdef __cplusplus
extern "C" {
#endif

extern int daos_init();
extern int daos_fini();
extern int daos_debug_init(char* p0);
extern void daos_debug_fini();
extern int daos_pool_connect2(const_char* pool, const_char* sys, unsigned int flags, daos_handle_t* poh, daos_pool_info_t* info, daos_event_t* ev);
extern int daos_pool_disconnect(daos_handle_t poh, daos_event_t* ev);
extern int daos_pool_query(daos_handle_t coh, d_rank_list_t** ranks, daos_pool_info_t* info, daos_prop_t* props, daos_event_t* ev);
extern int pool_autotest_hdlr(struct cmd_args_s* ap);
extern int duns_destroy_path(daos_handle_t poh, const_char* path);
extern int fs_dfs_get_attr_hdlr(struct cmd_args_s* ap, dfs_obj_info_t* attrs);
extern int fs_dfs_hdlr(struct cmd_args_s* ap);
extern int fs_dfs_resolve_pool(struct cmd_args_s* ap);
extern int fs_dfs_resolve_path(struct cmd_args_s* ap);
extern int cont_create_hdlr(struct cmd_args_s* ap);
extern int cont_create_uns_hdlr(struct cmd_args_s* ap);
extern int daos_cont_open2(daos_handle_t poh, const_char* cont, unsigned int flags, daos_handle_t* coh, daos_cont_info_t* info, daos_event_t* ev);
extern int daos_cont_close(daos_handle_t coh, daos_event_t* ev);
extern int daos_cont_query(daos_handle_t coh, daos_cont_info_t* info, daos_prop_t* props, daos_event_t* ev);
extern int daos_cont_set_prop(daos_handle_t coh, daos_prop_t* props, daos_event_t* ev);
extern int daos_cont_get_acl(daos_handle_t coh, daos_prop_t** acl, daos_event_t* ev);
extern int daos_cont_destroy2(daos_handle_t coh, const_char* cont, int force, daos_event_t* ev);
extern int daos_pool_list_attr(daos_handle_t poh, char* buf, size_t* size, daos_event_t* ev);
extern int daos_pool_get_attr(daos_handle_t poh, int n, char_const_in_arr names, void_const_out_arr buffers, size_t_out_arr sizes, daos_event_t* ev);
extern int daos_pool_del_attr(daos_handle_t poh, int n, char_const_in_arr names, daos_event_t* ev);
extern int daos_pool_set_attr(daos_handle_t poh, int n, char_const_in_arr names, void_const_in_arr values, size_t_in_arr sizes, daos_event_t* ev);
extern int daos_cont_list_attr(daos_handle_t poh, char* buf, size_t* size, daos_event_t* ev);
extern int daos_cont_get_attr(daos_handle_t poh, int n, char_const_in_arr names, void_const_out_arr buffers, size_t_out_arr sizes, daos_event_t* ev);
extern int daos_cont_del_attr(daos_handle_t poh, int n, char_const_in_arr names, daos_event_t* ev);
extern int daos_cont_set_attr(daos_handle_t poh, int n, char_const_in_arr names, void_const_in_arr values, size_t_in_arr sizes, daos_event_t* ev);

#ifdef __cplusplus
}
#endif
