// ElemStiffScaleFactorDlg.cpp : implementation file
//
// 2021. 01. 29    by Jeongwoo Kwon
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemStiffScaleFactorDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DbToolMPhi_JP.h"
#include "..\wg_db\DB_QUERY_SECT.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\MIT_lib\MMath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemStiffScaleFactorDlg dialog


CElemStiffScaleFactorDlg::CElemStiffScaleFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemStiffScaleFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemStiffScaleFactorDlg)
	m_nAddOrDel = 0;
	//}}AFX_DATA_INIT


}

void CElemStiffScaleFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemStiffScaleFactorDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_ADD_REP_RADIO, m_nAddOrDel);

	DDX_Control(pDX, IDC_TM_REDU_AREA_EDIT, m_wndArea);
	DDX_Control(pDX, IDC_TM_REDU_ASY_EDIT, m_wndAsy);
	DDX_Control(pDX, IDC_TM_REDU_ASZ_EDIT, m_wndAsz);
	DDX_Control(pDX, IDC_TM_REDU_IXX_EDIT, m_wndIxx);
	DDX_Control(pDX, IDC_TM_REDU_IYY_EDIT, m_wndIyy);
	DDX_Control(pDX, IDC_TM_REDU_IZZ_EDIT, m_wndIzz);
	DDX_Control(pDX, IDC_TM_REDU_WEI_EDIT, m_wndWeight);
	DDX_Control(pDX, IDC_TM_MPHI_STIFF_SCALE_TYPE_COMBO, m_wndStiffScaleTypeCombo);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemStiffScaleFactorDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemStiffScaleFactorDlg)
	ON_BN_CLICKED(IDC_TM_ADD_REP_RADIO, OnTmOption)
	ON_BN_CLICKED(IDC_TM_DELETE_RADIO, OnTmOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_UPDATE_BTN, OnTmSectEffStiffUpdateBtn)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemStiffScaleFactorDlg message handlers

BOOL CElemStiffScaleFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	T_ESSF_D data;
	data.Initialize();

	Data2Dlg(data);

	if(CDBLib::AllowJPGrid())
	{
		GetDlgItem(IDC_TM_STATIC_GR6)                 ->ShowWindow(TRUE);
		GetDlgItem(IDC_TM_MPHI_STIFF_SCALE_TYPE_COMBO)->ShowWindow(TRUE);
		GetDlgItem(IDC_TM_SEIS_CVL_UPDATE_BTN)        ->ShowWindow(TRUE);

		m_wndStiffScaleTypeCombo.AddString(_LS(IDS_IDD_TM_ELEM_STIFF_SCALE_FACTOR__STIFF_SCALE_TYPE_COMBO_INITIAL_STIFFNESS_RATIO));
		m_wndStiffScaleTypeCombo.AddString(_LS(IDS_IDD_TM_ELEM_STIFF_SCALE_FACTOR__STIFF_SCALE_TYPE_COMBO_YIELD_STIFFNESS_RATIO));
		m_wndStiffScaleTypeCombo.SetCurSel(0);
	}
	else {
		GetDlgItem(IDC_TM_STATIC_GR6)                 ->ShowWindow(FALSE);
		GetDlgItem(IDC_TM_MPHI_STIFF_SCALE_TYPE_COMBO)->ShowWindow(FALSE);
		GetDlgItem(IDC_TM_SEIS_CVL_UPDATE_BTN)        ->ShowWindow(FALSE);


		CRect rSSF, rWeight;
		GetDlgItem(IDC_TM_STATIC_GR5)   ->GetWindowRect(rSSF);
		GetDlgItem(IDC_TM_SCALE_WEI_TXT)->GetWindowRect(rWeight);
		ScreenToClient(rSSF);
		ScreenToClient(rWeight);
		const int iDistY  = rSSF.bottom - rWeight.bottom;
		const int iWidth  = rSSF.right - rSSF.left;
		const int iHeight = rSSF.bottom - rSSF.top;
		GetDlgItem(IDC_TM_STATIC_GR5)->MoveWindow(rSSF.left, rSSF.top, iWidth, iHeight - iDistY + 10);
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemStiffScaleFactorDlg::Data2Dlg(T_ESSF_D &data)
{
	CString csFactor;
	csFactor.Format(_T("%.4f"), data.dScaleFactor[0]); m_wndArea.SetWindowText(csFactor);
	csFactor.Format(_T("%.4f"), data.dScaleFactor[1]); m_wndAsy.SetWindowText(csFactor);
	csFactor.Format(_T("%.4f"), data.dScaleFactor[2]); m_wndAsz.SetWindowText(csFactor);
	csFactor.Format(_T("%.4f"), data.dScaleFactor[3]); m_wndIxx.SetWindowText(csFactor);
	csFactor.Format(_T("%.4f"), data.dScaleFactor[4]); m_wndIyy.SetWindowText(csFactor);
	csFactor.Format(_T("%.4f"), data.dScaleFactor[5]); m_wndIzz.SetWindowText(csFactor);
	csFactor.Format(_T("%.4f"), data.dScaleFactor[6]); m_wndWeight.SetWindowText(csFactor);
}

void CElemStiffScaleFactorDlg::OnTmOption()
{
	UpdateData();
// 	GetDlgItem(IDC_TM_REDU_SHEAR_EDIT)->EnableWindow(m_nAddOrDel==0);  
// 	GetDlgItem(IDC_TM_REDU_BENDING_EDIT)->EnableWindow(m_nAddOrDel==0);
	this->m_wndGroupCombo.ShowHideAll(m_nAddOrDel==1);
}

void CElemStiffScaleFactorDlg::Execute()
{
	if (!UpdateData(TRUE)) return;
	
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	ASSERT(pDoc);

	// Wall List 가져오기 - Wall 요소에만 적용 
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	I_GENModelBase::GetCurMySelfST()->GetSelectedElemKeyList(aElemList);

	T_ELEM_D ElemD;
	CArray<T_ELEM_K, T_ELEM_K> aFrameList; 
	CArray<T_ELEM_K, T_ELEM_K> aCRBWallList; 
	for(int i = 0; i< aElemList.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetElem(aElemList[i], ElemD)) continue;    
		if (pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))  
			aFrameList.Add(aElemList[i]);
		
	}
	if (aFrameList.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Frame));
		return;
	}

	// Data 가져오기 
	T_ESSF_D data; data.Initialize();
	Dlg2Data(data);

	BOOL bOK = FALSE;
	if (m_nAddOrDel == 0)   // Add/Replace
	{
		bOK = pDoc->m_pDataCtrl->AddEssf(aFrameList, data);	
	}
	else if (m_nAddOrDel == 1)    // Delete 
	{
		bOK = pDoc->m_pDataCtrl->DelEssf(aFrameList, data.GroupKey);
	}
	else ASSERT(0);

	if (bOK)  pDoc->m_pViewCtrl->UnselectAll(NULL);  
}

void CElemStiffScaleFactorDlg::Dlg2Data(T_ESSF_D &data)
{
	m_wndGroupCombo.GetSelectedBngr(data.GroupKey);
	
	CString csFactor;
	m_wndArea.GetWindowText(csFactor); 	data.dScaleFactor[0] = _tstof(csFactor);
	m_wndAsy.GetWindowText(csFactor);	data.dScaleFactor[1] = _tstof(csFactor);
	m_wndAsz.GetWindowText(csFactor);	data.dScaleFactor[2] = _tstof(csFactor);
	m_wndIxx.GetWindowText(csFactor);	data.dScaleFactor[3] = _tstof(csFactor);
	m_wndIyy.GetWindowText(csFactor);	data.dScaleFactor[4] = _tstof(csFactor);
	m_wndIzz.GetWindowText(csFactor);	data.dScaleFactor[5] = _tstof(csFactor);
	m_wndWeight.GetWindowText(csFactor); data.dScaleFactor[6] = _tstof(csFactor);
}

// For WorkTree
BOOL CElemStiffScaleFactorDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	T_ESSF_K Key;
	T_ESSF_D data;
	Key.keymap = key;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetEssf(Key, data);

	m_wndGroupCombo.ChangeSelect(data.GroupKey);
	
	Data2Dlg(data);

	m_nAddOrDel = 0;
	UpdateData(FALSE);

	return TRUE;
}

void CElemStiffScaleFactorDlg::OnTmDefineGroupButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_DEF_BNDR_GROUP,0));	
}

void CElemStiffScaleFactorDlg::OnTmSectEffStiffUpdateBtn()
{
	if (!UpdateData(TRUE)) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	ASSERT(pDoc);

	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	I_GENModelBase::GetCurMySelfST()->GetSelectedElemKeyList(aElemList);
 
	T_ELEM_D ElemD;
	CMapEx<T_ELEM_K, T_ELEM_K, int, int> mapFrameList;
	mapFrameList.InitHashTable(HASHSIZEELEM);
	for (int i = 0; i < aElemList.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetElem(aElemList[i], ElemD)) continue;
		if (pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			mapFrameList.SetAt(aElemList[i], i);

	}
	if (mapFrameList.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Frame));
		return;
	}

	int nCountIehp = pDoc->m_pAttrCtrl->GetCountIehp();
	int nCountIehg = pDoc->m_pAttrCtrl->GetCountIehg();
	if (nCountIehp == 0) return;
	if (nCountIehg == 0) return;

     // CDbToolMphi::Cal_IehpD 참고
    T_UNIT_INDEX CurrUnit;
    pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
    auto L_Set_INIT_EI = [pDoc, CurrUnit](double dUltimateMoment[2], double dYieldRotn3rd[2], double& dInitStiffness)
    {
        double dEI[2] = { 0.0, };
        for(int i=0; i<2; ++i)
        {
            const double dPhi    = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_PERLENGTH, dYieldRotn3rd[i]);
            const double dMoment = pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_MOMENT,    dUltimateMoment[i]);
            dEI[i] = MMath::IsZero(dPhi) ? 0.0 : fabs(dMoment/ dPhi);
            dEI[i] = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_FORCE,    dEI[i]);
            dEI[i] = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_AREA,     dEI[i]);
        }

        int iIdxT = 0, iIdxS = 0;
            
        if(dEI[0] < dEI[1]) // 강성이 높은 쪽 정보로 Set
        {
			dInitStiffness = dEI[1];
        }
        else
        {
			dInitStiffness = dEI[0];
		}
    };

		const double dRhoConcrete = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, 24.5);
		const double dRhoSteel    = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_DENSITY, 77. );

		auto CalculateWieghtFactor = [this, pDoc, dRhoConcrete, dRhoSteel](const T_ELEM_K ElemK, const T_ELEM_D ElemD, const double dDefaultWeightRatio)
			{
				// weight factor
				T_SECT_SSM_D SectSsmD; SectSsmD.Initialize();
				if (pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(ElemK, SectSsmD))
				{
					T_REIN_SSM_D ReinSSMD; ReinSSMD.Initialize();
					if (pDoc->m_pAttrCtrl2->GetQReinSsm()->Get(SectSsmD.kReinSsm, ReinSSMD))
					{
						double dAreaExist      = 0.;
						double dAreaRCRein     = 0.;
						double dAreaSteelPlate = 0.;

						const int iSect = ElemD.elpro;
						T_SECT_D SectD;
						SectD.Initialize();
						if(!pDoc->m_pAttrCtrl->GetSectDesign(iSect, SectD))
							return dDefaultWeightRatio;
						CDBDoc::GetDocPoint()->m_pSectDB->CalcSectData(SectD);
						CString strSectShape = SectD.SectBefore.Shape;
						dAreaExist = SectD.SectBefore.SectI.Stiffness.Area;
						if (MMath::IsZero(dAreaExist))
							return dDefaultWeightRatio;

						// RC reinforced section
						T_SECT_D SectDReinSect;
						if (ReinSSMD.ReinSectRC.bUse)
						{
							// Check Sect Type.
							if(!(SectD.nStype==D_SECT_TYPE_REGULAR || SectD.nStype==D_SECT_TYPE_TAPERED || SectD.nStype==D_SECT_TYPE_USER))
								return dDefaultWeightRatio;

							SectDReinSect = SectD;
							CDbToolMphi_JP::ModifyReinSectSize(ReinSSMD.ReinSectRC.LocalY.dThick, ReinSSMD.ReinSectRC.LocalZ.dThick, SectDReinSect);
							if (SectDReinSect.nStype==D_SECT_TYPE_USER)
								SectDReinSect.nStype = D_SECT_TYPE_REGULAR;
							CDBDoc::GetDocPoint()->m_pSectDB->CalcSectData(SectDReinSect);

							// the centroid of both the existing and reinforced sections are assumed to be the same.
							dAreaRCRein = SectDReinSect.SectBefore.SectI.Stiffness.Area;
						}

						// Steel plate
						T_SECT_D SectDSteelPlate;
						if (ReinSSMD.ReinSectSP.bUse)
						{
							SectDSteelPlate = ReinSSMD.ReinSectRC.bUse ? SectDReinSect : SectD;
							const double dSteelThickY = ReinSSMD.ReinSectSP.LocalY.dThick;
							double       dSteelThickZ = ReinSSMD.ReinSectSP.LocalZ.dThick;
							if ( (strSectShape!=D_SECT_SHAPE_REG_B && strSectShape!=D_SECT_SHAPE_REG_SB) && (strSectShape!=D_SECT_SHAPE_REG_SOCT && strSectShape!=D_SECT_SHAPE_REG_OCT) )
								dSteelThickZ = ReinSSMD.ReinSectSP.LocalY.dThick;
							CDbToolMphi_JP::ModifyReinSectSize(dSteelThickY, dSteelThickZ, SectDSteelPlate);
							CDBDoc::GetDocPoint()->m_pSectDB->CalcSectData(SectDSteelPlate);

							dAreaSteelPlate = SectDSteelPlate.SectBefore.SectI.Stiffness.Area;
						}

						const double dWeightExist      = dAreaExist*dRhoConcrete;
						const double dWeightRCRein     = dAreaRCRein*dRhoConcrete;
						const double dWeightSteelPlate = dAreaSteelPlate*dRhoSteel;
						const double dWeightRatio      = (dWeightExist + dWeightRCRein + dWeightSteelPlate)/dWeightExist;

						return dWeightRatio;
					}
				}
				return dDefaultWeightRatio;
			};

	BOOL bInitRatio = (m_wndStiffScaleTypeCombo.GetCurSel() == 0);

	// 	// Data 가져오기 
 	T_ESSF_D data; data.Initialize();
 	Dlg2Data(data);

	CArray<T_ESSF_K, T_ESSF_K> rKey;
	CArray<T_ESSF_D, T_ESSF_D&> rData;

	CArray<T_IEHG_K, T_IEHG_K> aIehgKeyList;
	pDoc->m_pAttrCtrl->GetIehgKeyList(aIehgKeyList);
	for (int index = 0; index < aIehgKeyList.GetSize(); ++index)
	{ 
		T_IEHG_D IehgD;
		if (!pDoc->m_pAttrCtrl->GetIehg(aIehgKeyList[index], IehgD)) { ASSERT(0); continue; }

		T_IEHP_D IehpD;
		if(!pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) { ASSERT(0); continue; }

		T_ELEM_K ElemK = aIehgKeyList[index].key.objK;
		int nTemp;
		if(!mapFrameList.Lookup(ElemK, nTemp)) { continue; }

		T_ELEM_D DataElem;
		if(!pDoc->m_pAttrCtrl->GetElem(ElemK, DataElem)) { ASSERT(0); continue; }

		T_SECT_K SectK = DataElem.elpro;
		T_SECT_D SectD;
		if (!pDoc->m_pAttrCtrl->GetSect(SectK, SectD)) { ASSERT(0); continue; }
		CSectUtil::ConvertSectByTsgr(ElemPairK(ElemK, EN_EL_BEAM), SectD, FALSE);
		
		if (!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		int iMatl = ElemD.elmat;

		T_MATD_D MatdD;
		MatdD.Initialize();
		pDoc->m_pAttrCtrl->GetMatlDesign(iMatl, MatdD);
		if (MatdD.Type != _T("C")) { ASSERT(0); continue; }
		double dEc = MatdD.Data1.Analysis.Elast;

		double dEff_y = 1.0;
		double dEff_z = 1.0;
		for (int j = 0; j < 6; j++)
		{
			if(j<4) continue;  // My, Mz 만 고려
			if (IehpD.cDir[j] != '1') continue;

			int iHysModel = IehpD.nHysModel[j];

			T_IEHP_PROP& CurProp = IehpD.AllProp.PROP[j]; // 비대칭 고려?

			double dInitStiffness = 0.0;
			switch (iHysModel)
			{
			case D_IEHP_KINE:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.KINEMA.dCrackMoment, CurProp.KINEMA.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.KINEMA.dYieldMoment, CurProp.KINEMA.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_ORIG:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.ORIGIN.dCrackMoment, CurProp.ORIGIN.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.ORIGIN.dYieldMoment, CurProp.ORIGIN.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_PICK:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.PEAK.dCrackMoment, CurProp.PEAK.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.PEAK.dYieldMoment, CurProp.PEAK.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_CLOU:
					L_Set_INIT_EI(CurProp.CLOUGH.dYieldMoment, CurProp.CLOUGH.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_DEGR:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.DEGRAD.dCrackMoment, CurProp.DEGRAD.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.DEGRAD.dYieldMoment, CurProp.DEGRAD.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_TAKE:
			case D_IEHP_MTAK:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.TAKEDA.dCrackMoment, CurProp.TAKEDA.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.TAKEDA.dYieldMoment, CurProp.TAKEDA.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_TTET:
			case D_IEHP_MTTE:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.TAKTET.dCrackMoment, CurProp.TAKTET.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.TAKTET.dYieldMoment, CurProp.TAKTET.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_SRCT:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.SRCTET.dCrackMoment, CurProp.SRCTET.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.SRCTET.dYieldMoment, CurProp.SRCTET.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_NBIL:
				L_Set_INIT_EI(CurProp.NORBIL.dYieldMoment, CurProp.NORBIL.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_EBIL:
				L_Set_INIT_EI(CurProp.ELABIL.dYieldMoment, CurProp.ELABIL.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_ETRI:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.ELATRI.dCrackMoment, CurProp.ELATRI.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.ELATRI.dYieldMoment, CurProp.ELATRI.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_ETET:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.ELATET.dCrackMoment, CurProp.ELATET.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.ELATET.dYieldMoment, CurProp.ELATET.dYieldRotn2nd, dInitStiffness);
				break;
			case D_IEHP_SLPB:
			case D_IEHP_SLBT:
			case D_IEHP_SLBC:
			case D_IEHP_SLPT:
			case D_IEHP_SLTT:
			case D_IEHP_SLTC:
				if (bInitRatio)
					L_Set_INIT_EI(CurProp.SLIP.dCrackMoment, CurProp.SLIP.dYieldRotn1st, dInitStiffness);
				else
					L_Set_INIT_EI(CurProp.SLIP.dUltimateMoment, CurProp.SLIP.dYieldRotn3rd, dInitStiffness);
				break;
			default:
				ASSERT(0);
			}

			if      (j == 4) dEff_y = dInitStiffness / (dEc*SectD.SectBefore.SectI.Stiffness.Ryy);
			else if (j == 5) dEff_z = dInitStiffness / (dEc*SectD.SectBefore.SectI.Stiffness.Rzz);
			else ASSERT(0);
		} // j

		data.dScaleFactor[4] = dEff_y;
		data.dScaleFactor[5] = dEff_z;
		data.dScaleFactor[6] = CalculateWieghtFactor(ElemK, ElemD, data.dScaleFactor[6]);

		pDoc->m_pDataCtrl->AddEssf(ElemK, data);

	} // aIehgKeyList

	pDoc->m_pViewCtrl->UnselectAll(NULL);
}