#if !defined (__ETC_DESIGN_SPECT_H__)
#define __ETC_DESIGN_SPECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyChildDialog.h"
#include "..\wg_base\DlgReplaceMgr.h"
#include "..\wg_base\MySRGraph.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "HeaderPre.h"

enum EN_DESIGN_SPECTRA_SHOWMODE
{
    EN_MODE_RSFUNC, EN_MODE_PO_FEMA440, EN_MODE_PO_ATC
};

enum EN_CHILD_DIALOG_ORDER
{
    EN_CHILD_ORDER_KS = 0,
    EN_CHILD_ORDER_KS2000,
    EN_CHILD_ORDER_KS_BRG,
    EN_CHILD_ORDER_UBC88,
    EN_CHILD_ORDER_UBC97,
    EN_CHILD_ORDER_EURO1996,
    EN_CHILD_ORDER_CH2002,
    EN_CHILD_ORDER_IBC2000,
    EN_CHILD_ORDER_JPN2000,
    EN_CHILD_ORDER_CH_BRG89,
    EN_CHILD_ORDER_CH_GBJ111_87,
    EN_CHILD_ORDER_NBC95,
    EN_CHILD_ORDER_EURO1996_ELA,
    EN_CHILD_ORDER_IS2002,
    EN_CHILD_ORDER_TAIWAN99H,
    EN_CHILD_ORDER_TAIWAN99V,
    EN_CHILD_ORDER_TAIWAN89H_BRG,
    EN_CHILD_ORDER_TAIWAN89V_BRG,
    EN_CHILD_ORDER_JP_BRG2002,
    EN_CHILD_ORDER_KBC2005,
    EN_CHILD_ORDER_CHSH2003,
    EN_CHILD_ORDER_EURO2004,
    EN_CHILD_ORDER_TAIWAN06,
    EN_CHILD_ORDER_GB50111_2006,
    EN_CHILD_ORDER_JTJXXX_2008,
    EN_CHILD_ORDER_KBC2009,
    EN_CHILD_ORDER_USER,
    EN_CHILD_ORDER_TAIWANBRG98,
    EN_CHILD_ORDER_CH2010,
    EN_CHILD_ORDER_CJJ166_2011,
    EN_CHILD_ORDER_IBC2009,
    EN_CHILD_ORDER_IBC2012,
    EN_CHILD_ORDER_AASHTO_LRFD12,
    EN_CHILD_ORDER_NSR2010,
    EN_CHILD_ORDER_KBC2015,
    EN_CHILD_ORDER_P100_2013,
    EN_CHILD_ORDER_IS1893_2016,
    EN_CHILD_ORDER_KISTEC2013,
    EN_CHILD_ORDER_KSCE2019,
    EN_CHILD_ORDER_NTC2018,
    EN_CHILD_ORDER_DPWH_LRFD_2013,
    EN_CHILD_ORDER_KDS2019,
    EN_CHILD_ORDER_AS_5100_2_2017,
    EN_CHILD_ORDER_CH2019,
    EN_CHILD_ORDER_IRCSP114_2018,
    EN_CHILD_ORDER_JTG_T_2231_01_2020,
    EN_CHILD_ORDER_JP_BRG2012,
    EN_CHILD_ORDER_JP_BRG2017,
    EN_CHILD_ORDER_DPT2018,
    EN_CHILD_ORDER_CH_GB_T_51408_2021,
    EN_CHILD_ORDER_TAIWAN22,
	EN_CHILD_ORDER_RUS_SP2016,
    EN_CHILD_ORDER_NSCP_2024,
    EN_CHILD_ORDER_SANS10160_4_2010,
    EN_CHILD_ORDER_RUS_SP14_2018,
    EN_CHILD_ORDER_IS1893_2025,
};

class __MY_EXT_CLASS__ CETC_DESIGN_SPECT : public CDialogMove
{
public:
    CString m_strFuncName;
    CETC_DESIGN_SPECT(T_SPFC_D*, BOOL bModify, int nCase=0, BOOL bSeismicEvaluation=FALSE, CWnd* pParent = NULL);
    virtual ~CETC_DESIGN_SPECT();   // standard destructor

    void SetShowMode(int nShowMode);

    CMap <int, int, CMyChildDialog*, CMyChildDialog*> m_mapChildDlg;
    CArray <double, double>* m_parPeriod, *m_parAccel;
    CArray <double, double> m_aPOPeriod[5];
    CArray <double, double> m_aPOAccel[5];

    double m_dPODamping[5];

    CString m_sDescript;
    int m_nCurrentDlg;
    int m_nCurrentDlgSeq;
    CString m_strCodeName;
    CString m_strOldCodeName;
    void UpdateCurDlg();
    void CalcCurrentPOSpectra();
    void SetCurrentDlg(int nSpecCodeType);
    void CalcPOSpectrumSRaSRv(double SRa, double SRv, CArray <double, double>& aPODispl, CArray <double, double>& aPOAccel);
    void CalcPOSpectrumByDamping(double dDamp, CArray <double, double>& aPODispl, CArray <double, double>& aPOAccel);
    int GetCodeFromItem(int nItem); // 20071203 mylee - SPFC 코드 저장위해 변환하는 함수

    int	 m_nShowMode; // 0 : by Response Spectrum Function Generation Mode // Default
                      // 1 : by Pushover Analysis Curve Mode
    int  m_nCase;     // 0 : Default, 1 : Elastic Spectrum 삭제, 2 : Design Spectrum 삭제
                      // 1 : by Pushover Analysis Curve Mode
    CWnd* m_pDampingWnd;
    void AlignDialog();

protected:
    int		 m_nSpectralType; // Normal, Acc, Vel, Displ
    CArray<UINT, UINT> m_aCtrlSpectral, m_aCtrlScaling;
    CArray<UINT, UINT> m_aMoveCtrlID;//National Annex Ctrl Hidden / Show 로 인해서 위 아래로 움직일 컨트롤 모음.
    SREGraphView m_GraphView;
    CMySRGraph   m_Graph;

    double m_dMnMxXVal[2];
    double m_dMnMxYVal[2];

public:
    void SpectralDataChanged();
    void NewSpectralData();
    void SetValue(int nIndex, int nGroup, double x, double y);
    void SetAxisLabelText(int nComponent, LPCTSTR lbl);
    BOOL ReadSGSData(FILE *fp);
    BOOL ReadSPDData(FILE *fp);
    int lex(FILE *fp, char *yytext);
    void ChangeDataType(int nType);
    double UnitCvt(double dReadData);
    void SetSeismicEvaluation(BOOL bSet) { m_bSeismicEvaluation = bSet; }

protected:
    void InitChart();
    void EnableGravity(BOOL bEnable);
    BOOL EnableNationalAnnex();

    enum { IDD = IDD_ETC_DESIGN_SPECTRUM };
    MComboBox m_cmbDesignCode;
	MComboBox m_cmbNationalAnnex;

    CEditUnit m_edtScaleFactor;
    CEditUnit m_edtMaxValue;
    CTextUnit m_untMaxValue;
    CEditUnit m_edtGravity;
    CTextUnit m_untGravity;
    CEditUnit m_edtDampRatio;
	MButton   m_chkXLog;
	MButton   m_chkYLog;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy();

protected:
    CArray<int, int>         m_aCodeSeq;
    CArray<int, int>         m_aLockSeq;
    CArray<CString, LPCTSTR> m_aCodeName;
    CArray<UINT, UINT> m_aCtrlMethod;
    CArray<BOOL, BOOL> m_aCodeEnable;
    T_SPFC_D*                m_pParamData;
    T_SPFC_D                 m_TempParamData;    // Cancel 시 복구 용
    CDBDoc* m_pDoc;
    BOOL m_bModify;
    BOOL    m_bInitialCH; // used for CH2002, CHSH2003
                          // TRUE : 초기화가 필요한 상태
    BOOL m_bSeismicEvaluation;

    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnSelchangeEtcGdsSpectrumCmb();
    afx_msg void OnSelchangeCmdNationalAnnex();
    afx_msg void OnCloseupEtcGdsSpectrumCmb();
    virtual void OnCancel();
    afx_msg void OnClose();
    afx_msg void OnETCXlogChk();
    afx_msg void OnETCYlogChk();
    afx_msg void OnETCImportBtn();
    afx_msg void OnChangeDataType();
    afx_msg void OnETCScalingType();
    DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif // !defined(AFX_ETC_DESIGN_SPECT_H__E39085CF_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
