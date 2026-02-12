#if !defined(__THISDLG_H__)
#define __THISDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CThisDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CThisDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CThisDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThisDlg)
	enum { IDD = IDD_ETC_THIS };
	CEdit	    m_wndLoadcaseName;
	CListCtrl	m_List;
	CEdit   	m_wndDesc;
	CListCtrl	m_DampingList;
	CEditUnit	m_wndDamping;
	CEditUnit	m_wndMode;
	CEditUnit	m_wndDampAllMode;
	CEditUnit	m_wndOutputStep;
	CTextUnit	m_wndTimeIncUnit;
	CEditUnit	m_wndTimeInc;
	CTextUnit	m_wndEndTimeUnit;
	CEditUnit	m_wndEndTime;
	int		    m_nAnalType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	T_THIS_D m_Data;
	CArray<T_THIS_BASE, T_THIS_BASE&> m_aDampData;
protected:
	void SetListCtrlHeader();
	BOOL GetSelectedData(int &nIndex);
	void MakeItemEx();
	CString DataToStr(int i, T_THIS_D &data);
	void SetItem(int nIndex, T_THIS_K key, T_THIS_D &data);
	BOOL ValidData(T_THIS_D& data);
	BOOL Dlg2Data(T_THIS_D &data);
	BOOL Data2Dlg(T_THIS_D &data);
	BOOL ChangeAnalType(int nAnalType);

	void SetListCtrlHeaderDamping();
	BOOL GetSelectedDataDamping(int &nIndex);
	void MakeItemExDamping();
	CString DataToStrDamping(int i, T_THIS_BASE &data);
	void SetItemDamping(int nIndex, T_THIS_BASE &data);
	BOOL Dlg2ThisBase(T_THIS_BASE &data);
	BOOL ThisBase2Dlg(T_THIS_BASE &data);

	// Generated message map functions
	//{{AFX_MSG(CThisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposDouble(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposInteger(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeAnalType();
	afx_msg void OnDampingAdd();
	afx_msg void OnDampingModify();
	afx_msg void OnDampingDelete();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdSpecEigvDlg();
	afx_msg void OnChangedCurThisBase(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCurThis(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__THISDLG_H__)
