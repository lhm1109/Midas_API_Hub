#include "stdafx.h"
#include "wg_treemenu.h"
#include "EffectStiffnessGenerate.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_dgn\DgnDataCtrl.h"

#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEffectStiffnessGenerate::CEffectStiffnessGenerate(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CEffectStiffnessGenerate::IDD, pParent)
{
	m_pDoc = nullptr;
}

void CEffectStiffnessGenerate::Execute()
{
	if ( !Dlg2Data() ) return;
	GSaveHistoryNF(_LS(IDS_SEIS_GENERATE_EFFCT_STIFF_FINISH_GENERATE));
}

void CEffectStiffnessGenerate::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BEAM_MOMENT_EDIT, m_edtBM);
	DDX_Control(pDX, IDC_BEAM_SHEAR_EDIT, m_edtBS);
	DDX_Control(pDX, IDC_COLMN_MOMENT_EDIT, m_edtCM);
	DDX_Control(pDX, IDC_COLMN_SHEAR_EDIT, m_edtCS);
	DDX_Control(pDX, IDC_COLMN_AXIAL_EDIT, m_edtCA);
	DDX_Control(pDX, IDC_WALL_MOMENT_EDIT, m_edtWM);
	DDX_Control(pDX, IDC_WALL_SHEAR_EDIT, m_edtWS);
	DDX_Control(pDX, IDC_WALL_AXIAL_EDIT, m_edtWA);
}
BEGIN_MESSAGE_MAP(CEffectStiffnessGenerate, CMenuBarChildDlg)
END_MESSAGE_MAP()

BOOL CEffectStiffnessGenerate::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	m_edtBM.SetUnitType(D_UNITSYS_NONE);
	m_edtBS.SetUnitType(D_UNITSYS_NONE);
	m_edtCM.SetUnitType(D_UNITSYS_NONE);
	m_edtCS.SetUnitType(D_UNITSYS_NONE);
	m_edtCA.SetUnitType(D_UNITSYS_NONE);
	m_edtWM.SetUnitType(D_UNITSYS_NONE);
	m_edtWS.SetUnitType(D_UNITSYS_NONE);
	m_edtWA.SetUnitType(D_UNITSYS_NONE);

	switch ( m_pDoc->m_pAttrCtrl2->GetSemeMethod() )
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
	case D_SEME_METHOD_KISTEC2019:
		{
			m_edtBM.SetEditUnit(0.5);
			m_edtBS.SetEditUnit(1.0);
			m_edtCM.SetEditUnit(0.5);
			m_edtCS.SetEditUnit(1.0);
			m_edtCA.SetEditUnit(1.0);
			m_edtWM.SetEditUnit(0.5);
			m_edtWS.SetEditUnit(1.0);
			m_edtWA.SetEditUnit(1.0);
		}
		break;
	case D_SEME_METHOD_AIK_G_001_2021:
		{
			m_edtBM.SetEditUnit(0.30);
			m_edtBS.SetEditUnit(1.00);
			m_edtCM.SetEditUnit(0.30);
			m_edtCS.SetEditUnit(1.00);
			m_edtCA.SetEditUnit(1.00);
			m_edtWM.SetEditUnit(0.35);
			m_edtWS.SetEditUnit(0.50);
			m_edtWA.SetEditUnit(1.00);
		}
		break;
	default: ASSERT(0); break;
	}

	return TRUE;
}

void CEffectStiffnessGenerate::OnTmClose()
{
	CloseDlg();
}

void CEffectStiffnessGenerate::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if ( !hWnd || !IsWindow(hWnd) ) return;

	switch ( lHint )
	{
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	case D_UPDATE_BUFFER_BEFORE:
	case D_UPDATE_BUFFER_AFTER:
	case D_UPDATE_UNIT:
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	default:
		break;
	}
}

BOOL CEffectStiffnessGenerate::ExternalInit(UINT key)
{
	if ( key == 0 ) return FALSE;

	UpdateData(FALSE);
	return TRUE;
}

BOOL CEffectStiffnessGenerate::Dlg2Data()
{
	UpdateData(TRUE);

	T_BCCT_D BcctD;
	BOOL bExistBcct = m_pDoc->m_pAttrCtrl->GetBcct(BcctD);

	UINT nDoDisable = IDOK;
	if ( BcctD.cSelect[4] == '1' || BcctD.cSelect[5] == '1' )
	{
		nDoDisable = AfxMessageBox(_LS(IDS_SEIS_GENERATE_EFFCT_STIFF_EXIST_BOUNDARY), MB_OKCANCEL);
	}

	if ( nDoDisable == IDOK )
	{
		if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_KISTEC_Series() || m_pDoc->m_pAttrCtrl2->IsSemeMethod_MOE_Series())
		{
			IUsageCounter::Use(_T("SPE_BCCT"));
		}
		else if (m_pDoc->m_pAttrCtrl2->IsSemeMethod_AIK_2021())
		{
			IUsageCounter::Use(_T("SPD_BCCT"));
		}
		else
		{
			ASSERT(0);
		}

		if ( !m_pDoc->m_pUndoCtrl->StartEditDB(_LS(IDS_TM_DATACTRL_Modify_Stiffness_Scale_Fact), CMDTYPE_REMOVE_ANALYSIS) ) return FALSE;
		if ( !Save_Stiffness_BC() )
		{
			m_pDoc->m_pUndoCtrl->CancelEditDB();
			return FALSE;
		}
		if ( !Save_Stiffness_WALL() )
		{
			m_pDoc->m_pUndoCtrl->CancelEditDB();
			return FALSE;
		}
		if ( bExistBcct && ( BcctD.cSelect[4] == '1' || BcctD.cSelect[5] == '1' ) )
		{
			BcctD.cSelect[4] = BcctD.cSelect[5] = '0';
			if ( !m_pDoc->m_pEditData->AddBcct(BcctD) )
			{
				m_pDoc->m_pUndoCtrl->CancelEditDB();
				return FALSE;
			}
		}
		GSaveHistoryNF(_LS(IDS_SEIS_GENERATE_EFFCT_STIFF_WARNING));
		m_pDoc->m_pUndoCtrl->CloseEditDB(TRUE, TRUE, LT_SECF_CMD);
	}
	else if ( nDoDisable == IDCANCEL )
	{
		return FALSE;
	}

	return TRUE;
}

int CEffectStiffnessGenerate::TypeByDefaultSecfValue(T_SECF_K key)
{
	return TypeByDefaultSecfValue(key.key.entity);
}

// Composite Box, I / 기타 Composite / Non Composite의 세 타입으로 나눔
int CEffectStiffnessGenerate::TypeByDefaultSecfValue(T_SECT_K key)
{
	T_SECT_D SectData;
	if ( !m_pDoc->m_pAttrCtrl->GetSect(key, SectData) ) { ASSERT(0); return FALSE; }

	switch ( SectData.nStype )
	{
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_TUB:
		{
			return 0; // Composite Box, I
		}
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:
	case D_SECT_TYPE_COMPO_PC:
		{
			return 1; // Composite Others
		}
	default:
		{
			return 2; // Not Composite
		}
	}
}

BOOL CEffectStiffnessGenerate::IsBeam(T_ELEM_K Key)
{
	return m_pDoc->m_pAttrCtrl->IsBeamTypeElem(Key);
}

BOOL CEffectStiffnessGenerate::IsColumn(T_ELEM_K Key)
{
	return m_pDoc->m_pAttrCtrl->IsColmTypeElem(Key);
}

BOOL CEffectStiffnessGenerate::Save_Stiffness_BC()
{
	CArray<T_SECT_K, T_SECT_K> aSectKey;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectKey);
	int nSectCount = aSectKey.GetSize();
	if ( nSectCount < 1 ) return FALSE;

	for ( int i = 0; i < nSectCount; i++ )
	{
		BOOL bModify = FALSE;
		CArray<T_SECF_K, T_SECF_K> aSecfK;
		CArray<T_SECF_D, T_SECF_D&> aSecfD;

		if ( !m_pDoc->m_pAttrCtrl->IsUsedSect(aSectKey[i]) ) continue;

		m_pDoc->m_pAttrCtrl->GetSecfSect(aSectKey[i], aSecfK, aSecfD);                //섹터가 기존에 가지고있는 Secf들을 불러옴

		CArray<T_ELEM_K, T_ELEM_K> aSectElem;
		CArray<T_SECT_K, T_SECT_K> aSectKeyTemp;
		aSectKeyTemp.Add(aSectKey[i]);
		m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&aSectKeyTemp, 0, &aSectElem);        //그 섹터가 가지고 있는 요소들 찾기 !!
		int nSectElemCount = aSectElem.GetSize();
		if ( nSectElemCount < 1 ) continue;

		BOOL bOnlyConc = TRUE;
		for ( int j = 0; j < nSectElemCount; j++ )
		{
			if ( CheckElemConcMatl(aSectElem[j]) ) continue;
			bOnlyConc = FALSE;
			break;
		}
		if ( !bOnlyConc )
		{
			CString strMsg;
			strMsg.Format(_LS(IDS_WARNING_SEIS__EFFCT_STIFF_MATL_d), aSectKey[i]);
			m_pDoc->DisplayHistoryMessage(strMsg);
			continue;
		}

		BOOL bExistBeam = FALSE, bExistColmn = FALSE;
		for ( int j = 0; j < nSectElemCount; j++ )
		{
			if ( IsBeam(aSectElem[j]) )
			{
				bExistBeam = TRUE;
			}
			else if ( IsColumn(aSectElem[j]) )
			{
				bExistColmn = TRUE;
			}
			if ( bExistBeam && bExistColmn ) break;
		}
		if ( !bExistBeam && !bExistColmn ) continue;

		int nSecfCount = aSecfD.GetSize();          //이미 있는 Secf들
		BOOL bOtherGroup = TRUE;
		if ( nSecfCount > 0 )
		{
			bOtherGroup = FALSE;
			for ( int k = 0; k < nSecfCount; k++ )
			{
				if ( aSecfD[k].GroupKey != D_BNGR_KEY_DEFAULT )
				{
					bOtherGroup = TRUE;
					if ( !m_pDoc->m_pEditData->DelSecf(aSecfK[k]) ) return FALSE;
					continue;
				}
				ChangeSecf(bExistBeam, bExistColmn, aSecfD[k]);

				if ( !m_pDoc->m_pEditData->DelSecf(aSecfK[k]) ) return FALSE;
				if ( !m_pDoc->m_pEditData->AddSecf(aSecfK[k], aSecfD[k]) ) return FALSE;
			}
		}
		if ( bOtherGroup )
		{
			T_SECF_D data;
			T_SECF_K key;

			data.Initialize();
			data.GroupKey = D_BNGR_KEY_DEFAULT;
			ChangeSecf(bExistBeam, bExistColmn, data);

			m_pDoc->m_pAttrCtrl->MakeKeySecf(aSectKey[i], data.GroupKey, key);
			if ( !m_pDoc->m_pEditData->AddSecf(key, data) ) return FALSE;
		}
	}

	return TRUE;
}

BOOL CEffectStiffnessGenerate::Save_Stiffness_WALL()
{
	CArray<T_BNGR_K, T_BNGR_K> aBngrKey;
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(aBngrKey);
	aBngrKey.InsertAt(0, D_BNGR_KEY_DEFAULT, 1);
	int nBngrCount = aBngrKey.GetSize();
	if ( nBngrCount < 1 ) return FALSE;

	CArray<T_ELEM_K, T_ELEM_K> aElemK;

	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemK);
	int nElemCount = aElemK.GetSize();

	for ( int i = 0; i < nElemCount; i++ )
	{
		BOOL bModify = FALSE;
		T_ELEM_D ElemD;
		CArray<T_WSSF_D, T_WSSF_D &> aWssfD;
		CArray<T_WSSF_K, T_WSSF_K> aWssfK;

		if ( !m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD) ) continue;

		if ( !m_pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp) ) continue;

		m_pDoc->m_pAttrCtrl->GetWssfElem(aElemK[i], aWssfK, aWssfD);

		int nWssfCount = aWssfK.GetSize();
		BOOL bOtherGroup = TRUE;
		if ( nWssfCount > 0 )
		{
			bOtherGroup = FALSE;
			for ( int k = 0; k < nWssfCount; k++ )
			{
				if ( aWssfD[k].GroupKey != D_BNGR_KEY_DEFAULT )
				{
					bOtherGroup = TRUE;
					if ( !m_pDoc->m_pEditData->DelWssf(aWssfK[k]) ) return FALSE;
					continue;
				}
				ChangeWssf(ElemD.nWallType, aWssfD[k]);
				if ( !m_pDoc->m_pEditData->DelWssf(aWssfK[k]) ) return FALSE;
				if ( !m_pDoc->m_pEditData->AddWssf(aWssfK[k], aWssfD[k]) ) return FALSE;
			}
		}
		if ( bOtherGroup )
		{
			T_WSSF_D WssfD;

			WssfD.Initialize();
			WssfD.GroupKey = D_BNGR_KEY_DEFAULT;

			ChangeWssf(ElemD.nWallType, WssfD);

			T_WSSF_K WssfK;
			m_pDoc->m_pAttrCtrl->MakeKeyWssf(aElemK[i], WssfD.GroupKey, WssfK);
			if ( !m_pDoc->m_pEditData->AddWssf(WssfK, WssfD) ) return FALSE;
		}
	}
	return TRUE;
}

BOOL CEffectStiffnessGenerate::CheckElemConcMatl(T_ELEM_K ElemK)
{
	T_ELEM_D ElemD;
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) ) { ASSERT(0); return FALSE; }

	T_MATL_D MatlD;
	if ( !m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD) ) { ASSERT(0); return FALSE; }
	if ( MatlD.Type == _T("C") ) return TRUE;

	return FALSE;
}

void CEffectStiffnessGenerate::ChangeSecf(BOOL bBeam, BOOL bColmn, T_SECF_D &rData)
{
	if ( rData.GroupKey != D_BNGR_KEY_DEFAULT )
	{
		rData.Initialize();
	}
	if ( bBeam && bColmn )
	{
		rData.dScaleFactor[4] = m_edtBM.GetEditValue() < m_edtCM.GetEditValue() ? m_edtBM.GetEditValue() : m_edtCM.GetEditValue();
		rData.dScaleFactor[5] = m_edtBM.GetEditValue() < m_edtCM.GetEditValue() ? m_edtBM.GetEditValue() : m_edtCM.GetEditValue();
		rData.dScaleFactor[1] = m_edtBS.GetEditValue() < m_edtCS.GetEditValue() ? m_edtBS.GetEditValue() : m_edtCS.GetEditValue();
		rData.dScaleFactor[2] = m_edtBS.GetEditValue() < m_edtCS.GetEditValue() ? m_edtBS.GetEditValue() : m_edtCS.GetEditValue();
		rData.dScaleFactor[0] = m_edtCA.GetEditValue();
	}
	else if ( bBeam )
	{
		rData.dScaleFactor[4] = m_edtBM.GetEditValue();
		rData.dScaleFactor[5] = m_edtBM.GetEditValue();
		rData.dScaleFactor[1] = m_edtBS.GetEditValue();
		rData.dScaleFactor[2] = m_edtBS.GetEditValue();
	}
	else if ( bColmn )
	{
		rData.dScaleFactor[4] = m_edtCM.GetEditValue();
		rData.dScaleFactor[5] = m_edtCM.GetEditValue();
		rData.dScaleFactor[1] = m_edtCS.GetEditValue();
		rData.dScaleFactor[2] = m_edtCS.GetEditValue();
		rData.dScaleFactor[0] = m_edtCA.GetEditValue();
	}
}

void CEffectStiffnessGenerate::ChangeWssf(UINT WallType, T_WSSF_D &rData)
{
	if ( rData.GroupKey != D_BNGR_KEY_DEFAULT )
	{
		rData.Initialize();
	}

	switch ( WallType )
	{
	case 0:
		{
			rData.dShear   = m_edtWS.GetEditValue();
			rData.dBending = rData.dAxial = m_edtWM.GetEditValue() < m_edtWA.GetEditValue() ? m_edtWM.GetEditValue() : m_edtWA.GetEditValue();
		}
		break;
	case 1: // CRB Type
		{
			rData.dBending = m_edtWM.GetEditValue();
			rData.dShear   = m_edtWS.GetEditValue();
			rData.dAxial   = m_edtWA.GetEditValue();
		}
		break;
	}
}