#if !defined(__CMREBARPSCDLG_H__)
#define __CMREBARPSCDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "CMSecViewWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscDlg dialog
class CCMRebarPscMoment;
class CCMRebarPscShear;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMRebarPscDlg : public CChildDialog
{
// Construction
public:
	CCMRebarPscDlg(CWnd* pParent = NULL);   // standard constructor
	
// interface
public:
	void SetParamData(T_RPSC_K RpscK);
	void UpdateView(BOOL bEndI);

// Dialog Data
	//{{AFX_DATA(CCMRebarPscDlg)
	enum { IDD = IDD_CMD_REBAR_PSC };
	CButton	m_btnDel;
	CButton	m_btnAdd;
	CListCtrl	m_wndList;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarPscDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	T_RPSC_K m_Key;
	T_RPSC_D m_Data;
	T_SECT_D m_SectD, m_SectViewD;
	BOOL m_bModify;

	CCMRebarPscMoment* m_pMomentDlg;
	CCMRebarPscShear* m_pShearDlg;

	CCMSecViewWnd m_wndSecView;

protected:
	void LayoutChildDlg();
	void ChangeItem(int nIndex);

	void		MakeItemEx();
	void		SetHeaderTitle();
	int			FindInsertionPos(T_SECT_K nID);
	BOOL		InsertItem(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr(int i, T_SECT_K Key, T_SECT_D &Data, T_RPSC_D &RData);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarPscDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdDelete();
	afx_msg void OnItemchangedCmdSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCmdSectList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCopyBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARPSCDLG_H__)
