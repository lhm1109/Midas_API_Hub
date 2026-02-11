#if !defined(AFX_CMSTAGEDEFITEMBASE_H__)
#define      AFX_CMSTAGEDEFITEMBASE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemBase.h : header file
//

#include "..\wg_base\wg_base_DlgChild.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageItemBase dialog
class CDBDoc; // public CDlgChild
struct T_STAG_D;
class CCMStageItemBase : public CChildDialog , public CDBUpdateConnector
{
// Construction
public:
	CCMStageItemBase(CWnd* pParent = NULL);   // standard constructor
	CCMStageItemBase(UINT nResID, CWnd* pParent);
	~CCMStageItemBase();
	static BOOL IsCurStageBase();
//---------------------------------------------------
// Interface
//---------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void SetCurStageData(T_STAG_D* pStageD);
	virtual void ResetParam(); // 각 Item별 입력 필드를 초기화 한다. 
//---------------------------------------------------
// Implementation
//---------------------------------------------------
protected:
protected:
	CDBDoc*   m_pDoc;
protected:
// Dialog Data
	//{{AFX_DATA(CCMStageItemBase)
	enum { IDD = 0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageItemBase)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageItemBase)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // 
