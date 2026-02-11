#if !defined(AFX_ETC_GDS_TAIWAN86_BRG_H__2F6BA2FE_6BD1_4149_A80E_37F97981F0E7__INCLUDED_)
#define AFX_ETC_GDS_TAIWAN86_BRG_H__2F6BA2FE_6BD1_4149_A80E_37F97981F0E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_TAIWAN86_BRG.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN86_BRG dialog
#include "MyChildDialog.h"

class CSpfcTWBR89HUtil;

class CETC_GDS_TAIWAN86_BRG : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_TAIWAN86_BRG(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetPushOverData(BOOL bInit=TRUE);
	BOOL Dlg2Data();
	void Dlg2UtilData() override;

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcTWBR89HUtil> m_pSpfcUtil;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_TAIWAN86_BRG)
	enum { IDD = IDD_ETC_GDS_TAIWAN86_BRG };
	MComboBox	m_wndRCmb;
	MComboBox	m_wndImportanceCmb;
	MComboBox m_wndAlphaCmb;
	CEditUnit m_wndDamping;
	int		m_nZone;
	int		m_nSoil;
	double m_dMaxPeriod;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_TAIWAN86_BRG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_TAIWAN86_BRG)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_TAIWAN86_BRG_H__2F6BA2FE_6BD1_4149_A80E_37F97981F0E7__INCLUDED_)
