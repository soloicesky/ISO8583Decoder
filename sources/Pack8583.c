#include "ST8583.h"
#include "Pack8583.h"
#include "Utils.h"



int packNumericField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd)
{
	//add your to do here
	int len = 0;
	int ret = 0;

	if(CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd))
	{
		return ERR_NULL_POINTER;
	}

	unsigned char *bcdBuf = NULL;

	bcdBuf = (unsigned char *)malloc(sizeof(unsigned char) * (fd->content.length / 2));

	if(CHECK_M(bcdBuf))
	{
			return ERR_ALOM_FAILED;
	}

	CLEAR_BUFF(bcdBuf);
	ret = hexStringToByteArray(bcdBuf, desMsgLen, (char *)fd->content.value);

	if(ret)
	{
		return ret;
	}

	switch(fd->attr.lenAtr)
	{
		case VAR:
			switch(fd->attr.varlenLen)
			{
				case L:
					*desMsg = (fd->content.length / 2);
					*desMsgLen = 1;
					break;
				case LL:
					*desMsg = ((fd->content.length / 2)/10 << 4) | ((fd->content.length / 2)%10);
					*desMsgLen = 1;
					break;
				case LLL:
					*desMsg = (fd->content.length / 2)/100;
					*(desMsg + 1) = (((fd->content.length / 2)%100)/10 << 4) | ((fd->content.length / 2)%10);
					*desMsgLen = 2;
					break;
			}

			break;
		default:
			*desMsgLen = 0;
			break;
	}

	memcpy(desMsg+*desMsgLen, bcdBuf, (fd->content.length / 2));
	return ret;
}

int packAField(unsigned char *desMsg, unsigned short *desMsgLen, FIELD *fd)
{
	//add your to do here
	int len = 0;

	if(CHECK_M(desMsg) || CHECK_M(desMsgLen) || CHECK_M(fd))
	{
		return ERR_NULL_POINTER;
	}

	memcpy(desMsg, fd->content.value, fd->content.length);
	*desMsgLen = fd->content.length;
	return 0;
}
