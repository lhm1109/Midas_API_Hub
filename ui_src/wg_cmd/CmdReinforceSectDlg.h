#pragma once

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\MyCheckComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "CRFSecViewWnd.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

class CCmdReinforceSectDlg : public CDialogMove
{
	// Construction
public:
	CCmdReinforceSectDlg(CWnd* pParent = NULL, T_RFST_K* pSelectRfstK = NULL);

	// Dialog Data
	//{{AFX_DATA(CCmdReinforceSectDlg)
	enum
	{
		IDD = IDD_CMD_REINFORCE_DEFINE_SECT_DLG
	};
private:
	CComboBox		m_OriginCombox;
	CComboBox		m_TypeCombox;
	CComboBox		m_MatlCombox;
	CComboBox		m_UserSectCombox;
	CFormulaEdit	m_SectNameEdit;
	CFormulaEdit	m_OmegaYEdit;
	CFormulaEdit	m_OmegaZEdit;
	CRFSecViewWnd	m_wndSecView;
	CListCtrl		m_ListCtrl;
	CButton			m_CheckAutoCalc;

	CStatic		m_wndSectTitle[14];
	CEditUnit	m_wndSectSize[14];
	CTextUnit	m_wndSectUnit[14];

	CTextUnit	m_wndOmegaUnit[2];

	CBitmap* m_pBitmap;         // Dimension Guide Bitmap

private:
	BOOL m_bChangeReinforceTypeInteraction;
	BOOL m_bIgnoreEnChange;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CDBDoc*	m_pDoc;
	T_SECT_D m_ReSectData;
	T_SECT_D m_OriSectData;
	T_SECT_D m_PSC_USER_SectData;
	T_RFST_K m_RfstKey;
	T_RFST_D m_Data;
	T_RFST_WELD_D m_WeldData;
	int m_nConnectType;
	int m_nSideType;
	CMap<T_SECT_K, T_SECT_K, int, int> m_mapSectCombox2Index;
	CMap<T_SECT_K, T_SECT_K, int, int> m_mapPSCSectCombox2Index;
	void Data2Dlg();
	BOOL Dlg2Data();
	void InitCombox();
	void UpdateDlg(const T_RFST_D& rfstD);
	void ChangeBitmapByType();
	void ChangeBitmapBySect();
	void GetReinforceSizeTitle(int nReinforceType,CStringArray& aSizeTitle);
	void SetListHeaderTitle();
	void InitListData();
	void InsertItemList (T_RFST_K RfstK,const T_RFST_D& rfstD,BOOL bModify = FALSE);
	//void InitListData();
	//void InitListData();
	/*根据截面名称在数据库中,修改名字-1,-2*/
	void ForceModifySectName(T_RFST_K RfstK, T_RFST_D& DBrfstD);
public:
	virtual BOOL OnInitDialog();
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

private:
	BOOL CalcStiff();
protected:

	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnSect();
	afx_msg void OnMatl();
	afx_msg void OnWeldParam();
	afx_msg void OnListAdd();
	afx_msg void OnListModify();
	afx_msg void OnListDel();
	afx_msg void OnChangeSect();
	afx_msg void OnChangeReinforceType();
	afx_msg void OnChangeReinforceSize();
	afx_msg void OnChangePscUserSect();
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAutoCalc();
	afx_msg void OnStiff();
	afx_msg void OnChangeConnectType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

