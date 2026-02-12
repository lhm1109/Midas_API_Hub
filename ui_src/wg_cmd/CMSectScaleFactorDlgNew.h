#if !defined(__CMSECTSCALEFACTORDLGNEW_H__)
#define __CMSECTSCALEFACTORDLGNEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectScaleFactorDlgNew.h : header file
//
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

class CDBDoc;
class CCMSectScaleFactorGrid;

/////////////////////////////////////////////////////////////////////////////
// CCMSectScaleFactorDlgNew dialog
class CCMSectScaleFactorDlgNew : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMSectScaleFactorDlgNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSectScaleFactorDlgNew();

// Dialog Data
	//{{AFX_DATA(CCMSectScaleFactorDlgNew)
	enum { IDD = IDD_CMD_SECT_SCALE_DLG_NEW };
	CButton	    m_btnStif;
	CButton	    m_btnAdd;
	CListCtrl	  m_listSect;
	CComboBox	  m_cobxBdgr;
	//}}AFX_DATA
	CEditUnit m_editStif[7];
	CEditUnit m_editStif2[7];
	CButton m_chkBefore;
	CButton m_chkAfter;
	CButton m_chkIJ;
	CArray<UINT, UINT> m_aCtrlEditJ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectScaleFactorDlgNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	
public:
	void OnChangeCurrentSecf(T_SECF_K nKey);
	//void SetGridI(CCMSectScaleFactorGrid* pGrid) {m_pGridI = pGrid;}
	//void SetGridJ(CCMSectScaleFactorGrid* pGrid) {m_pGridJ = pGrid;}

protected:
	void    SetHeaderTitle();
	void    MakeItemEx(CArray<T_SECT_K, T_SECT_K>* paKey=NULL);
	BOOL    InsertItem(T_SECT_K Key, T_SECT_D& Data, T_SECF_D*);
	int     FindInsertionPos(T_SECT_K nID);
	CString DataToStr(int i, T_SECT_K Key, T_SECT_D &Data);

	void      ApplyScaleFactor(T_SECT_STIFFNESS& Stiff, T_SECF_D& Secf, BOOL bJ=FALSE);

	void      SetBngrCombo();
	T_BNGR_K  GetSelectedBngr();
	void      ChangeSelect(T_BNGR_K Key);
	int       TypeByDefaultSecfValue(T_SECF_K nKey);
	int       TypeByDefaultSecfValue(T_SECT_K nKey);
	void      ChangeCheckIJ(BOOL bTapered); // Tapered 섹션이면 I = J 체크박스 비활성화 처리
	void      OnCmdIChkIJ2(); // OnCmdIChkIJ() 와 같으나 J 활성화시 I 와 같게 해주지 않음

protected:
	CDBDoc* m_pDoc;
	CCMSectScaleFactorGrid* m_pGrid;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMSectScaleFactorDlgNew)
	virtual BOOL OnInitDialog();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnStiff();
	afx_msg void OnCmdBtnBdgr();
	afx_msg void OnClickCmdListSection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdIChkIJ(); // I = J 체크박스 상태에 따라 J Editbox 활성/비활성
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Size 조절
protected:
	void SaveWindowSizeData();
	void ArrangeControls(LPRECT pRect);

	CRect m_MinimumRect;
	int m_nDlgHeight;
	int m_nGridFrmHeight;

	BOOL IsCmpTapBoxITub(T_SECT_D& Sect) { return (Sect.nStype==D_SECT_TYPE_TAPERED && (Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_B || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_I || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_TUB || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_B || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_I || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_TUB));}
	BOOL IsCmpTapCICT(T_SECT_D& Sect) { return (Sect.nStype==D_SECT_TYPE_TAPERED && (Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_CI || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_CT || Sect.SectBefore.nStype==D_SECT_TYPE_COMPO_G));}

	//int m_nRuleGridWidth;
	//int m_nStatusFrmWidth;
	//int m_nStatusGridWidth;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTSCALEFACTORDLGNEW_H__)
