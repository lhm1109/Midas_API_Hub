#if !defined(AFX_DGNSTLSERVICELIMITDLG_H__67582C46_0EAF_4B78_97E6_42345627350F__INCLUDED_)
#define AFX_DGNSTLSERVICELIMITDLG_H__67582C46_0EAF_4B78_97E6_42345627350F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlServiceLimitDlg.h : header file
//
// Coded by Seungjun MNet:No.2393 ('20061011)
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlServiceLimitDlg dialog

class CDgnStlServiceLimitDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlServiceLimitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlServiceLimitDlg)
	enum { IDD = IDD_DGN_STL_SERVICE_DLG };
	int     m_iVertical;
	double  m_dVerticalUser;
	double  m_dDAF;
	int     m_nOption;
	int     m_nSeletType;
	int		m_iHorizontal;
	double	m_dHorizontalUser;
	//}}AFX_DATA

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Data();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlServiceLimitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//void EnableItems(BOOL bEnable);
	CDBDoc* m_pDoc;

	int  m_iCode;
	CArray<UINT,UINT> m_arIDBeam;
	CArray<UINT,UINT> m_arIDColm;
	CArray<UINT,UINT> m_arIDEtc;

	void Update_InitDataByCode();
	void UpdateBuffer();
	void EnableItems(BOOL bEnable);
	  
	void MoveCtrl();

	BOOL IsShowOneDeflLimit(int nCode);

	// Generated message map functions
	//{{AFX_MSG(CDgnStlServiceLimitDlg)
	virtual void Execute();
	afx_msg void OnDgnConServAr();
	afx_msg void OnDgnConServDel();
	afx_msg void OnHorizontalUser();
	afx_msg void OnVerticalUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_strDeflectFrm1Init;
	CString m_strDeflectFrm2Init;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLSERVICELIMITDLG_H__67582C46_0EAF_4B78_97E6_42345627350F__INCLUDED_)
