#if !defined(AFX_CMLOADPNLDPOINTDLG_H__8F233281_C5DB_40E3_8E54_05BC87EB30B6__INCLUDED_)
#define AFX_CMLOADPNLDPOINTDLG_H__8F233281_C5DB_40E3_8E54_05BC87EB30B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadPNLDPointDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointDlg dialog
#include "..\wg_base\wg_base_InternationalDlg.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "CMLoadPNLDPointGrid.h" 

class CCMLoadPNLDPointDlg : public CInternationalDlg
{
	T_PNLD_D m_Data;	// 데이타를 따로 갖고있어야함.. 
	T_PNLD_D *m_pData;
	CCMLoadPNLDPointGrid m_wndGrid;
	BOOL  m_bInit;

// Construction
public:
	CCMLoadPNLDPointDlg(T_PNLD_D *pData,CWnd* pParent = NULL);   // standard constructor

// data exchagne
public:
	void	SetData2Dlg(void);
	BOOL	SetDlg2Data(void);
	void	InitUnit(void);
	void  UpdateUnit(void);

// Dialog Data
	//{{AFX_DATA(CCMLoadPNLDPointDlg)
	enum { IDD = IDD_CMD_PNLD_PAGE_POINTLD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadPNLDPointDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLoadPNLDPointDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADPNLDPOINTDLG_H__8F233281_C5DB_40E3_8E54_05BC87EB30B6__INCLUDED_)
