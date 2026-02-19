#if !defined(__DGNREBARCPGSTIFFMAIN_H__)
#define __DGNREBARCPGSTIFFMAIN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarCpgStiffMain.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\ChildDialog.h"

#include "..\wg_cmd\CMSecViewWnd.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiffMain dialog
class CDgnRebarCpgStiff;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnRebarCpgStiffMain : public CChildDialog
{
// Construction
public:
	CDgnRebarCpgStiffMain(CWnd* pParent = NULL);   // standard constructor
	
// interface
public:
	//void SetParamData(T_CLRS_K ClrsK);
	void UpdateView(BOOL bEndI);

// Dialog Data
	//{{AFX_DATA(CDgnRebarCpgStiffMain)
	enum { IDD = IDD_DGN_REBAR_CPG_STIFF_MAIN };
	MButton	m_btnDel;
	MButton	m_btnAdd;
	CListCtrl	m_wndList;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarCpgStiffMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	T_CLRS_K m_Key;
	T_CLRS_D m_Data;
	T_SECT_D m_SectD, m_SectViewD;
	BOOL m_bModify;

	CDgnRebarCpgStiff* m_pStiffDlg;
	
	CCMSecViewWnd m_wndSecView;

protected:
	void LayoutChildDlg();
	void ChangeItem(int nIndex);

	void		MakeItemEx();
	void		SetHeaderTitle();
	int			FindInsertionPos(T_SECT_K nID);
	BOOL		InsertItem(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_CLRS_D &RData);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarCpgStiffMain)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDgnDelete();
	afx_msg void OnItemchangedDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDgnSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCopyBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARCPGSTIFFMAIN_H__)
