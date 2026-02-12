#if !defined(AFX_CMMVLDITEMTRDLG_H__AAD80D6D_4A5A_4FAC_A177_88C6F262BD1D__INCLUDED_)
#define AFX_CMMVLDITEMTRDLG_H__AAD80D6D_4A5A_4FAC_A177_88C6F262BD1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemTrDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

#include "CMMvldItemTrGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTrDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldItemTrDlg : public CDialogMove
{
// Construction
public:
	CCMMvldItemTrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMMvldItemTrDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_TR };
	CFormulaEditSpin	m_edtNum;
	MEdit	m_edtScale;
	MComboBox	m_cmbLane;
	MComboBox	m_cmbVehicle;
	MEdit	m_wndDesc;
	MEdit	m_wndLoadCase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemTrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemTrDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnChangeCmdMlcNumEdt();
	afx_msg void OnCmdMlcVehicleBtn();
	afx_msg void OnCmdMlcRemoveBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	CDBDoc*   m_pDoc;
	T_MVLDtr_D  m_Data;
	CString   m_csOldName;
	BOOL      m_bModify;
	BOOL      m_bSurface;
	CCMMvldItemTrGrid m_wndMvldTrGrid;

public:
	void SetParamData(T_MVLDtr_D &ParamData) 
	{ m_csOldName = ParamData.LoadCaseName; m_Data = ParamData; m_bModify = TRUE; }
protected:
 
	

	

	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_MVLD_BASE &Data);
	BOOL DeleteItem(int nIndex, T_MVLD_BASE &Data);
	BOOL ModifyItem(int nIndex, T_MVLD_BASE &Data);
	CString DataToStr(int i, T_MVLD_BASE &Data);
	CString GetLaneName(UINT nLaneKey);

};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMTRDLG_H__AAD80D6D_4A5A_4FAC_A177_88C6F262BD1D__INCLUDED_)
