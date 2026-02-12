#if !defined(__CM_WIND_VELO_ASCE7_16_H__)
#define __CM_WIND_VELO_ASCE7_16_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MButton.h"
using namespace mit::frx;
class CCMWindPressureVelocityAddMod;

class CCMWindPressureVelo_ASCE7_16 : public CChildDialog
{
	// Construction
public:
	CCMWindPressureVelo_ASCE7_16(CWnd* pParent = NULL);   // standard constructor
	
	void SetData2Dlg(T_WVEP_ASCE7_16& data);
	BOOL SetDlg2Data(T_WVEP_ASCE7_16& data);
	
	// Dialog Data
	//{{AFX_DATA(CCMWindPressureVelo_ASCE7_16)
	enum {IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_ASCE7_16	};
	MComboBox	m_cobxCategory;
// 	CTextUnit m_unitSpeed;        // by hp, 단위변환 안되야함..
	CEditUnit	m_editSpeed;
	CTextUnit	m_unitRoofHeight;
	CEditUnit	m_editRoofHeight;
	MButton	m_chkTopoEffect;
	CEditUnit	m_editKZT;
	
	//}}AFX_DATA
	
protected:
	void InitDefaultData();
	void InitCategoryCombo();
	void Data2Dlg();
	BOOL Dlg2Data();

private:
	CCMWindPressureVelocityAddMod* m_pParent;

protected:
	int m_nClassifyStructure;
	int m_nClassifyPlane;
	double m_dClassifyBX;
	double m_dClassifyBY;
	double m_dAreadKBC2015;
	
	BOOL m_bInit;
	T_WVEP_ASCE7_16  m_Data;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindPressureVelo_ASCE7_16)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CCMWindPressureVelo_ASCE7_16)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAutoCalcBtn();
	afx_msg void OnCmdTopoEffectCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CM_WIND_VELO_ASCE7_16_H__)
