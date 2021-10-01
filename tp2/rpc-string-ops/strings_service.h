/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _STRINGS_SERVICE_H_RPCGEN
#define _STRINGS_SERVICE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define STRINGS_SERVICE 0x20000001
#define STRINGS_SERVICE_VER 1

#if defined(__STDC__) || defined(__cplusplus)
#define NUM_CHAR_S 1
extern  int * num_char_s_1(char **, CLIENT *);
extern  int * num_char_s_1_svc(char **, struct svc_req *);
#define NUM_CHAR_WS 2
extern  int * num_char_ws_1(char **, CLIENT *);
extern  int * num_char_ws_1_svc(char **, struct svc_req *);
#define NUM_WORDS 3
extern  int * num_words_1(char **, CLIENT *);
extern  int * num_words_1_svc(char **, struct svc_req *);
extern int strings_service_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define NUM_CHAR_S 1
extern  int * num_char_s_1();
extern  int * num_char_s_1_svc();
#define NUM_CHAR_WS 2
extern  int * num_char_ws_1();
extern  int * num_char_ws_1_svc();
#define NUM_WORDS 3
extern  int * num_words_1();
extern  int * num_words_1_svc();
extern int strings_service_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_STRINGS_SERVICE_H_RPCGEN */