#if !defined(AFX_SDVPLISTDLG_H__D36FE2A7_A431_4378_8610_606D7666FE13__INCLUDED_)
#define AFX_SDVPLISTDLG_H__D36FE2A7_A431_4378_8610_606D7666FE13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SdvpListDlg.h : header file

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSdvpListDlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CSdvpListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CSdvpListDlg(CWnd* pParent = NULL);   // standard constructor
	void SetInitSelect(CString strInit);

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	void SelectItem(CString str);

protected:  
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CSdvpListDlg)
	enum { IDD = IDD_ETC_SDVP_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSdvpListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strInitSelect;
	BOOL m_bInitSelect;  
	//CArray<T_SDVP_D, T_SDVP_D&> m_aSdvpData;  
	// Generated message map functions
	//{{AFX_MSG(CSdvpListDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

	virtual void SetTitleText()=0;
	virtual void OnBtnAdd()=0;
	virtual void OnBtnModify()=0;
	virtual void OnBtnDel()=0;
	virtual void OnBtnCopy()=0;
	virtual void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)=0;
	virtual void MakeItemEx()=0;
	virtual void UpdateBuffer()=0; 
	virtual void GetHeaderTitleWidth(CString* aTitle, int* aColWidth)=0;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class __MY_EXT_CLASS__ CSdviListDlg : public CSdvpListDlg
{
	virtual void SetTitleText();
	virtual void OnBtnAdd();
	virtual void OnBtnModify();
	virtual void OnBtnDel();
	virtual void OnBtnCopy();
	virtual void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void MakeItemEx();
	virtual void UpdateBuffer(); 
	virtual void GetHeaderTitleWidth(CString* aTitle, int* aColWidth);
	BOOL InsertItem(T_SDVI_K Key, T_SDVI_D &Data);
	BOOL DeleteItem(T_SDVI_K Key, T_SDVI_D &Data);
	BOOL ModifyItem(T_SDVI_K KeyOld, T_SDVI_K Key, T_SDVI_D &Data);
	CString DataToStr(int i, T_SDVI_K Key, T_SDVI_D &Data);
};

class __MY_EXT_CLASS__ CSdveListDlg : public CSdvpListDlg
{
	virtual void SetTitleText();
	virtual void OnBtnAdd();
	virtual void OnBtnModify();
	virtual void OnBtnDel();
	virtual void OnBtnCopy();
	virtual void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void MakeItemEx();
	virtual void UpdateBuffer(); 
	virtual void GetHeaderTitleWidth(CString* aTitle, int* aColWidth);
	BOOL InsertItem(T_SDVE_K Key, T_SDVE_D &Data);
	BOOL DeleteItem(T_SDVE_K Key, T_SDVE_D &Data);
	BOOL ModifyItem(T_SDVE_K KeyOld, T_SDVE_K Key, T_SDVE_D &Data);
	CString DataToStr(int i, T_SDVE_K Key, T_SDVE_D &Data);
};

class __MY_EXT_CLASS__ CSdstListDlg : public CSdvpListDlg
{
	virtual void SetTitleText();
	virtual void OnBtnAdd();
	virtual void OnBtnModify();
	virtual void OnBtnDel();
	virtual void OnBtnCopy();
	virtual void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void MakeItemEx();
	virtual void UpdateBuffer(); 
	virtual void GetHeaderTitleWidth(CString* aTitle, int* aColWidth);
	BOOL InsertItem(T_SDST_K Key, T_SDST_D &Data);
	BOOL DeleteItem(T_SDST_K Key, T_SDST_D &Data);
	BOOL ModifyItem(T_SDST_K KeyOld, T_SDST_K Key, T_SDST_D &Data);
	CString DataToStr(int i, T_SDST_K Key, T_SDST_D &Data);
};

class __MY_EXT_CLASS__ CSdhyListDlg : public CSdvpListDlg
{
	virtual void SetTitleText();
	virtual void OnBtnAdd();
	virtual void OnBtnModify();
	virtual void OnBtnDel();
	virtual void OnBtnCopy();
	virtual void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void MakeItemEx();
	virtual void UpdateBuffer(); 
	virtual void GetHeaderTitleWidth(CString* aTitle, int* aColWidth);
	BOOL InsertItem(T_SDHY_K Key, T_SDHY_D &Data);
	BOOL DeleteItem(T_SDHY_K Key, T_SDHY_D &Data);
	BOOL ModifyItem(T_SDHY_K KeyOld, T_SDHY_K Key, T_SDHY_D &Data);
	CString DataToStr(int i, T_SDHY_K Key, T_SDHY_D &Data);
};

class __MY_EXT_CLASS__ CSdisListDlg : public CSdvpListDlg
{
	virtual void SetTitleText();
	virtual void OnBtnAdd();
	virtual void OnBtnModify();
	virtual void OnBtnDel();
	virtual void OnBtnCopy();
	virtual void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void MakeItemEx();
	virtual void UpdateBuffer(); 
	virtual void GetHeaderTitleWidth(CString* aTitle, int* aColWidth);
	BOOL InsertItem(T_SDIS_K Key, T_SDIS_D &Data);
	BOOL DeleteItem(T_SDIS_K Key, T_SDIS_D &Data);
	BOOL ModifyItem(T_SDIS_K KeyOld, T_SDIS_K Key, T_SDIS_D &Data);
	CString DataToStr(int i, T_SDIS_K Key, T_SDIS_D &Data);
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SDVPLISTDLG_H__D36FE2A7_A431_4378_8610_606D7666FE13__INCLUDED_)
