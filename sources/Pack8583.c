#include <errno.h>
#include <stdio.h>
#include "ST8583.h"
#include "Pack8583.h"
#include "Utils.h"

static unsigned char bitmapSize = STANDARD_BITMAP_SIZE;

const static unsigned char bitmapBits[] = { BITMAPBIT1, BITMAPBIT2, BITMAPBIT3,
BITMAPBIT4, BITMAPBIT5, BITMAPBIT6, BITMAPBIT7, BITMAPBIT8 };

/**
 * @description: setup bitmap field's size
 * @param: size - bitmap field's size
 * @retval: ERR_INVALID_BITMAPSIZE - size is out of ranges
 * @retval: 0 - at return zero means success
 */
int set_bitmapSize(unsigned char size) {
	if ((size != STANDARD_BITMAP_SIZE) || (size != EXTEND_BITMAP_SIZE)) {
		return ERR_INVALID_BITMAPSIZE;
	
}
	bitmapSize = size;
	return 0;
}

/**
 * @description: get bitmap field's size
 * @param: none
 * @retval: bitmapSize - size of the bitmap
 */
int get_bitmapSize(void) {
	return bitmapSize;
}

/**
 * @description: check if the length is valid
 * @param: FIELD - *fd the field that you want to check
 * @retval: (ERR_BASELEN_WRONG - fd->filedNo) - err of field no
 * @retval: 0 - success
 */
static int invalidateLength(FIELD *fd) {
	assert(fd);

	switch (fd->attr.lenAtr) {
	case VAR:
		if ((fd->content.length <= 0)
				|| (fd->content.length > fd->attr.maxLen)) {
			return (ERR_BASELEN_WRONG - fd->filedNo);
		}
		break;
	default:
		if ((fd->content.length != fd->attr.maxLen)) {
			return (ERR_BASELEN_WRONG - fd->filedNo);
		}
		break;
	}

	return 0;
}


/**
 * @description: pack the length of a var field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packLength(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	int ret = 0;

	ret = invalidateLength(fd);

	if (ret) {
		return ret;
	}

	if (fd->attr.lenAtr == FIX) {
		*desMsgLen = 0;
		return 0;
	}

	switch (fd->attr.varlenLen) {
	case L:
		*desMsg = (fd->content.length);
		*desMsgLen = 1;
		break;
	case LL:
		*desMsg = ((fd->content.length) / 10 << 4)
				| ((fd->content.length) % 10);
		*desMsgLen = 1;
		break;
	case LLL:
		*desMsg = (fd->content.length) / 100;
		*(desMsg + 1) = (((fd->content.length) % 100) / 10 << 4)
				| ((fd->content.length) % 10);
		*desMsgLen = 2;
		break;
	}

	return 0;
}

/**
 * @description: pack numeric field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packNumericField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	int len = 0;
	int ret = 0;
	unsigned char *bcdBuf = NULL;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	bcdBuf = (unsigned char *) malloc(
			sizeof(unsigned char) * (fd->content.length / 2));

	if (CHECK_M(bcdBuf)) {
		return ERR_ALOM_FAILED;
	}

	memset(bcdBuf, 0x00, fd->content.length / 2);
	ret = hexStringToByteArray(bcdBuf, &len, (char *) fd->content.value);
//	*desMsgLen = len;

	if (ret) {
		//	printf("hexStringToByteArray ret=%d", ret);
		return ret;
	}

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, bcdBuf, len);
	*desMsgLen += ((fd->content.length + 1) / 2);

	return ret;
}

/**
 * @description: pack alpha field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packAField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	int ret = 0;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	*desMsgLen = 0;

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, fd->content.value, fd->content.length);
	*desMsgLen += fd->content.length;
	return 0;
}

/**
 * @description: pack special characer field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packSField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	int ret = 0;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	*desMsgLen = 0;

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, fd->content.value, fd->content.length);
	*desMsgLen += fd->content.length;
	return 0;
}

/**
 * @description: pack alpha numeric field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packANField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	int ret = 0;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	*desMsgLen = 0;

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, fd->content.value, fd->content.length);
	*desMsgLen += fd->content.length;
	return 0;
}

/**
 * @description: pack alpha numeric special character field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packANSField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	int ret = 0;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	*desMsgLen = 0;

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, fd->content.value, fd->content.length);
	*desMsgLen += fd->content.length;
	return 0;
}

/**
 * @description: pack bit field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packBField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	int ret = 0;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	ret = invalidateLength(fd);

	if (ret) {
		return ret;
	}

	*desMsgLen = 0;

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, fd->content.value, fd->content.length);
	*desMsgLen += fd->content.length;
	return 0;
}

/**
 * @description: pack track field
 * @param: desMsg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fd - the field that you want to pack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int packZField(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	//add your to do here
	unsigned short len = 0;
	int ret = 0;
	unsigned char *bcdBuf = NULL;

	if (CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd)) {
		return ERR_NULL_POINTER;
	}

	*desMsgLen = 0;

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	bcdBuf = (unsigned char *) malloc(
			sizeof(unsigned char) * (fd->content.length / 2));

	ret = compressTrackData(bcdBuf, &len, (char *) fd->content.value);

	memcpy(desMsg + *desMsgLen, bcdBuf, (fd->content.length / 2));
	*desMsgLen += (fd->content.length / 2);
	return 0;
}

/**
 * @description: pack ISO8583 message
 * @param: des8583Msg - buffer for the iso8583 message
 * @param: desMsgLen - des message length out
 * @param: fns - the field set that you want to use for build the iso8583 message
 * @retval: 0 - success
 * @retval: none zero - fail
 */
int packISO8583Msg(unsigned char *des8583Msg, unsigned short *desMsgLen,
		FdNoSet *fns) {
	//add your to do here
	int i = 0;
	unsigned short swapLen;
	int ret = 0;
	unsigned char *bitmap = NULL;

	if (CHECK_M(des8583Msg) || CHECK_M(desMsgLen) || CHECK_M(fns)) {
		return ERR_NULL_POINTER;
	}

	*desMsgLen = 0;
	bitmap = des8583Msg + FS.fdSet[0].content.length / 2;

	for (i = 0; i < fns->size; i++) {
		swapLen = 0;
	//	printf("field:%d\r\n", fns->fnSet[i]);
		SETBITS(bitmap[fns->fnSet[i] / 8], bitmapBits[(fns->fnSet[i] % 8) - 1]); //setup bitmap

		//use the packing methods according to the field's attribute
		switch (FS.fdSet[fns->fnSet[i] - 1].attr.contentAtr) {
		case N:
			ret = packNumericField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		case A:
			ret = packAField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		case AN:
			ret = packANField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		case ANS:
			ret = packANSField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		case Z:
			ret = packZField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		case B:
			ret = packBField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		case S:
			ret = packSField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i] - 1]);
			break;
		default:
			return ERR_INVALID_CONTENTATTR;
			break;
		}

		*desMsgLen += swapLen;

		if (fns->fnSet[i] == 1) { //because bitmap is after message code so when pack field two should skip the bitmap's room
			*desMsgLen += bitmapSize; //bitmap size may be standard 64 bits or extend 128 bits
		}
	}

	return ret;
}

/**
 * @description: unpack the length of a var field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: contentLen - the field's content's length
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpackLength(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned short *contentLen, unsigned char fieldNo) {
//	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	if (FS.fdSet[fieldNo - 1].attr.lenAtr == VAR) {
		switch (FS.fdSet[fieldNo - 1].attr.varlenLen) {
		case L:
			len = *backupMsg;
			backupMsg++;
			*srcMsgLen -= 1;
			break;
		case LL:
			len = ((*backupMsg & 0xF0) >> 4) * 10 + (*backupMsg & 0x0F);
			backupMsg++;
			*srcMsgLen -= 1;
			break;
		case LLL:
//			printf("len1:[%02X]\n",*backupMsg);
//			printf("len2:[%02X]\n",backupMsg[1]);
			len = (*backupMsg & 0x0F) * 100 + ((backupMsg[1] & 0xF0) >> 4) * 10
					+ (backupMsg[1] & 0x0F);
			backupMsg += 2;
			*srcMsgLen -= 2;
			break;
		default:
			return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
			break;
		}

		if ((len > FS.fdSet[fieldNo - 1].attr.maxLen) || (len <= 0)) {
			return (ERR_BASELEN_WRONG - fieldNo);
		}
	} else {
		len = FS.fdSet[fieldNo - 1].attr.maxLen;
	}

	*contentLen = len;
	*srcMsg = backupMsg;
//	printf("len->%d, *srcMsg[%d]\n", len, *srcMsg[0]);
	return 0;
}

/**
 * @description: unpack the a numeric field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakNumericField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;
	char *tempPtr = NULL;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}
	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {
		FS.fdSet[fieldNo - 1].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, (len + 2));
	ret = byteArrayToHexString((char *) FS.fdSet[fieldNo - 1].content.value,
			backupMsg, (int) ((len + 1) / 2));
	tempPtr = (char *) FS.fdSet[fieldNo - 1].content.value;
	tempPtr[len] = '\0';
//	printf("value->%s\n", tempPtr);
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsgLen -= (len + 1) / 2;
	*srcMsg = backupMsg + ((len + 1) / 2);
	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}
	return ret;
}

/**
 * @description: unpack the a alpha field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakAField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;
	char *temPtr;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {
		FS.fdSet[fieldNo - 1].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo - 1].content.value, backupMsg, len);
	temPtr = (char *) FS.fdSet[fieldNo - 1].content.value;
	temPtr[len] = '\0';
//	printf("value->%s\n", temPtr);
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}
	return ret;
}


/**
 * @description: unpack the a alpha numeric field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakANField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;
	char *temPtr;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {
		FS.fdSet[fieldNo - 1].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo - 1].content.value, backupMsg, len);
	temPtr = (char *) FS.fdSet[fieldNo - 1].content.value;
	temPtr[len] = '\0';
//	printf("value->%s\n", temPtr);
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}
	return ret;
}


/**
 * @description: unpack the a alpha numeric special character field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakANSField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;
	char *temPtr;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {
		FS.fdSet[fieldNo - 1].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo - 1].content.value, backupMsg, len);
	temPtr = (char *) FS.fdSet[fieldNo - 1].content.value;
	temPtr[len] = '\0';
//	printf("value->[%s]\n", temPtr);
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;

	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}
	return ret;
}

/**
 * @description: unpack the a special character field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakSField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;
	char *temPtr;
	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {

		FS.fdSet[fieldNo - 1].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo - 1].content.value, backupMsg, len);
	temPtr = (char *) FS.fdSet[fieldNo - 1].content.value;
	temPtr[len] = '\0';
//	printf("value->%s\n", temPtr);
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}

	return ret;
}


/**
 * @description: unpack the a bit field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakBField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {
		FS.fdSet[fieldNo - 1].content.value = (unsigned char *) malloc(
				sizeof(unsigned char) * len);

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, len);
	memcpy(FS.fdSet[fieldNo - 1].content.value, backupMsg, len);
//	FS.fdSet[fieldNo].content.value[len] = '\0';
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}
	return ret;
}

/**
 * @description: unpack the a track field
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in and out
 * @param: PsaveData - save the field's callback
 * @param: fieldNo - the field you want to unpack
 * @retval: 0 - success
 * @retval: none zero - fail
 */
static int unpakTrackZField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo, saveData PsaveData) {
	int ret = 0;
	unsigned short len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize * 8) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	ret = unpackLength(&backupMsg, srcMsgLen, &len, fieldNo);

	if (ret) {
		return ret;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo - 1].content.value)
			|| (FS.fdSet[fieldNo - 1].content.length < len)) {
		FS.fdSet[fieldNo - 1].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo - 1].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo - 1].content.value, 0x00, (len + 2));
	ret = byteArrayToHexString((char *) FS.fdSet[fieldNo - 1].content.value,
			backupMsg, (int) ((len + 1) / 2));
	backupMsg = (unsigned char *) FS.fdSet[fieldNo - 1].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo - 1].content.length = len;
	*srcMsg = backupMsg + ((len + 1) / 2);
	*srcMsgLen -= ((len + 1) / 2);
	if (PsaveData) {
		PsaveData(fieldNo, FS.fdSet[fieldNo - 1].content.value,
				FS.fdSet[fieldNo - 1].content.length);
	}
	return ret;
}


/**
 * @description: unpack the iso8583 message
 * @param: srcMsg -  the iso8583 message that you wan to decode
 * @param: srcMsgLen - des message length in
 * @param: PsaveData - save the field's callback
 * @retval: 0 - success
 * @retval: none zero - fail
 */
int unpackISO8583Msg(unsigned char *srcMsg, unsigned short srcMsgLen,
		saveData PsaveData) {
	int ret = 0;
	FdNoSet fns;
	unsigned char *bitmap = srcMsg + FS.fdSet[0].attr.maxLen / 2;
	unsigned char *backupMsg = bitmap + bitmapSize;;
	int i = 0;
	int j = 0;

	if (CHECK_M(srcMsg) || !(srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	memset(&fns, 0x00, sizeof(fns));
	fns.fnSet = (int *) malloc(sizeof(int) * (bitmapSize * 8));

	if (CHECK_M(fns.fnSet)) {
//		printf("faild this[%s]\n", strerror(errno));
		return ERR_ALOM_FAILED;
	}

	for (i = 0; i < bitmapSize; i++) {
		for (j = 0; j < 8; j++) {
			if (CHECKBITSSET(bitmap[i], bitmapBits[j])) {
				fns.fnSet[fns.size++] = i * 8 + j + 1;
			}
		}
	}

	for (i = 0; i < fns.size; i++) {
	//	printf("field->%d\n", fns.fnSet[i]);
	//	printf("fn=:%d, ctr:%d\n", FS.fdSet[fns.fnSet[i] - 1].filedNo,FS.fdSet[fns.fnSet[i] - 1].attr.contentAtr);
		switch (FS.fdSet[fns.fnSet[i] - 1].attr.contentAtr) {
		case N:
		//	printf("N\n");
			ret = unpakNumericField(&backupMsg, &srcMsgLen, fns.fnSet[i],
					PsaveData);
			break;
		case A:
		//	printf("A\n");
			ret = unpakAField(&backupMsg, &srcMsgLen, fns.fnSet[i], PsaveData);
			break;
		case AN:
		//	printf("AN\n");
			ret = unpakANField(&backupMsg, &srcMsgLen, fns.fnSet[i], PsaveData);
			break;
		case ANS:
		//	printf("ANS\n");
			ret = unpakANSField(&backupMsg, &srcMsgLen, fns.fnSet[i],
					PsaveData);
			break;
		case S:
		//	printf("S\n");
			ret = unpakSField(&backupMsg, &srcMsgLen, fns.fnSet[i], PsaveData);
			break;
		case Z:
		//	printf("Z\n");
			ret = unpakTrackZField(&backupMsg, &srcMsgLen, fns.fnSet[i],
					PsaveData);
			break;
		case B:
		//	printf("B\n");
			ret = unpakBField(&backupMsg, &srcMsgLen, fns.fnSet[i], PsaveData);
			break;
		default:
			return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
			break;
		}

		if (ret) {
			return ret;
		}
	}

	return ret;
}
