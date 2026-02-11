#if !defined(AFX_CMMVHLITEMUSRDLG_H__8A601087_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMVHLITEMUSRDLG_H__8A601087_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemUsrDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUsrDlg dialog
#include "HeaderPre.h"

class CCMMvhlItemUsrTrckDlg;
class CCMMvhlItemUsrBSDlg;
class CCMMvhlItemUsrBSHAPage;
class CCMMvhlItemUsrBSHBPage;
class CCMMvhlItemUsrBSHAHBPage;
class CCMMvhlItemUsrBSBD3701Dlg;
class CCMMvhlItemUsrBSPedDlg;
class CCMMvhlItemUsrBSSpecialPage;
class CCMMvhlItemUsrTrainDlg;

class __MY_EXT_CLASS__ CCMMvhlItemUsrDlg : public CDialogMove
{
// Construction
public:
	CCMMvhlItemUsrDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVHL_D &ParamData);     
	void ResizeBSDialogByFoldGroupBox(); // Page 들에서 호출(Fold GroupBox 때문에 추가)
//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	void Data2DlgOfTrnLd();	
	BOOL Dlg2Data();
	BOOL Dlg2DataOfTrnLd();	

	// v6.7.1부터 변경....by HP
	void Data2DlgOfTrainLd();	
	BOOL Dlg2DataOfTrainLd();	

	BOOL ApplyOrOK();
	int  GetTruckLoadCount();
	int  GetTruckDistanceCount();
	void AlignControl();        // 한번만 불리는 녀석
	void AlignControlDynamic(); // 여러번 불리는 녀석

	void SetHeaderTitle();
	virtual void MakeItemEx();

	BOOL InsertItem(int nPos, int nNo, double dLoad, double dDist);
	BOOL DeleteItem(int nPos);
	BOOL ModifyItem(int nPos, int nNo, double dLoad, double dDist);

	BOOL GetLoadAndDistValue(double &dLoad, double &dDist);

	// 계홍..
	CCMMvhlItemUsrTrckDlg* m_pTrnDlg;

	CCMMvhlItemUsrBSDlg*          m_pBS5400Dlg;
	CCMMvhlItemUsrBSHAPage*       m_pBSHADlg;
	CCMMvhlItemUsrBSHBPage*       m_pBSHBDlg;
	CCMMvhlItemUsrBSHAHBPage*     m_pBSHAHBDlg;
	CCMMvhlItemUsrBSBD3701Dlg*    m_pBSHAHBAUTODlg;
	CCMMvhlItemUsrBSPedDlg*       m_pBSPedDlg;
	CCMMvhlItemUsrBSSpecialPage*  m_pBSSpecialDlg;

	CCMMvhlItemUsrTrainDlg* m_pTrainDlg;	//v6.7.1부터...변경...by HP...

	BOOL	CreateTranLdDlg(void);
	void	DestroyTranLdDlg(void);
	void	ShowChildDlg(int which);
	void	ChangeBitMap(int which);	  
	void  EnableChinaRadio(int which);    
	void  ShowHideCtrl();

	BOOL  CreateBSLoadDlg();
	void  DestroyBSLoadDlg();
	void  CtrlBSLoadDlg();
	void  LoadBSBitmap();
	void  ResizeBSDialog();
	void  ChangeBSDlg();

	void  SetInitChTruckCombo();
	void  SetInitChTrainCombo();
	void  SetInitChCrowdCombo();
	void  SetInitKoreaLaneCombo();
	
	int GetTrkRadio();
	void SetTrkRadio(int nVal);

	void EnableLegalPermitRadio();

protected:  

	CDBDoc*   m_pDoc;
	T_MVHL_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	MillustViewer  m_wndPicture;
	MillustViewer  m_wndPicture2;
	int m_nMovingType;
	int m_bPermitLoadCall;
	BOOL m_bPatchLoad;

	double m_dHTrnDlg;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlItemUsrDlg)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMU };
	CTextUnit	m_wndWCrawlerUnit;	
	CTextUnit	m_wndDCrawlerUnit;	
	CTextUnit	m_stQqUnit;
	CTextUnit	m_stQmUnit;
	CTextUnit	m_stPUnit;
	CTextUnit	m_stWUnit;
	CTextUnit	m_stPLVUnit;
	CTextUnit	m_stPLMUnit;
	CTextUnit	m_stPLUnit;
	CTextUnit m_stIrcNtDist;
	CTextUnit m_stIrcD1;
	CTextUnit m_stIrcD2;
	CTextUnit m_stIrcP;
	CTextUnit m_stIrcD;
	CTextUnit m_stIrcPb;
	CTextUnit m_stIrcDb;
	CListCtrl	m_List;
	MEdit	m_wndDCrawler;
	MEdit	m_wndWCrawler;
	MEdit	m_wndQq;
	MEdit	m_wndQm;
	MEdit	m_wndP;
	MEdit	m_wndPlvEdit;
	MEdit	m_wndPlmEdit;
	MEdit	m_wndPlEdit;
	MEdit	m_wndWEdit;
	MEdit	m_wndVehicleName;
	MEdit	m_wndDEdit;
	MEdit	m_wndPEdit;
	MEdit m_wndIrcNtDist;
	MEdit m_wndIrcD1;
	MEdit m_wndIrcD2;
	MEdit m_wndIrcP;
	MEdit m_wndIrcD;
	MEdit m_wndIrcPb;
	MEdit m_wndIrcDb;
	int		m_nTrkRadio;	
	int		m_nTrkLegalRadio;	
	MButton	m_wndBtnApply;
	MEdit m_edtCrowd;
	CTextUnit m_untCrowd;
	MComboBox m_cmbChTruck;
	MComboBox m_cmbChTrain;
	MComboBox m_cmbChCrowd;
	MEdit	m_wndQkLane;
	MEdit	m_wndPk1Lane;
	MEdit	m_wndPk2Lane;
	MEdit	m_wndL1Lane;
	MEdit m_wndL2Lane;
	MEdit m_wndW1Crowd;
	MEdit m_wndW2Crowd;
	MEdit m_wndL1Crowd;
	MEdit m_wndL2Crowd;
	MEdit m_wndWdCrowd;  
	CTextUnit	m_untQkLane;
	CTextUnit	m_untPk1Lane;
	CTextUnit	m_untPk2Lane;
	CTextUnit m_untL1Lane;
	CTextUnit m_untL2Lane;
	CTextUnit m_untW1Crowd;
	CTextUnit m_untW2Crowd;
	CTextUnit m_untL1Crowd;
	CTextUnit m_untL2Crowd;
	CTextUnit m_untWdCrowd;
	int   m_nBSType;
	int m_nLegalPermitRadio;

	// Korea - Lane(Length Specifired (탱크 하중))
	MComboBox m_cmbUniformLoadNum;
	CEditUnit m_edtUniformLoadDistance;
	CTextUnit m_untUniformLoadDistance;
	CEditUnit m_edtUniformLoadW;
	CTextUnit m_untUniformLoadW;
	CEditUnit m_edtUniformLoadLengthL;
	CTextUnit m_untUniformLoadLengthL;

	int       m_nDynaType;
	CEditUnit m_edt1Axle;
	CEditUnit m_edt2Axle;
	CEditUnit m_edt3Axle;

	// Legal Load
	CEditUnit m_edtImpactFactor;
	CTextUnit m_untImpactFactor;
	CEditUnit m_edtW;
	CTextUnit m_untW;
	CEditUnit m_edtR;
	CTextUnit m_untR;
	CEditUnit m_edtDist;
	CTextUnit m_untDist;

	// AASHTO-LRFD
	MButton	m_chkCentF;

	MButton	m_chkPatch;
	CEditUnit m_edtPatchW;
	CTextUnit m_untPatchW;
	CEditUnit m_edtPatchL;
	CTextUnit m_untPatchL;

	CArray<UINT, UINT> m_aCtrlPatch;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemUsrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strText;
	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemUsrDlg)
	virtual BOOL OnInitDialog();

	void SetUnit();

	virtual void OnOK();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnInsert();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnItemchangedCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLegalPermitRd();
	afx_msg void OnCmdMlMvhlItemuTrkRd();
	afx_msg void OnSelchangeChTruckCombo();
	afx_msg void OnSelchangeChTrainCombo();  
	afx_msg void OnSelchangeChCrowdCombo();	
	afx_msg void OnCmdMlMvhlItemuBSRdo();
	afx_msg void OnCmdMlMvhlItemuUniformLoadNumCmb();
	afx_msg void OnCmdMlMvhlItemuDynaTypeRdo();
	afx_msg void OnChkPatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMUSRDLG_H__8A601087_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
