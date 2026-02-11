#if !defined(__CMSECTSCALEFACTORTAB_J_H__)
#define __CMSECTSCALEFACTORTAB_J_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectScaleFactorTab_J.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMSectScaleFactorGrid;
/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorTab_J dialog

class CCMSectScaleFactorDlgNew;
class CCMSectScaleFactorTab_J : public CChildDialog
{
// Construction
public:
	CCMSectScaleFactorTab_J(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectScaleFactorTab_J();

// interface
public:
	CCMSectScaleFactorGrid* GetGrid() {return m_wndGrid;}
	//void SetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData);
	//void GetCurData(CArray<T_RPSC_MBAR, T_RPSC_MBAR&>* pData, BOOL bSaveAlways);

// Dialog Data
	//{{AFX_DATA(CCMSectScaleFactorTab_J)
	enum { IDD = IDD_CMD_SECT_SCALE_TAB_J };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectScaleFactorTab_J)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMSectScaleFactorDlgNew* m_pParent;
	CCMSectScaleFactorGrid* m_wndGrid;
	//CArray<T_RPSC_MBAR, T_RPSC_MBAR&> m_Data;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectScaleFactorTab_J)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTSCALEFACTORTAB_J_H__)
