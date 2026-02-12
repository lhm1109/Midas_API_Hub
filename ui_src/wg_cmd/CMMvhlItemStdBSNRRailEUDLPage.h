#if !defined(AFX_CMMVHLITEMSTDBSNRRAILEUDLPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
#define AFX_CMMVHLITEMSTDBSNRRAILEUDLPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdBSNRRailEUDLPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdBSNRRailEUDLPage dialog
#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"

using namespace mit::frx;

class CCMMvhlItemStdBSDlg;
class CCMMvhlItemStdBSNRRailEUDLPage : public CInternationalDlg
{
// Construction
public:
	CCMMvhlItemStdBSNRRailEUDLPage(T_MVHL_D* pData, CWnd* pParent = NULL);   // standard constructor

public:
	T_MVHL_D* m_pData;

	int m_nLoadType;

// Dialog Data
	//{{AFX_DATA(CCMMvhlItemStdBSNRRailEUDLPage)
	enum { IDD = IDD_CMD_ML_MVHL_ITEMS_BS_NR_RAIL_EUDL };

	CEditUnit m_edtW;
	CTextUnit m_untW;
	
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
	//{{AFX_VIRTUAL(CCMMvhlItemStdBSNRRailEUDLPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	BOOL SetDlg2Data();

	void EnableDisableControls();

protected:
	void InitUnit();

// Implementation
protected:
	CCMMvhlItemStdBSDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlItemStdBSNRRailEUDLPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDynFactRdo();
	afx_msg void OnLongiDistChk();
	afx_msg void OnEccenVertLoadChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDBSNRRAILEUDLPAGE_H__A8C86F82_324E_46D8_95AF_EB5FD77A8FD2__INCLUDED_)
