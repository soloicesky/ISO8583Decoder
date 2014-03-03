#include "Pack8583.h"
#include "ST8583.h"
#include "Utils.h"
#include "stdio.h"

void printHexarray(unsigned char *hexArray, unsigned short len)
{
	int  i = 0;

	for(i = 0; i<len;i++)
	{
		printf("%02X", hexArray[i]);
	}

	printf("\r\n");
}

int testBuildISO8583Msg(void)
{
	//add your to do here
	int ret = 0;
	FdNoSet fns;	
	unsigned char desMsg[1024];
	unsigned short msgLen = 0;
	
	ret = setFieldContent(1,"0200", strlen("0200"));

	if(ret)
	{
		printf("setFieldContent ret=%d\n", ret);

		return ret;
	}

	ret = setFieldContent(2,"1234567890123456789", strlen("1234567890123456789"));

	if(ret)
	{
		printf("setFieldContent ret=%d\n", ret);

		return ret;
	}

	fns.fnSet = (int *)malloc(sizeof(int) * 2);

	if(CHECK_M(fns.fnSet))
	{
		return ERR_ALOM_FAILED;
	}

	fns.fnSet[0] = 1;
	fns.fnSet[1] = 2;
	fns.size = 2;

	memset(desMsg, 0x00, sizeof(desMsg));
	ret = packISO8583Msg(desMsg, &msgLen, &fns);

	if(ret)
	{
		printf("packISO8583Msg=%d\n", ret);
		return ret;
	}

	printHexarray(desMsg, msgLen);
	return 0;
}

int testUnpackISO8583Msg(void)
{
	//add your to do here
	char *testMsg="60052100006031000000000810003800010AC000140000161441490303081234567830303030303030313334343930303132333435363738313233343536373839303132333435001100000085004000606F3FB56EB7F7B2DF00657E864E9249C89CA2C689380B88B845D2DF350000000000000000A246BBB86F3FB56EB7F7B2DF00657E864E9249C89CA2C689";
	unsigned char testBuf[512];
	int len = 0;
	int ret = 0;

	memset(testBuf, 0x00, sizeof(testBuf));
	ret = hexStringToByteArray(testBuf, &len, testMsg);

	if(ret)
	{
		printf("hexStringToByteArray ret=%d\n", ret);
	}

	ret = unpackISO8583Msg(testBuf, (unsigned short)len);

	if(ret)
	{
		printf("unpackISO8583Msg ret=%d\n", ret);
	}
	
	return 0;
}


int main(void)
{

	testBuildISO8583Msg();
	testUnpackISO8583Msg();
	return 0;
}

