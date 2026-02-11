#if !defined(__CMMVLDSUBITEMIDDLG_H__)
#define __CMMVLDSUBITEMIDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldSubItemIDDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

class CCMMvldItemIDDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemIDDlg dialog

class CCMMvldSubItemIDDlg : public CDialogMove
{
// Construction
public:
	CCMMvldSubItemIDDlg(CWnd* pParent = NULL);   // standard constructor
	CCMMvldItemIDDlg* m_pMvldItemDlg;

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDid_BASE &ParamData, int nPos) 
	{ m_Data = ParamData; m_nPos = nPos; m_bModify = TRUE; }

	void SetAutoValue(BOOL bAuto) {m_bAuto = bAuto;}

public:
	T_MVLDid_BASE m_Data;
	int           m_nPos;
	BOOL          m_bAuto; // v761 Ãß°¡

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void FillVClassList();
	CString GetLaneName(UINT nLaneKey);

	void AlignControl();
	void ResizeDialog();

protected:
	CDBDoc*       m_pDoc;
	BOOL          m_bModify;
	BOOL          m_bListMode;
	BOOL          m_bSurface;

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aSelFootwayFlag;

	CArray<UINT, UINT> m_aAutoLoadCaseData;
	CArray<UINT, UINT> m_aUserLoadCaseData;
	CArray<UINT, UINT> m_aEtc; 

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldSubItemIDDlg)
	enum { IDD = IDD_CMD_ML_MVLD_SITEM_INDIA };

	// Auto Load Case - Sub Load Case 1
	MEdit	    m_wndScaleFactor;       // Scale Factor
	MEdit     m_wndLaneNum;           // Number of Design Lanes
	MComboBox	m_wndVClass;            // Vehicle Class 1
	MComboBox	m_wndVClass2;           // Vehicle Class 2
	MComboBox m_wndFootway;           // Footway
	CEditUnit m_wndWidthEdit;         // Carriageway Width
	CTextUnit m_wndWidthUnit;         // Carriageway Width Unit
	CEditUnit m_wndDLoadEdit;         // Carriageway Loading 
	CTextUnit m_wndDLoadUnit;         // Carriageway Loading Unit
	CListBox	m_wndUnselList;         // List of Lanes
	CListBox	m_wndSelList;  	        // Selected Lanes for Carriageway
	CListBox  m_wndSelFootwayList;    // Selected Lanes for footway
 
	// User Load Case - Sub Load Case 2
	CEditUnit       m_edtScaleFactor;     // Scale Factor
	CFormulaEditSpin       m_edtMinLaneNum;      // Min
	CFormulaEditSpin       m_edtMaxLaneNum;      // Max
	MComboBox       m_cmbVehicle;         // Vehicle
	CListBox	      m_wndUnselList2;      // List of Lanes
	CListBox	      m_wndSelList2;        // Selected Lanes
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldSubItemIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	double GetInitFootwayLoad();
	// Generated message map functions
	//{{AFX_MSG(CCMMvldSubItemIDDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdBtnVclass();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDel();
	afx_msg void OnCmdBtnFootwayAdd();
	afx_msg void OnCmdBtnFootwayDel();
	afx_msg void OnDblclkCmdUnselList();
	afx_msg void OnDblclkCmdSelList();
	afx_msg void OnDblclkCmdSelFootwayList();
	
	afx_msg void OnCmdBtnAdd2();
	afx_msg void OnCmdBtnDel2();
	afx_msg void OnDblclkCmdUnselList2();
	afx_msg void OnDblclkCmdSelList2();
	
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDSUBITEMIDDLG_H__)
