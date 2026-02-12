#if !defined(AFX_CMMVHLITEMSTDBSNRRAILPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSNRRAILPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSNRRailPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSNRRailPage dialog
#include "..\MIT_frx\MComboBox.h"

#include "CMMvhlItemStdLoadGrid.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSNRRailPage : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSNRRailPage(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

	int m_nLoadType;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSNRRailPage)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_NR_RAIL };

	CCMMvhlItemStdLoadGrid m_wndGrid1;
	CCMMvhlItemStdLoadGrid m_wndGrid2;

	CEditUnit m_edtW1;
	CTextUnit m_untW1;
	CEditUnit m_edtD1;
	CTextUnit m_untD1;
	CEditUnit m_edtD2;
	CTextUnit m_untD2;

	int m_nDynFact;
	CEditUnit m_edtTrainSpeed;

	BOOL      m_bLongiDist;
	CEditUnit m_edtDistPoint;
	CTextUnit m_untDistPoint;
	BOOL      m_bEccenVertLoad;
	CEditUnit m_edtEccenVertLoad;
	CTextUnit m_untEccenVertLoad;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSNRRailPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

	void EnableDisableControls();

	void SetLoadType(int nLoadType);

protected:
	void InitUnit();
	void InitGrid();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSNRRailPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDynFactRdo();
	afx_msg void OnLongiDistChk();
	afx_msg void OnEccenVertLoadChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSNRRAILPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
