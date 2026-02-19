#if !defined(AFX_DGNCONCOLMDLG_H__5D7AB3A4_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONCOLMDLG_H__5D7AB3A4_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConColmDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "DgnColmGridWnd.h"
#include "DgnSelSectBase.h"


class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConColmDlg dialog

class CDgnConColmDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConColmDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConColmDlg)
	enum { IDD = IDD_DGN_CON_COLM_DLG };
	CEditUnit	m_S;
	CComboBox	m_SubRebar;
	CEditUnit	m_Bc;
	CEditUnit	m_D0;
	CEditUnit	m_Hc;
	CTextUnit	m_HcUnit;
	CTextUnit	m_D0Unit;
	CTextUnit	m_BcUnit;
	int		m_nOption;
	int		m_nTied;
	//}}AFX_DATA

public:
	T_RCHK_K m_RchkKey;
	T_REBC_D m_RchkData;
	CMap<int,int,int,int> m_arColmData;   // Element 중 Concrete Colm의 Data 저장 

	CDgnColmGridWnd m_wndGrid;
	CDgnSelSectBase m_SelectGrid;

	BOOL m_bUpdate;
	CString m_Shape;
	double m_SubSpace;
	int m_iSubRebarYNum;
	int m_iSubRebarZNum;
	int m_iBCJointRebarNum;

public:
	void ShowBitmap(CString str);
	void Write_GridTitle();
	void Write_GridItem(int Nqrb,int Nrow,CString Norb);

	void Initial_ExistData();
	void Initial_WithoutData();
	void Initial_Unit();
	void Initial_GridWnd();

	void Set_SectionShape(CString strShape);
	void Set_ShowSRSection();
	void Set_ShowSBSection();
	void Set_ShowPSection();

	BOOL ErrorCheck();

	void SetData2Dlg();
	BOOL SetDlg2Data();
	void ChangeSectionInfo(); 
	void SelectElemOnView();
	void InitSectGrid();
	void ResetSectGridData(BOOL bKeepChk = FALSE);

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConColmDlg)
	afx_msg void OnDgnColumnAr();
	afx_msg void OnDgnColumnDel();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnRcCinputbar();
	afx_msg void OnDgnRcCtied();
	afx_msg void OnDgnRcCspiral();
	afx_msg void OnDgnDesignSection();
	afx_msg void OnDgnColmSelectAll();
	afx_msg void OnDgnColmUnSelectAll();
	//}}AFX_MSG
	LRESULT OnSelectCheckSectionGrid(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFocusSectionGrid(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCOLMDLG_H__5D7AB3A4_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
