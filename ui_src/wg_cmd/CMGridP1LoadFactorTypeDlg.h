#if !defined(__CMGRIDP1LOADFACTORTYPEDLG_H__)
#define __CMGRIDP1LOADFACTORTYPEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridP1LoadFactorTypeDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DialogMove.h"
//#include "CMDlgBase.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMGridP1LoadFactorTypeDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGridP1LoadFactorTypeDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMGridP1LoadFactorTypeDlg(CWnd* pParent = NULL);   // standard constructor
	void SetParam(T_P1LT_K key);

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CCMGridP1LoadFactorTypeDlg)
	enum { IDD = IDD_CMD_GRID_P1LT_DLG };
	CListCtrl	m_GridList;
	CEdit	m_wndName;
	int		m_nLoadItem;
	BOOL	m_bDxBend;
	BOOL	m_bDyBend;
	BOOL	m_bDzBend;
	BOOL	m_bRxBend;
	BOOL	m_bRyBend;
	BOOL	m_bRzBend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridP1LoadFactorTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bModify;
	T_P1LT_K m_Key;
	T_P1LT_D m_Data;

// Implementation
protected:
	CDBDoc* m_pDoc;
	void InitList();
	void MakeItemEx();
	void SetItem(int nIndex, T_P1LT_K key, T_P1LT_D& data);
	CString DataToStr(int nIndex, T_P1LT_D& data);
	void Dlg2Data(T_P1LT_D& P1ltD, int nSeq);
	void Data2Dlg(T_P1LT_D& data);
	BOOL GetSelectedData(int &nIndex);
	void SetDataSequence();
	void SetLabel(int nType);

	// Generated message map functions
	//{{AFX_MSG(CCMGridP1LoadFactorTypeDlg)
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdLoadItem();
	afx_msg void OnCmdDx1();
	afx_msg void OnCmdDy1();
	afx_msg void OnCmdDz1();
	afx_msg void OnCmdRx1();
	afx_msg void OnCmdRy1();
	afx_msg void OnCmdRz1();
	afx_msg void OnCmdDx2();
	afx_msg void OnCmdDy2();
	afx_msg void OnCmdDz2();
	afx_msg void OnCmdRx2();
	afx_msg void OnCmdRy2();
	afx_msg void OnCmdRz2();
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGRIDP1LOADFACTORTYPEDLG_H__)
