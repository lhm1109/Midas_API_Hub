#include "StdAfx.h"

#include "DgnTendonTemplateFileCtrl.h"
#include "DgnTendonFileStream.h"
#include "DgnTendonTemplateData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FILE_EXT				_T(".tdt")
#define VERSION_INFO_100		_T("v1.00") 

#define TNDN_USE_RATIO			_T("[USE RATIO]")
#define TNDN_START				_T("[TENDON START]")
#define TNDN_END				_T("[TENDON END]")
#define TNDN_SUB_XY				_T("[TENDON SUB XY]")
#define TNDN_SUB_XZ				_T("[TENDON SUB XZ]")
#define TNDN_SUB_IDX			_T("[TENDON SUB IDX]")

#define PUSH_STR(pVector, str)									{ pVector->push_back(str); }
#define PUSH_VAL_STR(pVector, str, strVal, strName)				{ str.Format(_T("%s=%s\n"), strName, strVal);		pVector->push_back(str); }
#define PUSH_VAL_INT(pVector, str, intVal, intName)				{ str.Format(_T("%s=%d\n"), intName, intVal);		pVector->push_back(str); }
#define PUSH_VAL_DOUBLE(pVector, str, doubleVal, doubleName)	{ str.Format(_T("%s=%f\n"), doubleName, doubleVal);	pVector->push_back(str); }
	
CDgnTendonTemplateFileCtrl::CDgnTendonTemplateFileCtrl(void)
{
}


CDgnTendonTemplateFileCtrl::~CDgnTendonTemplateFileCtrl(void)
{
}

BOOL CDgnTendonTemplateFileCtrl::Import(TCHAR* pFilePath, std::vector<TndnTemplate>* pVecTndn, BOOL& bUseRatio)
{
	CTendonFileStream FS;
	if(!FS.ReadOpen(pFilePath))
	{
		ASSERT(0);
		return FALSE;
	}

	int nPos;
	CString strLine;

	while(FS.GetLine(strLine))
	{
		if(strLine.IsEmpty()) continue;

		strLine.TrimLeft();
		strLine.TrimRight();
		strLine.MakeUpper();
		//strLine = strLine.MakeUpper();

		nPos = strLine.Find(TNDN_USE_RATIO);
		if(nPos >= 0) //찾으면 아래로~.
		{
			nPos = strLine.Find(_T("="));
			
			if(nPos < 0)
				continue;
			
			CString strValue;
			strValue = strLine.Right(strLine.GetLength() - nPos - 1);

			bUseRatio = (_ttoi(strValue) == 1);
			continue;
		}

		nPos = strLine.Find(TNDN_START);
		if(nPos < 0) //못찾으면 다음 줄.
		{
			continue;
		}

		TndnTemplate tendonInfo;

		CString strXYZMode = _T("");
		int nIdxXYZ = -1;

		while(FS.GetLine(strLine))
		{
			nPos = strLine.Find(TNDN_END);	//찾으면 해당 while 종료...
			if(nPos >= 0)
			{
				if(IsExist(tendonInfo.strName, pVecTndn))
				{	//이미 존재하는 이름일때는 수정한다.
					GetNewName(tendonInfo.strName, pVecTndn);
				}
				pVecTndn->push_back(tendonInfo);
				break;
			}

			if(strLine.Find(TNDN_SUB_XY) >= 0)			strXYZMode.Format(_T("%s"), TNDN_SUB_XY);
			else if(strLine.Find(TNDN_SUB_XZ) >= 0)		strXYZMode.Format(_T("%s"), TNDN_SUB_XZ);

			nPos = strLine.Find(_T("="));

			if(nPos < 0)
				continue;

			CString strType;
			CString strValue;
			strValue = strLine.Right(strLine.GetLength() - nPos - 1);
			strType = strLine.Left(nPos);

			SetValue(tendonInfo, strType, strValue, strXYZMode, nIdxXYZ);
		}
	}

	return TRUE;
}

BOOL CDgnTendonTemplateFileCtrl::Export(TCHAR* pFilePath, std::vector<TndnTemplate>* pVecTndn, BOOL bUseRatio)
{
	CStdioFile File;

	if(! File.Open(pFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone | CFile::typeText))
	{
		ASSERT(0);
		return FALSE;
	}

	std::vector<CString> vecStr;
	CString strOut;

	PUSH_STR((&vecStr), _T("# \n"));
	PUSH_STR((&vecStr), _T("# Tendon Template Export \n"));
	PUSH_STR((&vecStr), _T("# VER = 1.00 \n"));
	PUSH_STR((&vecStr), _T("# \n\n"));

	PUSH_VAL_INT((&vecStr), strOut, bUseRatio?1:0, TNDN_USE_RATIO);

	for(std::vector<TndnTemplate>::iterator itrVec = pVecTndn->begin(); itrVec != pVecTndn->end(); itrVec++)
	{
		const TndnTemplate& tndnTemplate = *itrVec;

		strOut.Format(_T("%s\n"), TNDN_START);
		PUSH_STR((&vecStr), strOut);

		BOOL bRetVal = ExportSub(tndnTemplate, &vecStr);

		strOut.Format(_T("%s\n\n"), TNDN_END);
		PUSH_STR((&vecStr), strOut);
	}

	for(std::vector<CString>::iterator itr = vecStr.begin(); itr != vecStr.end(); itr++)
	{
		File.WriteString((*itr).GetBuffer((*itr).GetLength()));
	}

	File.Close();

	return TRUE;
}

BOOL CDgnTendonTemplateFileCtrl::ExportSub(const TndnTemplate& tndnTemp, std::vector<CString>* pVecStrOut)
{
	CString strOut;

	PUSH_VAL_STR(pVecStrOut, strOut, tndnTemp.strName,					_T("strName")		);
	PUSH_VAL_INT(pVecStrOut, strOut, tndnTemp.keyGroup,					_T("keyGroup")		);
	PUSH_VAL_INT(pVecStrOut, strOut, tndnTemp.keyProperty,			_T("keyProperty")	);
	PUSH_VAL_DOUBLE(pVecStrOut, strOut,	tndnTemp.dOffsetFront,	_T("dOffsetFront")	);
	PUSH_VAL_DOUBLE(pVecStrOut, strOut,	tndnTemp.dOffsetEnd,		_T("dOffsetEnd")	);
	PUSH_VAL_INT(pVecStrOut, strOut,	tndnTemp.bSameSpanInterval ? 1:0,	_T("bSameSpanLength")	);
	PUSH_VAL_INT(pVecStrOut, strOut,	tndnTemp.bCurveTypeOut ? 1:0,     _T("bCurveTypeOut")	);
	PUSH_VAL_INT(pVecStrOut, strOut,	tndnTemp.nType,					  _T("nType")	);

	for(int i = 0 ; i < 2; i++)
	{
		const TndnTemplateSub* tndnTempSub;
		if(i == 0)
		{
			tndnTempSub = &(tndnTemp.tndnSubXY);
			strOut.Format(_T("%s\n"), TNDN_SUB_XY);
			PUSH_STR(pVecStrOut, strOut);
		}
		else
		{
			tndnTempSub = &(tndnTemp.tndnSubXZ);
			strOut.Format(_T("%s\n"), TNDN_SUB_XZ);
			PUSH_STR(pVecStrOut, strOut);
		}

		if(tndnTempSub->nType == TNDN_TEMPLATE_TYPE_UNKNOWN)
		{
			ASSERT(0);
			return FALSE;
		}

		if(tndnTempSub->bSameSpanInfo)	PUSH_VAL_INT(pVecStrOut, strOut, 1, _T("bSameSpanInfo")	)
		else							PUSH_VAL_INT(pVecStrOut, strOut, 0, _T("bSameSpanInfo")	)
		if(tndnTempSub->bXSymmetry)		PUSH_VAL_INT(pVecStrOut, strOut, 1,   _T("bSymmetry")	)
		else							PUSH_VAL_INT(pVecStrOut, strOut, 0, _T("bSymmetry")	)

		int nSizeSpan = (int)tndnTempSub->aSpanInfo.size();
		if(nSizeSpan != tndnTempSub->nNumOfSpan)
		{
			ASSERT(0);
		}

		PUSH_VAL_INT(pVecStrOut, strOut,	tndnTempSub->nType,			_T("nType")			);
		PUSH_VAL_INT(pVecStrOut, strOut,	nSizeSpan,					    _T("nNumOfSpan")	);
		PUSH_VAL_STR(pVecStrOut, strOut,	tndnTempSub->strSpanLength,	_T("strSpanLength")	);

		for(int j = 0 ; j < nSizeSpan; j++)
		{
			PUSH_VAL_INT	(pVecStrOut, strOut, j, TNDN_SUB_IDX);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDb,		_T("dDb")	);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDb1,	_T("dDb1")	);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDe,		_T("dDe")	);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDf,		_T("dDf")	);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDRad,	_T("dDRad")	);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDt,		_T("dDt")	);
			PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dDt1,	_T("dDt1")	);
						PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dR1,		_T("dR1")	);
						PUSH_VAL_DOUBLE	(pVecStrOut, strOut, tndnTempSub->aSpanInfo[j].dR2,		_T("dR2")	);
		}
	}

	return TRUE;
}

BOOL CDgnTendonTemplateFileCtrl::IsExist(const CString& str, std::vector<TndnTemplate>* pVecTndn)
{
	for(std::vector<TndnTemplate>::iterator itr = pVecTndn->begin(); itr != pVecTndn->end(); itr++)
	{
		if(itr->strName == str)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnTendonTemplateFileCtrl::GetNewName(CString& str, std::vector<TndnTemplate>* pVecTndn)
{
	int nameNum = 1;
	CString strTest;
	while(TRUE)
	{
		strTest.Format(_T("%s_%d"), str, nameNum++);

		if(!IsExist(strTest, pVecTndn))
		{
			str.Format(_T("%s"), strTest);
			break;
		}
	}
}

void CDgnTendonTemplateFileCtrl::SetValue(TndnTemplate& tndnInfo, const CString& strType, const CString& strValue, const CString& strXYZ, int& idx)
{
	TndnTemplateSub* pSubTemplate = NULL;

	BOOL bXYXZ = FALSE;
	if(strXYZ == TNDN_SUB_XY)
	{
		pSubTemplate = &(tndnInfo.tndnSubXY);
		bXYXZ = TRUE;

	}
	else if(strXYZ == TNDN_SUB_XZ)
	{
		pSubTemplate = &(tndnInfo.tndnSubXZ);
		bXYXZ = TRUE;
	}

	if(bXYXZ)
	{
		if(pSubTemplate == NULL)
			return;

		if(strType == _T("bSameSpanInfo"))
		{
			int nVal = _ttoi(strValue);
			if(nVal == 0)		    pSubTemplate->bSameSpanInfo = FALSE;
			else if(nVal == 1)	pSubTemplate->bSameSpanInfo = TRUE;
			else				ASSERT(0);
		}
		else if(strType == _T("bSymmetry"))
		{
			int nVal = _ttoi(strValue);
			if(nVal == 0)		pSubTemplate->bXSymmetry = FALSE;
			else if(nVal == 1)	pSubTemplate->bXSymmetry = TRUE;
			else				ASSERT(0);
		}
		else if(strType == _T("strSpanLength"))		pSubTemplate->strSpanLength = strValue;
		else if(strType == _T("nType"))				pSubTemplate->nType = (TNDN_TEMPLATE_TYPE)_ttoi(strValue); 
		else if(strType == _T("nNumOfSpan"))
		{
			pSubTemplate->nNumOfSpan = _ttoi(strValue);
			pSubTemplate->aSpanInfo.resize(pSubTemplate->nNumOfSpan);
		}
		else if(strType == TNDN_SUB_IDX)			idx = _ttoi(strValue);
		else
		{
			if(idx >= 0 && idx < (int)pSubTemplate->aSpanInfo.size())
			{
				if(strType == _T("dDb"))		pSubTemplate->aSpanInfo[idx].dDb	= _tstof(strValue);
				else if(strType == _T("dDb1"))	pSubTemplate->aSpanInfo[idx].dDb1	= _tstof(strValue);
				else if(strType == _T("dDe"))	pSubTemplate->aSpanInfo[idx].dDe	= _tstof(strValue);
				else if(strType == _T("dDf"))	pSubTemplate->aSpanInfo[idx].dDf	= _tstof(strValue);
				else if(strType == _T("dDRad"))	pSubTemplate->aSpanInfo[idx].dDRad	= _tstof(strValue);
				else if(strType == _T("dDt"))	pSubTemplate->aSpanInfo[idx].dDt	= _tstof(strValue);
				else if(strType == _T("dDt1"))	pSubTemplate->aSpanInfo[idx].dDt1	= _tstof(strValue);
								else if(strType == _T("dR1"))	pSubTemplate->aSpanInfo[idx].dR1	= _tstof(strValue);
								else if(strType == _T("dR2"))	pSubTemplate->aSpanInfo[idx].dR2	= _tstof(strValue);
				else							ASSERT(0);
			}
			else
			{
				ASSERT(0);
			}
		}
	}
	else
	{
		if(strType == _T("strName"))			    	tndnInfo.strName			= strValue;
		else if(strType == _T("keyProperty"))		tndnInfo.keyProperty		= _ttoi(strValue);
		else if(strType == _T("keyGroup"))			tndnInfo.keyGroup			= _ttoi(strValue);
		else if(strType == _T("dOffsetEnd"))		tndnInfo.dOffsetEnd			= _tstof(strValue);
		else if(strType == _T("dOffsetFront"))		tndnInfo.dOffsetFront		= _tstof(strValue);
		else if(strType == _T("bSameSpanLength"))	tndnInfo.bSameSpanInterval	= _ttoi(strValue) == 1;
		else if(strType == _T("bCurveTypeOut"))	tndnInfo.bCurveTypeOut = _ttoi(strValue) == 1;
		else if(strType == _T("nType"))				  tndnInfo.nType				= (UINT)_ttoi(strValue);
		else										ASSERT(0);
	}
}