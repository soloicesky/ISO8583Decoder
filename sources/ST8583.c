#include "ST8583.h"
#include "Utils.h"
#include "Pack8583.h"

//static FdContent fc;



FIELD fieldS[] = { { 1, { N, MAX_MSGCODE_LEN, FIX, L },  },  //message code
		{ 2, { N, MAX_PAN_LEN, VAR, LL },  }, //Primary Account Number
		{ 3, { N, MAX_TPC_LEN, FIX, L },  },  //Transaction Processing Code
		{ 4, { N, MAX_AOT_LEN, FIX, LL },  }, //Amount, transaction
		{ 5, { N, MAX_AOS_LEN, FIX, LL },  }, //Amount, settlement

		{ 6, { N, MAX_AOCB_LEN, FIX, LL },  },  //Amount, cardholder billing
		{ 7, { N, MAX_TDT_LEN, FIX, LL },  }, //Transmission date & time
		{ 8, { N, MAX_TPC_LEN, FIX, L },  },  //Amount, cardholder billing fee
		{ 9, { N, MAX_CROS_LEN, FIX, L },  }, //Conversion rate, settlement
		{ 10, { N, MAX_CROCB_LEN, FIX, LL },  }, //Conversion rate, cardholder billing

		{ 11, { N, MAX_STAN_LEN, FIX, L },  },  //System trace audit number
		{ 12, { N, MAX_TOLT_LEN, VAR, LL },  }, //Time, local transaction (hhmmss)
		{ 13, { N, MAX_DOLT_LEN, FIX, L },  }, //Date, local transaction (MMDD)
		{ 14, { N, MAX_DOE_LEN, FIX, L },  }, //Date, expiration
		{ 15, { N, MAX_DOS_LEN, FIX, L },  }, //Date, settlement

		{ 16, { N, MAX_DOC_LEN, FIX, L },  },  //Date, conversion
		{ 17, { N, MAX_DOCP_LEN, FIX, L },  }, //Date, capture
		{ 18, { N, MAX_MT_LEN, FIX, L },  },  //Merchant type
		{ 19, { N, MAX_AICC_LEN, FIX, L },  }, //Acquiring institution country code
		{ 20, { N, MAX_PCC_LEN, FIX, L },  }, //PAN extended, country code

		{ 21, { N, MAX_FICC_LEN, FIX, L },  }, //Forwarding institution. country code
		{ 22, { N, MAX_POSEM_LEN, FIX, L },  }, //Point of service entry mode
		{ 23, { N, MAX_PANSN_LEN, FIX, L },  }, //Application PAN sequence number
		{ 24, { N, MAX_FC_LEN, FIX, L },  }, //Function code (ISO 8583:1993)/Network International identifier (NII)
		{ 25, { N, MAX_POSCC_LEN, FIX, L },  }, //Point of service condition code

		{ 26, { N, MAX_POSPCC_LEN, FIX, L },  }, //Point of service capture code
		{ 27, { N, MAX_AIRL_LEN, FIX, L },  }, //Authorizing identification response length
		{ 28, { N, MAX_AOTF_LEN, FIX, L },  },  //Amount, transaction fee
		{ 29, { N, MAX_AOSF_LEN, FIX, L },  }, //Amount, settlement fee
		{ 30, { N, MAX_AOTPF_LEN, FIX, LL },  }, //Amount, transaction processing fee

		{ 31, { N, MAX_AOSPF_LEN, FIX, L },  }, //	Amount, settlement processing fee
		{ 32, { N, MAX_AIIC_LEN, VAR, LL },  }, //Acquiring institution identification code
		{ 33, { N, MAX_FIIC_LEN, FIX, L },  }, //Forwarding institution identification code
		{ 34, { N, MAX_PANEX_LEN, VAR, LL },  }, //Primary account number, extended
		{ 35, { Z, MAX_T2_LEN, VAR, LL },  }, //Track 2 data

		{ 36, { Z, MAX_T3_LEN, VAR, LLL },  },  //Track 3 data
		{ 37, { AN, MAX_RRN_LEN, FIX, LL },  }, //Retrieval reference number
		{ 38, { N, MAX_AIR_LEN, FIX, L },  }, //Authorization identification response
		{ 39, { AN, MAX_RC_LEN, FIX, L },  }, //Response code
		{ 40, { N, MAX_SRC_LEN, FIX, LL },  }, //Service restriction code

		{ 41, { ANS, MAX_CATI_LEN, FIX, L },  }, //	Card acceptor terminal identification
		{ 42, { ANS, MAX_CAIC_LEN, FIX, L },  }, //Card acceptor identification code
		{ 43, { N, MAX_CANL_LEN, FIX, L },  }, //Card acceptor name/location (1-23 address 24-36 city 37-38 state 39-40 country)
		{ 44, { AN, MAX_ARD_LEN, VAR, LL },  }, //Additional response data
		{ 45, { N, MAX_T1_LEN, VAR, LL },  }, //Track 1 data

		{ 46, { N, MAX_ADISO_LEN, VAR, LLL },  },  //Additional data - ISO
		{ 47, { N, MAX_ADN_LEN, VAR, LLL },  }, //Additional data - national
		{ 48, { N, MAX_AIR_LEN, VAR, LLL },  },  //Additional data - private
		{ 49, { N, MAX_CCOT_LEN, FIX, L },  }, //Currency code, transaction
		{ 50, { N, MAX_CCOS_LEN, FIX, L },  }, //Currency code, settlement

		{ 41, { N, MAX_CATI_LEN, FIX, L },  }, //	Card acceptor terminal identification
		{ 42, { N, MAX_CAIC_LEN, FIX, L },  }, //Card acceptor identification code
		{ 43, { N, MAX_CANL_LEN, FIX, L },  }, //Card acceptor name/location (1-23 address 24-36 city 37-38 state 39-40 country)
		{ 44, { N, MAX_ARD_LEN, VAR, LL },  }, //Additional response data
		{ 45, { N, MAX_T1_LEN, VAR, LL },  }, //Track 1 data

		{ 46, { N, MAX_ADISO_LEN, VAR, LLL },  },  //Additional data - ISO
		{ 47, { N, MAX_ADN_LEN, VAR, LLL },  }, //Additional data - national
		{ 48, { N, MAX_AIR_LEN, VAR, LLL },  },  //Additional data - private
		{ 49, { AN, MAX_CCOT_LEN, FIX, L },  }, //Currency code, transaction
		{ 50, { N, MAX_CCOS_LEN, FIX, L },  }, //Currency code, settlement

		{ 51, { AN, MAX_CCOCB_LEN, FIX, L },  }, //	Currency code, cardholder billing
		{ 52, { B, MAX_PIND_LEN, FIX, LL },  }, //Personal identification number data
		{ 53, { N, MAX_SRCI_LEN, FIX, LL },  }, //Security related control information
		{ 54, { AN, MAX_AA_LEN, VAR, LLL },  }, //Additional amounts
		{ 55, { B, MAX_RFISO_LEN, VAR, LLL },  }, //Reserved ISO

		{ 56, { ANS, MAX_RFISO2_LEN, VAR, LLL },  },  //Reserved ISO
		{ 57, { ANS, MAX_RFA_LEN, VAR, LLL },  }, //Reserved national
		{ 58, { ANS, MAX_RFA2_LEN, VAR, LLL },  },  //Reserved national
		{ 59, { ANS, MAX_RFAU_LEN, VAR, LLL },  }, //Reserved for national use
		{ 60, { N, MAX_ARC_LEN, VAR, LLL },  }, //Advice/reason code (private reserved)

		{ 61, { AN, MAX_RFRP1_LEN, VAR, LLL },  }, //	Reserved private
		{ 62, { ANS, MAX_RFRP2_LEN, VAR, LLL },  }, //Reserved private
		{ 63, { ANS, MAX_RFRP3_LEN, VAR, LLL },  }, //Reserved private
		{ 64, { B, MAX_MAC_LEN, FIX, L },  }, //Message authentication code (MAC)

		};

FieldSet FS = {
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

