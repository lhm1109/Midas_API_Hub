#if !defined(AFX_STRUCTTIMEDEPMATLINKDLG_H__9A49EBE1_E899_42B0_AD63_AE6626A531BD__INCLUDED_)
#define AFX_STRUCTTIMEDEPMATLINKDLG_H__9A49EBE1_E899_42B0_AD63_AE6626A531BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructTimeDepMatLinkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructTimeDepMatLinkDlg dialog
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
class CStructTimeDepMatLinkDlg : public MChildFormView
{
// Construction
public:
	CStructTimeDepMatLinkDlg();   // standard constructor
	DECLARE_DYNCREATE(CStructTimeDepMatLinkDlg)
// Dialog Data
	//{{AFX_DATA(CStructTimeDepMatLinkDlg)
	enum { IDD = IDD_TM_TIMEDEP_MAT_LINK };
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

public:
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;
// Operations
public:
	int _GetHeightFromFont();
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	void InitContent();
	void InitComboMenu();
	void LayoutChildDialog();

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructTimeDepMatLinkDlg)
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructTimeDepMatLinkDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	// Generated message map functions
	//{{AFX_MSG(CStructTimeDepMatLinkDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	virtual bool ShowApplyBtn() { return false; }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTTIMEDEPMATLINKDLG_H__9A49EBE1_E899_42B0_AD63_AE6626A531BD__INCLUDED_)
