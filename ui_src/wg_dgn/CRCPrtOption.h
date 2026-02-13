// CRCPrtOption.h: interface for the CCRCPrtOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCPRTOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CRCPRTOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CCRCPrtOption : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CCRCPrtOption(CWnd* pParent = NULL);
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CCRCPrtOption)
	enum { IDD = IDD_DGN_CRC_PRINT_OPTION };
	int		m_nOption;
	int 	m_nMPOption;
	int   m_nMNOption;
	int   m_nSOption;
	int   m_nTOption;
	int   m_nRPOption;
	int   m_nRNOption;
	int   m_nFOption;
	int   m_nCOption;

	//
	int   m_iDgnCode;
	int		m_nElemType;
	//}}AFX_DATA

	CArray<UINT, UINT> m_aElemTypeCtrl;
	CArray<UINT, UINT> m_aMoveCtrl;

public:
	void Initial_SelectItem();
	void Initial_Data();

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// Add by ZINU.('01.1.3).
	void Update_InitDataByCode();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCRCPrtOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CCRCPrtOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCRCPosxAr();
	afx_msg void OnDgnCRCPosxDel();
	afx_msg void OnDgnPSCElemTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRCPRTOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
