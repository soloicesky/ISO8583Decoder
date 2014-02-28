#ifndef _ERRORNO_H_
#define _ERRORNO_H_


#ifdef __cplusplus
extern "C"
{
#endif

#define ERR_INVALID_PARAMETER			-1
#define ERR_PARSE_TAG					-2
#define ERR_PARSE_LENGTH				-3
#define ERR_MEMALLOC_VALUE				-4
#define ERR_SAVEDATA_FAIL				-5

#define ERR_CONVERT_TYPE				-6
#define ERR_TAG_MISSING					-7
#define ERR_MEM_OVERFLOW				-8
#define ERR_PARSE_VALUE					-9
#define ERR_CAPK_NOT_FOUND				-10

#define ERR_RECOVER_ISSUER_PK			-11
#define ERR_INVALID_ISSUER_PKCERT		-12
#define ERR_RECOVER_ICC_PK				-13
#define ERR_INVALID_ICC_PKCERT			-14
#define ERR_INVALID_ICC_DATA			-15

#define ERR_SDA_FAILED					-16
#define ERR_INTERNAL_AUTH_FAILED		-17
#define ERR_DDA_FAILED					-18
#define ERR_RECOVER_PIN_PK				-19
#define ERR_TLVOBJ_NOT_FOUND			-20

#define ERR_TAG_NOT_IN_DICT				-21
#define ERR_CARD_ACTION_HIGHER_THAN_TERM	-22
#define ERR_SERVICE_NOT_ALLOWED			-23
#define ERR_OFFLINE_DECLINED			-24
#define ERR_GEN_PINBLOCK				-25

#define ERR_ENTER_PIN_CANCEL			-26
#define ERR_ISSUER_AUTH_FAILED			-27
#define ERR_INVALID_SCRIPT_DATA			-28


#define INFO_OFFLINE_DATA_AUTH_NOT_PERFORM	10
#define INFO_OFFLINE_DATA_AUTH_FAILED		11


#define INFO_GOOD_OFFINE_APPROVED			168
#define INFO_GO_ONLINE						169

#define STATUS_FAILED					 200	


#ifdef __cplusplus
}
#endif

#endif

