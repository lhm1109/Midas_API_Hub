#if !defined(AFX_STLDBODFDLG_H__66BE7328_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDBODFDLG_H__66BE7328_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldBodfDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CStldBodfDlg dialog

class CStldBodfDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CStldBodfDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CStldBodfDlg)
	enum { IDD = IDD_TM_STLD_BODF };
	CListCtrl	m_List;
	mit::frx::MEdit	m_FactorZ;
	mit::frx::MEdit	m_FactorY;
	mit::frx::MEdit	m_FactorX;
	CSelectLC	m_LoadCase;
	CCobxLdgr	m_wndLdgrCobx;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldBodfDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
protected:
	CDBDoc* m_pDoc;
	void SetListCtrlHeader();
	void GetFactor(double &x, double &y, double &z);
	BOOL GetSelectedBodyForce(T_BODF_K &key);
	void MakeItemEx();
	CString DataToStr(int i, T_BODF_K key, T_BODF_D &data);

	// Generated message map functions
	//{{AFX_MSG(CStldBodfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnCmdBfBtnLc();
	afx_msg void OnCmdBfAdd();
	afx_msg void OnCmdBfModify();
	afx_msg void OnCmdBfDelete();
	afx_msg void OnChangedItemSelection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDBODFDLG_H__66BE7328_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
