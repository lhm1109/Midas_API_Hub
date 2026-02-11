#if !defined(AFX_ETC_GDS_KS_BRG_H__E39085CD_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_ETC_GDS_KS_BRG_H__E39085CD_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_KS_BRG.h : header file
//
#include "MyChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"

class CSpfcKSBRGUtil;
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS_BRG dialog

class CETC_GDS_KS_BRG : public CMyChildDialog
{
// Construction
public:
	CETC_GDS_KS_BRG(T_SPFC_D* pParamData, BOOL bUseTs, CWnd* pParent = NULL);   // standard constructor
	void CETC_GDS_KS_BRG::End();
	void EndNotClose(){};
	BOOL ISVALID();
	void SetValue();
	BOOL Dlg2Data();
	void SetPushOverData(BOOL bInit=TRUE);
	void InitUnit();
	void AlignControls();
	void ShowHideControls();
	double GetTs() const;

protected:
	T_SPFC_D* m_pParamData;
	std::shared_ptr<CSpfcKSBRGUtil> m_pSpfcUtil;
	BOOL m_bUseTs;	

// Dialog Data
	//{{AFX_DATA(CETC_GDS_KS_BRG)
	enum { IDD = IDD_ETC_GDS_KS_BRIDGE };
	MComboBox	m_ctlRMF;
	MComboBox	m_ctlIF;
	CString	m_sIF;
	CString	m_sRMF;
	int		m_nSoil;
	int		m_nEA;
	double	m_dMaxPeriod;
	CEditUnit m_edtTs;
	CTextUnit m_untTs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_KS_BRG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_KS_BRG)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdKSBridgeSoilTypeRdo();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_KS_BRG_H__E39085CD_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
