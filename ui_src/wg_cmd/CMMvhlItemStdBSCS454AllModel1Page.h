#if !defined(AFX_CMMVHLITEMSTDBSCS454ALLMODEL1PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSCS454ALLMODEL1PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSCS454AllModel1Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSCS454AllModel1Page dialog
#include "..\MIT_frx\MComboBox.h"

#include "CMMvhlItemStdLoadGrid.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSCS454AllModel1Page : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSCS454AllModel1Page(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

	int m_nLoadType;
	int m_nSubType;


// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSCS454AllModel1Page)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_CS454_ALL_MODEL1 };

	CCMMvhlItemStdLoadGrid m_wndGrid;

	CEditUnit m_edtQ1;
	CTextUnit m_untQ1;
	CEditUnit m_edtQ2;
	CTextUnit m_untQ2;
	int m_nLoadCase;
	int m_nRoadSuf;
	int m_nTrafficFlow;
	BOOL m_bPatch;
	CEditUnit m_edtPatchW;
	CTextUnit m_untPatchW;
	CEditUnit m_edtPatchL;
	CTextUnit m_untPatchL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSCS454AllModel1Page)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

	void EnableDisableControls();

	void ChangeText();

	void SetLoadandSubType(int nLoadType, int nSubType);

protected:
	void InitUnit();
	void InitGrid();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSCS454AllModel1Page)
	virtual BOOL OnInitDialog();
	afx_msg void OnPatchBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSCS454ALLMODEL1PAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
