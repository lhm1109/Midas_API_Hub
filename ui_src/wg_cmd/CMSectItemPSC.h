#if !defined(__CMSECTITEMPSC_H__)
#define __CMSECTITEMPSC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC.h : header file
//
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "CMSectItemBase.h"

#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MEdit.h"

// 여기에 추가 *^^* 
// Define Dialog IDs
#include "CCMSectItemPSCDefine.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC dialog
class CCMSectItemDlg;
class CCMSectItmePSCView;
class CCMSectItemPSCBase;

class CCMSectItemPSC : public CCMSectItemBase
{
// Construction
public:
	CCMSectItemPSC(CWnd* pParent = NULL);   
	virtual ~CCMSectItemPSC();   

// Dialog Data
	//{{AFX_DATA(CCMSectItemPSC)
	enum { IDD = IDD_CMD_SECT_ITEM_PSC };
	CTextUnit	m_unitMeshSize;
	CEditUnit	m_editMeshSize;
	mit::frx::MButton	m_chkMeshSize;
	MComboBox	m_cTypeCB;
	CBCGPStatic	m_wndHolder;
	mit::frx::MEdit	m_wndName;
	mit::frx::MEdit	m_wndID;
	//}}AFX_DATA

// Overrides
public:
	BOOL OnOKPublic() override;
	BOOL OnApplyPublic() override;
	void ShowCalcResult() override;
	void DisplayOffsetPoint() override;
	void SetNewID(T_SECT_K nNewID) override;

	void ShowKey2Dlg();
	BOOL GetPeriAndStiff(BOOL bReCalc=FALSE);
	BOOL SaveData();
	BOOL IsPSCValue();
	void InsertItems();
	void InitSectionTypeCombo(void);
	void SetIDName(/*UINT nID, */CString strName);
	BOOL Check_ShearPos();

public:
	BOOL IsViewWindow();
	BOOL ChangeBitmap(UINT nIndex);
	void SetDataSource(T_SECT_D* pDataSrc);
	void RedrawSection();
	void DisplayCentoid();
	void DisplayShearCheck();
	void ShowSectView(BOOL bShow);
	void OnCalculateButton();

	CCMSectItmePSCView* GetViewPointer() { return m_pViewer; }

	static unsigned int WM_AFTERINITDIALOG;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL ShowWindow(int nCmdShow);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Dlg2Data();
	BOOL ChildDlg2Data();
	void CreateChildDlgs();
	void DeleteChildDlgs();
	void ShowCurChildDlg(int nDlgIndex);
	void InitChildDialog(BOOL bModify);
	void CreatePSCView();
	void DestroyPSCView();

protected:
	BOOL m_bCreateView;
	int m_nCurChildDlg;
	int m_nInitChildDlg;
	CCMSectItmePSCView* m_pViewer;
	CStringArray m_ComboMenuStr;
	CArray<CCMSectItemPSCBase* , CCMSectItemPSCBase*> m_ChildDlgs;
	CArray<int,int> m_DlgIDs;
	CCMSectItemDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCMSectItemPSC)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdPscTypeCb();
	afx_msg void OnCmdMeshSizeChk();
	//}}AFX_MSG
	afx_msg LRESULT OnAfterInitDialog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMPSC_H__)
