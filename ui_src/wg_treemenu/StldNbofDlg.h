#if !defined(AFX_STLDNBOFDLG_H__9930E01F_B6C1_4D9A_8046_AC53EE21CD32__INCLUDED_)
#define AFX_STLDNBOFDLG_H__9930E01F_B6C1_4D9A_8046_AC53EE21CD32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldNbofDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_CobxGrup.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\SelectCtrl.h" // 추가 
#include "..\wg_base\wg_base_NumericOptimizer.h" // 추가 

/////////////////////////////////////////////////////////////////////////////
// CStldNbofDlg dialog

class CStldNbofDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldNbofDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CStldNbofDlg)
	enum { IDD = IDD_TM_STLD_NBOF };
	CListCtrl	m_List;
	CEditUnit	m_wndFactorZ;
	CEditUnit	m_wndFactorY;
	CEditUnit	m_wndFactorX;
	CSelectLC	m_LoadCase;
	CCobxLdgr	m_wndLdgrCobx;
	CCobxGrup	m_wndGrupCobx;
	BOOL m_bUseChk;
	BOOL m_bNmasChk;
	BOOL m_bLtmsChk;
	BOOL m_bStmsChk;
	BOOL m_bGmasChk;
	CSelectEdit	m_edtSelectedNode;  
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldNbofDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
protected:
	CDBDoc* m_pDoc;
	void SetListCtrlHeader();
	void GetFactor(double &x, double &y, double &z);
	BOOL GetSelectedNodalBodyForce(T_NBOF_K &key);
	void MakeItemEx();
	CString DataToStr(int i, T_NBOF_K key, T_NBOF_D &data);
	BOOL Data2Dlg(T_NBOF_K key, T_NBOF_D &data);
	BOOL Dlg2Data(T_NBOF_D &data);
	// Generated message map functions
	//{{AFX_MSG(CStldNbofDlg)
	virtual BOOL IsAdd2ListCase(const T_STLD_D &Stld);
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnTmNbofLoadBtn();
	afx_msg void OnTmNbofLdgrBtn();
	afx_msg void OnTmNbofStgrBtn();
	afx_msg void OnTmNbofAdd();
	afx_msg void OnTmNbofModify();
	afx_msg void OnTmNbofDelete();
	afx_msg void OnTmNbofUseChk();
	afx_msg void OnSelchangeTmNbofStgrCbo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDNBOFDLG_H__9930E01F_B6C1_4D9A_8046_AC53EE21CD32__INCLUDED_)
