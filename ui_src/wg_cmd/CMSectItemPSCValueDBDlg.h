#if !defined(AFX_CMSECTITEMPSCVALUEDBDLG_H__FC197D81_457B_43F3_B477_293971C60A00__INCLUDED_)
#define AFX_CMSECTITEMPSCVALUEDBDLG_H__FC197D81_457B_43F3_B477_293971C60A00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCValueDBDlg.h : header file
//

#include "CMSecViewWnd.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCValueDBDlg dialog

class CCMSectItemPSCValueDBDlg : public CDialogMove
{
// Construction
public:
	CCMSectItemPSCValueDBDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCValueDBDlg)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC_VALUE_DB };
	CListBox	m_List;
	MComboBox	m_cmbType;
	MComboBox	m_cmbCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCValueDBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	T_SECT_D GetData() { return m_SectD; }
	int m_nCodeForLog;
	
protected:
	void SetDataSource(T_SECT_D *pSectD);
	void SetInitCode();
	int  GetTypeListByCode(const CString strCode, CStringArray &aList);
	BOOL GetCurSectData(const int nSel);
	void SetSectDBList(const CString strType);
	CString GetSectDBFileName(const CString strType);
	

// Implementation
protected:
	T_SECT_D m_SectD;
	CCMSecViewWnd m_wndSecView;
	CArray<UINT, UINT>          m_aImportID;
	CArray<T_SECT_D, T_SECT_D&> m_aImportSect;
	
	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCValueDBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdCodeCmb();
	afx_msg void OnSelchangeCmdTypeCmb();
	afx_msg void OnSelchangeCmdSectList();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCVALUEDBDLG_H__FC197D81_457B_43F3_B477_293971C60A00__INCLUDED_)
