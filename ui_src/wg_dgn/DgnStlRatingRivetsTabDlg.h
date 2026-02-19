#pragma once

// DgnStlRatingRivetsTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingRivetsTabDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDBDoc;
class __MY_EXT_CLASS__ CDgnStlRatingRivetsTabDlg : public CChildDialog
{
	// Construction
public:
	CDgnStlRatingRivetsTabDlg(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDgnStlRatingRivetsTabDlg)
	enum { IDD = IDD_DGN_STL_RATING_RIVETS_TAB_DLG };

	CEditUnit m_edtLongiSpacing;
	CTextUnit m_untLongiSpacing;
	CEditUnit m_edtUltTensStre;
	CTextUnit m_untUltTensStre;
	CEditUnit m_edtYieldStre;
	CTextUnit m_untYieldStre;

	// NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlRatingRivetsTabDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetRivtData(T_RIVT_SUB_D& rData);
	void SetRivtData(T_RIVT_SUB_D& Data);

	void AlignControls();
	void ShowHideControls();
	void EnableDisableControls(BOOL bEnable);

protected:
	CDBDoc *m_pDoc;

	T_RIVT_SUB_D m_Data;

	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnStlRatingRivetsTabDlg)
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

