#if !defined(__CMNLACDLG_MEC_H__)
#define __CMNLACDLG_MEC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMNlacDlg.h : header file

#include "../MIT_frx/MComboBox.h"
#include "../MIT_frx/MButton.h"

#include "../wg_base/SpinBtnExCtrl.h"
#include "../wg_base/MouseEdit.h"

#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_EditUnit.h"
#include "../wg_db/wg_db_TextUnit.h"
#include "../wg_db/FormulaEditUnitSpin.h"
#include "../wg_db/SelectLC.h"

#include "CMDlgBase.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMNlacDlg_MEC dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMNlacDlg_MEC : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMNlacDlg_MEC(CWnd* pParent = nullptr);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CCMNlacDlg_MEC)
	enum { IDD = IDD_ETC_NLAC_MEC };
	
	int				m_nMethod;
	int				m_nLoadStepType;
	CEditUnit		m_edtNumberOfIncrements;
	MComboBox		m_cmbIntermediateOutputRequest;

	MButton			m_chkDisp;
	MButton			m_chkLoad;
	MButton			m_chkWork;
	CEditUnit		m_edtDisp;
	CEditUnit		m_edtLoad;
	CEditUnit		m_edtWork;

	CEditUnit       m_edtMinArcRatio;
	CEditUnit       m_edtMaxArcRatio;
	CFormulaEditSpin m_edtMaxincr;

	CMouseEdit		m_edtDispCtrlMasterNode;
	CEditUnit		m_edtDispCtrlMaxDisp;
	CTextUnit		m_untDispCtrlMaxDisp;
	MComboBox		m_cmbDispCtrlDirection;
	MButton 		m_chkRelativeNode;
	CMouseEdit		m_edtRelativeNode;	

	MButton   m_chkGeom;
	MButton   m_chkMatl;

	CArray<UINT, UINT> m_rdoGlobalOrStld;
	BOOL m_bIsLastStateGlobal; // GlobalOrStld 라디오를 바꾸는 순간에 직전 상태가 Global 일 때 데이터 저장을 하기 위해 추가한 변수... 대화상자 진짜 개떡같다...

	CListCtrl	m_List;
	CSelectLC	m_LoadCase;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMNlacDlg_MEC)
protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	virtual void UpdateBuffer();
	//}}AFX_VIRTUAL

// Implementation
	void InitUnit();
	void InitCombo();
	void InitControl();	
	BOOL GetSelectedItem(int &nIndex) const;
	void MakeItemEx();
	void SetItem(int nIndex, T_NLCT_LCASE_D &data);
	CString DataToStr(int i, const T_NLCT_LCASE_D &data) const;
	void AlignControls();
	void ControlsShowHide();
	int GetCurNlType() const;
	void EnableDisableControls();
	void SetFirstState();	
	void SetDataByDB();
	void SetListCtrlHeader();
	void Data2Dlg();
	BOOL Dlg2Data();
	void SelectListCtrlRow(CListCtrl& listCtrl, int nRow) const;
	void OnCmdNlctAllLcaseRdo();
	void OnCmdNlctSelLcaseRdo();
	afx_msg void OnPaint();

	CDBDoc* m_pDoc;

	T_NLCT_DATA m_DataGUI;
	
	T_NLCT_D m_Data;
	
	CArray<UINT, UINT> m_aCtrlArcLeng;
	CArray<UINT, UINT> m_aCtrlDispCtrl;	

	// 라디오가 바뀔 때 이전에 쓰던 값이 있으면 설정함
	int	m_nPrevIncStep_ForceDisp;
	int	m_nPrevIncStep_Arc;

	void OnOK() override;
	void OnCancel() override;
	BOOL OnInitDialog() override;
	afx_msg void OnCmdNlctAcasMethodRdo();
	afx_msg void OnCmdNlctAcasLoadStepTypeRdo();
	afx_msg void OnCmdNlctAcasDispChk();
	afx_msg void OnCmdNlctAcasLoadChk();
	afx_msg void OnCmdNlctAcasWorkChk();
	afx_msg void OnCmdNlctAcasRelativeNodeChk();
	afx_msg void OnCmdNlctAcasLoadStepsBtn();
	afx_msg void OnCmdNlctAcasAdvancedBtn();
	afx_msg void OnCmdBtnGeom();
	afx_msg void OnCmdBtnMatl();
	afx_msg void OnCmdBtnAddMod();
	afx_msg void OnCmdBtnRemoveCtrlData();
	afx_msg void OnCmdBtnDel();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangedCurrentItem(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif