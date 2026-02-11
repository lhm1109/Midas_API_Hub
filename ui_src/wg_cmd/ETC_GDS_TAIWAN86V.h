#if !defined(AFX_ETC_GDS_TAIWAN86V_H__D33D578D_99F5_467B_924E_04CFEE657707__INCLUDED_)
#define AFX_ETC_GDS_TAIWAN86V_H__D33D578D_99F5_467B_924E_04CFEE657707__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_TAIWAN86V.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcTAIWAN99VUtil;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN86V dialog
#include "MyChildDialog.h"
class CETC_GDS_TAIWAN86V : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_TAIWAN86V(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetPushOverData(BOOL bInit=TRUE);
	BOOL Dlg2Data();
	void Dlg2UtilData() override;

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcTAIWAN99VUtil> m_pSpfcUtil;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_TAIWAN86V)
	enum { IDD = IDD_ETC_GDS_TAIWAN86V };
	MComboBox	m_wndRCmb;
	MComboBox	m_wndImportanceCmb;
	MComboBox m_wndAlphaCmb;
	CEditUnit m_wndDamping;
	int		m_nZone;
	int		m_nSoil;
	double m_dMaxPeriod;	
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_TAIWAN86V)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_TAIWAN86V)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_TAIWAN86V_H__D33D578D_99F5_467B_924E_04CFEE657707__INCLUDED_)
