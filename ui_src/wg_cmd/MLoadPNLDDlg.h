#if !defined(__MLOADPNLDDLG_H__)
#define __MLOADPNLDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MLoadPNLDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMLoadPNLDDlg dialog

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "CMDlgBase.h"
#include "CMLoadPNLDGrid.h"

class CCMLoadPNLDPointDlg;
class CCMLoadPNLDLineDlg;
class CCMLoadPNLDAreaDlg;


#include "HeaderPre.h"
class __MY_EXT_CLASS__ CMLoadPNLDDlg : public CCMDlgBase
{
// Construction
public:
	T_PNLD_D	m_Data;
	CMLoadPNLDDlg(CWnd* pParent = NULL);   // standard constructor

protected:
	//CDBDoc*		m_pDoc;
	int				m_nWhichLType;	// 0:Point Load , 1:Line Load  , 2:Area Load
	CCMLoadPNLDGrid			m_wndGrid;
	CCMLoadPNLDPointDlg	*m_pDlgPtPnLd;	// point load type dialog ptr	
	CCMLoadPNLDLineDlg	*m_pDlgLnPnLd;	// line	 load type dialog ptr	
	CCMLoadPNLDAreaDlg	*m_pDlgArPnLd;	// area load type dialog ptr	


	// data exchagne
public:
	BOOL	CreateChildDlgs(void);		// load type에 맞는 child dlg생성 
	BOOL	DestroyChildDlgs(void);		// load type에 맞는 child dlg삭제 
	BOOL	ShowChildDlgs(int which);	// load type에 맞는 child dlg출력 
	
	void	InitUnit(void);
	void	SetData2Dlg(void);
	BOOL	SetDlg2Data(void);
	void	UpdateAllGridRecord(void);

	void	UpdateBuffer(void);
	void  UpdateUnit(void);
	void	OnChangeCurrentPnld(CString lKey);
	BOOL  UpdateDataBuffer(CString lKey);
	
// Dialog Data
	//{{AFX_DATA(CMLoadPNLDDlg)
	enum { IDD = IDD_CMD_PNLD_DLG };
	CBCGPStatic	m_stDlgHolder;
	CTextUnit	m_stCpyYdirUnit;
	CTextUnit	m_stCpyXdirUnit;
	CString	m_sName;
	int		m_nType;
	CString	m_sDscp;
	CString	m_sCpyXdirEd;
	CString	m_sCpyYdirEd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMLoadPNLDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMLoadPNLDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdPlldAdd();
	afx_msg void OnCmdPlldClose();
	afx_msg void OnCmdPlldDelete();
	afx_msg void OnCmdPlldModify();
	afx_msg void OnDestroy();
	afx_msg void OnCmdPlldltypePoint();
	afx_msg void OnCmdPlldltypeLine();
	afx_msg void OnCmdPlldltypeArea();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLOADPNLDDLG_H__12C09FAE_7C5E_42FB_B3D5_97FBE3CF3CC5__INCLUDED_)
