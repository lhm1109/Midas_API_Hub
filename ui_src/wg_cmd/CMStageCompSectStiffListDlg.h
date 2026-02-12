#if !defined(AFX_CMSTAGECOMPSECTSTIFFLISTDLG_H__D342D683_C1E8_413C_904E_D8754EE1D456__INCLUDED_)
#define AFX_CMSTAGECOMPSECTSTIFFLISTDLG_H__D342D683_C1E8_413C_904E_D8754EE1D456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageCompSectStiffListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffListDlg dialog

class CCMStageCompSectStiffListDlg : public CDialogMove
{
// Construction
public:
	CCMStageCompSectStiffListDlg(int nType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMStageCompSectStiffListDlg)
	enum { IDD = IDD_CMD_SP_SECT_LIST_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageCompSectStiffListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void SetHeaderTitle();
	void MakeItemEx();
	CString DataToStr(int i, T_SECT_K Key, T_SECT_D &Data);
	void SetSectData();
	int  m_nType;
	// Generated message map functions
	//{{AFX_MSG(CCMStageCompSectStiffListDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nStiffListType; // 2:Stiffness, 3:Tapered Stiffness
	T_SECT_STIFFNESS m_StiffDataI;
	T_SECT_STIFFNESS m_StiffDataJ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGECOMPSECTSTIFFLISTDLG_H__D342D683_C1E8_413C_904E_D8754EE1D456__INCLUDED_)
