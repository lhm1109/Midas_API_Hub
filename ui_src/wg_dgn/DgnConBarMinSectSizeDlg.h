#if !defined(__DgnConBarMinSectSizeDlg_H__)
#define __DgnConBarMinSectSizeDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBarMinSectSizeDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConBarMinSectSizeDlg dialog

class __MY_EXT_CLASS__ CDgnConBarMinSectSizeDlg : public CDialogMove
{
// Construction
public:
	CDgnConBarMinSectSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConBarMinSectSizeDlg)
	enum { IDD = IDD_DGN_CON_BAR_MIN_SECT_DLG };

	CString	m_strCode;
	CString m_strRCCode;
	
	BOOL m_bBeam;
	BOOL m_bColm;
	BOOL m_bBrce;
	BOOL m_bWall;
	BOOL m_bSlab;
	BOOL m_bMat;

	//}}AFX_DATA

public:

	void Initial_DefaultData();
	void Initial_ExistData();
		
	int GetDataStatus(CString strCode);
		
	CString m_strNationalAnnex;  
	
	CString ConvertNationalAnnexbyCString(int nNationalAnnex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConBarMinSectSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConBarMinSectSizeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	/*afx_msg void OnDgnRbarRatioUpdateBtn();*/
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnConBarMinSectSizeDlg_H__B1565525_E5DB_11D3_888F_0000C0F30D4D__INCLUDED_)
