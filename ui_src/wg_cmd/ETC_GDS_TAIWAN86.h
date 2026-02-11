#if !defined(AFX_ETC_GDS_TAIWAN86_H__342080F5_A929_4611_8B4C_30308B956CAB__INCLUDED_)
#define AFX_ETC_GDS_TAIWAN86_H__342080F5_A929_4611_8B4C_30308B956CAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_TAIWAN86.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcTAIWAN99HUtil;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_TAIWAN86 dialog
#include "MyChildDialog.h"
class CETC_GDS_TAIWAN86 : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_TAIWAN86(T_SPFC_D* pParamData, CWnd* pParent = NULL);   // standard constructor
	void End();
	void EndNotClose(){};
	void UpdateDampingWnd(CWnd* pDampingWnd);
	BOOL ISVALID();
	void SetPushOverData(BOOL bInit=TRUE);
	BOOL Dlg2Data();
	void Dlg2UtilData() override;

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcTAIWAN99HUtil> m_pSpfcUtil;

// Dialog Data
	//{{AFX_DATA(CETC_GDS_TAIWAN86)
	enum { IDD = IDD_ETC_GDS_TAIWAN86 };
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
	//{{AFX_VIRTUAL(CETC_GDS_TAIWAN86)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_TAIWAN86)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_TAIWAN86_H__342080F5_A929_4611_8B4C_30308B956CAB__INCLUDED_)
