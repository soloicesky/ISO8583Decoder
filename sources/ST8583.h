#ifndef _ST8583_H_
#define _ST8583_H_
#ifdef __cplusplus
extern "C" {
#endif

enum CONTENTTYPE{
	A,
	N,
	S,
	AN,
	ANS,
	AS,
	B,
	X,
	Z,
};

enum LENATR{
	FIX,
	VAR,
};

enum VARLENLEN{
	L,
	LL,
	LLL,
};

typedef struct _fd_attr_
{
	int contentAtr;  //��������
	int maxLen;		//��󳤶�����
	int lenAtr;		//�������Զ������Ǳ䳤
	int varlenLen;	//�䳤���ȳ���(1 λ2λ3λ?)
}FdAttr;


typedef struct _fd_content_
{
	unsigned short length; //ʵ�ʳ���
	void *value;	//ʵ��ֵ
}FdContent;

typedef struct _field_
{
	unsigned char filedNo; //���
	FdAttr attr;
	FdContent content;
}FIELD;

typedef struct _field_set_
{
	FIELD *fdSet;
	int   size;
}FieldSet;

typedef struct _fd_no_set_
{
	int *fnSet;
	int size;
}FdNoSet;

#define MAX_MSGCODE_LEN			4
#define MAX_PAN_LEN				19
#define MAX_TPC_LEN				6
#define MAX_AOT_LEN				12
#define MAX_AOS_LEN				12

#define MAX_AOCB_LEN			12
#define MAX_TDT_LEN				6
#define MAX_AOCBF_LEN			8
#define MAX_CROS_LEN			8
#define MAX_CROCB_LEN			8

#define MAX_STAN_LEN			6
#define MAX_TOLT_LEN			6
#define MAX_DOLT_LEN			4
#define MAX_DOE_LEN			4
#define MAX_DOS_LEN			4

#define MAX_DOC_LEN			4
#define MAX_DOCP_LEN			4
#define MAX_MT_LEN			4
#define MAX_AICC_LEN			3
#define MAX_PCC_LEN			3

#define MAX_FICC_LEN			3
#define MAX_POSEM_LEN			3
#define MAX_PANSN_LEN			3
#define MAX_FC_LEN				3
#define MAX_POSCC_LEN			2

#define MAX_POSPCC_LEN			2
#define MAX_AIRL_LEN			1
#define MAX_AOTF_LEN			8
#define MAX_AOSF_LEN			8
#define MAX_AOTPF_LEN			8

#define MAX_AOSPF_LEN			8
#define MAX_AIIC_LEN			11
#define MAX_FIIC_LEN			3
#define MAX_PANEX_LEN			28
#define MAX_T2_LEN			    37

#define MAX_T3_LEN				104
#define MAX_RRN_LEN				12
#define MAX_AIR_LEN				322
#define MAX_RC_LEN				2
#define MAX_SRC_LEN				3

#define MAX_CATI_LEN			8
#define MAX_CAIC_LEN			15
#define MAX_CANL_LEN			40
#define MAX_ARD_LEN				25
#define MAX_T1_LEN			    76

#define MAX_ADISO_LEN			999
#define MAX_ADN_LEN				999
#define MAX_ADP_LEN				999
#define MAX_CCOT_LEN			3
#define MAX_CCOS_LEN			3

#define MAX_CCOCB_LEN			3
#define MAX_PIND_LEN			64
#define MAX_SRCI_LEN			16
#define MAX_AA_LEN				20
#define MAX_RFISO_LEN			 255

#define MAX_RFISO2_LEN			999
#define MAX_RFA_LEN				999
#define MAX_RFA2_LEN				100
#define MAX_RFAU_LEN			999
#define MAX_ARC_LEN			13

#define MAX_RFRP1_LEN			29
#define MAX_RFRP2_LEN				512
#define MAX_RFRP3_LEN				163
#define MAX_MAC_LEN			64


extern FieldSet FS;

#ifdef __cplusplus
}
#endif

#endif

