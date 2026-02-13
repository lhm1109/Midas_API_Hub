#pragma once

#if !defined(_RATING_REPORT_DATACTRL_H__)
#define _RATING_REPORT_DATACTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum enNRLiveLoadType;
struct CSG_MEMB_POSD_KSCE_LSD;
struct CSG_MEMBPOS_RES_CS457_D;
struct CSG_MEMBPOS_RES_NR_GN_CIV_025_D;
struct CSG_CHECK_RES_CS457;
struct CSG_CHECK_RES_NR_GN_CIV_025;
struct CSG_RPT_CS457_POS;
struct CSG_EFF_SECT_FLANGE_INFO;
struct CSG_RPT_CS457_D;


class CDBDoc;
class CAttrCtrl;
class CAttrCtrl2;
class CPostCtrl;
class CCRCDataCtrl;
class CAnalysisResult;

class CRatingReportDataCtrl
{
public:
	CRatingReportDataCtrl(CCRCDataCtrl* pDataCtrl, int nDgnCode);
	virtual ~CRatingReportDataCtrl();

public:
	bool Get_CS457_DetailResult(const ArrElemPairKey& aPrtElemK, CSG_RPT_CS457_D& rData);

#pragma region /// for CS457, NR2006

private:
	bool Get_CS457_DetailFlex(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_CS457_DetailShear(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_CS457_DetailCombinedMV(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_CS457_DetailLongShear(BOOL bULS, ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_CS457_DetailFlange4Box(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_CS457_DetailFlangeStiffener4Box(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_CS457_DetailWebYieldBuckling4Box(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);

	bool Get_CS457_DetailSLS(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS& rData);
#pragma endregion

#pragma region /// for NR2006
	bool Get_NR2006_DetailFlex(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailShear(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailFastener(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailCombinedMV(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailLongShear(BOOL bULS, ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailBearingStiffner(ElemPairK ElemK, const int& nPos, const enNR_TS_Check& enTSType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailTransveer(T_ELEM_K ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	bool Get_NR2006_DetailWebYieldBuckling4Box(T_ELEM_K ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, CSG_RPT_CS457_POS& rData);
	
#pragma endregion

protected:
	int GetSectPos4MyMax(int nPos4Mymax);
	enNRLiveLoadType GetNRLiveLoadType(int nLiveType);

private:
	void ConvertCsgDgnForce(T_ASCD_CSG_BS_BASE& TComD, CSG_MEMB_POSD_KSCE_LSD& rData);
	void ConvertCsgDngReaction(ElemPairK ElemK, T_ALCS_K AlcsK, int nPosIJ, int nMaxMinType, CSG_MEMB_POSD_KSCE_LSD& rData);
	CString GetRatingCaseName(T_ALCS_K AlcsK);

private:
	CDBDoc* m_pDoc;
	CAttrCtrl* m_pAttrCtrl;
	CAttrCtrl2* m_pAttrCtrl2;
	CPostCtrl* m_pPostCtrl;
	CCRCDataCtrl* m_pDataCtrl;
	CAnalysisResult* m_pDgnResult;
	int m_nDgnCode;
};

#endif // !defined(_RATING_REPORT_DATACTRL_H__)