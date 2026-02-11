#if !defined(AFX_STLDSTMPDLG_H__C92116E1_CA5B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDSTMPDLG_H__C92116E1_CA5B_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldStmpDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CStldStmpDlg dialog

class CStldStmpDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldStmpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {}
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CStldStmpDlg)
	enum { IDD = IDD_TM_STLD_STMP };
	CListCtrl	m_List;
	CTextUnit	m_wndT1Unit;
	CEditUnit	m_wndT1Edit;
	CTextUnit	m_wndT0Unit;
	CEditTemp	m_wndT0Edit;
	CSelectLC	m_LoadCase;
	CCobxLdgr m_wndLdgrCobx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldStmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	CDBDoc* m_pDoc;
	void SetListCtrlHeader();
	BOOL GetSelectedItem(T_STMP_K &key);
	void MakeItemEx();
	CString DataToStr(int i, T_STMP_K key, T_STMP_D &data);
	void SetItem(int nIndex, T_STMP_K key, T_STMP_D &data);
	BOOL ValidItem(T_STMP_K key, T_STMP_D &data);
	BOOL Dlg2Item(T_STMP_K &key, T_STMP_D &data);
	BOOL Item2Dlg(T_STMP_K key, T_STMP_D &data);
	BOOL GetSelectedSTMP(T_STMP_K &key);

	// Generated message map functions
	//{{AFX_MSG(CStldStmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnTmProjConfBtn();
	afx_msg void OnTmAdd();
	afx_msg void OnTmModify();
	afx_msg void OnTmDelete();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnItemchangedTmList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDSTMPDLG_H__C92116E1_CA5B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
