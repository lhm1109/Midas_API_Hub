#if !defined(AFX_RptSteelGirder_CSA_S6_H__)
#define AFX_RptSteelGirder_CSA_S6_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder_AASHTO12.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptSteelGirder_CSA_S6 : public CRptSteelGirder_AASHTO12
{
public:
	CRptSteelGirder_CSA_S6();
	virtual ~CRptSteelGirder_CSA_S6();

	BOOL Execute_CSGReport_CSA_S6(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);

	void SetRptData(ElemPairK ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_MEMB_RES_CSA_S6_D& RptData);
	BOOL GetDgnParamOption(CSG_MEMB_RES_CSA_S6_D& rRptData, const T_CPGD_D &CpgdD);
	void ConvertCsgCalcMVLcaseData(double dW, int nLane, CSG_LCOM_MV_IN &rData);
};

#include "HeaderPost.h"

#endif // !defined(AFX_RptSteelGirder_CSA_S6_H__)
