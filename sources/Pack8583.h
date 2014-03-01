#ifndef _PACK8583_H_
#define _PACK8583_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_INVALID_FIELDNO			-10  //invalid field no the field no is out of ranges
#define ERR_NULL_POINTER			-11 // null pointer
#define ERR_ALOM_FAILED				-12 // Alloc memery fails
#define ERR_BASELEN_WRONG			-1000 //base error of invalid length and real length errno will be (ERR_BASELEN_WRONG - field no)



#ifdef __cplusplus
}
#endif

#endif
