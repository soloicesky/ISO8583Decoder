#include "ST8583.h"
#include "Pack8583.h"
#include "Utils.h"

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

	switch (fd->attr.varlenLen) {
	case L:
		*desMsg = (fd->content.length / 2);
		*desMsgLen = 1;
		break;
	case LL:
		*desMsg = ((fd->content.length / 2) / 10 << 4)
				| ((fd->content.length / 2) % 10);
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

int packNumericField(unsigned char *desMsg, unsigned short *desMsgLen,
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

	CLEAR_BUFF(bcdBuf);
	ret = hexStringToByteArray(bcdBuf, &len, (char *) fd->content.value);
	*desMsgLen = len;

	if (ret) {
		return ret;
	}

	ret = packLength(desMsg, desMsgLen, fd);

	if (ret) {
		return ret;
	}

	memcpy(desMsg + *desMsgLen, bcdBuf, (fd->content.length / 2));
	*desMsgLen += (fd->content.length / 2);

	return ret;
}

int packAField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd) {
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

int packANField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd) {
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

int packANSField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd) {
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

int packBField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd) {
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

int packZField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd) {
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

	ret = compressTrackData(bcdBuf, &len,(char *)fd->content.value);

	memcpy(desMsg + *desMsgLen, bcdBuf, (fd->content.length / 2));
		*desMsgLen += (fd->content.length / 2);
	return 0;
}
