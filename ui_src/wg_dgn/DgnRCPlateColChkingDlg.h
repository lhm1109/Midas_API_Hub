#if !defined(__DgnRCPlateColChkingDlg_H__)
#define __DgnRCPlateColChkingDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCPlateColChkingDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateColChkingDlg dialog
class CDBDoc;

class CDgnRCPlateColChkingDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnRCPlateColChkingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDgnRCPlateColChkingDlg();

	// Dialog Data
	//{{AFX_DATA(CDgnRCPlateColChkingDlg)
	enum { IDD = IDD_DGN_RC_PLATE_COL_CHKING };

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRCPlateColChkingDlg)
protected:
	virtual void Execute() {}
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	// Implementation
protected:
	void InitCombo();
	void InitSbdoCombo(BOOL bKeepSel);
	void InitRipcCombo(BOOL bKeepSel);
	void Data2Dlg();
	BOOL Dlg2Data();
	void ControlsEnableDisable();
	void SetListCtrlHeader();
	void MakeItemEx();
	void UpdateBuffer();
	void SetCtrl();

	// Generated message map functions
	//{{AFX_MSG(CDgnRCPlateColChkingDlg)
	virtual BOOL OnInitDialog();		  
	afx_msg void OnDgnRadioElemAvg();  
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnAdd();
	afx_msg void OnMod();
	afx_msg void OnDel();
	afx_msg void OnClose();	
	afx_msg void OnSbdoBtn();
	afx_msg void OnRebarBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	CArray<UINT, UINT> m_aRdoElemAvg;
	MButton  	m_chkAvgNodal;
	CArray<UINT, UINT> m_aRdoXYDir;
	CListCtrl	m_List;
	MComboBox	m_cbxSbdo;
	MComboBox	m_cbxRebar;  


	CDBDoc* m_pDoc;
	T_PCDC_D m_Data; 


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnRCPlateColChkingDlg_H__)
