#include "ST8583.h"
#include "Utils.h"
#include "ST8583.h"

static FdContent fc;

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

FIELD fieldS[] = { { 1, { N, MAX_MSGCODE_LEN, FIX, L }, fc },  //message code
		{ 2, { N, MAX_PAN_LEN, VAR, LL }, fc }, //Primary Account Number
		{ 3, { N, MAX_TPC_LEN, FIX, L }, fc },  //Transaction Processing Code
		{ 4, { N, MAX_AOT_LEN, FIX, LL }, fc }, //Amount, transaction
		{ 5, { N, MAX_AOS_LEN, FIX, LL }, fc }, //Amount, settlement

		{ 6, { N, MAX_AOCB_LEN, FIX, LL }, fc },  //Amount, cardholder billing
		{ 7, { N, MAX_TDT_LEN, FIX, LL }, fc }, //Transmission date & time
		{ 8, { N, MAX_TPC_LEN, FIX, L }, fc },  //Amount, cardholder billing fee
		{ 9, { N, MAX_CROS_LEN, FIX, L }, fc }, //Conversion rate, settlement
		{ 10, { N, MAX_CROCB_LEN, FIX, LL }, fc }, //Conversion rate, cardholder billing

		{ 11, { N, MAX_STAN_LEN, FIX, L }, fc },  //System trace audit number
		{ 12, { N, MAX_TOLT_LEN, VAR, LL }, fc }, //Time, local transaction (hhmmss)
		{ 13, { N, MAX_DOLT_LEN, FIX, L }, fc }, //Date, local transaction (MMDD)
		{ 14, { N, MAX_DOE_LEN, FIX, L }, fc }, //Date, expiration
		{ 15, { N, MAX_DOS_LEN, FIX, L }, fc }, //Date, settlement

		{ 16, { N, MAX_DOC_LEN, FIX, L }, fc },  //Date, conversion
		{ 17, { N, MAX_DOCP_LEN, FIX, L }, fc }, //Date, capture
		{ 18, { N, MAX_MT_LEN, FIX, L }, fc },  //Merchant type
		{ 19, { N, MAX_AICC_LEN, FIX, L }, fc }, //Acquiring institution country code
		{ 20, { N, MAX_PCC_LEN, FIX, L }, fc }, //PAN extended, country code

		{ 21, { N, MAX_FICC_LEN, FIX, L }, fc }, //Forwarding institution. country code
		{ 22, { N, MAX_POSEM_LEN, FIX, L }, fc }, //Point of service entry mode
		{ 23, { N, MAX_PANSN_LEN, FIX, L }, fc }, //Application PAN sequence number
		{ 24, { N, MAX_FC_LEN, FIX, L }, fc }, //Function code (ISO 8583:1993)/Network International identifier (NII)
		{ 25, { N, MAX_POSCC_LEN, FIX, L }, fc }, //Point of service condition code

		{ 26, { N, MAX_POSPCC_LEN, FIX, L }, fc }, //Point of service capture code
		{ 27, { N, MAX_AIRL_LEN, FIX, L }, fc }, //Authorizing identification response length
		{ 28, { N, MAX_AOTF_LEN, FIX, L }, fc },  //Amount, transaction fee
		{ 29, { N, MAX_AOSF_LEN, FIX, L }, fc }, //Amount, settlement fee
		{ 30, { N, MAX_AOTPF_LEN, FIX, LL }, fc }, //Amount, transaction processing fee

		{ 31, { N, MAX_AOSPF_LEN, FIX, L }, fc }, //	Amount, settlement processing fee
		{ 32, { N, MAX_AIIC_LEN, VAR, LL }, fc }, //Acquiring institution identification code
		{ 33, { N, MAX_FIIC_LEN, FIX, L }, fc }, //Forwarding institution identification code
		{ 34, { N, MAX_PANEX_LEN, VAR, LL }, fc }, //Primary account number, extended
		{ 35, { Z, MAX_T2_LEN, VAR, LL }, fc }, //Track 2 data

		{ 36, { Z, MAX_T3_LEN, VAR, LLL }, fc },  //Track 3 data
		{ 37, { AN, MAX_RRN_LEN, FIX, LL }, fc }, //Retrieval reference number
		{ 38, { N, MAX_AIR_LEN, FIX, L }, fc }, //Authorization identification response
		{ 39, { AN, MAX_RC_LEN, FIX, L }, fc }, //Response code
		{ 40, { N, MAX_SRC_LEN, FIX, LL }, fc }, //Service restriction code

		{ 41, { ANS, MAX_CATI_LEN, FIX, L }, fc }, //	Card acceptor terminal identification
		{ 42, { ANS, MAX_CAIC_LEN, FIX, L }, fc }, //Card acceptor identification code
		{ 43, { N, MAX_CANL_LEN, FIX, L }, fc }, //Card acceptor name/location (1-23 address 24-36 city 37-38 state 39-40 country)
		{ 44, { AN, MAX_ARD_LEN, VAR, LL }, fc }, //Additional response data
		{ 45, { N, MAX_T1_LEN, VAR, LL }, fc }, //Track 1 data

		{ 46, { N, MAX_ADISO_LEN, VAR, LLL }, fc },  //Additional data - ISO
		{ 47, { N, MAX_ADN_LEN, VAR, LLL }, fc }, //Additional data - national
		{ 48, { N, MAX_AIR_LEN, VAR, LLL }, fc },  //Additional data - private
		{ 49, { N, MAX_CCOT_LEN, FIX, L }, fc }, //Currency code, transaction
		{ 50, { N, MAX_CCOS_LEN, FIX, L }, fc }, //Currency code, settlement

		{ 41, { N, MAX_CATI_LEN, FIX, L }, fc }, //	Card acceptor terminal identification
		{ 42, { N, MAX_CAIC_LEN, FIX, L }, fc }, //Card acceptor identification code
		{ 43, { N, MAX_CANL_LEN, FIX, L }, fc }, //Card acceptor name/location (1-23 address 24-36 city 37-38 state 39-40 country)
		{ 44, { N, MAX_ARD_LEN, VAR, LL }, fc }, //Additional response data
		{ 45, { N, MAX_T1_LEN, VAR, LL }, fc }, //Track 1 data

		{ 46, { N, MAX_ADISO_LEN, VAR, LLL }, fc },  //Additional data - ISO
		{ 47, { N, MAX_ADN_LEN, VAR, LLL }, fc }, //Additional data - national
		{ 48, { N, MAX_AIR_LEN, VAR, LLL }, fc },  //Additional data - private
		{ 49, { AN, MAX_CCOT_LEN, FIX, L }, fc }, //Currency code, transaction
		{ 50, { N, MAX_CCOS_LEN, FIX, L }, fc }, //Currency code, settlement

		{ 51, { AN, MAX_CCOCB_LEN, FIX, L }, fc }, //	Currency code, cardholder billing
		{ 52, { B, MAX_PIND_LEN, FIX, LL }, fc }, //Personal identification number data
		{ 53, { N, MAX_SRCI_LEN, FIX, LL }, fc }, //Security related control information
		{ 54, { AN, MAX_AA_LEN, VAR, LLL }, fc }, //Additional amounts
		{ 55, { B, MAX_RFISO_LEN, VAR, LLL }, fc }, //Reserved ISO

		{ 56, { ANS, MAX_RFISO2_LEN, VAR, LLL }, fc },  //Reserved ISO
		{ 57, { ANS, MAX_RFA_LEN, VAR, LLL }, fc }, //Reserved national
		{ 58, { ANS, MAX_RFA2_LEN, VAR, LLL }, fc },  //Reserved national
		{ 59, { ANS, MAX_RFAU_LEN, VAR, LLL }, fc }, //Reserved for national use
		{ 60, { N, MAX_ARC_LEN, VAR, LLL }, fc }, //Advice/reason code (private reserved)

		{ 61, { AN, MAX_RFRP1_LEN, VAR, LLL }, fc }, //	Reserved private
		{ 62, { ANS, MAX_RFRP2_LEN, VAR, LLL }, fc }, //Reserved private
		{ 63, { ANS, MAX_RFRP3_LEN, VAR, LLL }, fc }, //Reserved private
		{ 64, { B, MAX_MAC_LEN, FIX, L }, fc }, //Message authentication code (MAC)

		};

static FieldSet FS = {
		fieldS,
		sizeof(fieldS)/sizeof(FIELD)
};

/**
 * @description: setup a specify field's attribute and it's content
 * @param: FIELD - fd the field
 * @retval: ERR_INVALID_FIELDNO - the field no is out of ranges
 * @retval: 0 - at return zero means success
 */
int setField(FIELD *fd)
{
	if(fd->filedNo > FS.size - 1)
	{
		return ERR_INVALID_FIELDNO;
	}

	FS.fdSet[fd->filedNo-1] = *fd;
	return 0;
}

/**
 * @description: setup a specify field's attribute by specify the field no and attribute
 * @param: filedNo - the field's no
 * @param: attr - the field's attribute
 * @retval: ERR_INVALID_FIELDNO - the field no is out of ranges
 * @retval: 0 - at return zero means success
 */
int setFieldAttr(unsigned char filedNo,FdAttr attr)
{
	if(filedNo > FS.size - 1)
	{
		return ERR_INVALID_FIELDNO;
	}

	FS.fdSet[filedNo - 1].attr = attr;

	return 0;
}

/**
 * @description: setup a specify field's content by specify the field no and content
 * @param: filedNo - the field's no
 * @param: content - the field's content
 * @param: contentLen - the field's content's length
 * @retval: ERR_INVALID_FIELDNO - the field no is out of ranges
 * @retval: 0 - at return zero means success
 */
int setFieldContent(unsigned char filedNo,void *content, unsigned short contentLen)
{
	if(filedNo > FS.size - 1)
	{
		return ERR_INVALID_FIELDNO;
	}

	FS.fdSet[filedNo - 1].content.value = content;
	FS.fdSet[filedNo - 1].content.length = contentLen;
	return 0;
}

