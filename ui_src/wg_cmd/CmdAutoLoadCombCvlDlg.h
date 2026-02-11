#if !defined(AFX_CMDAUTOLOADCOMBCVLDLG_H__E4F1D694_75A2_488E_B911_B8F1439E70EF__INCLUDED_)
#define AFX_CMDAUTOLOADCOMBCVLDLG_H__E4F1D694_75A2_488E_B911_B8F1439E70EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlDlg.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "LoadCombCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlDlg dialog
class CCmdAutoLoadCombCvlKOREALRFD11Dlg;
class CCmdAutoLoadCombCvlAASHTOLRFD12Dlg;
class CCmdAutoLoadCombCvlCSAPage;
class CCmdAutoLoadCombCvlSP35133302011Page;
class CCmdAutoLoadCombCvlAS51002017Page;
class CCmdAutoLoadCombCvlKSCEUSD10Dlg;
class CCmdAutoLoadCombCvlBD21Page;
class CCmdAutoLoadCombCvlPNS10030Page;
class CCmdAutoLoadCombCvlEURO0Page;
class CCmdAutoLoadCombCvlIRC6LSDPage;
class CCmdAutoLoadCombCvlAASHTOLRFD02Dlg;
class CCmdAutoLoadCombCvlAASHTOLRFD08Dlg;
class CCmdAutoLoadCombCvlIRSPage;
class CCmdAutoLoadCombCvlTMH7Page;
class CCmdAutoLoadCombCvlDlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombCvlDlg(CWnd* pParent = NULL);   // standard constructor

public:
	void SetLcomType(int nLcomType) { m_nLcomType = nLcomType; }

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_CVL_DLG };
	MButton	  m_CurvedBridgeChk;
	CEditUnit	m_wndCulvertV;
	CEditUnit m_wndCulvertH;
	MComboBox	m_StrDlCmb;
	MComboBox	m_StrEpCmb;
	MComboBox	m_SerEpCmb;
	CListCtrl	m_MovingList;
	MComboBox	m_MoveLcase;
	MComboBox	m_Code;
	int		m_nOption;
	int   m_nDesignType;

    CEditUnit m_wndLossFactorTransfer;
	CEditUnit m_wndLossFactorService;
	MButton   m_wndLossFactorChk;
	MButton   m_wndCSAnalysisChk;
	BOOL      m_bAddEnvelope;
	BOOL  m_bIrcServCond;
	BOOL  m_bIrcConsCond;
	int   m_nConstLoad;
	BOOL  m_bJSCEChk1;
	BOOL  m_bJSCEChk2;
	BOOL  m_bJSCEChk3;
	BOOL  m_bJSCEChk4;
	BOOL  m_bJSCEChk5;
	BOOL  m_bJSCEChk6;
	BOOL  m_bJSCEChk7;
	BOOL  m_bJTJD60Capc;
	BOOL  m_bJTJD60Gen;
	BOOL  m_bJTJD60Acdn;
	BOOL  m_bJTJD60Serv;
	BOOL  m_bJTJD60Elst;
	BOOL  m_bTB05Main;//add by maxiao 2007-04-24
	BOOL  m_bTB05MainAddition;
	BOOL  m_bTB05MainSpecial;
	BOOL m_bTB10002_2017Main;
	BOOL m_bTB10002_2017MainAddition;
	BOOL m_bTB10002_2017MainSpecial;
	BOOL m_bCJJ166_2011Capc;//add by maxiao 2012-03-01
	BOOL m_bCJJ166_2011Gen;
	BOOL m_bCJJ166_2011Acdn;
	BOOL m_bCJJ166_2011Serv;
	BOOL m_bCJJ166_2011Elst;

	BOOL m_bJTG15Capc          ;//add by dongqiufeng 2012-03-01 承载能力
	BOOL m_bJTG15Gen           ;//基本组合
	BOOL m_bJTG15Acdn          ;//偶然组合
	BOOL m_bJTG15Serv          ;//正常使用
	BOOL m_bJTG15Elst          ;//弹性阶段
	BOOL m_bJTG15_Freq         ;//频遇值
	BOOL m_bJTG15_QuasiPerm    ;//准永久值
	BOOL m_bJTG15_FreqComb     ;//频遇组合
	BOOL m_bJTG15_QuasiPermComb;//准永久组合
	BOOL m_bJTG15_StandComb    ;//标准组合
	BOOL m_bJTG15_Factor       ;//调整系数  IDC_CMD_LOADCOM_AUTO_FACTOR_EDT
	BOOL m_bJTG15FoundationChk ;//基础验算
	BOOL m_bJTG15FdnCommonChk  ;//基础验算-通用组合
	BOOL m_bJTG15FdnCommonBasicChk;//基础验算-基本组合
	BOOL m_bJTG15FdnCommonAccChk  ;//基础验算-偶然组合
	BOOL m_bJTG15FdnCommonFreqChk ;//基础验算-频遇组合
	BOOL m_bJTG15FdnCommonPermChk ;//基础验算-准永久组合

	BOOL m_bIS456_Cvl; // IS456:2000

	TLCOMESCGB19D m_tLcomESCGB19;//工程结构通用规范

public:  
	CMap<int,int,int,int> m_MovingData;    
	CMap<int, int, int, int> m_lsdata;
public:
	BOOL OnInitDialog() override;

	void InitChildDialog(const CString& strCodeName) const;
	void Initial_Data();
	void Initial_CtrlData();
	void Initial_MovingLoadCaseComboBox();  
	void SetLoadCombSelData();
	void AlignControl();
	void MoveJTG15Widgets();
	void RecoverJTG15Widgets();
	void Show_MoveEditComboBox(BOOL bCheck);  
	
	void Initial_MoveListBox();
	void Write_MoveListBox(int Index, CString strLoadCase);
	
	BOOL CheckMovingCaseName(CString str,int& Index);
	BOOL ErrorCheckPS();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void CtrlManager();

private:
    UINT GetMovingKey(CString& strName);

protected:
	CDBDoc* m_pDoc;
	CString m_strCodeName;
	BOOL    m_bInit;
	CArray<UINT, UINT> m_aCtrlMoveCase;
	CArray<UINT, UINT> m_aCtrlAashto;
	CArray<UINT, UINT> m_aCtrlOkCancel;  
	CArray<UINT, UINT> m_aCtrlPS;
	CArray<UINT, UINT> m_aCtrlCS;
	CArray<UINT, UINT> m_aCtrlIRC6_2000;
	CArray<UINT, UINT> m_aCtrlJSCE02;
	CArray<UINT, UINT> m_aCtrlJTJD60;
	CArray<UINT, UINT> m_aCtrlTB05;
	CArray<UINT, UINT> m_aCtrlTB10002_2017;
	CArray<UINT, UINT> m_aCtrlAashtoLrfd08FoldGroup;
	CArray<UINT, UINT> m_aCtrlKOREALRFD11;
	CArray<UINT, UINT> m_aCtrlAASHTOLRFD12;
	CArray<UINT, UINT> m_aCtrlCSA;
	CArray<UINT, UINT> m_aCtrlCSA14;
	CArray<UINT, UINT> m_aCtrlSNiP;
	CArray<UINT, UINT> m_aCtrlSP3511;
	CArray<UINT, UINT> m_aCtrlCJJ166_2011;//add by maxiao 2012-2-29
	CArray<UINT, UINT> m_aCtrlJTG15;//add by dongqiufeng 2015-10-15
	CArray<UINT, UINT> m_aCtrlJTG15_Sup;//super structure
	CArray<UINT, UINT> m_aCtrlJTG15_Sub;//sub structure
	CArray<UINT, UINT> m_aCtrlESCGB19;//add by gongxing 2020-03-17
	CArray<UINT, UINT> m_aCtrlSTL_JTG15; // added by qiangeng 2019-1-18  钢结构
	CArray<UINT, UINT> m_aCtrlPNS10030;
	CArray<UINT, UINT> m_aCtrlAS5100;
	CArray<UINT, UINT> m_aCtrlKSCEUSD10;
    CArray<UINT, UINT> m_aCtrlBD21;
    CArray<UINT, UINT> m_aCtrlEC4;
    CArray<UINT, UINT> m_aCtrlIRC6_LSD;//PinakinIRC6LSD
    CArray<UINT, UINT> m_aCtrlAASHTOLRFD02;
    CArray<UINT, UINT> m_aCtrlAASHTOLRFD08;
    CArray<UINT, UINT> m_aCtrlIRS;
    CArray<UINT, UINT> m_aCtrlTMH7;

	CCmdAutoLoadCombCvlKOREALRFD11Dlg*    m_pKoreaLRFD11;
	CCmdAutoLoadCombCvlAASHTOLRFD12Dlg*   m_pAASHTOLRFD12;
	CCmdAutoLoadCombCvlCSAPage*           m_pCSA;
	CCmdAutoLoadCombCvlCSAPage*           m_pCSA14;
	CCmdAutoLoadCombCvlSP35133302011Page* m_pSNiP;
	CCmdAutoLoadCombCvlSP35133302011Page* m_pSP3511;
	CCmdAutoLoadCombCvlAS51002017Page*	  m_pAS5100;
	CCmdAutoLoadCombCvlKSCEUSD10Dlg*      m_pKSCEUSD10;
    CCmdAutoLoadCombCvlBD21Page*          m_pBD21;  
    CCmdAutoLoadCombCvlPNS10030Page*      m_pPNS10030;  
    CCmdAutoLoadCombCvlEURO0Page*         m_pEURO0;  
    CCmdAutoLoadCombCvlIRC6LSDPage*       m_pIRC6LSD;
    CCmdAutoLoadCombCvlAASHTOLRFD02Dlg*   m_pAASHTOLRFD02;
    CCmdAutoLoadCombCvlAASHTOLRFD08Dlg*   m_pAASHTOLRFD08;
    CCmdAutoLoadCombCvlIRSPage*           m_pIRS;  
	CCmdAutoLoadCombCvlTMH7Page*          m_pTMH7;

	CArray<T_MVLD_K, T_MVLD_K> m_aMvldChar;
	CArray<T_MVLD_K, T_MVLD_K> m_aMvldFreq;
    CArray<T_STLD_K, T_STLD_K> m_aIRSUlt;
    CArray<T_STLD_K, T_STLD_K> m_aIRSServ;
	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlDlg)

	afx_msg void OnCmdLoadcombAdd();
	afx_msg void OnCmdLoadcombReplace();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangePage();
	afx_msg void OnSelchangeLoadcombCode();
	afx_msg void OnCmdLcomSelBtn();
	afx_msg void OnCmdMoveAdd();
	afx_msg void OnCmdMoveDel();
	afx_msg void OnSelchangeCmdAashtoSerEpCmb();
	afx_msg void OnCmdDesignTypeRdo();
	afx_msg void OnCmdLossFactorChk();
	afx_msg void OnCmdLossFactorBtn();
	afx_msg void OnCmdConstStageRdo();
	afx_msg void OnCmdJTJD60CapcChk();
	afx_msg void OnCmdCJJ166_2011CapcChk();

	//}}AFX_MSG
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
private:
	int m_nLcomType;

	BOOL m_bUlti1;
	BOOL m_bUlti2;
	BOOL m_bUlti3;
	BOOL m_bUlti4;
	BOOL m_bUlti5;
	BOOL m_bExtr1;
	BOOL m_bExtr2;
	BOOL m_bServ1;
	BOOL m_bServ2;
	BOOL m_bServ3;
	BOOL m_bServ4;
	BOOL m_bServ5;
	BOOL m_bFati1;
public:
	afx_msg void OnBnClickedJtg15FoundationChk(void);
	afx_msg void OnBnClickedJtg15FoundationCommonChk(void);
	afx_msg void OnBnClickedJtg15CapcChk();
	afx_msg void OnBnClickedJtg15AcdnChk();
	afx_msg void OnBnClickedJtg15ServChk();
	afx_msg void OnBnClickedJtg15ElstChk();
	afx_msg void OnBnClickedJtg15FreqChk();
	afx_msg void OnBnClickedJtg15QuasipermChk();
	afx_msg void OnBnClickedCmdLoadcomAutoServiceLife();//add by dongqiufeng 2015-10-15
	afx_msg void OnBnClickedCmdAashtoLrfd08DeformationFrame();
	afx_msg void OnStnClickedMmbTxt();
	afx_msg void OnBnClickedDMaxRdo();

	afx_msg void OnBnClickedESCGB19CapcChk();
	afx_msg void OnBnClickedESCGB19AcdnChk();
	afx_msg void OnBnClickedESCGB19ServChk();
	afx_msg void OnBnClickedESCGB19ElstChk();
	afx_msg void OnBnClickedESCGB19FreqChk();
	afx_msg void OnBnClickedESCGB19QuasipermChk();
	afx_msg void OnBnClickedCmdLoadcomESCGB19ServiceLife();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDAUTOLOADCOMBCVLDLG_H__E4F1D694_75A2_488E_B911_B8F1439E70EF__INCLUDED_)
