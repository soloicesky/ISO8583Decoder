#include "Pack8583.h"
#include "ST8583.h"
#include "Utils.h"
#include "stdio.h"

int SaveData(unsigned char fieldNo, void *content, unsigned short len)
{
	printf("%d:%s:%d\n", fieldNo, (char *)content, len);
	return 0;
}


void printHexarray(unsigned char *hexArray, unsigned short len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		printf("%02X", hexArray[i]);
	}

	printf("\r\n");
}

int testBuildISO8583Msg(void) {
	//add your to do here
	int ret = 0;
	FdNoSet fns;
	unsigned char desMsg[1024];
	unsigned short msgLen = 0;

	ret = setFieldContent(1, "0200", strlen("0200"));

	if (ret) {
		printf("setFieldContent ret=%d\n", ret);

		return ret;
	}

	ret = setFieldContent(2, "1234567890123456789",
			strlen("1234567890123456789"));

	if (ret) {
		printf("setFieldContent ret=%d\n", ret);

		return ret;
	}

	fns.fnSet = (int *) malloc(sizeof(int) * 2);

	if (CHECK_M(fns.fnSet)) {
		return ERR_ALOM_FAILED;
	}

	fns.fnSet[0] = 1;
	fns.fnSet[1] = 2;
	fns.size = 2;

	memset(desMsg, 0x00, sizeof(desMsg));
	ret = packISO8583Msg(desMsg, &msgLen, &fns);

	if (ret) {
		printf("packISO8583Msg=%d\n", ret);
		return ret;
	}

	printHexarray(desMsg, msgLen);
	return 0;
}

int testUnpackISO8583Msg(void) {
	//add your to do here
	char *testMsg =
			"02007024068000c08213196217800100900072756000000000000000000100063122120720000000313233343536373731323334353637383930313233343531353601309f74064543433030319f260817ffdc4f137878289f2701409f101307010103900000010a010000077347a057ab9e9f3704bd5836019f36025973950500000000009a031403049c01009f02060000000000015f2a02015682027c009f1a0201569f03060000000000009f3303e0e1c89f3501229f1e0831303130353232398a02593100133600000300060000034355503834434530423534";
	unsigned char testBuf[512];
	int len = 0;
	int ret = 0;

	memset(testBuf, 0x00, sizeof(testBuf));
	ret = hexStringToByteArray(testBuf, &len, testMsg);

	if (ret) {
		printf("hexStringToByteArray ret=%d\n", ret);
	}

	ret = unpackISO8583Msg(testBuf, (unsigned short) len, SaveData);

	if (ret) {
		printf("unpackISO8583Msg ret=%d\n", ret);
	}

	return 0;
}

int main(void) {

	testBuildISO8583Msg();
	testUnpackISO8583Msg();
	return 0;
}

