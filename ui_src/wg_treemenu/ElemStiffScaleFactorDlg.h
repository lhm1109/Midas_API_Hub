#if !defined(__ELEMSTIFFSCALEFACTORDLG_H__)
#define      __ELEMSTIFFSCALEFACTORDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemStiffScaleFactorDlg.h : header file
//

#include "..\MIT_frx\MEdit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

/////////////////////////////////////////////////////////////////////////////
// CElemStiffScaleFactorDlg dialog

class CElemStiffScaleFactorDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemStiffScaleFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	void Dlg2Data(T_ESSF_D &data);

	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CElemStiffScaleFactorDlg)
	enum { IDD = IDD_TM_ELEM_STIFF_SCALE_FACTOR	};
	CCobxBngr	m_wndGroupCombo;
	CComboBox m_wndStiffScaleTypeCombo;
	int		m_nAddOrDel;
	
	mit::frx::MEdit m_wndArea;
	mit::frx::MEdit m_wndAsy;
	mit::frx::MEdit m_wndAsz;
	mit::frx::MEdit m_wndIxx;
	mit::frx::MEdit m_wndIyy;
	mit::frx::MEdit m_wndIzz;
	mit::frx::MEdit m_wndWeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemStiffScaleFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CElemStiffScaleFactorDlg)
	virtual BOOL OnInitDialog();

	void Data2Dlg(T_ESSF_D &data);

	afx_msg void OnTmOption();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnTmWallType();
	afx_msg void OnTmSectEffStiffUpdateBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__ELEMSTIFFSCALEFACTORDLG_H__)
