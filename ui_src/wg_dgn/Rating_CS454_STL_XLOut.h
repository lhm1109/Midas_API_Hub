#pragma once
// Rating_CS454_XLOut.h: interface for the CRating_CS454_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Rating_CS454_STL_XLOut_H__)
#define _Rating_CS454_STL_XLOut_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Rating_XLOut.h"
#include "Dgn_XLOut.h"

#include "HeaderPre.h"

struct CSG_MEMB_POSD_KSCE_LSD;
struct CSG_MEMBPOS_RES_CS457_D;
struct CSG_CHECK_RES_CS457;
struct CSG_RPT_CS457_POS;
struct CSG_EFF_SECT_FLANGE_INFO;
struct CSG_RPT_CS457_D;

class CCRCDataCtrl;
class CAnalysisResult;

struct _T_CS454_STL_XLOUT
{
    UINT ElemK;
    UINT nBeamType;
    BOOL bIJ[2]; // 0:I, 1:J
    // Rating case °³¼ö * 12 (concurrent force)
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mPosiFlex;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mNegaFlex;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mShear;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mComb;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mTorsion;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mSLS;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mLShearULS;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mLShearSLS;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mFlangeBox;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mFlangeStiff;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mWebYielding;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mWebBuckling;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mWebStiff;

    void Initialize()
    {
        ElemK = 0;
        nBeamType = 0;
        bIJ[0] = FALSE;
        bIJ[1] = FALSE;
        mPosiFlex.RemoveAll();
        mNegaFlex.RemoveAll();
        mShear.RemoveAll();
        mComb.RemoveAll();
        mTorsion.RemoveAll();
        mSLS.RemoveAll();
        mLShearULS.RemoveAll();
        mLShearSLS.RemoveAll();
        mFlangeBox.RemoveAll();
        mFlangeStiff.RemoveAll();
        mWebYielding.RemoveAll();
        mWebBuckling.RemoveAll();
        mWebStiff.RemoveAll();
    }
    _T_CS454_STL_XLOUT() { Initialize(); }
    _T_CS454_STL_XLOUT& operator = (const _T_CS454_STL_XLOUT& rData)
    {
        ElemK = rData.ElemK;
        nBeamType = rData.nBeamType;
        bIJ[0] = rData.bIJ[0];
        bIJ[1] = rData.bIJ[1];
        T_ALCS_K AlcsK;
        BOOL     bChk;
        CopyMap(rData.mPosiFlex, mPosiFlex, AlcsK, bChk);
        CopyMap(rData.mNegaFlex, mNegaFlex, AlcsK, bChk);
        CopyMap(rData.mShear, mShear, AlcsK, bChk);
        CopyMap(rData.mComb, mComb, AlcsK, bChk);
        CopyMap(rData.mTorsion, mTorsion, AlcsK, bChk);
        CopyMap(rData.mSLS, mSLS, AlcsK, bChk);
        CopyMap(rData.mLShearULS, mLShearULS, AlcsK, bChk);
        CopyMap(rData.mLShearSLS, mLShearSLS, AlcsK, bChk);
        CopyMap(rData.mFlangeBox, mFlangeBox, AlcsK, bChk);
        CopyMap(rData.mFlangeStiff, mFlangeStiff, AlcsK, bChk);
        CopyMap(rData.mWebYielding, mWebYielding, AlcsK, bChk);
        CopyMap(rData.mWebBuckling, mWebBuckling, AlcsK, bChk);
        CopyMap(rData.mWebStiff, mWebStiff, AlcsK, bChk);

        return *this;
    }
	ElemPairK GetElemPairK() const
	{
		return ElemPairK(ElemK, nBeamType);
	}
};

class __MY_EXT_CLASS__ CRating_CS454_STL_XLOut : public CDgn_XLOut//CRating_XLOut
{
public:
    CRating_CS454_STL_XLOut(void);
    virtual ~CRating_CS454_STL_XLOut(void);

    void PrintReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<_T_CS454_STL_XLOUT, _T_CS454_STL_XLOUT&>& arXlout);

    void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);

protected:
    virtual CString GetFlexTableHeadName();
    virtual CString GetShearTableHeadName();
    virtual CString GetAssessFactorContentsName();

private:
    void SetSheetPage(dgn::lib::IExcel* pExcel, CString strNameCurrent);
    void SetSummarySheetPage(dgn::lib::IExcel* pExcel);
    void SetDetailSheetPage(dgn::lib::IExcel* pExcel, const T_ELEM_K ElemK, const int& iPosi);
    void SetAssessmentSheetPage(dgn::lib::IExcel* pExcel, const T_ELEM_K ElemK, const int& iPosi);
    void DelBaseSheetPage(dgn::lib::IExcel* pExcel);

    void GenerateSummarySheet(dgn::lib::IExcel* pExcel, CArray<_T_CS454_STL_XLOUT, _T_CS454_STL_XLOUT&>& arXlout);
    void GenerateAssementSheet(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD);
    void GenerateAssFlexTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssShearTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssMomentShearTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssTorsionTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssSLSTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssLongitudinalShearTable(dgn::lib::IExcel* pExcel, const bool& bULS, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssBoxFlangeTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssBoxFlangeStiffenerTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssWebYieldingTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssWebBucklingTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);
    void GenerateAssWebStiffenerTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow);    
    

    BOOL GenerateDetailRpt(dgn::lib::IExcel* pExcel, CString szProgramDir, CString szProjectFileDir, CString strPath, D_XL_PROGRESS_INTERFACE* pProgDlg);

    BOOL GetDetailResult(const ArrElemPairKey& aPrtElemK, CSG_RPT_CS457_D &rData);
    BOOL GetDetailFlex(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    BOOL GetDetailShear(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    BOOL GetDetailCombinedMV(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    BOOL GetDetailLongShear(BOOL bULS, ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    BOOL GetDetailFlange4Box(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    BOOL GetDetailFlangeStiffener4Box(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    BOOL GetDetailWebYieldBuckling4Box(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);

    BOOL GetDetailSLS(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData);
    //     BOOL GetDetailStrs(T_ELEM_K ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData);
    //     BOOL GetDetailCrck(T_ELEM_K ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData);

    int  GetDetailCount();

    void ConvertCsgDgnForce(T_ASCD_CSG_BS_BASE& TComD, CSG_MEMB_POSD_KSCE_LSD& rData);
    CString GetRatingCaseName(T_ALCS_K AlcsK);

	void GetReportElemPairKey(ArrElemPairKey& aElemPairKey);
    BOOL GetPrintElemData(ElemPairK EPairK, T_SRAS_D& SrasD, T_SRAR_D& SrarD);

protected:
    CDBDoc *m_pDoc;
    CCRCDataCtrl* m_pDataCtrl;

    CAnalysisResult* m_pDgnResult;

private:
    int GetConcurrentSize();

};

#include "HeaderPost.h"

#endif // !defined(_Rating_CS454_STL_XLOut_H__)
