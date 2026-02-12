#if !defined(AFX_CMCONCRSTEELMATLDLG_H__9347CF01_2C7C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMCONCRSTEELMATLDLG_H__9347CF01_2C7C_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMConcrSteelMatlDlg.h : header file
//
#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

//class CCMSecPageSrcPage;
class CDBDoc;
class CCMSectItemSrc;
class CCMSectItemCon;
class CCMSectItemPSC_CMP;
class CCMSectItemTap;

/////////////////////////////////////////////////////////////////////////////
// CCMConcrSteelMatlDlg dialog
class CCMConcrSteelMatlDlg : public CDialogMove
{
// Construction
public:
	CCMConcrSteelMatlDlg(CDBDoc* pDoc,
		CRect& rect, UINT ParentID, CWnd* pParent=NULL, BOOL bDumbbell=FALSE);   // standard constructor

// Attributes
public:
	CDBDoc* m_pDoc;
	CRect m_rect;
	BOOL m_bDumbbell;
	//CCMSecPageSrcPage *m_pParent;
	CCMSectItemCon     *m_pParentCon;
	CCMSectItemSrc     *m_pParentSrc;
	CCMSectItemPSC_CMP *m_pParentPSC;
	CCMSectItemTap     *m_pParentTap;
	UINT  m_ParentID;

	int m_nCurDBConcr;
	int m_nCurNameConcr;
	int m_nCurDBSteel;
	int m_nCurNameSteel;

// Dialog Data
	//{{AFX_DATA(CCMConcrSteelMatlDlg)
	enum { IDD = IDD_CMD_SP_CODE_DLG };
	MComboBox	m_cboNameSteel;
	MComboBox	m_cboDBSteel;
	MComboBox	m_cboNameConcr;
	MComboBox	m_cboDBConcr;
	MComboBox m_cboCodeConcr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMConcrSteelMatlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int ChangeDBNameList();
	int ChangeCodeNameList();
	int ChangeMaterialNameList(CString MatlType);
	void ChangeValueInParentDialog();

	// Generated message map functions
	//{{AFX_MSG(CCMConcrSteelMatlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDBConcr();
	afx_msg void OnChangeCodeConcr();
	afx_msg void OnChangeDBSteel();
	afx_msg void OnChangeNameConcr();
	afx_msg void OnChangeNameSteel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMCONCRSTEELMATLDLG_H__9347CF01_2C7C_11D3_92DE_0000C0B0E6B3__INCLUDED_)
