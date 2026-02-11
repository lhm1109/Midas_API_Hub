#if !defined(AFX_CMCOMPOMATDLG_H__932FFC4B_4054_4C91_B645_B8C7EAE66E02__INCLUDED_)
#define AFX_CMCOMPOMATDLG_H__932FFC4B_4054_4C91_B645_B8C7EAE66E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMCompoMatDlg.h : header file
//
#include "..\mit_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"	
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMCompoMatDlg dialog
class CDBDoc;
//class CCMSectItemSrc;
class CCMSectItemCon;
//class CCMSectItemPSC_CMP;
class CCMSectItemTap;

class CCMCompoMatDlg : public CDialogMove
{
// Construction
public:
	CCMCompoMatDlg(CDBDoc* pDoc,
		CRect& rect, UINT ParentID, CWnd* pParent=NULL);   // standard constructor

// Attributes
public:
	CDBDoc* m_pDoc;
	CRect m_rect;
	//CCMSecPageSrcPage *m_pParent;
	CCMSectItemCon     *m_pParentCon;
	//CCMSectItemSrc     *m_pParentSrc;
	//CCMSectItemPSC_CMP *m_pParentPSC;
	CCMSectItemTap     *m_pParentTap;
	UINT  m_ParentID;

	int m_nCurDBSlab;
	int m_nCurNameSlab;
	int m_nCurDBGirder;
	int m_nCurNameGirder;
// Dialog Data
	//{{AFX_DATA(CCMCompoMatDlg)
	enum { IDD = IDD_CMD_SP_CODE_COMPO_DLG };
	MComboBox	m_cboNameGirder;
	MComboBox	m_cboDBGirder;
	MComboBox m_cboCodeGirder;
	MComboBox	m_cboNameSlab;
	MComboBox	m_cboDBSlab;
	MComboBox	m_cboCodeSlab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMCompoMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int ChangeDBNameList();
	int ChangeCodeNameList();
	int ChangeMaterialNameList(CString StructuralType);
	void ChangeValueInParentDialog();

	// Generated message map functions
	//{{AFX_MSG(CCMCompoMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDBSlab();
	afx_msg void OnChangeCodeSlab();
	afx_msg void OnChangeDBGirder();
	afx_msg void OnChangeCodeGirder();
	afx_msg void OnChangeNameSlab();
	afx_msg void OnChangeNameGirder();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMCOMPOMATDLG_H__932FFC4B_4054_4C91_B645_B8C7EAE66E02__INCLUDED_)
