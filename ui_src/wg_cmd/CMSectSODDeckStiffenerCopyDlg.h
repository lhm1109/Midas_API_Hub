#if !defined(__CMSectSODDeckStiffenerCopyDlg_H__)
#define __CMSectSODDeckStiffenerCopyDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectSODDeckStiffenerCopyDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"


/////////////////////////////////////////////////////////////////////////////
// CCMSectSODDeckStiffenerCopyDlg dialog
class CCMSectSODDeckStiffenerCopyDlg : public CDialogMove
{
	// Construction
public:
	CCMSectSODDeckStiffenerCopyDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCMSectSODDeckStiffenerCopyDlg)
	enum { IDD = IDD_CMD_SECT_SOD_DECK_STIFFENER_COPY_DLG };

	CListBox m_lstStiff;
	CListBox m_lstSelStiff;
	CButton	 m_chkCopyMirror;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectSODDeckStiffenerCopyDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData(T_SECT_D& Data, int nDeckPos, int nDeckPart, CArray<int, int>& arDeckPart);
	void GetData(CArray<int, int>& arDeckPartR);
	void SetInitList();
	void InitCtrls();

	BOOL GetUnSelData(CArray<int, int&>& aKey, BOOL bAll);
	BOOL GetSelData(CArray<int, int&>& aKey, BOOL bAll);

	CDBDoc *m_pDoc;
	T_SECT_D m_Data;
	int m_nPos;
	int m_nPart;
	int m_nDeckPartCnt;
	BOOL m_bWeb2;
	BOOL m_bCopyMirror;
	CArray<int, int> m_arDeckPart;
	CArray<int, int> m_arDeckPartSel;

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectSODDeckStiffenerCopyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnSODSMCopySelBtn();
	afx_msg void OnSODSMCopyUnSelBtn();
	afx_msg void OnSODSMCopyAllBtn();
	afx_msg void OnSODSMCopyNoneBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:


protected:


private:
	//bool m_bStlCompSect;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSectSODDeckStiffenerCopyDlg_H__)
