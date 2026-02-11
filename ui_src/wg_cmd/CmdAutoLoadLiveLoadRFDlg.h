#if !defined(__CMDAUTOLOADLIVELOADRFDLG_H__)
#define __CMDAUTOLOADLIVELOADRFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\mit_frx\MComboBox.h"
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "HeaderPre.h"

using namespace mit::frx;
class __MY_EXT_CLASS__ CCmdAutoLoadLiveLoadRFDlg : public CDialogMove, public CDBUpdateConnector
{
	// Construction
public:
	CCmdAutoLoadLiveLoadRFDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadLiveLoadRFDlg)
	enum { IDD = IDD_CMD_LOADCOMB_DEF_LL_RF_DLG	};

	MComboBox	m_LoadCase;
	MEdit		m_edtRLFactor;
	CListCtrl	m_ListCtrl;
		//}}AFX_DATA

public:
	T_LLRF_D m_Data;

public:

	void Initial_LCaseComboBox();

	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_LLRF_BASE &data);
	void SetItem(int nIndex, T_LLRF_BASE &data);
	BOOL ValidItem(T_LLRF_BASE& data);
	BOOL Dlg2Item(T_LLRF_BASE &data);
	BOOL Item2Dlg(int nIndex, T_LLRF_BASE &data);
	BOOL CheckLoadCaseName(CString str, int& Index);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadLiveLoadRFDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	CDBDoc* m_pDoc;
// 	CArray<_CMD_SEIS_LCTP, _CMD_SEIS_LCTP> m_arLCtype;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadLiveLoadRFDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClickCmdGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdGenSeisList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnSelchangeCmdGenSeis();
	afx_msg void OnCmdRFAddBtn();
	afx_msg void OnCmdRFModBtn();
	afx_msg void OnCmdRFDelBtn();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADLIVELOADRFDLG_H__)
