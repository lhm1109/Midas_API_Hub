#pragma once

#include "CMDlgBase.h"
#include "DgnTendonTemplateCtrl.h"

#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_base\NotifyCtrl.h"

#include "MyListCtrl.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\mit_frx\MEdit.h"

#include "HeaderPre.h"

enum EN_DLG_TYPE
{
	EN_DLG_TYPE_NONE = -1,
	EN_DLG_TYPE_COPY,
	EN_DLG_TYPE_AUTOGEN,
	EN_DLG_TYPE_ALL,
};

using namespace mit::frx;

// CCMDTendonTemplateDlg
class CCMDTendonTemplateCopyDlg;
class CCMDTendonTemplateAutoGenDlg;
class CDrawEditBtn_TndnTemplate;
class CCMDControlMover;
class CAttrCtrl;
class __MY_EXT_CLASS__ CCMDTendonTemplateDlg : public CCMDlgBase
{
//	DECLARE_DYNAMIC(CCMDTendonTemplateDlg)

public:
	CCMDTendonTemplateDlg(CWnd* pParent = NULL);
	virtual ~CCMDTendonTemplateDlg();

	enum { IDD = IDD_CMD_TENDON_TEMPLATE };

	CDgnTendonTemplateCtrl* GetTemplateCtrl() { return m_pTemplateCtrl; }
	BOOL IsUsePrefix() { return m_bUsePrefix; }
	void GetPrefixName(CString& strName);
	BOOL IsCurveElem() { return m_pTemplateCtrl->IsCurveElem(); }
	void SetSelectedKeyList(const vecTndnKey& keyList);
	void UpdateListDraw(BOOL bFit = TRUE);
	void closeCopyDlg() { m_pCopyDlg = NULL; }
	int	 GetSelectedKeyList(vecTndnKey& keyList);
	void UnSelectAllKey();
	void EnableDisableRdoBtn(BOOL bEnable);
	void OnOkSubDlgADDMOD(TndnTemplate& tndnData, int nKey = -1);

	void SetShowWnd(BOOL bShow) { m_bIsShowWnd = bShow; }
	BOOL GetShowWnd() { return m_bIsShowWnd; }
	CStringArray* GetAssignElemAry() { return &m_arKeyStr; }
	void RefreshAssignElemCombo();
	void ResetDropWidth(CComboBox* pCbx);
	void SetElemsCbxInit(BOOL bLast = FALSE);
	void UnselectAll();
	void SetChidDlgNULL(EN_DLG_TYPE enType);

	void SetUseOriginDraw(BOOL bUse);
	BOOL GetUseOriginDraw();
	BOOL GetOriginPos_Draw(double* pPos);
	void SetOriginPos_Draw(double* pPos);
	BOOL GetOriginPos_AutoDlg(double* dPos);
	void SetOriginPos_AutoDlg(double* dPos);


protected:
	CDgnTendonTemplateCtrl* m_pTemplateCtrl;
	CAttrCtrl*		m_pAttrCtrl;

	CSelectEdit		m_wndAssignElem;
	CMyListCtrl		m_wndTendonList;
	MButton			m_btnUsePrefix;
	MEdit			m_edtPrefixName;
	CTreeCtrl		m_ctlTree;
	MButton			m_rdoEndI;
	MButton			m_rdoEndJ;
	MComboBox	  m_cbxAssignElem;
	HTREEITEM   m_hTreeItemSection; //Ã¹¹øÂ° Section;

	BOOL			m_bEndI;
	T_SECT_K		m_sectK;
	T_ELEM_K		m_elemK;

	DWORD			m_nType;

	CDrawEditBtn_TndnTemplate*    m_pDraw; 
	CCMDTendonTemplateCopyDlg*    m_pCopyDlg;
	CCMDTendonTemplateAutoGenDlg* m_pAutoGenDlg;
	CCMDControlMover*             m_pControlMover;

	BOOL m_bOnChangeElemList;
	BOOL m_bUsePrefix;
	BOOL m_bIgnoreMsg;
	BOOL m_bUseAfxMsgBox;
	BOOL m_bIsShowWnd;
	BOOL m_bAfter;
	BOOL m_bMinimized;
	
	CStringArray  m_arKeyStr;	
	
protected:
	void SetControl();
	BOOL DoApply();
	BOOL Dlg2Data();
	BOOL Data2Dlg();
	void MakeListHeader();
	void UpdateTdnaList();
	BOOL EndEdit(BOOL bEnd, BOOL bDirectDrawLabel=FALSE, unsigned int nCommand=0);
	void changeRdoAndSection(BOOL bRedraw);
	void SetMover();
	void AppendStr(CString& strOrg, const CString& strAppend);

	void SetTreeCtrl();
	void OnUnitChange();
	void OnResetName(BOOL bUpdateList = TRUE);

	void ShowHideChildDlg(EN_DLG_TYPE enType);
	void DeleteChildDlg(EN_DLG_TYPE enType = EN_DLG_TYPE_ALL, EN_DLG_TYPE enTypeExcept = EN_DLG_TYPE_NONE);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnAssignElemCbxChanged();
	afx_msg void OnCmdAssignElemAdd();
	afx_msg void OnCmdAssignElemDetail();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdCopy();
	afx_msg void OnCmdDelete();
	afx_msg void OnCmdImport();
	afx_msg void OnCmdExport();
	afx_msg void OnCmdResetName();
	afx_msg void OnCmdAutoGen();
	afx_msg void OnCmdSetProperty();
	afx_msg void OnCmdAssignElemList();
	afx_msg void OnCmdUsePrefix();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdModifyDbClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDbClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nFlag, int nMoveX, int nMoveY);
	afx_msg void OnRdoClicked();

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

