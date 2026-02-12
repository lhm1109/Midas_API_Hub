#if !defined(AFX_BndrSsprCalcDlg_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BndrSsprCalcDlg_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSsprCalcDlg.h : header file
//

#include "wg_treemenuRes2.h"

#include "..\MIT_frx\MillustViewer.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

struct T_SOSD_DATA
{
	CString strName;                // Soil Support Define Name
	int     nSupportType;           // 1:Soil Stress, 2:Value
	double  dStress;                // valid if soil stress
	double  dModulus;               // valid if value
	int     nSoilType;              // 1:normal, 2:tension, 3:compression
	int     nAxis;                  // 0:x, 1:y, 2:z

	void Initialize()
	{
		strName.Empty();
		nSupportType = 1;
		dStress = 0.0;
		dModulus = 0.0;
		nSoilType = 1;
		nAxis = 2;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CBndrSsprCalcDlg dialog
class CBndrSsprCalcDlg : public CDialogMove
{
	// Construction
public:
	CBndrSsprCalcDlg(CWnd* pParent = NULL);   // standard constructor
	void SetSoilType();
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	void CalcModulus();
	void AlignControl();
	void SetSosdData(T_SOSD_DATA& data) { m_pData = &data; }
	void ChangeText();

	// Dialog Data
	//{{AFX_DATA(CBndrSsprCalcDlg)
	enum { IDD = IDD_TM_BNDR_SSPR_CALC_DLG};
	int m_nAxis;
	CEditUnit	m_modulus2;
	CEdit	m_name;
	CTextUnit	m_stressunit;
	CEditUnit	m_stress;
	CTextUnit	m_modulusunit;
	CEditUnit	m_modulus;
	int		m_soiltype; // 0:Normal, 1:Tension Only, 2:Compression Only
	int   m_supporttype;
	mit::frx::MillustViewer m_wndPicture;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSsprCalcDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CArray<UINT, UINT> m_group1, m_group2, m_group3;
	BOOL         m_bInit;
	T_SOSD_DATA* m_pData;

	BOOL m_bUseAxis;

	// Generated message map functions
	//{{AFX_MSG(CBndrSsprCalcDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeCmdSsAxis();
	afx_msg void OnChangeCmdSsStress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL CalcSpring(T_SOSD_DATA* pData);
public:
	void SetSpringType(int nType);
	void SetUseAxisOpt(BOOL bUseAxis);
	double GetCalcValue();
	void OnCalcualteBtn() ;

};
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_BndrSsprCalcDlg_H__66BE7321_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)