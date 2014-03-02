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

static int packLength(unsigned char *desMsg, unsigned short *desMsgLen,
		FIELD *fd) {
	int ret = 0;

	ret = invalidateLength(fd);

	if (ret) {
		return ret;
	}

	if(fd->attr.lenAtr == FIX)
	{
		*desMsgLen = 0;
		return 0;
	}

	switch (fd->attr.varlenLen) {
	case L:
		*desMsg = (fd->content.length / 2);
		*desMsgLen = 1;
		break;
	case LL:
		*desMsg = ((fd->content.length) / 10 << 4)
				| ((fd->content.length) % 10);
		*desMsgLen = 1;
		break;
	case LLL:
		*desMsg = (fd->content.length / 2) / 100;
		*(desMsg + 1) = (((fd->content.length / 2) % 100) / 10 << 4)
				| ((fd->content.length / 2) % 10);
		*desMsgLen = 2;
		break;
	}

	return 0;
}

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
	*desMsgLen += ((fd->content.length+1) / 2);

	return ret;
}

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
		printf("field:%d\r\n", fns->fnSet[i]);
		SETBITS(bitmap[fns->fnSet[i] / 8], bitmapBits[(fns->fnSet[i] % 8) - 1]); //setup bitmap

		//use the packing methods according to the field's attribute
		switch (FS.fdSet[fns->fnSet[i] - 1].attr.contentAtr) {
		case N:
			ret = packNumericField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
			break;
		case A:
			ret = packAField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
			break;
		case AN:
			ret = packANField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
			break;
		case ANS:
			ret = packANSField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
			break;
		case Z:
			ret = packZField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
			break;
		case B:
			ret = packBField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
			break;
		case S:
			ret = packSField(des8583Msg + *desMsgLen, &swapLen,
					&FS.fdSet[fns->fnSet[i]- 1]);
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

static int unpakNumericField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {
		FS.fdSet[fieldNo].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, (len + 2));
	ret = byteArrayToHexString((char *) FS.fdSet[fieldNo].content.value,
			backupMsg, (int) ((len + 1) / 2));
	backupMsg = (unsigned char *) FS.fdSet[fieldNo].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsgLen -= (len + 1) / 2;
	*srcMsg = backupMsg + ((len + 1) / 2);
	return ret;
}

static int unpakAField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {
		FS.fdSet[fieldNo].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo].content.value, backupMsg, len);
	backupMsg = (unsigned char *) FS.fdSet[fieldNo].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	return ret;
}

static int unpakANField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {
		FS.fdSet[fieldNo].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo].content.value, backupMsg, len);
	backupMsg = (unsigned char *) FS.fdSet[fieldNo].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	return ret;
}

static int unpakANSField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {
		FS.fdSet[fieldNo].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo].content.value, backupMsg, len);
	backupMsg = (unsigned char *) FS.fdSet[fieldNo].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	return ret;
}

static int unpakSField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {

		FS.fdSet[fieldNo].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, (len + 2));
	memcpy(FS.fdSet[fieldNo].content.value, backupMsg, len);
	backupMsg = (unsigned char *) FS.fdSet[fieldNo].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;

	return ret;
}

static int unpakBField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {
		FS.fdSet[fieldNo].content.value = (unsigned char *) malloc(
				sizeof(unsigned char) * len);

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, len);
	memcpy(FS.fdSet[fieldNo].content.value, backupMsg, len);
//	FS.fdSet[fieldNo].content.value[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsg = backupMsg + len;
	*srcMsgLen -= len;
	return ret;
}

static int unpakTrackZField(unsigned char **srcMsg, unsigned short *srcMsgLen,
		unsigned char fieldNo) {
	int ret = 0;
	int len = 0;
	unsigned char *backupMsg = *srcMsg;

	//check if the message is valid
	if (CHECK_M(backupMsg) || !(*srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	//check if the field no is out of rang if yes then return ERR_INVALID_MSG
	if ((fieldNo > bitmapSize) || (fieldNo < 0)) {
		return ERR_INVALID_MSG;
	}

	switch (FS.fdSet[fieldNo].attr.varlenLen) {
	case L:
		len = *backupMsg;
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LL:
		len = (*backupMsg & 0xF0) * 10 + (*backupMsg & 0x0F);
		backupMsg++;
		*srcMsgLen -= 1;
		break;
	case LLL:
		len = (*backupMsg & 0x0F) * 100 + (backupMsg[1] & 0xF0) * 10
				+ (backupMsg[1] & 0x0F);
		backupMsg += 2;
		*srcMsgLen -= 2;
		break;
	default:
		return ERR_INVALID_LENLENATTR; //the length's length's attribute is out of range only L LL LLL is valid
		break;
	}

	// if it's not allocated mem then allocated the len + 2 space,additional two is for store the '\0' end of a string
	// and RFU one for dealing overflow
	if (CHECK_M(FS.fdSet[fieldNo].content.value)
			|| (FS.fdSet[fieldNo].content.length < len)) {
		FS.fdSet[fieldNo].content.value = (char *) malloc(
				sizeof(char) * (len + 2));

		if (CHECK_M(FS.fdSet[fieldNo].content.value))	//allocat fail
				{
			return ERR_ALOM_FAILED;
		}
	}

	memset(FS.fdSet[fieldNo].content.value, 0x00, (len + 2));
	ret = byteArrayToHexString((char *) FS.fdSet[fieldNo].content.value,
			backupMsg, (int) ((len + 1) / 2));
	backupMsg = (unsigned char *) FS.fdSet[fieldNo].content.value;
	backupMsg[len] = '\0';
	FS.fdSet[fieldNo].content.length = len;
	*srcMsg = backupMsg + ((len + 1) / 2);
	*srcMsgLen -= ((len + 1) / 2);
	return ret;
}

int unpackISO8583Msg(unsigned char *srcMsg, unsigned short srcMsgLen) {
	int ret = 0;
	FdNoSet fns;
	unsigned char *bitmap = srcMsg + bitmapSize;
	unsigned char *backupMsg = srcMsg;
	int i = 0;
	int j = 0;

	if (CHECK_M(srcMsg) || !(srcMsgLen > 0)) {
		return ERR_INVALID_MSG;
	}

	fns.fnSet = (int *) malloc(sizeof(int) * bitmapSize);
	memset(&fns, 0x00, sizeof(fns));

	if (CHECK_M(fns.fnSet)) {
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
		switch (FS.fdSet[fns.fnSet[i] - 1].attr.contentAtr) {
		case N:
			ret = unpakNumericField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
			break;
		case A:
			ret = unpakAField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
			break;
		case AN:
			ret = unpakANField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
			break;
		case ANS:
			ret = unpakANSField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
			break;
		case S:
			ret = unpakSField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
			break;
		case Z:
			ret = unpakTrackZField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
			break;
		case B:
			ret = unpakBField(&backupMsg, &srcMsgLen, fns.fnSet[i]);
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
