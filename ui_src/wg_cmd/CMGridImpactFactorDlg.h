#if !defined(AFX_CMGRIDIMPACTFACTORDLG_H__E4262136_E38A_47CE_879A_AD5993E059E5__INCLUDED_)
#define AFX_CMGRIDIMPACTFACTORDLG_H__E4262136_E38A_47CE_879A_AD5993E059E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridImpactFactorDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGridImpactFactorDlg dialog
#include "CMChildBarBase.h"
class CCMGridImpactFactorDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMGridImpactFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMGridImpactFactorDlg)
	enum { IDD = IDD_CMD_GRID_IMPACT_FACTOR_DLG };
	int		m_nOption;
	int		m_nFactorType;
	int		m_nTargetType;
	CTextUnit	m_dSpanLengthUnit;
	CEditUnit	m_dSpanLength;
	CEditUnit	m_dImpactFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridImpactFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_arFactorType;

	void Data2Dlg(T_GIMP_D& data);
	void Dlg2Data(T_GIMP_D& data);
	void OnTmGimpType();
	//void OnTdExecute();
	//void OnTdClose();

	// Generated message map functions
	//{{AFX_MSG(CCMGridImpactFactorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg void OnTmConsAddDelRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDIMPACTFACTORDLG_H__E4262136_E38A_47CE_879A_AD5993E059E5__INCLUDED_)
