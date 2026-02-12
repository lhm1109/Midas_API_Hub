#pragma once

#include <vector>
#include "DgnTendonTemplateData.h"
#include "../wg_db/AttrCtrl.h"

typedef CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> caTdnaBase2D;
typedef CArray<double, double>  caDouble;

class CDBDoc;
class CUnitCtrl;
class CDgnTendonElemLengthCtrl;
class CDgnTendonTemplateFileCtrl;
class CDgnTendonAutoGenCtrl;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnTendonTemplateCtrl
{
public:
	CDgnTendonTemplateCtrl(CDBDoc* pAttrCtrl);
	~CDgnTendonTemplateCtrl(void);

	BOOL Import(TCHAR* pFilePath, BOOL bReplace);
	BOOL Export(TCHAR* pFilePath, vecTndnKey* nKeyList, BOOL bUseRatio);
	BOOL AutoGeneration(BOOL bReset, T_TDNT_K tdntKey, T_TDGR_K tdgrKey, const CString& namePrefix, const CString& strCode, const CString& strT, const CString& strN, double* dOrigin);
	BOOL GetAutoGenSectName(CString& strCode, CString& strType, CString& strName);
	BOOL GetTdnaElemType(const TndnTemplate& tndnTemp, T_TDNA_D& tTdna, BOOL bConvertOut, BOOL bMsg = FALSE);
	BOOL GetTdnaCurveType(const TndnTemplate& tndnTemp, T_TDNA_D& tTdna, BOOL bConvertOut, BOOL bMsg = FALSE);
	
	TndnTemplate ConvertUnitIn(const TndnTemplate& data);
	TndnTemplate ConvertUnitOut(const TndnTemplate& data);
	void ConvertUnitIn(CString& strSpanLength);
	void ConvertUnitOut(CString& strSpanLength);
	
	BOOL IsAllSameSect() { return m_bAllSameSect; }
	BOOL IsValidTndnTemplate(const TndnTemplate& tempInfo, BOOL bPrintMsg);
	BOOL IsValidElem(BOOL bPrintMsg);
	BOOL IsCurveElem() { return m_bCurveElem; }
	BOOL IsExistTemplate(CString& strName);
	BOOL IsExistSpanInfo() { return (m_keySpan > 0); }
	BOOL IsSameElemList(const std::vector<T_ELEM_K>& keyList);

	CDgnTendonElemLengthCtrl* GetLengthCtrl()  { return m_lengthCtrl;   }
	CDgnTendonAutoGenCtrl*    GetAutoGenCtrl() { return m_pAutoGenCtrl; }

	double GetTotalLength();
	BOOL GetTendonLength(const TndnTemplate& tndnTemplate, double& dTndnLen, int* errMsg = NULL);
	void GetSameSpanStr(const TndnTemplate& tndnTemplate, BOOL bXY, CString& strSpan);
	void OnchangeTotalLength();
	BOOL GetPosStartEnd(int nKey, double& nPosY_Start, double& nPosZ_Start, double& nPosY_End, double& nPosZ_End);
	BOOL GetPosByElem( T_ELEM_K tElemKey, double dPosElem, double& dPos);
	void GetNewName(const CString& strPrefix, CString& newName);
	void CancelNewNameNum(const CString strPrefix);
	int  GetNewKey(BOOL bWithInc = FALSE);
	int  GetKeyList(vecTndnKey& keyList);
	BOOL GetInitSpanInfo(TndnTemplate& tempInfo);
	BOOL GetDefaultSpanValue(TndnTemplate& tempInfo, BOOL bXY, int nfromIdx, BOOL bDfDe, BOOL bDtDb); //nfromIdx로부터 끝까지 spanInfo를 돌면서 기본값을 체운다.
	BOOL GetTypeName(TNDN_TEMPLATE_TYPE enType, CString& strName);
	BOOL GetProfileDrawInfo( const TndnTemplate& tndnTemplate, BOOL bXY, std::vector<double>& dPosX,
		std::vector<double>& dPosYZ, std::vector<BOOL>& bInputPt );

	BOOL GetTemplateNU(int nKey, TndnTemplate& TempInfo);	
	BOOL GetTemplate(int nKey, TndnTemplate& TempInfo);	
	void AddTemplateNU(const TndnTemplate& TempInfo);
	void AddTemplate(const TndnTemplate& TempInfo);
	void DelTemplate(int nKey);
	void ModTemplateNU(int nKey, TndnTemplate& TempInfo);
	void ModTemplate(int nKey, TndnTemplate& TempInfo);

	int CopyTemplateNU(int nKey, double dOffZIn, double dOffYIn, int nCopyNum, vecTndnTemp& vecTemp, BOOL bWithName = TRUE);
	int CopyTemplateNU(const TndnTemplate& tndnInfo, double dOffZIn, double dOffYIn, int nCopyNum, vecTndnTemp& vecTemp, BOOL bWithName = TRUE);
	void CopyTemplateNU(vecTndnKey keyList, double dOffZIn, double dOffYIn, int nCopyNum, BOOL bWithName = TRUE);

	void ClearTemplate() { m_mapNameSuffix.clear();  m_mapTndnTemplate.clear(); }
	void ClearNameCounter() { m_mapNameSuffix.clear(); }
	void MakeZeroSpanValue(TndnTemplate& TempInfo);
	
	void SetExistSpanInfo();
	void SetAssignElemList(std::vector<T_ELEM_K>& vecElemKey);
	std::vector<T_ELEM_K>* GetAssignElemList() {return &m_vecElemKey; }

	T_SECT_K GetSectKeyByElemsStr(const CString& strElems);

	void PrintMsg(int nErrMsg);


private:
	void MapToVector( mapTndnTemp& mapData, vecTndnTemp& vecData, vecTndnKey* nKeyList = NULL);
	void VectorToMap( mapTndnTemp& mapData, vecTndnTemp& vecData);

	BOOL ConvertRatioLength_DfDe(BOOL bRatio2Len, TndnTemplate& tndnData);

	double GetTdnaSub_Factor(double dPosX, double dFactor, UINT nType, BOOL bPositive, BOOL bSymmetry);
	BOOL GetTdnaSub( UINT nType, const caDouble& caSpanLength, double dOffFront, const TndnTemplateSub* pSub, caTdnaBase2D& aProfile);
	BOOL GetTdnaSub_DtDt1( UINT nType, const CArray<double, double>& caSpanLength, double dOffFront, const TndnTemplateSub* pSub, caTdnaBase2D& aProfile);
	BOOL GetTdnaSub_DtDbDt1( UINT nType, const caDouble& caSpanLength, double dOffFront, const TndnTemplateSub* pSub, caTdnaBase2D& aProfile);
	BOOL GetTdnaSub_DtDbDb1Dt( UINT nType, const caDouble& caSpanLength, double dOffFront, const TndnTemplateSub* pSub, caTdnaBase2D& aProfile );
	BOOL GetTdnaSub_DtDbDb1( UINT nType, const caDouble& caSpanLength, double dOffFront, const TndnTemplateSub* pSub, caTdnaBase2D& aProfile );
	BOOL GetTdnaSub_DbDb1Dt1( UINT nType, const caDouble& caSpanLength, double dOffFront, const TndnTemplateSub* pSub, caTdnaBase2D& aProfile );
	
protected:
	CDgnTendonAutoGenCtrl*		    m_pAutoGenCtrl;

private:
	CDgnTendonElemLengthCtrl*     m_lengthCtrl;
	CDgnTendonTemplateFileCtrl*   m_FileCtrl;  
	
	std::map<CString, UINT>       m_mapNameSuffix;
	std::vector<T_ELEM_K>       m_vecElemKey;

	CAttrCtrl*		m_pAttrCtrl;
	CUnitCtrl*    m_pUnitCtrl;
	CDBDoc*				m_pDoc;

	mapTndnTemp		m_mapTndnTemplate;

	BOOL      m_bCurveElem;
	BOOL      m_bValidElem;
	BOOL      m_bAllSameSect;
	T_SECT_K  m_keySect;
	T_SPAN_K  m_keySpan;

	int       m_nNextKey;
	int       m_nPreErrMsg;
};

#include "HeaderPost.h"
