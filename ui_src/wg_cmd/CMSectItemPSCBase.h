#if !defined(__CMSECTITEMPSCBASE_H__)
#define __CMSECTITEMPSCBASE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemBase.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"

struct T_SECT_SECTION_D;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCBase dialog

class CCMSectItemPSCBase : public CChildDialog
{
// Construction
public:
	CCMSectItemPSCBase(CWnd* pParent = NULL);   // standard constructor
	CCMSectItemPSCBase(UINT nResID, CWnd* pParent);
	~CCMSectItemPSCBase();

// Interface
public:
	virtual BOOL GetDlgData(void* pData)=0;
	virtual void SetCurDlgData(void* pData, BOOL bModify)=0;

	virtual void DisplayOffsetPoint()=0;
	virtual void SetAutotShearCheckPos(double dShearPos[3])=0;
	virtual void SetAutoMinWebThick(double dShear[3], double dTorsion)=0;
	virtual void SetWarpingCheckPosition(double dWarpingCheckPosI[2][6], double dWarpingCheckPosJ[2][6])=0;
	virtual void OnAfterCalcPeriAndStiff(T_SECT_SECTION_D* pSectBefore) { }

protected:
// Dialog Data
	//{{AFX_DATA(CCMSectItemPSCBase)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCBase)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSCBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSCBASE_H__)
