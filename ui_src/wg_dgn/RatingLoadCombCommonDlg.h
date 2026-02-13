#if !defined(__CMRATINGLOADCOMBCOMMONDLG_H__)
#define __CMRATINGLOADCOMBCOMMONDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\NotifyCtrl.h"

#include "DgnDlgBase.h"

#include "RatingLoadCombCommonMainGrid.h"
#include "RatingLoadCombCommonSubGrid.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CRatingLoadCombCommonDlg : public CDgnDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CRatingLoadCombCommonDlg(int nIDD, CWnd* pParent = NULL);   // standard constructor
	~CRatingLoadCombCommonDlg();

// Dialog Data
	//{{AFX_DATA(CRatingLoadCombCommonDlg)
	
	
	MButton	m_wndAutoGenBtn;	
	//}}AFX_DATA
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer() = 0;
	
	void OnStageChanged();
	void UpdateStld();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombCommonDlg)
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
 

	// Generated message map functions
	//{{AFX_MSG(CRatingLoadCombCommonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnPaint(); 
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnClose();
	afx_msg void PostNcDestroy();	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	

protected:  
	CString GetCombActiveStr(int nLcomType, int nActive);
	CString GetCombTypeStr(int nCombType);  
	int GetCopyLcomType();			
	void SetControl();	

	CRatingLoadCombCommonMainGrid* m_pLoadCombList;
	CRatingLoadCombCommonSubGrid*  m_pContentList;   
	CRatingLoadCombCommonSubGrid*  m_pContentListMV;   

	BOOL m_bTB10002_1_05;
	CArray<UINT, UINT> m_aCtrlNormalStyle;	
	CDBDoc* m_pDoc;
// resizing
	CArray<UINT, UINT> m_aIDResize;
	CArray<UINT, UINT> m_aIDVectMoveHolResize;
	CArray<UINT, UINT> m_aIDHolMoveVertResize;
	CRect m_rectDlg;
	CRect m_rectResize[9];
	CRect m_rectVertMoveHorResize[9];
	CRect m_rectHorMoveVertResize[1];
	CRect m_rectClose;
};
#include "HeaderPost.h"    

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(__CMRATINGLOADCOMBCOMMONDLG_H__)