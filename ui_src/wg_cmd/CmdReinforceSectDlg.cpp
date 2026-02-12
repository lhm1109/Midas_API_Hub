#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdReinforceSectDlg.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\CMSectImportSPC.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "CmdReinforceSectWeldDlg.h"
#include "CMSecPageStiffDlg.h"
#include "CMSectItemDlg.h"
#include "CMMatPageItemDlg.h"
#include "../wg_db/PostCtrl.h"
#include "../wg_db/DesignResult.h"
#include "../wg_db/FileDesignMgrBase.h"
#include "../wg_db/ReinforceDgnMgr_CH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCmdReinforceSectDlg::CCmdReinforceSectDlg(CWnd* pParent /* = NULL */,T_RFST_K* pSelectRfstK)
	: CDialogMove(CCmdReinforceSectDlg::IDD, pParent)
{
	if (pSelectRfstK)
		m_RfstKey = *pSelectRfstK;
	else
		m_RfstKey = 0;

	m_nConnectType = 0;
	m_nSideType = 0;
	m_pBitmap = NULL;
	m_ReSectData.Initialize();
	m_OriSectData.Initialize();
	m_PSC_USER_SectData.Initialize();
	m_bChangeReinforceTypeInteraction = TRUE;
	m_bIgnoreEnChange = FALSE;
}

void CCmdReinforceSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdReinforceSectDlg)
	DDX_Radio(pDX, IDC_CMD_RADIO1, m_nConnectType);
	DDX_Radio(pDX, IDC_CMD_RADIO4, m_nSideType);
	DDX_Control(pDX, IDC_CMD_COMB11_EDIT, m_OriginCombox); 
	DDX_Control(pDX, IDC_CMD_COMB12_EDIT, m_TypeCombox);
	DDX_Control(pDX, IDC_CMD_COMB13_EDIT, m_MatlCombox);
	DDX_Control(pDX, IDC_CMD_COMB21_EDIT, m_UserSectCombox);
	DDX_Control(pDX, IDC_EDIT1, m_SectNameEdit);
	DDX_Control(pDX, IDC_CMD_STATIC8, m_CheckAutoCalc);
	DDX_Control(pDX, IDC_EDIT2, m_OmegaYEdit);
	DDX_Control(pDX, IDC_EDIT3, m_OmegaZEdit);
	DDX_Control(pDX, IDC_CMD_LIST, m_ListCtrl);
	UINT aFirstID[][14] = {
		{ IDC_CMD_SP_ID_TITLE1, IDC_CMD_SP_ID_TITLE2, IDC_CMD_SP_ID_TITLE3,
			IDC_CMD_SP_ID_TITLE4, IDC_CMD_SP_ID_TITLE5, IDC_CMD_SP_ID_TITLE6,
			IDC_CMD_SP_ID_TITLE7, IDC_CMD_SP_ID_TITLE8, IDC_CMD_SP_ID_TITLE9,
			IDC_CMD_SP_ID_TITLE10, IDC_CMD_SP_ID_TITLE11,IDC_CMD_SP_ID_TITLE12,
			IDC_CMD_SP_ID_TITLE13,IDC_CMD_SP_ID_TITLE14},
		{ IDC_CMD_SP_ID_VALUE1, IDC_CMD_SP_ID_VALUE2, IDC_CMD_SP_ID_VALUE3,
			IDC_CMD_SP_ID_VALUE4, IDC_CMD_SP_ID_VALUE5, IDC_CMD_SP_ID_VALUE6,
			IDC_CMD_SP_ID_VALUE7, IDC_CMD_SP_ID_VALUE8, IDC_CMD_SP_ID_VALUE9,
			IDC_CMD_SP_ID_VALUE10, IDC_CMD_SP_ID_VALUE11,IDC_CMD_SP_ID_VALUE12,
			IDC_CMD_SP_ID_VALUE13,IDC_CMD_SP_ID_VALUE14},
		{ IDC_CMD_SP_ID_UNIT1,  IDC_CMD_SP_ID_UNIT2,  IDC_CMD_SP_ID_UNIT3,
			IDC_CMD_SP_ID_UNIT4,  IDC_CMD_SP_ID_UNIT5,  IDC_CMD_SP_ID_UNIT6,
			IDC_CMD_SP_ID_UNIT7,  IDC_CMD_SP_ID_UNIT8,  IDC_CMD_SP_ID_UNIT9,
			IDC_CMD_SP_ID_UNIT10, IDC_CMD_SP_ID_UNIT11 ,IDC_CMD_SP_ID_UNIT12,
			IDC_CMD_SP_ID_UNIT13 ,IDC_CMD_SP_ID_UNIT14 },
	};
	for(int i = 0; i < 14; i++)
	{
		DDX_Control(pDX, aFirstID[0][i], m_wndSectTitle[i]);
		DDX_Control(pDX, aFirstID[1][i], m_wndSectSize[i]);
		DDX_Control(pDX, aFirstID[2][i], m_wndSectUnit[i]);
	}
	DDX_Control(pDX, IDC_CMD_STATIC10, m_wndOmegaUnit[0]);
	DDX_Control(pDX, IDC_CMD_STATIC12, m_wndOmegaUnit[1]);
}

BOOL CCmdReinforceSectDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocusWnd = GetFocus();
		if (pFocusWnd == GetDlgItem(IDC_CMD_SP_ID_VALUE9) || pFocusWnd == GetDlgItem(IDC_CMD_SP_ID_VALUE10) || pFocusWnd == GetDlgItem(IDC_CMD_SP_ID_VALUE11) ||
			pFocusWnd == GetDlgItem(IDC_CMD_SP_ID_VALUE12) || pFocusWnd == GetDlgItem(IDC_CMD_SP_ID_VALUE13) || pFocusWnd == GetDlgItem(IDC_CMD_SP_ID_VALUE14))
		{
			OnChangeReinforceSize();
			return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CCmdReinforceSectDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_REINFORCE_CANCEL_BTN, OnCancel)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS, OnSect)
	ON_BN_CLICKED(IDC_CMD_BTN_VCLASS2, OnMatl)
	ON_BN_CLICKED(IDC_BUTTON2, OnWeldParam)
	
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnListAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnListModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DEL, OnListDel)
	ON_BN_CLICKED(IDC_CMD_STATIC8, OnCheckAutoCalc)
	ON_BN_CLICKED(IDC_BUTTON3, OnStiff)

	ON_CBN_SELCHANGE(IDC_CMD_COMB11_EDIT, OnChangeSect)
	ON_CBN_SELCHANGE(IDC_CMD_COMB12_EDIT, OnChangeReinforceType)

	ON_CBN_SELCHANGE(IDC_CMD_COMB21_EDIT, OnChangePscUserSect)
	

	ON_EN_KILLFOCUS(IDC_CMD_SP_ID_VALUE9, OnChangeReinforceSize)
	ON_EN_KILLFOCUS(IDC_CMD_SP_ID_VALUE10, OnChangeReinforceSize)
	ON_EN_KILLFOCUS(IDC_CMD_SP_ID_VALUE11, OnChangeReinforceSize)
	ON_EN_KILLFOCUS(IDC_CMD_SP_ID_VALUE12, OnChangeReinforceSize)
	ON_EN_KILLFOCUS(IDC_CMD_SP_ID_VALUE13, OnChangeReinforceSize)
	ON_EN_KILLFOCUS(IDC_CMD_SP_ID_VALUE14, OnChangeReinforceSize)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_LIST, OnItemChanged)

	ON_BN_CLICKED(IDC_CMD_RADIO1, OnChangeConnectType)
	ON_BN_CLICKED(IDC_CMD_RADIO2, OnChangeConnectType)
	ON_BN_CLICKED(IDC_CMD_RADIO3, OnChangeConnectType)

END_MESSAGE_MAP()

BOOL CCmdReinforceSectDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	GetDlgItem(IDC_CMD_WARPING_EFFECT_CHK)->ShowWindow(SW_HIDE);

	// Unit control
	for(int i = 0; i < 14; i++)
	{
		m_wndSectSize[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
		m_wndSectUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	for(int i = 0; i < 8; i++)
	{
		m_wndSectTitle[i].EnableWindow(FALSE);
		m_wndSectSize[i].EnableWindow(FALSE);
		m_wndSectUnit[i].EnableWindow(FALSE);
	}

	m_wndOmegaUnit[0].SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndOmegaUnit[1].SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_pDoc->m_pUnitCtrl->ConvertUnitRfstOut(m_Data);

	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_ID_PREVIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);
	m_wndSecView.SetDataSource(&m_ReSectData,&m_Data);

	SetListHeaderTitle();
	InitCombox();
	InitListData();
	if (m_RfstKey == 0)
		Data2Dlg();
	return TRUE;
}

BOOL CCmdReinforceSectDlg::CalcStiff()
{
	m_ReSectData = m_OriSectData;
	int nReinforceType = m_TypeCombox.GetCurSel();
	int nRCount = 4;
	if (nReinforceType == 6)
	{
		m_ReSectData = m_PSC_USER_SectData;
		return TRUE;
	}
	else if (nReinforceType == 3 || nReinforceType == 4)
		nRCount = 6;
	for (int i = 0; i < nRCount; i++)
	{
		if (m_wndSectSize[8 + i].GetEditValue() < 1e-7)
			return FALSE;
	}
	//construct points
	double dRSize[6];
	for (int i = 0; i < 6; i++)
	{
		dRSize[i] = m_wndSectSize[i + 8].GetEditValue();
	}
	if (!CReinforceDgnMgr_CH::Instance()->GeneralteHReSectPolygon(m_ReSectData.SectBefore.SectI.aOuterPolygon, m_ReSectData.SectBefore.SectI.aInnerPolygon,
		nReinforceType, m_ReSectData.SectBefore.SectI.Size, dRSize))
	{
		return FALSE;
	}

	m_ReSectData.nStype = D_SECT_TYPE_PSC;
	m_ReSectData.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;
	m_ReSectData.SectBefore.bConsiderWarpingEffect = TRUE;// ((CButton*)GetDlgItem(IDC_CMD_WARPING_EFFECT_CHK))->GetCheck();
	m_ReSectData.SectBefore.bShearCheck = FALSE;

	BOOL bRet = CSectUtil::CalcSectData(m_ReSectData, TRUE, FALSE, FALSE, TRUE);
	double dQy, dQz;
	if (CSectUtil::GetPscValueDataQ(&m_ReSectData, dQy, dQz))
	{
		m_ReSectData.SectBefore.SectI.Stiffness.Qyb = dQy;
		m_ReSectData.SectBefore.SectI.Stiffness.Qzb = dQz;
	}
	return bRet;
}

void CCmdReinforceSectDlg::Data2Dlg()
{
	UpdateDlg(m_Data);
	m_WeldData = m_Data.mWeldData;
}

BOOL CCmdReinforceSectDlg::Dlg2Data()
{
	if (!CalcStiff())
	{
		return FALSE;
	}
	m_Data.Stiffness = m_ReSectData.SectBefore.SectI.Stiffness;
	UpdateData(TRUE);
	m_OmegaYEdit.GetEditValue(m_Data.dWeldOmegaY);
	m_OmegaZEdit.GetEditValue(m_Data.dWeldOmegaZ);
	m_SectNameEdit.GetWindowText(m_Data.strReinforceSectName);
	m_Data.nOriginSectK = m_OriginCombox.GetItemData(m_OriginCombox.GetCurSel());
	m_Data.nConnectionType = m_nConnectType;
	m_Data.nSideType = m_nSideType;
	m_Data.nMatlK = m_MatlCombox.GetItemData(m_MatlCombox.GetCurSel());
	m_Data.bAutoCalcDeflection = m_CheckAutoCalc.GetCheck();
	m_Data.mWeldData = m_WeldData;
	m_Data.nPSCSectK = m_UserSectCombox.GetItemData(m_UserSectCombox.GetCurSel());
	return TRUE;
}

void CCmdReinforceSectDlg::InitCombox()
{
	//Origin Sect
	m_OriginCombox.ResetContent();
	m_mapSectCombox2Index.RemoveAll();
	T_SECT_K_LIST aSectK;
	m_pDoc->m_pAttrCtrl->GetSectKeyListUsedInElem(aSectK);
	T_SECT_D sectD;
	int nIndex = 0;
	for(int i = 0; i < aSectK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], sectD);
		nIndex = m_OriginCombox.AddString(sectD.SName);
		m_OriginCombox.SetItemData(nIndex, aSectK[i]);
		m_mapSectCombox2Index[aSectK[i]] = nIndex;
	}
	m_OriginCombox.SetCurSel(0);

	//Reinforce Type 
	m_TypeCombox.ResetContent();
	CString str[] = { 
		_LS(IDS_WG_CMD_Reinforce_Sect_Type_1),_LS(IDS_WG_CMD_Reinforce_Sect_Type_2),_LS(IDS_WG_CMD_Reinforce_Sect_Type_3),
		_LS(IDS_WG_CMD_Reinforce_Sect_Type_4),_LS(IDS_WG_CMD_Reinforce_Sect_Type_5),_LS(IDS_WG_CMD_Reinforce_Sect_Type_6),
		_LS(IDS_WG_CMD_Reinforce_Sect_Type_7),
	};
	for(int i = 0; i < 7; i++)
	{
		m_TypeCombox.AddString(str[i]);
	}
	m_TypeCombox.SetCurSel(0);

	//Reinforce Matl
	m_MatlCombox.ResetContent();
	CArray<T_MATL_K, T_MATL_K> aMatlK;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlK);
	T_MATL_D matlD;
	nIndex = m_MatlCombox.AddString(_LS(IDS_CMD_REINFORCE_MATL));
	m_MatlCombox.SetItemData(nIndex, 0);
	for(int i = 0; i < aMatlK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetMatl(aMatlK[i], matlD);
		nIndex = m_MatlCombox.AddString(matlD.Name);
		m_MatlCombox.SetItemData(nIndex, aMatlK[i]);
	}
	m_MatlCombox.SetCurSel(0);

	//User Sect
	m_UserSectCombox.ResetContent();
	m_mapPSCSectCombox2Index.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	for (int i = 0; i < aSectK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetSect(aSectK[i], sectD);
		if (sectD.nStype == D_SECT_TYPE_PSC || sectD.nStype == D_SECT_TYPE_USER)
		{
			nIndex = m_UserSectCombox.AddString(sectD.SName);
			m_UserSectCombox.SetItemData(nIndex, aSectK[i]);
			m_mapPSCSectCombox2Index[aSectK[i]] = nIndex;
		}
	}
	m_UserSectCombox.SetCurSel(0);
}

void CCmdReinforceSectDlg::UpdateDlg(const T_RFST_D& rfstD)
{
	m_nConnectType = rfstD.nConnectionType;
	m_nSideType = rfstD.nSideType;
	m_CheckAutoCalc.SetCheck(rfstD.bAutoCalcDeflection);
	UpdateData(FALSE);
	
	CString str;
	str.Format(_T("%g"), rfstD.dWeldOmegaY);
	m_OmegaYEdit.SetWindowText(str);
	str.Format(_T("%g"), rfstD.dWeldOmegaZ);
	m_OmegaZEdit.SetWindowText(str);
	m_OriginCombox.SetCurSel(m_mapSectCombox2Index[rfstD.nOriginSectK]);
	m_pDoc->m_pAttrCtrl->GetSect(rfstD.nOriginSectK, m_OriSectData);
	m_UserSectCombox.SetCurSel(m_mapPSCSectCombox2Index[rfstD.nPSCSectK]);
	m_pDoc->m_pAttrCtrl->GetSect(rfstD.nPSCSectK, m_PSC_USER_SectData);
	m_TypeCombox.SetCurSel(rfstD.nReinforceType);
	CDlgUtil::CobxSetCurSelItemData(m_MatlCombox, rfstD.nMatlK);

	OnChangeSect();
	m_SectNameEdit.SetWindowText(rfstD.strReinforceSectName);

	m_bChangeReinforceTypeInteraction = FALSE;
	OnChangeReinforceType();
	OnChangeConnectType();
}

void CCmdReinforceSectDlg::ChangeBitmapByType()
{
	int nReinforceType = m_TypeCombox.GetCurSel();
	if (nReinforceType == 6)
	{
		ChangeBitmapBySect();
		return;
	}

	UINT aBitmapID[] = {
		IDB_CMD_SP_RFST_BMP01, IDB_CMD_SP_RFST_BMP02,IDB_CMD_SP_RFST_BMP03,
		IDB_CMD_SP_RFST_BMP04, IDB_CMD_SP_RFST_BMP05,IDB_CMD_SP_RFST_BMP06,
	};

	// set appropriate bitmap
	if (m_pBitmap != NULL) { delete m_pBitmap; m_pBitmap = NULL; }
	CStatic* SecImage = (CStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
	m_pBitmap = (CBitmap*)SecImage;
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nReinforceType]);
	SecImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCmdReinforceSectDlg::ChangeBitmapBySect()
{
	int nShapeIndex;
	nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_OriSectData.SectBefore.Shape);
	if (nShapeIndex < 0)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_regular_section_s));
		return;
	}

	UINT aBitmapID[] = {
		//IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP02, IDB_CMD_SP_ISGL_BMP03,
		IDB_CMD_SP_ISGL_BMP01, IDB_CMD_SP_ISGL_BMP19, IDB_CMD_SP_ISGL_BMP20,
		IDB_CMD_SP_ISGL_BMP04, IDB_CMD_SP_ISGL_BMP05, IDB_CMD_SP_ISGL_BMP06,
		IDB_CMD_SP_ISGL_BMP07, IDB_CMD_SP_ISGL_BMP08, IDB_CMD_SP_ISGL_BMP09,
		IDB_CMD_SP_ISGL_BMP10, IDB_CMD_SP_ISGL_BMP11, IDB_CMD_SP_ISGL_BMP12,
		IDB_CMD_SP_ISGL_BMP13, IDB_CMD_SP_ISGL_BMP14, IDB_CMD_SP_ISGL_BMP15,
		IDB_CMD_SP_ISGL_BMP16, IDB_CMD_SP_ISGL_BMP17, IDB_CMD_SP_ISGL_BMP18,
		IDB_CMD_SP_ISGL_BMP21, IDB_CMD_SP_ISGL_BMP23, IDB_CMD_SP_ISGL_BMP24,
		IDB_CMD_SP_ISGL_BMP25/*救静烙*/, IDB_CMD_SP_ISGL_BMP26, IDB_CMD_SP_ISGL_BMP27,
		IDB_CMD_SP_ISGL_BMP28, IDB_CMD_SP_ISGL_BMP29, IDB_CMD_SP_ISGL_BMP32,
		IDB_CMD_SP_ISGL_BMP31
	};

	// set appropriate bitmap
	if (m_pBitmap != NULL) { delete m_pBitmap; m_pBitmap = NULL; }
	CStatic* SecImage = (CStatic*)GetDlgItem(IDC_CMD_SP_ID_BMP);
	m_pBitmap = (CBitmap*)SecImage;
	m_pBitmap = new CBitmap;
	if (m_OriSectData.SectBefore.Shape == D_SECT_SHAPE_REG_B)
	{
		m_pBitmap->LoadBitmap(IDB_CMD_SP_ISGL_BMP05);
	}
	else 
		m_pBitmap->LoadBitmap(aBitmapID[nShapeIndex]);
	SecImage->SetBitmap(HBITMAP(*m_pBitmap));

}

void CCmdReinforceSectDlg::GetReinforceSizeTitle(int nReinforceType, CStringArray& aSizeTitle)
{
	aSizeTitle.RemoveAll();
	if(nReinforceType == 0 || nReinforceType == 2)
	{
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_1));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_2));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_3));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_4));
	}
	else if(nReinforceType == 1)
	{
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_5));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_6));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_7));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_8));
	}
	else if(nReinforceType == 3)
	{
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_1));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_2));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_3));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_4));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_9));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_10));
	}
	else if(nReinforceType == 4)
	{
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_11));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_1));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_12));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_3));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_9));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_10));
	}
	else if(nReinforceType == 5)
	{
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_13));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_14));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_5));
		aSizeTitle.Add(_LS(IDS_WG_CMD_Reinforce_Sect_Size_15));
	}
}

void CCmdReinforceSectDlg::OnOK()
{
	//if(!Dlg2Data())	
	//	return;
	//if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddRfst(m_Data)) 
	//	return;
	CDialogMove::OnOK();
}

void CCmdReinforceSectDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCmdReinforceSectDlg::OnSect()
{
	CCMSectItemDlg SecDlg;
	SecDlg.DoModal();
}

void CCmdReinforceSectDlg::OnMatl()
{
	CCMMatPageItemDlg MatDlg(m_pDoc);
	int matlKey = m_MatlCombox.GetItemData(m_MatlCombox.GetCurSel());
	T_MATL_D matlD;
	matlD.Initialize();
	if (matlKey == 0)
	{
		matlKey = m_pDoc->m_pAttrCtrl->GetStartNumMatl();
		matlD.Type = _ULS(S);
	}
	else
		m_pDoc->m_pAttrCtrl->GetMatl(matlKey, matlD);
	MatDlg.SetModifyData(matlKey, matlD);
	MatDlg.DoModal();
}

void CCmdReinforceSectDlg::OnWeldParam()
{
	CmdReinforceSectWeldDlg dlg(&m_WeldData);
	dlg.DoModal();
}

void CCmdReinforceSectDlg::OnListAdd()
{
	if (!Dlg2Data())
	{
		AfxMessageBox(_LS(IDS_WG_CMD_Reinforce_Sect_Value_Error));
		return;
	}
	T_RFST_D data = m_Data;
	T_RFST_K RfstK = m_pDoc->m_pAttrCtrl->GetStartNumRfst();
	ForceModifySectName(RfstK,data);
	CStlDesignMgr* pStlDesignMgr = CDBDoc::GetDocPoint()->m_pPostCtrl->GetDesignResult()->GetStlDgnMgr();
	BOOL bReinforceDgnEnable = pStlDesignMgr->GetReinforceDgnMgr()->IsDesignResultEnable() && pStlDesignMgr->GetReinforceDgnMgr()->ReadDesignType() == 1;
	if (bReinforceDgnEnable)
	{
		if (AfxMessageBox(_LS(IDS_DB_REMOVE_REINFORCE_DESIGN), MB_YESNO) != IDYES)
			return;
	}
	if(m_pDoc->m_pDataCtrl->AddRfst(RfstK, data))
	{
		InsertItemList(RfstK, data);
		if (bReinforceDgnEnable)
		{
			pStlDesignMgr->GetReinforceDgnMgr()->ClearDesignDataByType(0);
		}
	}
}

void CCmdReinforceSectDlg::OnListModify()
{
	int iItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(iItem == -1)
	{
		AfxMessageBox(_LS(IDS_CMD_REINFORCE_no_select));
		return;
	}
	m_RfstKey = m_ListCtrl.GetItemData(iItem);
	if (!Dlg2Data())
	{
		AfxMessageBox(_LS(IDS_WG_CMD_Reinforce_Sect_Value_Error));
		return;
	}
	T_RFST_D data = m_Data;
	ForceModifySectName(m_RfstKey, data);

	CStlDesignMgr* pStlDesignMgr = CDBDoc::GetDocPoint()->m_pPostCtrl->GetDesignResult()->GetStlDgnMgr();
	BOOL bReinforceDgnEnable = pStlDesignMgr->GetReinforceDgnMgr()->IsDesignResultEnable() && pStlDesignMgr->GetReinforceDgnMgr()->ReadDesignType() == 1;
	if (bReinforceDgnEnable)
	{
		if (AfxMessageBox(_LS(IDS_DB_REMOVE_REINFORCE_DESIGN), MB_YESNO) != IDYES)
			return ;
	}

	if (m_pDoc->m_pDataCtrl->AddRfst(m_RfstKey, data))
	{
		InsertItemList(m_RfstKey, data, TRUE);
		if (bReinforceDgnEnable)
		{
			pStlDesignMgr->GetReinforceDgnMgr()->ClearDesignDataByType(0);
		}
	}
}

void CCmdReinforceSectDlg::OnListDel()
{
	int iItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(iItem == -1)
	{
		AfxMessageBox(_LS(IDS_CMD_REINFORCE_no_select));
		return;
	}
	m_RfstKey = m_ListCtrl.GetItemData(iItem);
	CStlDesignMgr* pStlDesignMgr = CDBDoc::GetDocPoint()->m_pPostCtrl->GetDesignResult()->GetStlDgnMgr();
	BOOL bReinforceDgnEnable = pStlDesignMgr->GetReinforceDgnMgr()->IsDesignResultEnable() && pStlDesignMgr->GetReinforceDgnMgr()->ReadDesignType() == 1;
	if (bReinforceDgnEnable)
	{
		if (AfxMessageBox(_LS(IDS_DB_REMOVE_REINFORCE_DESIGN), MB_YESNO) != IDYES)
			return ;
	}
	if(m_pDoc->m_pDataCtrl->DelRfst(m_RfstKey))
	{
		m_RfstKey = 0;
		m_ListCtrl.DeleteItem(iItem);
		if (bReinforceDgnEnable)
		{
			pStlDesignMgr->GetReinforceDgnMgr()->ClearDesignDataByType(0);
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CCmdReinforceSectDlg::OnChangeSect()
{
	T_SECT_K sectK = m_OriginCombox.GetItemData(m_OriginCombox.GetCurSel());
	m_pDoc->m_pAttrCtrl->GetSect(sectK, m_OriSectData);
	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_OriSectData.SectBefore.Shape);
	if(nShapeIndex < 0)
	{
		return;
	}
	CStringArray aSizeTitle;
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	CString strReinforceSectName = _LS(IDS_WG_CMD_Reinforce_Sect_Name_Prefix) + m_OriSectData.SName;
	m_SectNameEdit.SetWindowText(strReinforceSectName);

	CString csValue;
	int i = 0;
	for(i = 0; i < aSizeTitle.GetSize(); i++)
	{
		m_wndSectTitle[i].ShowWindow(SW_SHOW);
		m_wndSectSize[i].ShowWindow(SW_SHOW);
		m_wndSectUnit[i].ShowWindow(SW_SHOW);
		m_wndSectTitle[i].SetWindowText(aSizeTitle[i]);
		csValue.Format(_T("%g"), m_OriSectData.SectBefore.SectI.Size[i]);
		m_wndSectSize[i].SetWindowText(csValue);
	}
	for(; i < 8; i++)
	{
		m_wndSectTitle[i].ShowWindow(SW_HIDE);
		m_wndSectSize[i].ShowWindow(SW_HIDE);
		m_wndSectUnit[i].ShowWindow(SW_HIDE);
	}

	if (m_OriSectData.SectBefore.Shape == D_SECT_SHAPE_REG_H)
	{
		GetDlgItem(IDC_CMD_COMB12_EDIT)->EnableWindow(TRUE);
		m_bChangeReinforceTypeInteraction = FALSE;
		OnChangeReinforceType();
	}
	else
	{
		m_TypeCombox.SetCurSel(6);
		GetDlgItem(IDC_CMD_COMB12_EDIT)->EnableWindow(FALSE);
		ChangeBitmapBySect();

	}
	m_wndSecView.Invalidate();
}

void CCmdReinforceSectDlg::OnChangeReinforceType()
{
	int nReinforceType = m_TypeCombox.GetCurSel();
	CStringArray aSizeTitle;
	GetReinforceSizeTitle(nReinforceType, aSizeTitle);
	if(m_bChangeReinforceTypeInteraction)
	{
		//由用户的combox交互改变加固形式,则把加固数据设为默认值
		for(int i = 0; i < 6; i++)
			m_Data.Sect[i] = 0.0;
	}
	CString csValue;
	int i = 8;
	m_bIgnoreEnChange = TRUE;
	for(; i < aSizeTitle.GetSize() + 8; i++)
	{
		m_wndSectTitle[i].ShowWindow(SW_SHOW);
		m_wndSectSize[i].ShowWindow(SW_SHOW);
		m_wndSectUnit[i].ShowWindow(SW_SHOW);
		m_wndSectTitle[i].SetWindowText(aSizeTitle[i-8]);
		//切换加固形式,清空加固EditValue数据.
		csValue.Format(_T("%g"), m_Data.Sect[i-8]);
		m_wndSectSize[i].SetWindowText(csValue);
	}
	for(; i < 14; i++)
	{
		m_wndSectTitle[i].ShowWindow(SW_HIDE);
		m_wndSectSize[i].ShowWindow(SW_HIDE);
		m_wndSectUnit[i].ShowWindow(SW_HIDE);
		m_wndSectSize[i].SetWindowText(_T(""));
	}
	m_Data.nReinforceType = nReinforceType;
	m_bChangeReinforceTypeInteraction = TRUE;
	m_bIgnoreEnChange = FALSE;
	CalcStiff();
	m_wndSecView.Invalidate();
	if (nReinforceType == 6)
	{
		GetDlgItem(IDC_CMD_COMB21_EDIT)->ShowWindow(TRUE);
		int nUserCursel = m_UserSectCombox.GetCurSel();
		if (nUserCursel != -1)
		{
			T_SECT_K sectK = m_UserSectCombox.GetItemData(nUserCursel);
			m_pDoc->m_pAttrCtrl->GetSect(sectK, m_PSC_USER_SectData);
			CString strReinforceSectName = _LS(IDS_WG_CMD_Reinforce_Sect_Name_Prefix) + m_PSC_USER_SectData.SName;
			m_SectNameEdit.SetWindowText(strReinforceSectName);
		}
	}
	else
	{
		GetDlgItem(IDC_CMD_COMB21_EDIT)->ShowWindow(FALSE);
		int nUserCursel = m_OriginCombox.GetCurSel();
		if (nUserCursel != -1)
		{
			T_SECT_K sectK = m_OriginCombox.GetItemData(nUserCursel);
			m_pDoc->m_pAttrCtrl->GetSect(sectK, m_OriSectData);
			CString strReinforceSectName = _LS(IDS_WG_CMD_Reinforce_Sect_Name_Prefix) + m_OriSectData.SName;
			m_SectNameEdit.SetWindowText(strReinforceSectName);
		}
	}
	ChangeBitmapByType();
}

void CCmdReinforceSectDlg::OnChangeReinforceSize()
{
	if (m_bIgnoreEnChange)
		return;
	for (int i = 8; i < 14; i++)
		m_Data.Sect[i - 8] = m_wndSectSize[i].GetEditValue();
	if(CalcStiff())
		m_wndSecView.Invalidate();
}

void CCmdReinforceSectDlg::OnChangePscUserSect()
{
	int nCurSel = m_UserSectCombox.GetCurSel();
	if (nCurSel != -1)
	{
		T_SECT_K sectK = m_UserSectCombox.GetItemData(nCurSel);
		m_pDoc->m_pAttrCtrl->GetSect(sectK, m_PSC_USER_SectData);
	}
}

void CCmdReinforceSectDlg::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_ListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(iItem == -1)
	{
		return;
	}
	m_RfstKey = m_ListCtrl.GetItemData(iItem);
	if(m_pDoc->m_pAttrCtrl->GetRfst(m_RfstKey,m_Data))
		Data2Dlg();
}

void CCmdReinforceSectDlg::OnCheckAutoCalc()
{
	BOOL bEnable = !m_CheckAutoCalc.GetCheck();
	GetDlgItem(IDC_CMD_STATIC9)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_STATIC10)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_STATIC11)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_STATIC12)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT3)->EnableWindow(bEnable);
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(!bEnable);
}

void CCmdReinforceSectDlg::OnStiff()
{
	if (CalcStiff())
	{
		CCMStiffDlg dlg(m_pDoc, &m_ReSectData, NULL, this);
		dlg.DoModal();
	}
}

void CCmdReinforceSectDlg::OnChangeConnectType()
{
	UpdateData(TRUE);
	BOOL bEnable = FALSE;
	if (m_nConnectType == 2)
		bEnable = TRUE;
	GetDlgItem(IDC_CMD_STATIC7)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_STATIC8)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(bEnable);
	if(bEnable)
		OnCheckAutoCalc();
	else
	{
		GetDlgItem(IDC_CMD_STATIC9)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_STATIC10)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_STATIC11)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_STATIC12)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	}
}

void CCmdReinforceSectDlg::SetListHeaderTitle()
{
	CString aTitle[] = {
		_LS(IDS_WG_CMD_Reinforce_Sect_ListCtrl_Tile_1),_LS(IDS_WG_CMD_Reinforce_Sect_ListCtrl_Tile_2),
		_LS(IDS_WG_CMD_Reinforce_Sect_ListCtrl_Tile_3),_LS(IDS_WG_CMD_Reinforce_Sect_ListCtrl_Tile_4),
		_LS(IDS_WG_CMD_Reinforce_Sect_ListCtrl_Tile_5)
	};
	int nColWidth[5];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ListCtrl.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 40; nColWidth[1] = 170; nColWidth[2] = 150; nColWidth[3] = 120; nColWidth[4] = 100;

	// Set Title
	for(i = 0; i < 5; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_ListCtrl.InsertColumn(i, &lvcolumn);
		m_ListCtrl.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdReinforceSectDlg::InitListData()
{
	T_RFST_K_LIST aRfstK;
	T_RFST_D RfstD;
	m_pDoc->m_pAttrCtrl->GetRfstKeyList(aRfstK);
	if (!aRfstK.IsEmpty() && m_RfstKey == 0)
	{
		m_RfstKey = aRfstK[0];
	}
	m_ListCtrl.DeleteAllItems();
	int nIndex = 0;
	for (int i = 0; i < aRfstK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetRfst(aRfstK[i], RfstD);
		InsertItemList(aRfstK[i], RfstD);
		if (aRfstK[i] == m_RfstKey)
			nIndex = i;
	}
	m_ListCtrl.SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible(nIndex, FALSE);
}

void CCmdReinforceSectDlg::InsertItemList(T_RFST_K RfstK,const T_RFST_D& rfstD,BOOL bModify /* = FALSE */)
{
	CString strType[] = {
	_LS(IDS_WG_CMD_Reinforce_Sect_Type_1),_LS(IDS_WG_CMD_Reinforce_Sect_Type_2),_LS(IDS_WG_CMD_Reinforce_Sect_Type_3),
	_LS(IDS_WG_CMD_Reinforce_Sect_Type_4),_LS(IDS_WG_CMD_Reinforce_Sect_Type_5),_LS(IDS_WG_CMD_Reinforce_Sect_Type_6),
	_LS(IDS_WG_CMD_Reinforce_Sect_Type_7),
	};
	LVITEM lvitem;
	CString str;
	T_MATL_D matlD;
	T_SECT_D sectD;
	int nIndex = m_ListCtrl.GetItemCount();
	if (bModify)
	{
		nIndex = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
		if (nIndex == -1)
			return;
	}
	for(int i = 0; i < 5; i++)
	{
		lvitem.iItem = nIndex;
		lvitem.iSubItem = i;
		if(i == 0)
		{
			str.Format(_T("%d"), RfstK);
		}
		else if(i == 1)
			str = rfstD.strReinforceSectName;
		else if(i == 2)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSect(rfstD.nOriginSectK, sectD))
			{
				ASSERT(FALSE);
			}
			str = sectD.SName;
		}
		else if(i == 3)
			str = strType[rfstD.nReinforceType];
		else if(i == 4)
		{
			if (rfstD.nMatlK == 0)
				str = _LS(IDS_WG_CMD_Reinforce_Sect_Same_Matl);
			else
			{
				m_pDoc->m_pAttrCtrl->GetMatl(rfstD.nMatlK, matlD);
				str = matlD.Name;
			}
		}
		lvitem.pszText = str.GetBuffer();
		str.ReleaseBuffer();
		lvitem.mask = LVIF_TEXT;
		if(i == 0)
		{
			if (!bModify)
			{
				lvitem.mask |= LVIF_PARAM;
				lvitem.lParam = (LPARAM)nIndex;
				nIndex = m_ListCtrl.InsertItem(&lvitem);
				m_ListCtrl.SetItem(&lvitem);
				m_ListCtrl.SetItemData(nIndex, RfstK);
			}
		}
		else 
			m_ListCtrl.SetItemText(nIndex, i, str);
	}
}

void CCmdReinforceSectDlg::ForceModifySectName(T_RFST_K RfstK, T_RFST_D& DBrfstD)
{
	CString strReinforceSectName = DBrfstD.strReinforceSectName;
	CMap<CString, LPCTSTR, int, int> mapReinforceDBSectName;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartRfst();
	T_RFST_K rfstK;
	T_RFST_D rfstD;
	while (pos)
	{
		m_pDoc->m_pAttrCtrl->GetNextRfst(pos, rfstK, rfstD);
		mapReinforceDBSectName[rfstD.strReinforceSectName] = rfstK;
	}
	int nIndex = 1;
	CString strName = strReinforceSectName;
	while (mapReinforceDBSectName.PLookup(strReinforceSectName))
	{
		if (mapReinforceDBSectName[strReinforceSectName] == RfstK)
			break;
		strReinforceSectName.Format(_T("%s-%d"), strName, nIndex);
		nIndex++;
	}
	DBrfstD.strReinforceSectName = strReinforceSectName;
}
