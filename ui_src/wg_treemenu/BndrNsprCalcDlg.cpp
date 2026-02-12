// BndrNsprCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrNsprCalcDlg.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\PileDB.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBndrNsprCalcDlg dialog
CBndrNsprCalcDlg::CBndrNsprCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CBndrNsprCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrNsprCalcDlg)
	m_option = 0;
	m_type = 0;
	m_nShapeRdo = 0;
	//}}AFX_DATA_INIT

	m_group1.Add(IDC_TM_STATIC2);
	m_group1.Add(IDC_TM_STATIC3);
	m_group1.Add(IDC_TM_STATIC5);
	m_group1.Add(IDC_TM_STATIC6);
	m_group1.Add(IDC_TM_STATIC7);
	m_group1.Add(IDC_TM_STATIC9);
	m_group1.Add(IDC_TM_STATIC10);

#if !defined(_CH)
	m_group1.Add(IDC_TM_PS_CODENAME);
#endif
	m_group1.Add(IDC_TM_PS_PILETYPE);
	m_group1.Add(IDC_TM_PS_DRIVEN);
	m_group1.Add(IDC_TM_PS_AUGERED);
	m_group1.Add(IDC_TM_PS_THIK);
	m_group1.Add(IDC_TM_PS_ELASTIC);
	m_group1.Add(IDC_TM_PS_AREA);
	m_group1.Add(IDC_TM_PS_LENGTH);

	m_group1.Add(IDC_TM_PS_THIK_UNIT);
	m_group1.Add(IDC_TM_PS_ELASTIC_UNIT);
	m_group1.Add(IDC_TM_PS_AREA_UNIT);
	m_group1.Add(IDC_TM_PS_LENGTH_UNIT);

	m_group1_extra.Add(IDC_TM_STATIC1);
	m_group1_extra.Add(IDC_TM_STATIC2);
	m_group1_extra.Add(IDC_TM_PS_CODENAME);

	m_group2.Add(IDC_TM_PS_DIA);
	m_group2.Add(IDC_TM_PS_STIFFNESS);

	m_ps_dia_cmb.Add(IDC_TM_PS_DIA);

	m_group_stiffness.Add(IDC_TM_PS_STIFFNESS);
	
	m_PileTypeGroupNormal.Add(IDC_TM_PS_DIA);
	m_PileTypeGroupNormal.Add(IDC_TM_PS_THIK);
	m_PileTypeGroupNormal.Add(IDC_TM_PS_CAST_THICK);

	m_PileTypeGroupCast.Add(IDC_TM_PS_CAST_DIA);
	m_PileTypeGroupCast.Add(IDC_TM_PS_CAST_THICK);
	
	m_group3.Add(IDC_TM_PS_DIA2);  
	m_group3.Add(IDC_TM_PS_STIFFNESS2);

	m_group3_1.Add(IDC_TM_PS_DIA2);
	m_group3_2.Add(IDC_TM_PS_STIFFNESS2);

	m_group_jgj_shape_rdo.Add(IDC_TM_JGJ_SHAPE_TEXT);
	m_group_jgj_shape_rdo.Add(IDC_TM_JGJ_SHAPE_RDO1);
	m_group_jgj_shape_rdo.Add(IDC_TM_JGJ_SHAPE_RDO2);
	m_group_jgj_shape_rdo.Add(IDC_TM_JGJ_SHAPE_RDO3);

	m_group_jgj_shape_txt.Add(IDC_TM_JGJ_SHAPE_TEXT);
	m_group_jgj_shape_rdo1.Add(IDC_TM_JGJ_SHAPE_RDO1);
	m_group_jgj_shape_rdo2.Add(IDC_TM_JGJ_SHAPE_RDO2);
	m_group_jgj_shape_rdo3.Add(IDC_TM_JGJ_SHAPE_RDO3);
	
	m_group_sect.Add(IDC_TM_PS_DIA);
	m_group_sect.Add(IDC_TM_PS_DIA_TEXT);
	m_group_sect.Add(IDC_TM_PS_DIA_UNIT);
	m_group_sect.Add(IDC_TM_PS_THIK);
	m_group_sect.Add(IDC_TM_PS_THIK_TEXT);
	m_group_sect.Add(IDC_TM_PS_THIK_UNIT);

	m_group_sect_D.Add(IDC_TM_PS_DIA);
	m_group_sect_D.Add(IDC_TM_PS_DIA_TEXT);
	m_group_sect_D.Add(IDC_TM_PS_DIA_UNIT);
	m_group_sect_T.Add(IDC_TM_PS_THIK);
	m_group_sect_T.Add(IDC_TM_PS_THIK_TEXT);
	m_group_sect_T.Add(IDC_TM_PS_THIK_UNIT);
	
	m_group_jgj_Hsect.Add(IDC_TM_JGJ_SECT_NAME_TEXT);
	m_group_jgj_Hsect.Add(IDC_TM_JGJ_SECT_NAME_CMB);

	m_aCtrlCodeName.Add(IDC_TM_CODE_NAME);
	m_aCtrlCodeName.Add(IDC_TM_PS_CODENAME);

	m_aCtrlAlpha.Add(IDC_TM_STATIC11);
	m_aCtrlAlpha.Add(IDC_TM_PS_ALPHA);

	m_nCode = 1;
	m_spiletype = _T("");//_LS(IDS_SDS_CMD__PC);
	m_sHSectName= _T("");
	m_ddia      = -1;
	m_dthik     = -1;

	m_pBitmap = 0;
}

CBndrNsprCalcDlg::~CBndrNsprCalcDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}


void CBndrNsprCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrNsprCalcDlg)
	DDX_Control(pDX, IDC_TM_PS_ALPHA, m_editAlpha);
	DDX_Control(pDX, IDC_TM_PS_CAST_THICK, m_castThick);
	DDX_Control(pDX, IDC_TM_PS_CAST_DIA, m_castDia);
	DDX_Control(pDX, IDC_TM_PS_STIFFNESS2, m_stiffness2);
	DDX_Control(pDX, IDC_TM_PS_DIA2, m_dia2);
	DDX_Control(pDX, IDC_TM_PS_THIK_UNIT, m_thikunit);
	DDX_Control(pDX, IDC_TM_PS_THIK, m_thik);
	DDX_Control(pDX, IDC_TM_PS_PILETYPE, m_CobxType);
	DDX_Control(pDX, IDC_TM_PS_STIFFNESS_UNIT, m_stiffnessunit);
	DDX_Control(pDX, IDC_TM_PS_LENGTH_UNIT, m_lengthunit);
	DDX_Control(pDX, IDC_TM_PS_ELASTIC_UNIT, m_elasticunit);
	DDX_Control(pDX, IDC_TM_PS_DIA_UNIT, m_diaunit);
	DDX_Control(pDX, IDC_TM_PS_AREA_UNIT, m_areaunit);
	DDX_Control(pDX, IDC_TM_PS_STIFFNESS, m_stiffness);
	DDX_Control(pDX, IDC_TM_PS_LENGTH, m_length);
	DDX_Control(pDX, IDC_TM_PS_ELASTIC, m_elastic);
	DDX_Control(pDX, IDC_TM_PS_AREA, m_area);
	DDX_Control(pDX, IDC_TM_JGJ_SECT_NAME_CMB, m_HsectName);
	DDX_Control(pDX, IDC_TM_PS_DIA, m_dia);
	DDX_Control(pDX, IDC_TM_PS_CODENAME, m_codename);
	DDX_Radio(pDX, IDC_TM_PS_DRIVEN, m_type);
	DDX_Radio(pDX, IDC_TM_JGJ_SHAPE_RDO1, m_nShapeRdo);
	DDX_Control(pDX, IDC_TM_PS_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrNsprCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CBndrNsprCalcDlg)
	ON_CBN_SELCHANGE(IDC_TM_JGJ_SECT_NAME_CMB, OnSelchangeCmdHSectName)
	ON_CBN_SELCHANGE(IDC_TM_PS_PILETYPE, OnSelchangeCmdPsPiletype)
	ON_CBN_SELCHANGE(IDC_TM_PS_CODENAME, OnSelchangeCmdPsCodename)
	ON_CBN_SELCHANGE(IDC_TM_PS_DIA, OnSelchangeCmdPsDia)
	ON_CBN_SELCHANGE(IDC_TM_PS_THIK, OnSelchangeCmdPsThik)
	ON_EN_CHANGE(IDC_TM_PS_ELASTIC, OnChangeCmdPsElastic)
	ON_EN_CHANGE(IDC_TM_PS_AREA, OnChangeCmdPsArea)
	ON_EN_CHANGE(IDC_TM_PS_LENGTH, OnChangeCmdPsLength)
	ON_CBN_EDITCHANGE(IDC_TM_PS_DIA, OnEditchangeCmdPsDia)
	ON_CBN_EDITCHANGE(IDC_TM_PS_THIK, OnEditchangeCmdPsThik)
	ON_BN_CLICKED(IDC_TM_PS_AUGERED, OnSelectPileDetail)
	ON_EN_CHANGE(IDC_TM_PS_CAST_DIA, OnChangeCmdPsCastDia)
	ON_EN_CHANGE(IDC_TM_PS_CAST_THICK, OnChangeCmdPsCastThick)
	ON_BN_CLICKED(IDC_TM_JGJ_SHAPE_RDO1, OnCmdJgjShapeRdo)
	ON_BN_CLICKED(IDC_TM_PS_DRIVEN, OnSelectPileDetail)
	ON_BN_CLICKED(IDC_TM_JGJ_SHAPE_RDO2, OnCmdJgjShapeRdo)
	ON_BN_CLICKED(IDC_TM_JGJ_SHAPE_RDO3, OnCmdJgjShapeRdo)
	//ON_EN_CHANGE(IDC_TM_PS_ALPHA, OnChangeCmdPsAlpha)
	ON_BN_CLICKED(IDC_TM_PS_CALC_BTN, OnCalcualteBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrNsprCalcDlg message handlers

BOOL CBndrNsprCalcDlg::OnInitDialog() 
{
	AlignControl();
	CDialogMove::OnInitDialog();

	ASSERT(m_pData);

	m_pPileDB = CDBDoc::GetDocPoint()->m_pPileDB;

	// set unit/edit //////////////////////////////
	m_elastic.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_area.SetUnitType(D_UNITSYS_BASE_AREA);
	m_length.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_stiffness.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_stiffness2.SetUnitType(D_UNITSYS_BASE_SPRING);

	m_elasticunit.SetUnitType(D_UNITSYS_BASE_ELAST);
	m_areaunit.SetUnitType(D_UNITSYS_BASE_AREA);
	m_lengthunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_stiffnessunit.SetUnitType(D_UNITSYS_BASE_SPRING);
	m_diaunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_thikunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editAlpha.SetUnitType(D_UNITSYS_NONE);

	m_editAlpha .SetTextFormat(_T("%.4g"));
	m_castThick .SetTextFormat(_T("%.4g"));
	m_castDia   .SetTextFormat(_T("%.4g"));
	m_stiffness2.SetTextFormat(_T("%.4g"));
	m_dia2      .SetTextFormat(_T("%.4g"));
	m_stiffness .SetTextFormat(_T("%.4g"));
	m_length    .SetTextFormat(_T("%.4g"));
	m_elastic   .SetTextFormat(_T("%.4g"));
	m_area      .SetTextFormat(_T("%.4g"));
	///////////////////////////////////////////////

	// 값 세팅
	Data2Dlg();

	m_bInit = TRUE;
	m_bElsMod = TRUE;
	SetCodeNameCobx();  
	SetPileTypeCobx();  
	SetDiaCobx();
	SetThickCobx();  
	SetHSectionCobx();
	SetModulusArea();

	// temp //////////////////////////////////////////
	OnCmdSupportType();
	//////////////////////////////////////////////////
	OnSelchangeCmdPsPiletype(); 
	ChangeItemByCode(m_pData->nCode);
	if(m_nCode==2) SectionDefinitionShowHide();

	UpdateData(FALSE);
	m_bElsMod = FALSE;
	m_bInit = FALSE;

#if defined(_CH) 
	GetDlgItem(IDC_TM_CODE_NAME)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_PS_CODENAME)->ShowWindow(FALSE);
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);
	ChangeItemByCode(m_nCode);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CBndrNsprCalcDlg::AlignControl()
{
	CRect rectBase1, rectBase2, rectMove1, rectMove2, rectMove3, rectMove4;
	((CBCGPStatic*)GetDlgItem(IDC_TM_PS_DIA))->GetWindowRect(rectBase1);
	((CBCGPStatic*)GetDlgItem(IDC_TM_PS_DIA2))->GetWindowRect(rectMove1);
	((CBCGPStatic*)GetDlgItem(IDC_TM_PS_CAST_DIA))->GetWindowRect(rectMove2);  
	((CBCGPStatic*)GetDlgItem(IDC_TM_JGJ_SECT_NAME_CMB))->GetWindowRect(rectMove4);  
	
	((CBCGPStatic*)GetDlgItem(IDC_TM_PS_STIFFNESS))->GetWindowRect(rectBase2);
	((CBCGPStatic*)GetDlgItem(IDC_TM_PS_STIFFNESS2))->GetWindowRect(rectMove3);

	int nMove1DistY = rectBase1.top - rectMove1.top;
	int nMove2DistY = rectBase1.top - rectMove2.top;
	int nMove3DistY = rectBase2.top - rectMove3.top;
	int nMove4DistY = rectBase1.top - rectMove4.top;

	CDlgUtil::CtrlMoveDistY(this, m_group3_1, nMove1DistY);
	CDlgUtil::CtrlMoveDistY(this, m_group3_2, nMove3DistY);
	CDlgUtil::CtrlMoveDistY(this, m_group_jgj_Hsect, nMove4DistY);
	CDlgUtil::CtrlMoveDistY(this, m_PileTypeGroupCast, nMove2DistY);

	CDlgUtil::CtrlShowHide(this, m_group2, TRUE);    // m_dia,  m_stiffness
	CDlgUtil::CtrlShowHide(this, m_group3, FALSE);   // m_dia2, m_stiffness2
	CDlgUtil::CtrlShowHide(this, m_group_jgj_Hsect, FALSE);   // m_HsectName
	CDlgUtil::CtrlShowHide(this, m_PileTypeGroupCast, FALSE);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDOK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(r);
}

void CBndrNsprCalcDlg::SetCodeNameCobx()
{
	m_codename.ResetContent();

	CArray<CString, CString&> codename;
	CArray<int, int> aCode;
	m_pPileDB->GetDBNameAndCodeList(codename, aCode);

	if (m_nCode == 0) m_nCode = 1;
	int num = codename.GetSize();
	int active = 0;
	for(int i=0; i < num; i++)
	{
		CDlgUtil::CobxAddItem(m_codename, codename[i], aCode[i]);
		if (aCode[i] == m_nCode) active = i;
	}

#if defined(_US)
	int n_PILEDB_US = 3;
	CDlgUtil::CobxSetCurSelItemData(m_codename, n_PILEDB_US);   // default select D_PILEDB_US
#else
	m_codename.SetCurSel(active);
#endif 

	UpdateData(FALSE);
}


void CBndrNsprCalcDlg::SetPileTypeCobx()
{
	m_CobxType.ResetContent();
	CArray<CString, CString&> typelist;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);

	m_pPileDB->GetPileTypeList(m_nCode, typelist);

	if(m_spiletype.IsEmpty()) m_spiletype = typelist[0];
	int num = typelist.GetSize();
	int active = 0;
	for(int i=0; i < num; i++)
	{
		m_CobxType.AddString(typelist[i]);
		if(typelist[i].Compare(m_spiletype) == 0) active = i;
	}
	m_CobxType.SetCurSel(active);

	UpdateData(FALSE);
}

void CBndrNsprCalcDlg::SetDiaCobx()
{
	m_dia.ResetContent();
	CArray<T_PILE_DB, T_PILE_DB&> data;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);

	m_pPileDB->GetPileDataList(m_nCode, m_spiletype, m_nShapeRdo, data);

	if(m_ddia == -1) m_ddia = data[0].Diameter;
	
	CString str, str1;
	int num = data.GetSize();
	int active = 0;
	for(int i=0; i < num; i++)
	{
		str.Format(_T("%g"), data[i].Diameter);
		if(m_dia.FindString(0, str) < 0)
		{
			m_dia.AddString(str);
			str1.Format(_T("%g"), m_ddia);
			if(str.Compare(str1) == 0) active = m_dia.GetCount()-1;
		}
	}

	m_dia.SetCurSel(active);
	m_dia.GetWindowText(str);
	m_ddia = _tstof(str);

	UpdateData(FALSE);
}

void CBndrNsprCalcDlg::SetThickCobx()
{
	m_thik.ResetContent();
	CArray<T_PILE_DB, T_PILE_DB&> data;
	CArray<double, double> astr;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);

	m_pPileDB->GetPileDataList(m_nCode, m_spiletype, m_nShapeRdo, data);

	int num = data.GetSize();
	int active = 0;
	CString str;

	for(int i=0; i < num; i++)
	{
		str.Format(_T("%g"), data[i].Diameter);
		double dbdiameter = _tstof(str);
		if(fabs(m_ddia-dbdiameter)<1.e-8) astr.Add(data[i].Thickness);
	}

	num = astr.GetSize();
	if(num > 0)
	{
		if(m_dthik == -1) m_dthik = astr[0];
		num = astr.GetSize();
		for(int i=0; i < num; i++)
		{
			str.Format(_T("%g"), astr[i]);
			m_thik.AddString(str);
			if(fabs(astr[i]-m_dthik)<1.e-8) active = i;
		}
	}

	m_thik.SetCurSel(active);
	if(m_dthik == -1) m_dthik = astr[active];

	UpdateData(FALSE);
}

void CBndrNsprCalcDlg::SetHSectionCobx()
{
	m_HsectName.ResetContent();
	CArray<T_PILE_DB, T_PILE_DB&> data;  
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);

	m_pPileDB->GetPileDataList(m_nCode, m_spiletype, m_nShapeRdo, data);
	
	if(m_nCode == 2 && m_nShapeRdo == 2)
	{
		CString strH, strH1, strD;
		int num = data.GetSize();
		int active = 0;
		for(int i=0; i < num; i++)
		{ 
			strH = data[i].sHName;    
			if(m_HsectName.FindString(0, data[i].sHName) < 0)
			{
				m_HsectName.AddString(strH);
				strH1 = m_sHSectName;
				if(strH.Compare(strH1) == 0) active = m_HsectName.GetCount() - 1;
			}
		}
		m_HsectName.SetCurSel(active);
		m_HsectName.GetWindowText(strH);
		m_ddia = data[active].Diameter;  
	  UpdateData(FALSE);
	}
}

void CBndrNsprCalcDlg::SetModulusArea()
{
	UpdateData();
	double diameter, thickness, dbdiameter, dbthickness;  
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);

	// cast-in-place 인경우 db가 없다. (여기서 계산, 단, 중국은 있음)
	int nNum = m_CobxType.GetCurSel();
	if(nNum < 0) return ;
	
	if ((m_nCode == 1 || m_nCode == 3) && nNum == 3) // KS, US + cast-in-place
	{
		double diaI=0., area=0.;
		
		double diaO = m_castDia.GetEditValue();
		if (diaO < 0.)
		{
			AfxMessageBox(_LS(IDS_CMD_ERR_MINUS_VALUE));
			m_castDia.SetEditUnit(0.);
			return;
		}
		if (m_nCode == 1) diaI = 0.;
		else 
		{
			double thik = m_castThick.GetEditValue();
			diaI = diaO/2. - thik;
			if (diaI < 0.)
			{
				AfxMessageBox(_LS(IDS_CMD_ERR_THICKNESS_DIA));
				m_castThick.SetEditUnit(diaO);
				return;
			}
		}
		
		area = 3.141592654 * (pow(diaO/2, 2) -  pow(diaI, 2));
		m_area.SetEditUnit(area);
		return;
	}
	/////////////////////////////////////////////////

	CString str;
	m_dia.GetLBText(m_dia.GetCurSel(), str);
	diameter = _tstof(str);
	m_thik.GetLBText(m_thik.GetCurSel(), str);
	thickness = _tstof(str);

	CArray<T_PILE_DB, T_PILE_DB&> data;

	m_pPileDB->GetPileDataList(m_nCode, m_spiletype, m_nShapeRdo, data);

	int pt=0;
	int num = data.GetSize();
	if (m_nCode == 1 || m_nCode == 3)
	{
		for(int i=0; i < num; i++)
		{
			str.Format(_T("%g"), data[i].Diameter);    
			dbdiameter = _tstof(str);
			str.Format(_T("%g"), data[i].Thickness);
			dbthickness = _tstof(str);
			if(fabs(diameter-dbdiameter)<1.e-8 && fabs(thickness-dbthickness)<1.e-8)
			{ pt = i; break; }
		}
	}
	else if (m_nCode == 2)
	{
		for(int i=0; i < num; i++)
		{
			str.Format(_T("%g"), data[i].Diameter);      
			dbdiameter = _tstof(str);
			str.Format(_T("%g"), data[i].Thickness);
			dbthickness = _tstof(str);
				
			if(m_spiletype == _LS(IDS_DB_DATA_PILE_STEEL) && m_nShapeRdo == 0)
			{      
				if(fabs(diameter-dbdiameter)<1.e-8 && fabs(thickness-dbthickness)<1.e-8)  { pt = i; break; }
			}
			else if((m_spiletype == _LS(IDS_DB_DATA_PILE_PC) && m_nShapeRdo == 1) ||               
							(m_spiletype == _LS(IDS_DB_DATA_PILE_CASE_IN_PLACE) && m_nShapeRdo == 0)) 
			{
				if(fabs(diameter-dbdiameter)<1.e-8) { pt = i; break; }
			}      
		}
		if(m_spiletype == _LS(IDS_DB_DATA_PILE_STEEL) && m_nShapeRdo == 2)
		{
			pt = m_HsectName.GetCurSel();   
			m_ddia = data[pt].Diameter;
		}
	}

	if(m_bInit)
	{
		m_elastic.SetEditUnit(m_pData->dElast);
		m_area.SetEditUnit(m_pData->dArea);
	}
	else
	{
		m_elastic.SetEditUnit(data[pt].Elast);
		m_area.SetEditUnit(data[pt].Area);
	}  
}

BOOL CBndrNsprCalcDlg::Dlg2Data()
{
	UpdateData();
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, nCode);
	CString csName, strDia, strThk;
	csName.TrimLeft(); 
	csName.TrimRight();

	m_pData->Initialize();
	m_pData->strName      = csName;
	m_pData->nCode        = nCode;
	m_pData->nType        = m_CobxType.GetCurSel() + 1;  // PC, PHC(JGJ는 없음), STEEL, Cast-in-place
	m_pData->nConstraint  = m_type + 1;                  // Driven, Augered, Pre-Boring  
	m_pData->nShape       = m_nShapeRdo + 1;             // Round, Square, H  
	m_pData->nPileType    = m_option + 1;                // Normal, Tension Only, Compression Only
	
	m_dia.GetWindowText(strDia);
	m_thik.GetWindowText(strThk);
	m_pData->dStiffness = m_stiffness.GetEditValue();
		  
	if (m_pData->nCode == 1)
	{
		if(m_pData->nType == 4) 
		{
			m_castDia.GetWindowText(strDia);
			m_castThick.GetWindowText(strThk);
		}
		m_pData->dDiameter = _tstof(strDia);      
		m_pData->dThickness = _tstof(strThk);	  
	}
	else if (m_pData->nCode == 2)
	{
		if(m_pData->nShape == 1 && (m_pData->nType == 2 || m_pData->nType == 3)) m_pData->dDiameter = _tstof(strDia);
		else if(m_pData->nShape == 2 && m_pData->nType == 1) m_pData->dDiameter = _tstof(strDia);
		else if(m_pData->nShape == 3 && m_pData->nType == 2) 
		{
			m_pData->dDiameter = m_ddia;
			m_pData->nHID = m_HsectName.GetCurSel() + 1;
		}
		if(m_pData->nShape == 1 && m_pData->nType == 2) m_pData->dThickness = _tstof(strThk);
	}
	else if (m_pData->nCode == 3)
	{
		if(m_pData->nType == 4) 
		{
			m_castDia.GetWindowText(strDia);
			m_castThick.GetWindowText(strThk);
		}
		m_pData->dDiameter = _tstof(strDia);      
		m_pData->dThickness = _tstof(strThk);
		m_pData->dAlpha = m_editAlpha.GetEditValue();
	}
	
	m_pData->dElast  = m_elastic.GetEditValue();
	m_pData->dArea   = m_area.GetEditValue   ();
	m_pData->dLength = m_length.GetEditValue ();

	return TRUE;
}

BOOL CBndrNsprCalcDlg::Data2Dlg()
{
	CString strDia, strThk;  
	m_CobxType.SetCurSel(m_pData->nType - 1);     // PC, PHC(JGJ는 없음), STEEL, Cast-in-place
	m_nShapeRdo     = m_pData->nShape - 1;        // Round, Square, H  
	m_option        = m_pData->nPileType - 1;     // Normal, Tension Only, Compression Only   
	m_HsectName.SetCurSel(m_pData->nHID - 1);     // H-Section Name(JGJ94-94인 경우)

	m_nCode     = m_pData->nCode;
	m_spiletype = m_pPileDB->GetTypeNameByCode(m_pData->nCode, m_pData->nType);  // PC, PHC, Steel, CIP        
	m_sHSectName= m_pPileDB->GetPileHName(m_pData->nHID); 
	m_dthik     = m_pData->dThickness;
	m_type      = m_pData->nConstraint - 1;
	strDia.Format(_T("%g"), m_pData->dDiameter);    
	strThk.Format(_T("%g"), m_pData->dThickness);
	m_dia.SetWindowText(strDia);            
	m_thik.SetWindowText(strThk);
	m_ddia = m_pData->dDiameter;
	m_dthik = m_pData->dThickness;
		
	if ((m_nCode == 1 || m_nCode == 3) && m_pData->nType == 4)
	{
		m_castDia.SetWindowText(strDia);		
		m_castThick.SetWindowText(strThk);      
	}    		    
	m_stiffness.SetEditUnit(m_pData->dStiffness);
	m_elastic.SetEditUnit(m_pData->dElast);
	m_area.SetEditUnit(m_pData->dArea);
	m_length.SetEditUnit(m_pData->dLength);
	m_editAlpha.SetEditUnit(m_pData->dAlpha);
	
	UpdateData(FALSE);

	return TRUE;
}


void CBndrNsprCalcDlg::OnOK() 
{
	if(!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CBndrNsprCalcDlg::OnCmdSupportType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int num = m_CobxType.GetCurSel();
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, nCode);

	CDlgUtil::CtrlEnableDisable(this, m_group1, TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_group_jgj_shape_rdo3, TRUE);
	CDlgUtil::CtrlShowHide(this, m_group2, TRUE);    
	CDlgUtil::CtrlShowHide(this, m_group3, FALSE);  
	CDlgUtil::CtrlEnableDisable(this, m_group_stiffness, nCode == 2);
	SectionDefinitionShowHide();
	if((nCode == 1 && num == 3) || (nCode == 2 && num == 2) || (nCode == 3 && num == 3))
	{
		CDlgUtil::CtrlShowHide(this, m_PileTypeGroupNormal, FALSE);
		CDlgUtil::CtrlShowHide(this, m_PileTypeGroupCast, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_PileTypeGroupCast, TRUE);
	}
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlAlpha, TRUE);
	OnSelchangeCmdPsPiletype(); 
	
	ChangePicture();
	UpdateData(FALSE);
}

void CBndrNsprCalcDlg::OnSelchangeCmdPsCodename()   
{
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);

	SetPileTypeCobx();
	SectionDefinitionShowHide();
	OnSelchangeCmdPsPiletype(); 
	SetDiaCobx();
	SetThickCobx();
	SetHSectionCobx();
	ChangePicture();    

	// 추가 
	ChangeItemByCode(m_nCode);
}

void CBndrNsprCalcDlg::ChangeItemByCode(int nCode)
{
	BOOL bCodeUS = FALSE;
	if (nCode == 3)
	{
		bCodeUS = TRUE;
	}

	if (bCodeUS) m_castThick.EnableWindow(TRUE);

	CDlgUtil::CtrlShowHide(this, m_aCtrlAlpha, bCodeUS);
	ChangeAlpha();
}

void CBndrNsprCalcDlg::ChangeAlpha()
{
	UpdateData(TRUE);

	double dAlpha = 0.;
	double dLeng, dDia;

	CString csDia(_T(""));
	int nPileType = m_CobxType.GetCurSel();
	if (nPileType == 3) m_castDia.GetWindowText(csDia);
	else 
	{
		int ix = m_dia.GetCurSel();
		if (ix != CB_ERR) m_dia.GetLBText(ix, csDia);
	}
	if (csDia.IsEmpty()) return;

	dDia = _tstof(csDia);
	dLeng = m_length.GetEditValue();
	
	if (dDia != 0.)
	{
		int nPileType = m_CobxType.GetCurSel();
		switch(nPileType)
		{
		case 0: // PC, PHC
		case 1: 
			if (m_type == 0) dAlpha = 0.013 * (dLeng/dDia) + 0.61;       // 타입 PC, PHC 말뚝 
			else if (m_type == 1) dAlpha = 0.011 * (dLeng/dDia) + 0.36;  // 내부굴착  PC, PHC 말뚝 
			else ASSERT(0);
			break;
		case 2: // Steel
			if (m_type == 0) dAlpha = 0.014 * (dLeng/dDia) + 0.78;       // 타입 PC, PHC 말뚝 
			else if (m_type == 1) dAlpha = 0.009 * (dLeng/dDia) + 0.39;  // 내부굴착  PC, PHC 말뚝 
			else ASSERT(0);
			break;
		case 3: // Cast-in-place  
			dAlpha = 0.031 * (dLeng/dDia) - 0.15;                        // 현장타설말뚝 
			break;
		default: 
			ASSERT(0);
			break;
		}
	}

	m_editAlpha.SetEditUnit(dAlpha);
}

void CBndrNsprCalcDlg::OnCmdJgjShapeRdo()
{
	UpdateData();  

		SectionDefinitionShowHide();   
		SetDiaCobx();       //2
		SetThickCobx();     //2 
		SetHSectionCobx();
		SetModulusArea();
}

void CBndrNsprCalcDlg::SectionDefinitionShowHide()
{
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, nCode);
	int nPileType = m_CobxType.GetCurSel();
		CDlgUtil::CtrlShowHide(this, m_group_jgj_Hsect, nCode == 2 && m_nShapeRdo == 2);
		CDlgUtil::CtrlShowHide(this, m_group_sect, nCode == 1 || nCode == 3 ||
																							 (nCode == 2 && m_nShapeRdo != 2));        
		CDlgUtil::CtrlShowHide(this, m_ps_dia_cmb, (nCode == 1 && nPileType != 3) ||
																							 (nCode == 3 && nPileType != 3) ||
																							 (nCode == 2 && m_nShapeRdo != 2));    
		CDlgUtil::CtrlEnableDisable(this, m_group_stiffness, nCode == 2);
}

void CBndrNsprCalcDlg::ChangePicture()
{
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);
	if (m_nCode == 1 || m_nCode == 3)
	{
		// 0
		m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\suppile_JGJ.svg"));
	}
	else if (m_nCode == 2)
	{
		// 1
		// 기존 코드가 일단 같은 이미지를 출력.
		m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\suppile_JGJ.svg"));
	}
}

void CBndrNsprCalcDlg::OnSelchangeCmdPsPiletype() 
{
	UpdateData();
	//CString strCodeName;
	//m_codename.GetWindowText(strCodeName);    // KS, JGJ94-94
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, nCode);

	int num = m_CobxType.GetCurSel();
	CButton* btn1 = (CButton*)GetDlgItem(IDC_TM_PS_DRIVEN);
	CButton* btn2 = (CButton*)GetDlgItem(IDC_TM_PS_AUGERED);
	CButton* ShapeBtn1 = (CButton*)GetDlgItem(IDC_TM_JGJ_SHAPE_RDO1);
	CButton* ShapeBtn2 = (CButton*)GetDlgItem(IDC_TM_JGJ_SHAPE_RDO2);
	CButton* ShapeBtn3 = (CButton*)GetDlgItem(IDC_TM_JGJ_SHAPE_RDO3);
	CWnd   * ed_Dia = GetDlgItem(IDC_TM_PS_DIA);
	CWnd   * ed_Thk = GetDlgItem(IDC_TM_PS_THIK);
	CWnd   * ed_Dia_cast = GetDlgItem(IDC_TM_PS_CAST_DIA);  
	CWnd   * ed_Thk_cast = GetDlgItem(IDC_TM_PS_CAST_THICK);  
	CWnd   * ed_elastic  = GetDlgItem(IDC_TM_PS_ELASTIC);
	
		if (nCode == 1 || nCode == 3)
		{
			CDlgUtil::CtrlShowHide(this, m_PileTypeGroupNormal, num != 3);
		  CDlgUtil::CtrlShowHide(this, m_PileTypeGroupCast, num == 3);
			m_nShapeRdo = 0;
			ShapeBtn1->SetCheck(TRUE); 
			ShapeBtn2->SetCheck(FALSE);  
			ShapeBtn3->SetCheck(FALSE);         
			ShapeBtn1->EnableWindow(TRUE);
			ShapeBtn2->EnableWindow(FALSE);
			ShapeBtn3->EnableWindow(FALSE);        
				
			if(num == 0) // PC
			{
				btn1->EnableWindow(TRUE);
				btn2->EnableWindow(TRUE);
				ed_Dia->EnableWindow(TRUE);
				ed_Thk->EnableWindow(TRUE);
			}
			else if(num == 1) // PHC
			{
				btn1->EnableWindow(TRUE);
				btn2->EnableWindow(TRUE);
				ed_Dia->EnableWindow(TRUE);
				ed_Thk->EnableWindow(TRUE);
			}
			else if(num == 2) // Steel
			{      
				btn1->EnableWindow(TRUE);
				btn2->EnableWindow(TRUE);
				ed_Dia->EnableWindow(TRUE);
				ed_Thk->EnableWindow(TRUE);
			}
			else if(num == 3) // cast-in-place
			{
				btn1->EnableWindow(FALSE);
				btn2->EnableWindow(FALSE);
				ed_Dia->EnableWindow(FALSE);
				ed_Thk->EnableWindow(FALSE);
				ed_Thk_cast->EnableWindow(nCode == 3);		
				ed_elastic->SetWindowText(_T("0"));
			}

			m_CobxType.GetWindowText(m_spiletype);

			if(num != 3 && !m_bInit)
			{
				m_ddia = -1;
				SetDiaCobx();
				SetThickCobx();
			}
			SetModulusArea();

			if (nCode == 3) ChangeAlpha();
		}
		else if(nCode == 2)
		{    
			CDlgUtil::CtrlShowHide(this, m_PileTypeGroupNormal, TRUE);
		  CDlgUtil::CtrlShowHide(this, m_PileTypeGroupCast, FALSE);
			if(num == 0) // PC
			{
				if(ShapeBtn1->GetCheck() || ShapeBtn3->GetCheck()) 
				{
					m_nShapeRdo = 1;
					ShapeBtn1->SetCheck(FALSE); 
					ShapeBtn2->SetCheck(TRUE);  
					ShapeBtn3->SetCheck(FALSE);         
				}
				ShapeBtn1->EnableWindow(FALSE);
				ShapeBtn2->EnableWindow(TRUE);
				ShapeBtn3->EnableWindow(FALSE);        
				btn1->EnableWindow(TRUE);   // Driven
				btn2->EnableWindow(TRUE);   // Augered
				ed_Dia->EnableWindow(TRUE);  
				ed_Thk->EnableWindow(FALSE); 		  
			}
			else if(num == 1) // Steel
			{
				if(ShapeBtn2->GetCheck()) 
				{
					m_nShapeRdo = 0;
					ShapeBtn1->SetCheck(TRUE);        
					ShapeBtn2->SetCheck(FALSE);           
				}
				ShapeBtn1->EnableWindow(TRUE);
				ShapeBtn2->EnableWindow(FALSE);
				ShapeBtn3->EnableWindow(TRUE);        
				btn1->EnableWindow(TRUE);
				btn2->EnableWindow(TRUE);
				ed_Dia->EnableWindow(TRUE);  
				ed_Thk->EnableWindow(TRUE); 		  
			}
			else if(num == 2) // cast-in-place
			{
				if(ShapeBtn2->GetCheck() || ShapeBtn3->GetCheck()) 
				{
					m_nShapeRdo = 0;
					ShapeBtn1->SetCheck(TRUE);
					ShapeBtn2->SetCheck(FALSE);
					ShapeBtn3->SetCheck(FALSE);          
				}      
				ShapeBtn1->EnableWindow(TRUE);
				ShapeBtn2->EnableWindow(FALSE);
				ShapeBtn3->EnableWindow(FALSE);        
				btn1->EnableWindow(FALSE);
				btn2->EnableWindow(FALSE);
				ed_Dia->EnableWindow(TRUE);  
				ed_Thk->EnableWindow(FALSE); 	
				ed_elastic->SetWindowText(_T("0"));
			}

			m_CobxType.GetWindowText(m_spiletype);  

			if(!m_bInit)
			{
				m_ddia = -1;
				OnCmdJgjShapeRdo();
			}      
		}   

}

void CBndrNsprCalcDlg::OnSelchangeCmdPsDia() 
{
	UpdateData();
	CString str;
	m_dia.GetLBText(m_dia.GetCurSel(), str);
	m_ddia = _tstof(str);
	SetThickCobx();
	SetModulusArea();
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnSelchangeCmdHSectName()
{
	UpdateData();
	CString str;  
	m_HsectName.GetLBText(m_HsectName.GetCurSel(), m_sHSectName);    
	SetModulusArea();
}

void CBndrNsprCalcDlg::OnSelchangeCmdPsThik() 
{
	// TODO: Add your control notification handler code here
	SetModulusArea();
}

void CBndrNsprCalcDlg::OnChangeCmdPsElastic() 
{
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnChangeCmdPsArea() 
{
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnChangeCmdPsLength() 
{
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnEditchangeCmdPsDia() 
{
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnEditchangeCmdPsThik() 
{
	SetModulusArea();
}

void CBndrNsprCalcDlg::CalcStiffness()
{
	UpdateData();
	CDlgUtil::CobxGetItemDataByCurSel(m_codename, m_nCode);
	CString str;

	m_pData->nCode = m_nCode;
	m_pData->nPileType = m_option + 1;
	m_pData->nType = m_CobxType.GetCurSel() + 1;
	m_pData->nConstraint = m_type + 1;
	m_pData->nShape = m_nShapeRdo + 1;

	m_dia.GetWindowText(str);
	if(m_pData->nCode == 1 && m_pData->nType == 4) m_castDia.GetWindowText(str); // CIP  
	if(m_pData->nCode == 3 && m_pData->nType == 4) m_castDia.GetWindowText(str); // CIP  
	m_pData->dDiameter = _tstof(str);
	
	if(m_pData->nCode == 3 && m_pData->nType == 4) m_castThick.GetWindowText(str);
	else m_thik.GetWindowText(str);
	m_pData->dThickness = _tstof(str);
	
	if(m_pData->nCode == 3)
	{
		m_editAlpha.GetWindowText(str);
		m_pData->dAlpha = _tstof(str);
	}

	m_pData->dElast   = m_elastic.GetEditValue();
	m_pData->dArea    = m_area.GetEditValue();
	m_pData->dLength  = m_length.GetEditValue();

	if(CalcSpring(m_pData)) m_stiffness.SetEditUnit(m_pData->dStiffness, 3);
	else m_stiffness.SetEditUnit(0.);

	m_pData->dStiffness = m_stiffness.GetEditValue();
}

void CBndrNsprCalcDlg::OnSelectPileDetail() 
{
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnChangeCmdPsCastDia() 
{
	SetModulusArea();
	ChangeAlpha();
}

void CBndrNsprCalcDlg::OnChangeCmdPsCastThick() 
{
	SetModulusArea();
}

void CBndrNsprCalcDlg::OnChangeCmdPsAlpha() 
{

}

void CBndrNsprCalcDlg::OnCalcualteBtn() 
{
	CalcStiffness();
}

BOOL CBndrNsprCalcDlg::CalcSpring(T_PISD_DATA* pData)
{
	if(pData->nSupportType != 1)return TRUE;
	double D = pData->dDiameter;
	double L = pData->dLength;  
	double alp = 0.;

	if(pData->nCode == 1)
	{    
		D = pData->dDiameter;        
		if(L == 0. || D == 0.)return FALSE;
		if(pData->nType == 1 || pData->nType == 2)  // PC, PHC
		{
			if(pData->nConstraint == 1)alp=0.013*(L/D)+0.61;
			if(pData->nConstraint == 2)alp=0.009*(L/D)+0.39;
			if(pData->nConstraint == 3)alp=0.009*(L/D)+0.81;
		}
		else if(pData->nType == 3)  // Steel
		{
			if(pData->nConstraint == 1)alp=0.014*(L/D)+0.78;
			if(pData->nConstraint == 2)alp=0.009*(L/D)+0.39;
		}
		else if(pData->nType == 4)  // Cast-in-place
		{
			alp=0.031*(L/D)-0.15;
			if(alp < 0) alp = 0.;
		}
	}
	else if(pData->nCode == 2)
	{     
		if(L == 0. || D == 0.)return FALSE;
		if(pData->nType == 1 && pData->nShape == 2)  // PC && Square
		{
			switch(pData->nConstraint)
			{
			case 1: alp=0.013*(L/D)+0.61; break;
			case 2: alp=0.009*(L/D)+0.39; break;
			case 3: alp=0.009*(L/D)+0.81; break;
			}
		}
		else if(pData->nType == 2 && (pData->nShape == 1 || pData->nShape == 3))  // Steel && (Round || H)
		{
			switch(pData->nConstraint)
			{
			case 1: alp=0.014*(L/D)+0.78; break;
			case 2: alp=0.009*(L/D)+0.39; break;        
			}
		}
		else if(pData->nType == 3 && pData->nShape == 1)  // Cast-in-place && Round
		{
			alp=0.031*(L/D)-0.15;
			if(alp < 0) alp = 0.;
		}
	}
	else if (pData->nCode == 3) // US
	{
		alp = pData->dAlpha;
	}

	pData->dStiffness=alp*pData->dArea*pData->dElast/L;
	if(alp == 0) pData->dStiffness = 1.0e-6;
	return TRUE;
}

void CBndrNsprCalcDlg::SetSpringType(int nType)
{
	m_option = nType;
}

double CBndrNsprCalcDlg::GetCalcValue()
{
	return m_pData->dStiffness;
}
