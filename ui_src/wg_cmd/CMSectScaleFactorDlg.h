#if !defined(AFX_CMSECTSCALEFACTORDLG_H__D5065121_2F39_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMSECTSCALEFACTORDLG_H__D5065121_2F39_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectScaleFactorDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSectScaleFactorDlg : public CDialogMove
{
// Construction
public:
	CCMSectScaleFactorDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc*	m_pDoc;

public:
	void		MakeItemEx_Sect();
	void		SetHeaderTitle_Sect();
	int			FindInsertionPos_Sect(T_SECT_K nID);
	BOOL		InsertItem_Sect(T_SECT_K Key, T_SECT_D &Data);
	CString	DataToStr_Sect(int i, T_SECT_K Key, T_SECT_D &Data);

	void		MakeItemEx_Scal();
	void		SetHeaderTitle_Scal();
	int			FindInsertionPos_Scal(T_SECF_K nID);
	BOOL		InsertItem_Scal(T_SECF_K Key, T_SECF_D &Data);
	BOOL		DeleteItem_Scal(T_SECF_K Key, T_SECF_D &Data);
	CString	DataToStr_Scal(int i, T_SECF_K Key, T_SECF_D &Data);
// Dialog Data
	//{{AFX_DATA(CCMSectScaleFactorDlg)
	enum { IDD = IDD_CMD_SECT_SCALE_DLG };
	CButton	m_StiffBtn;
	CButton	m_DelBtn;
	CButton	m_AddBtn;
	CListCtrl	m_sflist;
	CListCtrl	m_slist;
	CEditUnit	m_fzzz;
	CEditUnit	m_fzyy;
	CEditUnit	m_fzxx;

	CEditUnit	m_fasz;
	CEditUnit	m_fasy;
	CEditUnit	m_farea;
	CEditUnit	m_fwgt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectScaleFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ApplyScaleFactor(T_SECT_STIFFNESS& Stiff, T_SECF_D& Secf);

	// Generated message map functions
	//{{AFX_MSG(CCMSectScaleFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSscaleAdd();
	afx_msg void OnCmdSscaleDel();
	afx_msg void OnClickCmdSscaleSflist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdSscaleSflist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickCmdSscaleSectlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdSscaleSectlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdSscaleShowstif();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTSCALEFACTORDLG_H__D5065121_2F39_11D5_880F_00010263A1F4__INCLUDED_)
