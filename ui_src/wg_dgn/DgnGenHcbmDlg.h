#if !defined(AFX_DGNGENHCBMDLG_H__63D3BE61_FED6_4ED1_BC9A_8368A91249F7__INCLUDED_)
#define AFX_DGNGENHCBMDLG_H__63D3BE61_FED6_4ED1_BC9A_8368A91249F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenHcbmDlg.h : header file
//

#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenHcbmDlg dialog

class CDgnGenHcbmDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenHcbmDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() {  }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CDgnGenHcbmDlg)
	enum { IDD = IDD_DGN_GEN_HCBM_DLG };
	CTextUnit	  m_untL2;
	CTextUnit	  m_untL1;
	CListCtrl	  m_lstHcbm;
	CEditUnit	  m_edtL2;
	CEditUnit	  m_edtL1;
	CMouseEdit	m_edtPartC;
	CMouseEdit	m_edtPartB;
	CMouseEdit	m_edtPartA;
	int	       	m_nPosition;
	CString	m_strName;
	mit::frx::MillustViewer m_wndHcbm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenHcbmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;

protected:
	BOOL Dlg2Data(BOOL bModify=FALSE);

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(T_HCBM_K key, T_HCBM_D& data);
	BOOL DeleteItem(T_HCBM_K key, T_HCBM_D& data);
	BOOL ModifyItem(T_HCBM_K KeyOld, T_HCBM_K key, T_HCBM_D& data);
	CString DataToStr(int i, T_HCBM_K key, T_HCBM_D &data);

	BOOL IsHaunchElem(CMouseEdit* pMEdit, T_ELEM_K ElemK);
	CMouseEdit* GetMouseEditCtrl(LPARAM lParam);
	// Coded by Seungjun ('20070516) MNet:No.XXXX.
	void Update_InitDataByCode();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnGenHcbmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnHcbmPosition();
	afx_msg void OnDgnHcbmAddBtn();
	afx_msg void OnDgnHcbmModBtn();
	afx_msg void OnDgnHcbmDelBtn();
	afx_msg void OnDgnHcbmCloseBtn();
	afx_msg void OnItemchangedDgnHcbmLst(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	LRESULT	OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnClickMeNotifyWindow(WPARAM wParam, LPARAM lParam);
	LRESULT OnElemListEnter(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENHCBMDLG_H__63D3BE61_FED6_4ED1_BC9A_8368A91249F7__INCLUDED_)
