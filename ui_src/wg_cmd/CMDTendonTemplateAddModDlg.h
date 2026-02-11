#pragma once

#include "CMDlgBase.h"

#include "..\wg_db\CobxTdgr.h"
#include "..\wg_db\CobxTdnt.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "DgnTendonTemplateCtrl.h"
#include "CMDTendonTemplateGrid.h"

class CDBDoc;
class CCMDTendonTemplateGrid;
class CDrawEditBtn_TndnTemplate;
class CCMDTendonTemplateDlg;
class CCMDTendonTemplateViewDlg;
class CCMDTendonTemplateAddModDlg : public CCMDlgBase
{

public:
	CCMDTendonTemplateAddModDlg(CDgnTendonTemplateCtrl* pCtrl, CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateAddModDlg();
	
	enum { IDD = IDD_CMD_TENDON_TEMPLATE_ADDMOD_DLG};
	
	void RemakeGridDraw(BOOL bXY, BOOL bXZ, BOOL bOnlyDraw = FALSE);

protected:
	void SetControl();
	void SetCurSel();
	void ResetCombobox(BOOL bBoth, UINT nType); // nType 0 : Spline, 1 : Round
	void ComboBoxDynamicChange(CComboBox* pCombo);
	void GetSameSpanStr(BOOL bXY, CString& strSpan);  
	void OnUnitChange();
//   void EnableDisableCtrlSameSpanLength(BOOL bSet, BOOL bXY, BOOL bXZ);
//   void EnableDisableCtrlSpin(BOOL bEnableXY, BOOL bEnableXZ);
	void EnableDisableAllCtrl(BOOL bData2Dlg, TndnTemplate& tndnTemp);
	void DestroyViewWindow();
	
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	
	afx_msg void OnCmdTendonPropButton();
	afx_msg void OnCmdTendonGroupButton();
	afx_msg void OnCmdChangeSpanNumXZ();
	afx_msg void OnCmdChangeSpanNumXY();
	afx_msg void OnDeltaposCmdSpanXY(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdSpanXZ(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTypeChangeXY();
	afx_msg void OnTypeChangeXZ();
	afx_msg void OnCmdChangeSpanLenXY();
	afx_msg void OnCmdChangeSpanLenXZ();
	afx_msg void OnCmdCheckUseSameSpanInfoXY();
	afx_msg void OnCmdCheckUseSameSpanInfoXZ();
	afx_msg void OnCmdCheckUseSameInterval();
	afx_msg void OnChangeOffset();
	afx_msg void OnCmdCheckSymmXY();
	afx_msg void OnCmdCheckSymmXZ();

public:
	BOOL Data2Dlg();
	BOOL Dlg2Data();
	BOOL CheckValidData();

	void SetTemplateData(TndnTemplate* pTemplate) { m_templateData = *pTemplate; }
	TndnTemplate* GetTemplateDate() {return &m_templateData; }

public:
	int							m_nKey; //Modify : key, Add : -1 (not use)

protected:
	BOOL m_bIgnoreMessage;
	BOOL m_bPreSuccess;          //그림글릴때...
	BOOL m_bPreSuccess_Dlg2Data; //Dlg2Data 실패하면 다음에는 XY,XZ둘다 그린다.
	BOOL m_bOK;

	CDBDoc*							m_pDoc;
	CDgnTendonTemplateCtrl*			m_pTemplateCtrl;
	TndnTemplate					m_templateData;
	TndnTemplate					m_templateData_Draw;

	std::vector<TNDN_TEMPLATE_TYPE>	m_vecSplineType;
	std::vector<TNDN_TEMPLATE_TYPE>	m_vecRoundType;
	std::vector<TNDN_TEMPLATE_TYPE> m_vecEnType;
	CImageList*						m_pImgList;
	MComboBox						m_cbxTypeXY;
	MComboBox						m_cbxTypeXZ;
	
	MButton		m_chkSymXAxisXY;
	MButton		m_chkSymXAxisXZ;
	MButton		m_chkSameSpanInfoXY;
	MButton		m_chkSameSpanInfoXZ;
	MButton		m_chkSameSpanInterval;
	MButton		m_rdoRefAxisCurve;
	MButton		m_rdoRefAxisElem;

	CTextUnit m_wndUnitOffsetFront;
	CTextUnit	m_wndUnitOffsetEnd;
	MEdit			m_wndSpanLengthXY;
	MEdit			m_wndSpanLengthXZ;
	CEditUnit	m_wndOffsetFront;
	CEditUnit	m_wndOffsetEnd;
	MEdit			m_wndTndnName;
	CFormulaEditSpin			m_wndNumOfSpanXY;
	CFormulaEditSpin			m_wndNumOfSpanXZ;
	CCobxTdgr	m_wndTdgrCobx;
	CCobxTdnt	m_wndTdntCobx;

	CCMDTendonTemplateGrid			m_gridXZ;
	CDrawEditBtn_TndnTemplate*	m_pDrawXZ;
	CCMDTendonTemplateGrid			m_gridXY;
	CDrawEditBtn_TndnTemplate*	m_pDrawXY;

	CCMDTendonTemplateViewDlg*  m_pViewer;

	CCMDTendonTemplateDlg* m_pParent;

	std::vector<CString> m_svStrComboImage;

	DECLARE_MESSAGE_MAP()
};
