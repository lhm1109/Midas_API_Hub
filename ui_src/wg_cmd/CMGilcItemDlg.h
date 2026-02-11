#if !defined(AFX_CMGILCITEM_H__D0F3093F_6920_4CAC_B667_936B3E393DD3__INCLUDED_)
#define AFX_CMGILCITEM_H__D0F3093F_6920_4CAC_B667_936B3E393DD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGilcItem.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMGilcItemDlg dialog

class CCMGilcItemDlg : public CDialogMove
{
// Construction
public:
	CCMGilcItemDlg(CWnd* pParent = NULL);   // standard constructor
	void SetParamData(LPCTSTR name);
// Dialog Data
	//{{AFX_DATA(CCMGilcItemDlg)
	enum { IDD = IDD_CMD_GRID_GILC_ITEM };
	MEdit	m_ctrlName;
	MComboBox	m_ctrlType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGilcItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL Dlg2Data();
	void Data2Dlg();

	void InitComboBox();

protected:
	BOOL m_bModify;
	CString   m_csOldName;
	T_GILC_D m_Data;
	//UINT      m_nOldID;
	//int m_Key;
	CDBDoc* m_pDoc;
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGilcItemDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGILCITEM_H__D0F3093F_6920_4CAC_B667_936B3E393DD3__INCLUDED_)
