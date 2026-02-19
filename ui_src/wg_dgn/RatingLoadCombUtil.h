#if !defined(__CMRATINGLOADCOMBUTIL_H__)
#define __CMRATINGLOADCOMBUTIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../wg_db/DB_ST_DN.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CRatingLoadCombUtil
{
// Construction
public:
	CRatingLoadCombUtil(){};
	~CRatingLoadCombUtil(){};
		
	static void CopySteelRatingToLCom(CDBDoc* pDoc);	
	static void CopyPSCAssessmentToLCom(CDBDoc* pDoc);
	static void CopySteelAssessmentToLCom(CDBDoc* pDoc);
	static void CopySteelNRToLCom(CDBDoc* pDoc);
	static void CopyPSCRatingToLCom(CDBDoc* pDoc);
	static void CopyRCRatingToLCom(CDBDoc* pDoc);
	static void GetCopiedLcomKFromRatingCase(CArray<T_LCOM_K,T_LCOM_K>& aRatingLcomK, int nMode);
	static int  GetStatusMVStldList(int nMode, CArray<BOOL,BOOL>& aIsMVStld);
	static void CopyPSCAssessmentLCom(CDBDoc* pDoc, CArray<T_ALCS_K, T_ALCS_K>& aAlcsK);	
	static void CopySteelAssessmentLCom(CDBDoc* pDoc, CArray<T_ALCS_K, T_ALCS_K>& aAlcsK);
protected:
	static void GetLComByRlcs(CDBDoc* pDoc, T_RLCS_D &tRlcs, T_LCOM_D& tLcom, int nCopyType);  // nCopyType: 0) Main + Combinaton 1) Main + MV 2) Main + Combinaton + MV
	static void GetLComByAlcs(CDBDoc* pDoc, T_ALCS_D &tAlcs, T_LCOM_D& tLcom, int nCopyType);
	static void GetLComByAlcsNR(CDBDoc* pDoc, T_ALCS_D& tAlcs, T_LCOM_D& tLcom, int nCopyType);
	static void GetLComByRklc(CDBDoc* pDoc, T_RKLC_D &tRklc, T_LCOM_D& tLcom, int nCopyType);  // nCopyType: 0) Main + Combinaton 1) Main + MV 2) Main + Combinaton + MV
	//static void ChangeToUniqueLcomName(CDBDoc* pDoc, CString& strLcomName);
	static void GetLComByGenTypeCombination(CDBDoc* pDoc, CArray<T_LCOM_D, T_LCOM_D&>& aCombination, CString strLComName, CString Description, T_LCOM_D& tLcomOut);
	static BOOL ReplaceLcom(CDBDoc* pDoc, T_LCOM_D& tLcom, int nTransectionType);
	static void GetNewAlcsName(CDBDoc* pDoc, CString strName, CString& strNewName);
};
#include "HeaderPost.h"

#endif // !defined(__CMRATINGLOADCOMBUTIL_H__)
