
#pragma once

/////////////////////////////////////////////////////////////////////////////
// ResponseDispLoadMode1Dlg dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_base\DlgChild.h"
#include "..\wg_db\SelectLC.h"
#include "..\wg_db\CobxLdgr.h"
#include "..\wg_db\UserDefType.h"
struct ResponseDispLoad2CalPara
{
		T_KEY m_StldK;
		T_KEY m_LdGpK;
		int   m_nOption;

		BOOL   m_bCalAbsDisp;
		int    m_nDispDir;
		double m_dGroudZ;
		int    m_nLoadType;
		int    m_nDir;
		double m_dUmax;
		double m_dDepth;
		double m_dTs;
		double m_dTsd;
		double m_dTsbd;     
		T_POINT3D m_PtOrg;
		T_POINT3D m_PtXDir;
};

class ResponseDispLoadMode2Dlg : public CMenuBarChildDlg
{
// Construction
public:
	ResponseDispLoadMode2Dlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(ResponseDispLoadMode2Dlg)
	enum { IDD = IDD_TM_RESPONS_DISP_LOAD_MODE2 };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResponseDispLoadMode2Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(ResponseDispLoadMode2Dlg)
	virtual BOOL OnInitDialog();
	virtual void Execute() override { return OnBnClickedTmExecute(); }
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
		afx_msg void OnBnClickedTmExecute();
		afx_msg void OnBnClickedTmClose();
private:
		CSelectLC	m_LoadCase;
		CCobxLdgr	m_wndLdgrCobx;

		CTextUnit m_UnitDisp ;
		CTextUnit m_UnitDepth;
		CTextUnit m_UnitTs   ;
		CTextUnit m_UnitTsd  ;
		CTextUnit m_UnitTsdn ;

		CEditUnit m_EditDisp ;
		CEditUnit m_EditDepth;
		CEditUnit m_EditTs   ;
		CEditUnit m_EditTsd  ;
		CEditUnit m_EditTsdn ;

		CTextUnit m_DistUnit1;
		CTextUnit m_DistUnit2;
		CMouseEdit	m_EditXDir;
		CMouseEdit	m_EditOrigin;
		CDBDoc *m_pDoc;
public:
		afx_msg void OnBnClickedTmBfBtnLc();
		afx_msg void OnBnClickedTmDefineLdgrBtn();
		afx_msg void OnCbnSelchangeComboLoadType();
		afx_msg void OnBnClickedRadioRelatDisp();
		afx_msg void OnBnClickedRadioAbsDisp();
private:
		int    GetLoadType() const;
		double GetGroudZ();
		BOOL   GetPara(ResponseDispLoad2CalPara &para);
		double Cal_L(const ResponseDispLoad2CalPara &para );
		 double CalUmaxZ(const ResponseDispLoad2CalPara &para, double dCalZ);
		double CalAbsUmax(const ResponseDispLoad2CalPara &para, const T_NODE_D &nodeCal, double dCalZ);
		BOOL   AddData(T_KEY_LIST &lstNodes, const ResponseDispLoad2CalPara &para);
		double GetStuctMinZBySlab();
};
