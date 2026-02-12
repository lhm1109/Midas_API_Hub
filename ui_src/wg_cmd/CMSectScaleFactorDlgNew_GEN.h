#if !defined(__CMSECTSCALEFACTORDLGNEW_GEN_H__)
#define __CMSECTSCALEFACTORDLGNEW_GEN_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectScaleFactorDlgNew_GEN.h : header file
//
#include "../MIT_frx/MButton.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

class CDBDoc;
class CCMSectScaleFactorGrid_GEN;

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlgNew_GEN dialog
class CCMSectScaleFactorDlgNew_GEN : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMSectScaleFactorDlgNew_GEN(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectScaleFactorDlgNew_GEN();

// Dialog Data
	//{{AFX_DATA(CCMSectScaleFactorDlgNew_GEN)
	enum { IDD = IDD_CMD_SECT_SCALE_DLG_NEW_MGEN };
	MButton	  m_btnStif;
	MButton	  m_btnAdd;
	CListCtrl	m_listSect;
	MComboBox	m_cobxBdgr;
	//}}AFX_DATA
	CEditUnit m_editStif[7];
	MButton m_chkBefore;
	MButton m_chkAfter;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectScaleFactorDlgNew_GEN)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	
public:
	void OnChangeCurrentSecf(T_SECF_K nKey);

protected:
	void    SetHeaderTitle();
	void    MakeItemEx(CArray<T_SECT_K, T_SECT_K>* paKey=NULL);
	BOOL    InsertItem(T_SECT_K Key, T_SECT_D& Data, T_SECF_D*);
	int     FindInsertionPos(T_SECT_K nID);
	CString DataToStr(int i, T_SECT_K Key, T_SECT_D &Data);

	void      ApplyScaleFactor(T_SECT_STIFFNESS& Stiff, T_SECF_D& Secf);

	void      SetBngrCombo();
	T_BNGR_K  GetSelectedBngr();
	void      ChangeSelect(T_BNGR_K Key);
	int       TypeByDefaultSecfValue(T_SECF_K nKey);
	int       TypeByDefaultSecfValue(T_SECT_K nKey);

protected:
	CDBDoc* m_pDoc;
	CCMSectScaleFactorGrid_GEN* m_pGrid;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectScaleFactorDlgNew_GEN)
	virtual BOOL OnInitDialog();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnStiff();
	afx_msg void OnCmdBtnBdgr();
	afx_msg void OnClickCmdListSection(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Size Á¶Àý
protected:
	void SaveWindowSizeData();
	void ArrangeControls(LPRECT pRect);

	CRect m_MinimumRect;
	int m_nDlgHeight;
	int m_nGridFrmHeight;

	//int m_nRuleGridWidth;
	//int m_nStatusFrmWidth;
	//int m_nStatusGridWidth;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTSCALEFACTORDLGNEW_GEN_H__)
