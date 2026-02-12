#if !defined(AFX_STLDPNLDDLG_H__1BE21766_6732_4F41_86C8_2CD19CF33C1F__INCLUDED_)
#define AFX_STLDPNLDDLG_H__1BE21766_6732_4F41_86C8_2CD19CF33C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldPnldDlg.h : header file
//
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_Cobxpnld.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_db\wg_db_CobxGrup.h"

#include "..\mit_frx\MillustViewer.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CStldPnldDlg dialog

class CStldPnldDlg : public CMenuBarChildDlg
{
private:
	CDBDoc* m_pDoc;

// Construction
	int m_nCurDir;
public:
	CStldPnldDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	void	SetDataToDlg();
	BOOL	SetDlgToData();

	void	DestroyUCSDlg(CString sel_list);		// UCS Named Plane close될때.. 
	void	DestroyNPLNDlg(CString sel_list);		// Named Plane close될때.. 
	void	CalP3CoorNamedUCS(double p[3][3],T_NUCS_D &data);
	void	CalP3CoorNPLN(double p[3][3],T_NPLN_D &data);

	void	InitPtEditValue(void);
	void	EnableCopyPnldCtrl(void);
	void	EnableNodeDefLdArea(void);
	void	EnablePrjCbx(void);
	void  InitUnit(void);
	void  ElemSelCtrlInit();
	void  ElemSelCtrlMan();


// Dialog Data
	//{{AFX_DATA(CStldPnldDlg)
	enum { IDD = IDD_TM_STLD_PNLD };
	mit::frx::MComboBox	m_cbxPrj;
	CCobxGrup	m_CobxStGroup;
	mit::frx::MComboBox	m_CobxSelType;
	mit::frx::MComboBox	m_CobxFaceNo;
	CEditUnit	  m_edTrlc;
	CSelectLC		m_cbxLdc;
	CMouseEdit	m_edFstPt;
	CMouseEdit	m_edTrdPt;
	CMouseEdit	m_edSndPt;
	CMouseEdit	m_edNodeList;
	CCobxPnld		m_cbxLType;
	mit::frx::MComboBox		m_cbxLdDir;
	mit::frx::MComboBox		m_cbxEType;
	CCobxLdgr		m_cbxLdgr;
	CString			m_sDesc;
	CTextUnit		m_stUnitTrlc;
	CTextUnit		m_stUnitTrdPt;
	CTextUnit		m_stUnitSnd;
	CTextUnit		m_stUnitFstPt;
	CTextUnit		m_stUnitDist;
	CString	m_sFstPt;
	CString	m_sSndPt;
	CString	m_sTrdPt;
	BOOL	m_bNodeDefChk;
	BOOL	m_bCopyLdChk;
	int		m_nAxisRd;
	CString	m_sDist;
	CString	m_sNodeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldPnldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldPnldDlg)
	afx_msg void OnTmClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnTmPnlNodedefChk();
	afx_msg void OnTmPnlCopyPnld();
	afx_msg void OnTmPnlUcsBtn();
	afx_msg void OnTmPnlNmdplaneBtn();
	afx_msg void OnTmPnlLoadtypeBtn();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmExecute();
	afx_msg void OnTmBtnLc();
	afx_msg void OnSelchangeTmPnlLoadDir();
	afx_msg void OnSetfocusTmPnlNodeList();
	afx_msg void OnSelchangeTmSeltypeCombo();
	afx_msg void OnSelchangeTmPnlEleType();
	//}}AFX_MSG
	afx_msg LRESULT OnMouseEditEnter(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseEditEscape(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDPNLDDLG_H__1BE21766_6732_4F41_86C8_2CD19CF33C1F__INCLUDED_)
