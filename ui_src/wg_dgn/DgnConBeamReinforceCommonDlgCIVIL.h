#if !defined(__DgnConBeamReinforceCommonDlgCIVIL_H__)
#define __DgnConBeamReinforceCommonDlgCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBeamReinforceCommonDlgCIVIL.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_cmd\wg_cmd_CMSectViewWnd.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "../MIT_frx/MTabCtrl.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamReinforceCommonDlgCIVIL dialog

#include "DgnBeamGridWnd.h"
#include "DgnSectionDrawWnd.h"

#include "HeaderPre.h"

using namespace mit::frx;
class CDgnBeamGridWndCIVIL;
class __MY_EXT_CLASS__ CDgnConBeamReinforceCommonDlgCIVIL : public CDialogMove
{
// Construction
public:
	CDgnConBeamReinforceCommonDlgCIVIL(int nIDD, CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {}
 
// Dialog Data
	//{{AFX_DATA(CDgnConBeamReinforceCommonDlgCIVIL)
	enum { IDD = NULL };  // IDD는 자식껄로 씀
	MButton	    m_btnDel;
	MButton	    m_btnAdd;
	MButton	    m_btnRedraw;
	CListCtrl	  m_wndList;
	mit::frx::MTabCtrl	  m_ctrlTab;

	CTextUnit	m_HfUnit;
	CEditUnit	m_Hf;
	CTextUnit	m_HcUnit;
	CEditUnit	m_Hc;
	CTextUnit	m_BfUnit;
	CEditUnit	m_Bf;
	CTextUnit	m_BcUnit;
	CEditUnit	m_Bc;
	int		    m_nOption;
	int		    m_bCrack;

	CTextUnit	m_AsTopUT;
	CEditUnit	m_AsTop;
	CTextUnit	m_AsBotUT;
	CEditUnit	m_AsBot;
	CTextUnit	m_cStirrupSpaceUT;
	CEditUnit	m_cStirrupSpace;
	CEditUnit	m_cStirrupNum;
	MComboBox	m_cStirrupSize;
	MComboBox	m_cTopLayerCB;
	MComboBox	m_cBotLayerCB;
	BOOL      m_bTorsionalBar;
	MComboBox m_cmbTorBar;
	CEditUnit m_edtTorSpacing;
	CTextUnit m_untTorSpacing;
	BOOL      m_bBundledBar;
	CEditUnit	m_edtBundledBarNum;
	MComboBox m_cmbLongiBar;
	CEditUnit	m_edtLongiBarNum;
	CEditUnit m_edtStirrupAngle;
	CTextUnit m_untStirrupAngle;
	BOOL      m_bBentUpBar;
	MComboBox m_cmbBentUpBatSize;
	CEditUnit m_edtBentUpBarNum;
	CEditUnit m_edtBentUpBarAngle;
	CTextUnit m_untBentUpBarAngle;
	//}}AFX_DATA

public:
		

protected:
	CDBDoc* m_pDoc;
	T_RCHK_K m_RchkKey;
	T_RCHK_D m_RchkData;
	T_SECT_D m_SectD;
	CMap<int,int,int,int> m_arBeamData;   // Element 중 Concrete Beam의 Data 저장 
	BOOL m_bModify;
	
	CCMSecViewWnd m_wndSecView;
	
	CDgnBeamGridWnd m_InfoGrid;

	MillustViewer m_wndPictureRect;
	MillustViewer m_wndPictureTee;
	 
	void SetInitStirCB();
	void SetInitUnit();
	BOOL SetDlg2Data();
	void SetData2Dlg();
	void ShowRECTBitmap();
	void ShowTEEBitmap();
	BOOL CheckInputData();
	BOOL CheckInputData_New();
	void ShowHideSectCtrl(BOOL bShow);
	void CopyRebarData(int FromNode, int ToNode);
	void ChangeSectionInfo();
	void ChangeDlgItemInfo();
	void SelectElemOnView();
	BOOL SelectSection(CArray<T_RCHK_K, T_RCHK_K>&  arRchKey);
	void CheckDiffSections(T_SECT_D OrginSectData, T_SECT_D SectData, int& iResult);

	void ChangeItem(int nIndex);
	void MakeItemEx();
	void SetHeaderTitle();
	void InitTabCtrl();
	int  FindInsertionPos(T_SECT_K nID);
	BOOL InsertItem(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RCHK_D &RData);

protected:
	//Tab Control.
	int m_nTopLayer, m_nBotLayer;
	int m_nIMJ;     // 0:I  1:M  2:J
	
	T_RCHK_BEAM*  m_pBeamData;
	CMap<int,int,T_RCHK_K,T_RCHK_K> m_arRchkKey;

	int m_nTPosLayer[3];  // 0:I  1:M  2:J
	int m_nBPosLayer[3];
	int m_nStiCBox[3];
	int m_nBentUpBar[3];
	int m_nTorBar[3];
	int m_nTorLongiBar[3];
	BOOL bExistData[3];
	
	T_RCHK_BEAM  m_TabBeamData; // Tab Selchange.

	CDgnSectionDrawWnd     m_SectWnd;  // Veiw Window

	void SetRchkData(int nIMJ, T_RCHK_K RchkKey, T_RCHK_BEAM* pBeamData);
	void UpdateViewWnd();

	void SetData2Dlg_Rebar();
	void SetDlg2Data_Rebar();    
	void InitStirrupCB();
	void InitLayerCB();
	BOOL CheckReDraw_Rebar();
	void UpdateAsInfo(double dAst, double Asb);

	BOOL CheckInputData_Rebar_New();
	BOOL CheckPosiInputData_New(int iIMJ);

	
	void GetDataFromGrid(int iIMJ);

	void SetLayer_Change();
	void SetData_TabSelChange();
	void SetDlg2Data_Rebar_TabCtrl(int iIMJ);
	
	void CalcAs(T_RCHK_BEAM& RchkBeamD, double& dAsTop, double& dAsBot);
	
	void ControlsEnableDisable();

	// 자식에서 상속받아서 구연
	// 여기에 추가로 구연을 하려면 CDgnConBeamReinforceCommonDlgCIVIL에 있던 내용은 가상함수로 만들고, CDgnConBeamReinforceCommonDlgCIVIL을 상속받은 모든 Class에서 실제 내용을 추가하시오..
	virtual void SetLayerNumToGrid(int nTopBot, int nLayerNum) = 0;
	virtual BOOL CheckReDraw_RebarGrid(_DGNC_RBAR& RebarData, int& iChkResult) = 0;
	virtual void SetDataToGrid() = 0;
	virtual void InitGridData() = 0;	
	virtual void SetDataToGrid_TabCtrl() = 0;
	virtual void GetDataFromGrid_TabCtrl(int iIMJ) = 0;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBeamReinforceCommonDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConBeamReinforceCommonDlgCIVIL)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnBentUpBarChk();
	afx_msg void OnDgnTorsionalBar();
	afx_msg void OnDgnBundledStirrups();
	afx_msg void OnDgnDelete();
	afx_msg void OnClickOptCrack();
	afx_msg void OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDgnRcBotLayerCb();
	afx_msg void OnSelchangeDgnRcTopLayerCb();
	afx_msg void OnRedrawBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnConBeamReinforceCommonDlgCIVIL_H__)
