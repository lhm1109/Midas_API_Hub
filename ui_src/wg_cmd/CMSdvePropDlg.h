#if !defined(AFX_CMSdvePropDlg_H__INCLUDED_)
#define AFX_CMSdvePropDlg_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSdvePropDlg dialog
class CCMSdvePropGrid;
class __MY_EXT_CLASS__ CCMSdvePropDlg : public CDialogMove
{
// Construction
public:
	CCMSdvePropDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSdvePropDlg();

// Dialog Data
//{{AFX_DATA(CCMSdvePropDlg)
	enum { IDD = IDD_ETC_SDVP_VISCOELA_PROP_DLG };
//}}AFX_DATA

protected:
	CCMSdvePropGrid* m_wndGrid;
	int m_nMatlType;
	double m_dValue[20];

public:
	void SetMatlType(int nMatlType) { m_nMatlType = nMatlType; }
	void SetValue(double* dVal) { memcpy(m_dValue, dVal, sizeof(m_dValue)); }  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSdvePropDlg)
protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CCMSdvePropDlg)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void SetTitleInGrid();
	void SetDataInGrid();
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSdvePropDlg_H__INCLUDED_)
