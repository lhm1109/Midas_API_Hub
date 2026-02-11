#if !defined(AFX_CMGRIDMASSCONVERTERDLG_H__FC2E6E03_8279_41E1_9565_25B37515F688__INCLUDED_)
#define AFX_CMGRIDMASSCONVERTERDLG_H__FC2E6E03_8279_41E1_9565_25B37515F688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridMassConverterDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

#include "CMChildBarBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGridMassConverterDlg dialog

class CCMGridMassConverterDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMGridMassConverterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute(){}

//-------------------------------------------------
// Interfaces
//-------------------------------------------------
public:
	BOOL m_bWorkTab;
	int  m_WTLoadCaseNum;
	void SetCurrentLC();
	//void SetParamData(int iItem);

//-------------------------------------------------
// Implementations
//-------------------------------------------------
protected:
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_GLTM_BASE &data);
	void SetItem(int nIndex, T_GLTM_BASE &data);
	BOOL ValidItem(T_GLTM_BASE& data);
	BOOL Dlg2Item(T_GLTM_BASE &data);
	BOOL Item2Dlg(T_GLTM_BASE &data);
	void Data2Dlg();
	void Dlg2Data();
	void CtrlManager();
	void ResetData();
	BOOL CheckData();
	BOOL DelAllGmasData();

protected:
	CDBDoc* m_pDoc;
	T_GLTM_D m_Data;  

// Dialog Data
	//{{AFX_DATA(CCMGridMassConverterDlg)
	enum { IDD = IDD_CMD_GRID_MASS_CONVERTER };
	CListCtrl	m_List;  
	CSelectLC m_wndGilcCmb;
	MComboBox	m_wndMgdrCmb;
	CEditUnit	m_wndScaleFactor;
	CTextUnit	m_wndGravityUnit;
	CEditUnit	m_wndGravityEdit;
	MButton	m_wndFishBoneChk;
	MButton	m_wndDirZChk;
	MButton	m_wndDirYChk;
	MButton	m_wndDirXChk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridMassConverterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGridMassConverterDlg)
	virtual BOOL OnInitDialog();
	virtual void OnTmClose();
	afx_msg void OnCmdGilcBtn();
	afx_msg void OnCmdGltmFishBoneChk();
	afx_msg void OnCmdGltmAddBtn();
	afx_msg void OnCmdGltmModBtn();
	afx_msg void OnCmdGltmDelBtn();
	afx_msg void OnCmdGltmOkBtn();
	afx_msg void OnCmdGltmCancelBtn();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdGltmRemoveBtn();
	afx_msg void OnCmdGltmCalcMass();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDMASSCONVERTERDLG_H__FC2E6E03_8279_41E1_9565_25B37515F688__INCLUDED_)
