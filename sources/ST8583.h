#ifndef _ST8583_H_
#define _ST8583_H_

#define ERR_INVALID_FIELDNO			-10  //invalid field no the field no is out of ranges
#define ERR_NULL_POINTER			-11 // null pointer
#define ERR_ALOM_FAILED				-12 // Alloc memery fails


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


extern FieldSet FS;

#endif

