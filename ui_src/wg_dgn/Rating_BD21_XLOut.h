// Rating_BD21_XLOut.h: interface for the CRating_BD21_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Rating_BD21_XLOut_H__)
#define _Rating_BD21_XLOut_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../wg_base/MSExcel.h"
//#include "Rating_XLOut.h"
#include "Dgn_XLOut.h"


struct PSC_RPT_BS_D;
struct PSC_RPT_BS_POS;

class CCRCDataCtrl;

#include "HeaderPre.h"

struct _T_BD21_XLOUT
{
    ElemPairK ElemK;
    BOOL bIJ[2]; // 0:I, 1:J
    // Rating case °³¼ö * 12 (concurrent force)
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mPosiFlex;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mNegaFlex;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mShear;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mTorsion;
    CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mSLS;

    void Initialize()
    {
        ElemK = { 0u, 0U };
        bIJ[0] = FALSE;
        bIJ[1] = FALSE;
        mPosiFlex.RemoveAll();
        mNegaFlex.RemoveAll();
        mShear.RemoveAll();
        mTorsion.RemoveAll();
        mSLS.RemoveAll();
    }
    _T_BD21_XLOUT& operator = (const _T_BD21_XLOUT& rData)
    {
        ElemK = rData.ElemK;
        bIJ[0] = rData.bIJ[0];
        bIJ[1] = rData.bIJ[1];
        T_ALCS_K AlcsK;
        BOOL     bChk;
        CopyMap(rData.mPosiFlex, mPosiFlex, AlcsK, bChk);
        CopyMap(rData.mNegaFlex, mNegaFlex, AlcsK, bChk);
        CopyMap(rData.mShear   , mShear   , AlcsK, bChk);
        CopyMap(rData.mTorsion , mTorsion , AlcsK, bChk);
        CopyMap(rData.mSLS     , mSLS     , AlcsK, bChk);

        return *this;
    }
};

class __MY_EXT_CLASS__ CRating_BD21_XLOut : public CDgn_XLOut//public CRating_XLOut
{
public:
    CRating_BD21_XLOut(void);
    virtual ~CRating_BD21_XLOut(void);

    void PrintReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<_T_BD21_XLOUT, _T_BD21_XLOUT&>& arXlout);

    void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);

protected:
    virtual CString GetFlexTableHeadName();
    virtual CString GetShearTableHeadName();
    virtual CString GetAssessFactorContentsName();

private:
    void SetSheetPage(dgn::lib::IExcel* pExcel, CString strNameCurrent);
    void SetSummarySheetPage(dgn::lib::IExcel* pExcel);
    void SetDetailSheetPage(dgn::lib::IExcel* pExcel, const ElemPairK ElemK, const int& iPosi);
    void SetAssessmentSheetPage(dgn::lib::IExcel* pExcel, const ElemPairK ElemK, const int& iPosi);
    void DelBaseSheetPage(dgn::lib::IExcel* pExcel);

    void GenerateSummarySheet   (dgn::lib::IExcel* pExcel, CArray<_T_BD21_XLOUT, _T_BD21_XLOUT&>& arXlout);
    void GenerateAssementSheet  (dgn::lib::IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD);
    void GenerateAssFlexTable   (dgn::lib::IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow);
    void GenerateAssShearTable  (dgn::lib::IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow);
    void GenerateAssTorsionTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow);
    void GenerateAssSLSTable    (dgn::lib::IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow);

    BOOL GenerateDetailRpt(dgn::lib::IExcel* pExcel, CString szProgramDir, CString szProjectFileDir, CString strPath, D_XL_PROGRESS_INTERFACE* pProgDlg);

    BOOL GetDetailResult(const ArrElemPairKey &aPrtElemK, PSC_RPT_BS_D &rData);
    BOOL GetDetailFlex(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, int nPosNeg, PSC_RPT_BS_POS &rData);
    BOOL GetDetailShrr(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData);
    BOOL GetDetailTorr(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData);
    BOOL GetDetailStrs(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData);
    BOOL GetDetailCrck(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData);

    int  GetDetailCount();

protected:
    CCRCDataCtrl* m_pDataCtrl;

};

#include "HeaderPost.h"

#endif // !defined(_Rating_BD21_XLOut_H__)
