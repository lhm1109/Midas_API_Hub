// ReportUnitCustomizeDlg.h: interface for the CReportUnitCustomizeDlg class.
//////////////////////////////////////////////////////////////////////

#if !defined(__ReportUnitCustomizeDlg_H__)
#define __ReportUnitCustomizeDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wg_treemenuRes2.h"
#include "..\wg_base\wg_base_DialogMove.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MTabCtrl.h"

using namespace mit::frx;
struct T_UNIT_CUSTOM_DATA
{
	CString strName;
	int   nType;    // TYPE_UTBL=0, TYPE_UCHT=1, TYPE_SECTSUMM=2, TYPE_REINFORC=3, TYPE_COMPSECT=4;
	UINT  nKey;
	int   nUnitSys; // 0:Current Unit System, 1:Defined Unit System
	BOOL  bApplyAll;
	T_UNIT_INDEX UnitIdx;

	void Initialize()
	{
		strName = _T("");
		nType = 0;
		nKey  = 0;
		nUnitSys = 0;
		UnitIdx.nBase_Length  = 0;
		UnitIdx.nBase_Force   = 0;
		UnitIdx.nBase_Heat    = 0;
		UnitIdx.nBase_Temper  = 0;
		bApplyAll = FALSE;
	}
};

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CReportUnitCustomizeDlg : public CDialogMove
{
// Construction
public:
	CReportUnitCustomizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReportUnitCustomizeDlg();
	
// Dialog Data
	//{{AFX_DATA(CReportUnitCustomizeDlg)
	enum { IDD = IDD_TM_REPORT_UNIT_DLG };
	MTabCtrl m_tabItem;
	CListCtrl m_List;
	int m_nSystemRdo;
	MComboBox m_cmbLeng;
	MComboBox m_cmbForc;
	MComboBox m_cmbHeat;
//  MComboBox m_cmbTemp;
	BOOL m_bApplyAll;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportUnitCustomizeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CReportUnitCustomizeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnUnitSysRdo();
	afx_msg void OnDefSysRdo();
//  afx_msg void OnSetDefaultBtn();
	afx_msg void OnDefaultAllBtn();
	afx_msg void OnApplyAllChk();
	afx_msg void OnApplyBtn();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangeLst(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnComboBoxSelChange();
	//}}AFX_MSG  
	DECLARE_MESSAGE_MAP()

public:
	void Initialize();
	void Dlg2Data();
	void Dlg2Data(int nIndex);
	void Data2Dlg();
	void Data2Dlg(int nIndex);
	//void  SetData(UINT Key, int nType); // [nType] 0:UtblK, 1:UchtK, 2:Special Tables

public:
	int  m_nMode; // WORK_XXXX_LST(트리에서 선택한 아이템에 따라 결정)
	UINT m_Key;   // m_nMode가 개별 아이템일 때 사용

protected:
	CArray<T_UNIT_CUSTOM_DATA, T_UNIT_CUSTOM_DATA> m_aData; // 데이터의 Index는 ListCtrl과 동일
	CArray<T_UNIT_CUSTOM_DATA, T_UNIT_CUSTOM_DATA> m_aData_Table;
	CArray<T_UNIT_CUSTOM_DATA, T_UNIT_CUSTOM_DATA> m_aData_Chart;

	CArray<UINT, UINT> m_aCtrlID_SetUnit;
	T_UNIT_INDEX m_UnitIdxCur; // 현재 단위계

protected:
	void InitComboBox();
	void SetListCtrlHeader();
	void SetListCtrlItem();
	void Apply();
	BOOL EndEdit(BOOL bEnd);
	void MakeData();
};
#include "HeaderPost.h"

#endif // !defined(__ReportUnitCustomizeDlg_H__)
