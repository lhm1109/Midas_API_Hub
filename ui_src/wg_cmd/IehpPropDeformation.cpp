#include "stdafx.h"
#include "IehpPropDeformation.h"

CIehpPropDeformation::CIehpPropDeformation(int nSymmetry, int nModelType, int nDeformDefineType, int nDof, T_IEHP_PROP* PROP, CWnd* pParent)
	: CDialogMove(CIehpPropDeformation::IDD, pParent)
{
	m_nSymmetry = nSymmetry;
	m_nModelType = nModelType;
	m_nDeformDefineType = nDeformDefineType;
	m_bCompOnly = (nModelType==32 || nModelType==35);
	m_nDof = nDof;
	if(PROP != NULL)
		m_pPROP = PROP;
}

void CIehpPropDeformation::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_IEHP_DFOM_RDO_C, m_nDeformDefineType);

	UINT nDeformCapacity[2][5] = {{IDC_IEHP_LEVEL_1PLUS, IDC_IEHP_LEVEL_2PLUS, IDC_IEHP_LEVEL_3PLUS,
		IDC_IEHP_LEVEL_4PLUS, IDC_IEHP_LEVEL_5PLUS},
	{IDC_IEHP_LEVEL_1MNUS, IDC_IEHP_LEVEL_2MNUS, IDC_IEHP_LEVEL_3MNUS,
	IDC_IEHP_LEVEL_4MNUS, IDC_IEHP_LEVEL_5MNUS}};
	for(int i=0; i<2; i++) for(int j=0; j<5; j++)
		DDX_Control(pDX, nDeformCapacity[i][j], m_edtDeformCapacity[i][j]);
}

BEGIN_MESSAGE_MAP(CIehpPropDeformation, CDialogMove)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_1PLUS,        OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_2PLUS,        OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_3PLUS,        OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_4PLUS,        OnChangeDefomCapacity)
	ON_EN_CHANGE(IDC_IEHP_LEVEL_5PLUS,        OnChangeDefomCapacity)
END_MESSAGE_MAP()

BOOL CIehpPropDeformation::OnInitDialog()
{
		CDialogMove::OnInitDialog();

		GetDlgItem(IDC_IEHP_LEVEL_1PLUS)->EnableWindow(!m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_2PLUS)->EnableWindow(!m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_3PLUS)->EnableWindow(!m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_4PLUS)->EnableWindow(!m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_5PLUS)->EnableWindow(!m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_1MNUS)->EnableWindow(m_nSymmetry == 1 || m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_2MNUS)->EnableWindow(m_nSymmetry == 1 || m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_3MNUS)->EnableWindow(m_nSymmetry == 1 || m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_4MNUS)->EnableWindow(m_nSymmetry == 1 || m_bCompOnly);
		GetDlgItem(IDC_IEHP_LEVEL_5MNUS)->EnableWindow(m_nSymmetry == 1 || m_bCompOnly);

		Data2Dlg();

		return TRUE;
}

void CIehpPropDeformation::OnChangeDefomCapacity()
{
	// 31: SLIP Bilinear/Tensile, 32: SLIP Bilinear/Compressive, 34: SLIP Trilinear/Tensile, 35: SLIP Trilinear/Compressive
	// 31, 32, 34, 35는 symmetry와 관계없음.
	if(m_nModelType != 31 && m_nModelType != 32 && m_nModelType != 34 && m_nModelType != 35)
	{
		UpdateData();
		if(m_nSymmetry == 0)
		{
			for(int i=0; i<5; i++)
				m_edtDeformCapacity[1][i].SetEditUnit(m_edtDeformCapacity[0][i].GetEditValue());
		}
	}
	UpdateData(FALSE);
}


void CIehpPropDeformation::OnOK()
{
	UpdateData(TRUE);

	for(int i=0; i<2; i++) for(int j=0; j<5; j++)
	{
		if(fabs(m_edtDeformCapacity[i][j].GetEditValue())<0.) return GSaveHistoryFormatNF(_LS(IDS_DB_DEFORMATION_CAPACITY));
		if(j>0) if(fabs(m_edtDeformCapacity[i][j-1].GetEditValue()) >= fabs(m_edtDeformCapacity[i][j].GetEditValue())) 
			 return GSaveHistoryFormatNF(_LS(IDS_DB_DEFORMATION_CAPACITY_COM));
	}

	if(!Dlg2Data()) return;
	CDialogMove::OnOK();
}

void CIehpPropDeformation::OnCancel()
{
	//IDCANCEL
	CDialogMove::OnCancel();
}

void CIehpPropDeformation::Data2Dlg()
{
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;
	
	switch(m_nModelType)
	{
		case 0: case 7: // 0:Kinematic Hardening, 7:비선형탄성형(대칭)
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].KINEMA.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].KINEMA.nDeformDefineType;
			break;
		case 1:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].ORIGIN.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].ORIGIN.nDeformDefineType;
			break;
		case 2:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].PEAK.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].PEAK.nDeformDefineType;
			break;
		case 3:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].CLOUGH.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].CLOUGH.nDeformDefineType;
			break;
		case 4:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].DEGRAD.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].DEGRAD.nDeformDefineType;
			break;
		case 5: case 6: case 8: case 9: // 5: Takeda, 6: Modified Takeda, 8: 비대칭역향형, 9:비선형탄성형(비대칭)
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].TAKEDA.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].TAKEDA.nDeformDefineType;
			break;
		case 10:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].NORBIL.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].NORBIL.nDeformDefineType;
			break;
		case 11:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].ELABIL.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].ELABIL.nDeformDefineType;
			break;
		case 12:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].ELATRI.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].ELATRI.nDeformDefineType;
			break;
		case 13:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].ELATET.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].ELATET.nDeformDefineType;
			break;
		case 14: case 15: // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].TAKTET.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].TAKTET.nDeformDefineType;
			break;
		case 92: // 92:SRC Tetralinear
			for (int i = 0; i < 2; i++) for (int j = 0; j < 5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].SRCTET.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].SRCTET.nDeformDefineType;
			break;
		case 30: case 31: case 32:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].SLIP.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].SLIP.nDeformDefineType;
			break;
		case 33: case 34: case 35:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].SLIP.dDeformCapacity[i][j]);
			m_nDeformDefineType = m_pPROP[nD].SLIP.nDeformDefineType;
			break;
		case D_IEHP_AXIAL:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].AXIAL.dDeformCapacity[i][j]);
			break;
		case D_IEHP_PMMC:
			for(int i=0; i<2; i++) for(int j=0; j<5; j++)
				m_edtDeformCapacity[i][j].SetEditUnit(m_pPROP[nD].AXIAL.dDeformCapacity[i][j]); // 임시저장장소임.
			break;

	}

	UpdateData(FALSE);
}

BOOL CIehpPropDeformation::Dlg2Data()
{
	int nD = m_nDof;
	if(m_nDof>5) nD = m_nDof-3;

	switch(m_nModelType)
	{
	case 0: case 7: // 0:Kinematic Hardening, 7:비선형탄성형(대칭)
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].KINEMA.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].KINEMA.nDeformDefineType = m_nDeformDefineType;
		break;
	case 1:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].ORIGIN.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].ORIGIN.nDeformDefineType = m_nDeformDefineType;
		break;
	case 2:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].PEAK.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].PEAK.nDeformDefineType = m_nDeformDefineType;
		break;
	case 3:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].CLOUGH.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].CLOUGH.nDeformDefineType = m_nDeformDefineType;
		break;
	case 4:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].DEGRAD.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].DEGRAD.nDeformDefineType = m_nDeformDefineType;
		break;
	case 5: case 6: case 8: case 9: // 5: Takeda, 6: Modified Takeda, 8: 비대칭역향형, 9:비선형탄성형(비대칭)
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].TAKEDA.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].TAKEDA.nDeformDefineType = m_nDeformDefineType;
		break;
	case 10:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].NORBIL.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].NORBIL.nDeformDefineType = m_nDeformDefineType;
		break;
	case 11:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].ELABIL.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].ELABIL.nDeformDefineType = m_nDeformDefineType;
		break;
	case 12:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].ELATRI.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].ELATRI.nDeformDefineType = m_nDeformDefineType;
		break;
	case 13:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].ELATET.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].ELATET.nDeformDefineType = m_nDeformDefineType;
		break;
	case 14: case 15: // 14:Takeda Tetralinear, 15:Modified Takeda Tetralinear
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].TAKTET.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].TAKTET.nDeformDefineType = m_nDeformDefineType;
		break;
	case 92: // 92:SRC Tetralinear
		for (int i = 0; i < 2; i++) for (int j = 0; j < 5; j++)
			m_pPROP[nD].SRCTET.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].SRCTET.nDeformDefineType = m_nDeformDefineType;
		break;
	case 30: case 31: case 32:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].SLIP.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].SLIP.nDeformDefineType = m_nDeformDefineType;
		break;
	case 33: case 34: case 35:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].SLIP.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		m_pPROP[nD].SLIP.nDeformDefineType = m_nDeformDefineType;
		break;
	case D_IEHP_AXIAL:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].AXIAL.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		break;
	case D_IEHP_PMMC:
		for(int i=0; i<2; i++) for(int j=0; j<5; j++)
			m_pPROP[nD].AXIAL.dDeformCapacity[i][j] = m_edtDeformCapacity[i][j].GetEditValue();
		break;
	}
	return TRUE;
}