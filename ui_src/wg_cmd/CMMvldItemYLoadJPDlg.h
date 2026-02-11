#if !defined(AFX_CMMVLDITEMYLOADJPDLG_H__18A75928_6E84_4313_9546_F6D5B4D49555__INCLUDED_)
#define AFX_CMMVLDITEMYLOADJPDLG_H__18A75928_6E84_4313_9546_F6D5B4D49555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemYLoadJPDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemYLoadJPDlg dialog
class CCMvldItemJPGrid;
class CCMMvldItemYLoadJPDlg : public CChildDialog
{
// Construction
public:
	CCMMvldItemYLoadJPDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMMvldItemYLoadJPDlg(); 

public:
	BOOL GetCurDlgData(T_MVLDjp_D* pData);
	void SetCurDlgData(T_MVLDjp_D* pData);

protected:
	void SetData2Dlg();
	void InitControlUnit();

protected:
	CDBDoc*           m_pDoc;
	T_MVLDjp_D        m_Data;
	CCMvldItemJPGrid* m_pGrid;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemYLoadJPDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_YLOAD_JAPAN };
	CTextUnit	m_unitMoveDist;
	CEditUnit	m_editMoveDist;
	CTextUnit	m_unitP2L1;
	CTextUnit	m_unitGrLarge;
	CEditUnit	m_editGrLarge;
	CTextUnit	m_unitP2Small;
	CEditUnit	m_editP2Small;
	CTextUnit	m_unitP2Medium;
	CEditUnit	m_editP2Medium2;
	CEditUnit	m_editP2Medium1;
	CTextUnit	m_unitP2Large;
	CEditUnit	m_editP2Large;
	CEditUnit	m_editP2L2;
	CEditUnit	m_editP2L1;
	CTextUnit	m_unitP2L;
	CTextUnit	m_unitLength;
	CEditUnit	m_editLength;
	CTextUnit	m_unitGrSmall;
	CEditUnit	m_editGrSmall;
	CTextUnit	m_unitGrMedium;
	CEditUnit	m_editGrMedium2;
	CEditUnit	m_editGrMedium1;
	BOOL	m_bLoadCheck;
	double	m_dRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemYLoadJPDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemYLoadJPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMYLOADJPDLG_H__18A75928_6E84_4313_9546_F6D5B4D49555__INCLUDED_)
