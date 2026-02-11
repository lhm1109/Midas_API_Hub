#if !defined(__SEISCVLSECTEFFSTIFFDLG_H__)
#define __SEISCVLSECTEFFSTIFFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlSectEffStiffDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CSeisCvlSectEffStiffDlg dialog


struct T_EFFStif_D
{
	CString strName;
	T_MPCC_K MpccK;
	T_MPCC_K MpccKPre;
	double dEffL;
	double dEffT;
};

class CSeisCvlSectEffStiffDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSeisCvlSectEffStiffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Dialog Data
	//{{AFX_DATA(CSeisCvlSectEffStiffDlg)
	enum { IDD = IDD_TM_SEIS_CVL_SECT_EFF_STIFF };

	MComboBox m_cmbMPhi;
	mit::frx::MEdit     m_edtSect;
	mit::frx::MEdit     m_edtPos;
	CEditUnit m_edtIyL;
	CEditUnit m_edtIyT;
	CEditUnit m_edtIyLMod;
	CEditUnit m_edtIyTMod;

	CEditUnit m_edtMyL;
	CEditUnit m_edtMyT;
	CEditUnit m_edtMeL;
	CEditUnit m_edtMeT;
	CTextUnit m_untMyL;
	CTextUnit m_untMyT;
	CTextUnit m_untMeL;
	CTextUnit m_untMeT;

	CListCtrl	m_list;

	BOOL	m_bUserDefined;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlSectEffStiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	void InitUnit();
	void InitCtrl();
	void InitCombo();
	void AlignCtrl();
	void DoUnitChange();
	void Data2Dlg();
	void Dlg2SyefData(BOOL bDgnType1=FALSE);
	int GetSyefType();

	void SetHeaderTitle();
	void MakeItemEx();
	BOOL InsertItem(int nIndex, T_EFFStif_D& Data);
	BOOL ModifyItem(int nIndex, T_EFFStif_D& Data);
	CString DataToStr(int i, T_EFFStif_D& Data);

	CDBDoc* m_pDoc;
	T_SYEF_K m_SyefK;
	T_SYEF_D m_SyefD;

	double m_dIeffRatMod[2];

	T_SECT_K m_SectK;
	BOOL m_bPreDgnCode;

	int m_nDesignCode;
	// Generated message map functions
	//{{AFX_MSG(CSeisCvlSectEffStiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmSectEffStiffMPhiCmb();
	afx_msg void OnTmSectEffStiffMPhiBtn();
	afx_msg void OnTmSectEffStiffApplyBtn();
	afx_msg void OnTmSectEffStiffCloseBtn();
	afx_msg void OnTmSectEffStiffUpdateBtn();
	afx_msg void OnTmSectEffStiffUserDefinedChk();

	afx_msg void OnTmEvalGrupColumnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnTmEvalGrupColumnAddBtn();
	afx_msg void OnTmEvalGrupColumnModBtn();
	afx_msg void OnTmEvalGrupColumnDelBtn();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCVLSECTEFFSTIFFDLG_H__)
