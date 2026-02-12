#ifndef __DB_ST_DT_APPRAISAL_H__
#define __DB_ST_DT_APPRAISAL_H__

#define MIDAS_SDK


#pragma pack(push, 1)
#pragma warning ( disable : 4819 )
#pragma warning ( disable : 4244 )

/************************************************************************/
/*      Total Appraisal Information:T_TAIF_D							*/
/************************************************************************/

#define T_TAIF_K unsigned int
#define HASHSIZETAIF 1
struct T_TAIF_D
{
	CString strAppraisalCode;
	int nBldType;
	int nStructType;
	int nLayer;
	double dMainGradeA;
	double dMainGradeB;
	double dMainGradeC;
	double dSecondaryGradeA;
	double dSecondaryGradeB;
	double dSecondaryGradeC;
	int nMainLmt;
	int nGeneLmt;
	void Initialize()
	{
		strAppraisalCode.Empty();
		nBldType	 = 0;
		nStructType = 0;
		nLayer		= 0;
		dMainGradeA = 1.00;
		dMainGradeB = 0.95;
		dMainGradeC = 0.88;
		dSecondaryGradeA = 1.0;
		dSecondaryGradeB = 0.92;
		dSecondaryGradeC = 0.85;
		nMainLmt		 = 0;
		nGeneLmt		 = 0;
	}
};
struct T_TAIF_D_CH
{
	char strAppraisalCode[40];
	int nBldType;
	int nStructType;
	int nLayer;
	double dMainGradeA;
	double dMainGradeB;
	double dMainGradeC;
	double dSecondaryGradeA;
	double dSecondaryGradeB;
	double dSecondaryGradeC;
	int nMainLmt;
	int nGeneLmt;

	void ConvertToString(T_TAIF_D& rData)
	{
		ConvertCharStr(strAppraisalCode, rData.strAppraisalCode, sizeof(strAppraisalCode));
		rData.nBldType = nBldType;
		rData.nStructType = nStructType;
		rData.nLayer = nLayer;
		rData.dMainGradeA = dMainGradeA;
		rData.dMainGradeB = dMainGradeB;
		rData.dMainGradeC = dMainGradeC;
		rData.dSecondaryGradeA = dSecondaryGradeA;
		rData.dSecondaryGradeB = dSecondaryGradeB;
		rData.dSecondaryGradeC = dSecondaryGradeC;
		rData.nMainLmt = nMainLmt;
		rData.nGeneLmt = nGeneLmt;
	}
	void ConvertToChar(T_TAIF_D& rData)
	{
		ConvertStrChar(rData.strAppraisalCode, strAppraisalCode, sizeof(strAppraisalCode));
		nBldType = rData.nBldType;
		nStructType = rData.nStructType;
		nLayer = rData.nLayer;
		dMainGradeA = rData.dMainGradeA;
		dMainGradeB = rData.dMainGradeB;
		dMainGradeC = rData.dMainGradeC;
		dSecondaryGradeA = rData.dSecondaryGradeA;
		dSecondaryGradeB = rData.dSecondaryGradeB;
		dSecondaryGradeC = rData.dSecondaryGradeC;
		nMainLmt = rData.nMainLmt;
		nGeneLmt = rData.nGeneLmt;
	}
};
struct T_TAIF_UDRD_D
{
	T_TAIF_D data;
};

/************************************************************************/
/*      Important element:T_IMEL_D										*/
/************************************************************************/
#define T_IMEL_K T_ELEM_K
#define HASHSIZEIMEL HASHSIZEELEM
struct T_IMEL_D
{
	BOOL bImportant;

	void Initialize()
	{
		bImportant = FALSE;
	}
	
};
struct T_IMEL_UDRD_D
{
	T_IMEL_K key;
	T_IMEL_D data;
};

struct T_IMEL_UNIT
{
	T_IMEL_UNIT()
	{
	
	}
	
};

/************************************************************************/
/*      Design Steel Appraisal Security:T_DSAS_D						*/
/************************************************************************/
#define T_DSAS_K T_ELEM_K
#define HASHSIZEDSAS HASHSIZEELEM
struct T_DSAS_D
{
	int GetLevel()const
	{
		int nMaxLevel = -1;
		for (int i = 0; i < 4; i++)
		{
			nMaxLevel = nMaxLevel > nEvaluateLevel[i] ? nMaxLevel : nEvaluateLevel[i];
		}
		return nMaxLevel;
	}
	
	/*0:承载能力评定等级*/
	/*1:构造评定等级*/
	/*2:不适于承载的位移或变形评定等级 (仅当民用建筑时有效)*/
	/*3:其他评定等级(仅当民用建筑时有效)*/
	int nEvaluateLevel[4];

	void Initialize()
	{
		for (int i = 0; i < 4; i++)
		{
			nEvaluateLevel[i] = -1;
		}
	}

};
struct T_DSAS_UDRD_D
{
	T_DSAS_K key;
	T_DSAS_D data;
};

/************************************************************************/
/*      Design Steel Appraisal Usability:T_DSAU_D						*/
/************************************************************************/
#define T_DSAU_K T_ELEM_K
#define HASHSIZEDSAU HASHSIZEELEM
struct T_DSAU_D
{
	/*0-4:变形,偏差,一般构造,腐蚀,其他*/
	int nEvaluateLevel[5];

	int GetLevel()const
	{
		int nMaxLevel = -1;
		for (int i = 0; i < 5; i++)
		{
			nMaxLevel = nMaxLevel > nEvaluateLevel[i] ? nMaxLevel : nEvaluateLevel[i];
		}
		return nMaxLevel;
	}

	void Initialize()
	{
		for (int i = 0; i < 5; i++)
		{
			nEvaluateLevel[i] = -1;
		}
	}


};
struct T_DSAU_UDRD_D
{
	T_DSAU_K key;
	T_DSAU_D data;
};

#pragma pack(pop)
#endif 




