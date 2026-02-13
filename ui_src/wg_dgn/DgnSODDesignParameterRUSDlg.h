#if !defined(__DgnSODDesignParameterRUSDlg_H__)
#define __DgnSODDesignParameterRUSDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSODDesignParameterRUSDlg.h : header file
//
#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\DB_ST_DT.h"

/////////////////////////////////////////////////////////////////////////////
// DgnSODDesignParameterRUSDlg dialog

using namespace mit::frx;
class CDgnSODDesignParameterBaseDlg;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnSODDesignParameterRUSDlg : public CChildDialog
{
	// Construction
public:
	CDgnSODDesignParameterRUSDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnSODDesignParameterRUSDlg();
	
	// Dialog Data
	//{{AFX_DATA(DgnSODDesignParameterRUSDlg)
	enum { IDD = IDD_DGN_SOD_PARAM_RUS };
	
 	double	m_dFactor_m;
 	double	m_dFactor_gamma;

	MButton	m_bDgnParam_STN;
	MButton	m_bDgnParam_STB;
	MButton	m_bDgnParam_FAT;

	MComboBox m_CmbBRDType;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DgnSODDesignParameterRUSDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(DgnSODDesignParameterRUSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnClick_Factorm();
	//afx_msg void OnClick_Gamma();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	CDgnSODDesignParameterBaseDlg* m_pParent;
	
	T_SDPA_D *m_pData;

	CArray<UINT, UINT> m_aComponent;
	//CArray<UINT, UINT> m_aGamma;

	void InitBridgeType();

public:
	inline void SetData(T_SDPA_D* pData) { m_pData = pData; }
	void Data2Dlg();
	void Dlg2Data();		
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSODDesignParameterRUSDlg_H__)
