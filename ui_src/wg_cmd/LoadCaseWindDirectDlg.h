#if !defined(__LOADCASEWINDDIRECTDLG_H__)
#define			 __LOADCASEWINDDIRECTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadMLLCDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CLoadCaseWindDirectDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CLoadCaseWindDirectDlg : public CDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CLoadCaseWindDirectDlg(CWnd* pParent = NULL);   // standard constructor
	~CLoadCaseWindDirectDlg();
// Dialog Data
	//{{AFX_DATA(CLoadCaseWindDirectDlg)
	enum { IDD = IDD_CMD_LOAD_WIND_DIRECT_DLG };
	CButton     m_rdoMiddle;
	CButton     m_rdoHigh;

	CComboBox	m_LoadCaseAlong;
	CComboBox	m_LoadCaseAcross;
	CComboBox	m_LoadCaseTorsion;
	CComboBox	m_cmbWindDirection;
	CEditUnit m_edtFactorGD;
	CEditUnit m_edtFactorK ;
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA

public:
	T_LCWD_D m_Data;

	void InitCtrl();
	void Data2Dlg();

protected:
	void InitCtrl_LoadCaseCmb();
	void SetListCtrlHeader();
	BOOL GetSelectedItem(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_LCWD_BASE &data);
	void SetItem(int nIndex, T_LCWD_BASE &data);
	BOOL ValidItem(T_LCWD_BASE& data);
	BOOL Dlg2Item(T_LCWD_BASE &data);
	BOOL Item2Dlg(int nIndex, T_LCWD_BASE &data);
	BOOL CheckLoadCaseName(CString str, int& Index);
	int  GetLoadCaseType(int iXY, int iIndex);

	BOOL GetUsedEQKeyList();
	BOOL ValidItem(T_LCWD_BASE& data, CString& strErrorMsg);

	void CtrlEnable();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadCaseWindDirectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:
	CDBDoc* m_pDoc;
	int m_nType;
	int m_nBuildingType;
	CArray<UINT, UINT> m_aCtrlHigh;
	// Generated message map functions
	//{{AFX_MSG(CLoadCaseWindDirectDlg)
	
	virtual BOOL OnInitDialog();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnClickBuildingTypeRdo();
	afx_msg void OnCmdWindAddBtn();
	afx_msg void OnCmdWindModBtn();
	afx_msg void OnCmdWindDelBtn();
	afx_msg void OnCmdCancelBtn();
	afx_msg void OnCmdOkBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__LOADCASEWINDDIRECTDLG_H__)
