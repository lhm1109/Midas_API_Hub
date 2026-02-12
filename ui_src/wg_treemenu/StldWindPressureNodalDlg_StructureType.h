#if !defined(__TM_STLD_WIND_NODAL_STRUCTURE_TYPE_H___)
#define __TM_STLD_WIND_NODAL_STRUCTURE_TYPE_H___

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustKBC2009Dlg.h : header file
//
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMWindPressureArea_KBC2009_GustF dialog

class CStldWindPressureNodalDlg_StructureType : public CDialogMove
{
// Construction
public:
	CStldWindPressureNodalDlg_StructureType(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStldWindPressureNodalDlg_StructureType();

	enum { IDD = IDD_TM_STLD_WINDP_DLG_NODAL_STRUCT };

// Attribute
public:
	void    SetWdpr(T_WDPR_D* pWdpr);
	void    GetNodalStructure(T_WDPR_D* pWdpr);

protected:
	CComboBoxEx m_cbxType;

	CEditUnit   m_edtB1;
	CEditUnit   m_edtB2;
	CEditUnit   m_edtB3;
	CEditUnit   m_edtB4;
	CEditUnit   m_edtR1;
	CEditUnit   m_edtR2;
	CEditUnit   m_edtH;
	CFormulaEditSpin   m_edtAngle;

	CTextUnit   m_untB1;
	CTextUnit   m_untB2;
	CTextUnit   m_untB3;
	CTextUnit   m_untB4;
	CTextUnit   m_untR1;
	CTextUnit   m_untR2;
	CTextUnit   m_untH;
	CTextUnit   m_untAngle;
	
	T_WIND_NODAL_STRUCT m_Data;

	T_WDPR_D* m_pWdpr;

	CImageList* m_pImageList;

protected:
	void InitTypeCombo();
	void InitControl();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL bCheckValue();

	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
	afx_msg void OnDeltaposCmdAngle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangeType();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
