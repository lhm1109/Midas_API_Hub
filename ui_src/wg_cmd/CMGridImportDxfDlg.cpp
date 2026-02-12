// CMGridImportDxfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridImportDxfDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ReadDxfUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGridImportDxfDlg dialog


CCMGridImportDxfDlg::CCMGridImportDxfDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMGridImportDxfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGridImportDxfDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aAllLayers.RemoveAll();
	m_aSelLayers.RemoveAll();

	m_aCtrlBtn.Add(IDC_APPLY);
	m_aCtrlBtn.Add(IDOK);
}


void CCMGridImportDxfDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridImportDxfDlg)
	DDX_Control(pDX, IDC_LIST_ALL_LAYERS, m_listAllLayers);
	DDX_Control(pDX, IDC_LIST_SEL_LAYERS, m_listSelLayers);
	DDX_Control(pDX, IDC_EDIT_FILE_NAME, m_ctrlFileName);
	DDX_Control(pDX, IDC_CMD_IMPORT_NAME_EDT, m_ctrlName);
	DDX_Control(pDX, IDC_EDIT_ORIGIN_POINT, m_ctrlOriginPoint);
	DDX_Control(pDX, IDC_EDIT_SCALE_FACTOR, m_ctrlScaleFactor);
	DDX_Control(pDX, IDC_COMBO_ROTATION_Z, m_ctrlRotationZ);
	DDX_Control(pDX, IDC_COMBO_ROTATION_Y, m_ctrlRotationY);
	DDX_Control(pDX, IDC_COMBO_ROTATION_X, m_ctrlRotationX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridImportDxfDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMGridImportDxfDlg)
	ON_LBN_DBLCLK(IDC_LIST_ALL_LAYERS, OnDoubleclickedAllList)
	ON_LBN_DBLCLK(IDC_LIST_SEL_LAYERS, OnDoubleclickedSelList)
	ON_BN_CLICKED(IDC_BUTTON_SEL, OnButtonSelLayers)
	ON_BN_CLICKED(IDC_BUTTON_UNSEL, OnButtonUnSelLayers)
	ON_BN_CLICKED(IDC_BUTTON_DXF_BROWSE, OnButtonDxfBrowse)
	ON_EN_CHANGE(IDC_EDIT_FILE_NAME, OnChangeEditFileName)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridImportDxfDlg message handlers

BOOL CCMGridImportDxfDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBtn, FALSE);

	m_ctrlRotationX.AddString(_ULS(0));
	m_ctrlRotationX.AddString(_ULS(45));
	m_ctrlRotationX.AddString(_ULS(90));
	m_ctrlRotationX.SetCurSel(0);
	
	m_ctrlRotationY.AddString(_ULS(0));
	m_ctrlRotationY.AddString(_ULS(45));
	m_ctrlRotationY.AddString(_ULS(90));
	m_ctrlRotationY.SetCurSel(0);
	
	m_ctrlRotationZ.AddString(_ULS(0));
	m_ctrlRotationZ.AddString(_ULS(45));
	m_ctrlRotationZ.AddString(_ULS(90));
	m_ctrlRotationZ.SetCurSel(0);

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_EDIT_SCALE_FACTOR);
	pWnd->SetWindowText(_ULS(1));
	
	pWnd = GetDlgItem(IDC_EDIT_ORIGIN_POINT);
	pWnd->SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));

	m_ctrlOriginPoint.SetAttUcsPos();
	m_ctrlOriginPoint.SetModeToUse(MOUSEEDIT_USE_GET_SWPOSITION_DATA);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMGridImportDxfDlg::OnButtonDxfBrowse()
{
	CString csFileName;
	CFileDialog fd(TRUE, _T(".dxf"), NULL,
								 OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
								 _LS(IDS_CMD_DXF_IMPORT_FILTER), NULL);
	if(fd.DoModal() != IDOK) return;

	m_aAllLayers.RemoveAll();
	m_aSelLayers.RemoveAll();
	m_listAllLayers.ResetContent();
	m_listSelLayers.ResetContent();

	DXF_file_name = fd.GetFileName();
	DXF_file_name = fd.GetPathName();

	m_ctrlFileName.SetWindowText(DXF_file_name);
	m_ctrlFileName.SetFocus();
	m_ctrlFileName.SetSel(0, -1);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBtn, TRUE);

	// Read DXF Header & Fill Layer List
	CWaitCursor wait;
	
	CReadDxfUtil dxf(DXF_file_name);
	if(dxf.ReadDxfLayerInfo(m_aAllLayers))
	{
		SetAllLayerList();
	}
}

void CCMGridImportDxfDlg::OnChangeEditFileName()
{
	CString csFileName;
	m_ctrlFileName.GetWindowText(csFileName);
	csFileName.TrimRight();

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBtn, !csFileName.IsEmpty());
}

void CCMGridImportDxfDlg::OnButtonSelLayers()
{
	MoveAlltoSelList();
}

void CCMGridImportDxfDlg::OnButtonUnSelLayers()
{
	MoveSeltoAllList();
}

void CCMGridImportDxfDlg::OnDoubleclickedAllList()
{
	MoveAlltoSelList();
}

void CCMGridImportDxfDlg::OnDoubleclickedSelList()
{
	MoveSeltoAllList();
}

void CCMGridImportDxfDlg::SetAllLayerList()
{
	m_listAllLayers.ResetContent();

	int nIndex;
	int nCount = m_aAllLayers.GetSize();
	for(int i=0; i<nCount; i++)
	{
		nIndex = m_listAllLayers.AddString(m_aAllLayers[i]);
		if(nIndex != LB_ERR) m_listAllLayers.SetItemData(nIndex, i);
	}

	m_listAllLayers.SetCurSel(0);
}

void CCMGridImportDxfDlg::MoveAlltoSelList()
{
	int nCount = m_listAllLayers.GetSelCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	}

	CArray<int, int> aSelItems;
	aSelItems.SetSize(nCount);
	m_listAllLayers.GetSelItems(nCount, aSelItems.GetData());

	UINT nData;
	for(int i=0; i<nCount; i++)
	{
		nData = m_listAllLayers.GetItemData(aSelItems[i]);
		int nIndex = m_listSelLayers.AddString(m_aAllLayers[nData]);
		if(nIndex != LB_ERR) m_listSelLayers.SetItemData(nIndex, nData);
	}

	for(int i=nCount-1; i>=0; i--)
		m_listAllLayers.DeleteString(aSelItems[i]);
}

void CCMGridImportDxfDlg::MoveSeltoAllList()
{
	int nCount = m_listSelLayers.GetSelCount();
	if(nCount <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_Selcted_Item));
		return;
	}

	CArray<int, int> aSelItems;
	aSelItems.SetSize(nCount);
	m_listSelLayers.GetSelItems(nCount, aSelItems.GetData());

	UINT nData;
	for(int i=0; i<nCount; i++)
	{
		nData = m_listSelLayers.GetItemData(aSelItems[i]);
		int nIndex = m_listAllLayers.AddString(m_aAllLayers[nData]);
		if(nIndex != LB_ERR) m_listAllLayers.SetItemData(nIndex, nData);
	}

	for(int i=nCount-1; i>=0; i--)
		m_listSelLayers.DeleteString(aSelItems[i]);	
}

void CCMGridImportDxfDlg::OnApply()
{
	if (!Execute()) return;
}

void CCMGridImportDxfDlg::OnOK() 
{
	if (!Execute()) return;

	CCMDlgBase::OnOK();
}

BOOL CCMGridImportDxfDlg::Execute()
{
	if (!Dlg2Data()) return FALSE;
	if (!ImportDXF()) return FALSE;
	return TRUE;
}

BOOL CCMGridImportDxfDlg::Dlg2Data()
{
	DXF_file_name.Empty();
	m_ctrlFileName.GetWindowText(DXF_file_name);
	DXF_file_name.TrimRight();
	DXF_file_name.TrimLeft();
	if(DXF_file_name.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORT_DXF_No_File));
		return FALSE;
	}

	// Layers
	int nData;
	m_aSelLayers.RemoveAll();
	int nSel = m_listSelLayers.GetCount();
	for(int i=0; i<nSel; i++)
	{
		nData = m_listSelLayers.GetItemData(i);
		m_aSelLayers.Add(m_aAllLayers[nData]);
	}

	// Load Line Name
	strLoadLineName.Empty();
	m_ctrlName.GetWindowText(strLoadLineName);
	if (strLoadLineName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORTDXF_NAMEERROR));
		return FALSE;
	}

	// Origin Position
	double ux, uy, uz;
	CFormulaEdit::GetPosition(&m_ctrlOriginPoint, ux, uy, uz);
	m_ctrlOriginPoint.GetWcsPos(ux, uy, uz, m_dOrgX, m_dOrgY, m_dOrgZ);
	
	// Scale Factor
	CString csScale;
	m_ctrlScaleFactor.GetWindowText(csScale);
	m_dScaleFactor = _tstof(csScale);
	 
	// Rotate && Intersect
	CString csText;
	m_ctrlRotationX.GetWindowText(csText);
	m_aRotateAngle[0] = _tstof(csText);
	m_ctrlRotationY.GetWindowText(csText);
	m_aRotateAngle[1] = _tstof(csText);
	m_ctrlRotationZ.GetWindowText(csText);
	m_aRotateAngle[2] = _tstof(csText);

	return TRUE;
}

BOOL CCMGridImportDxfDlg::ImportDXF()
{
	CWaitCursor wait;
	
	CReadDxfUtil DXF_file(DXF_file_name);
	if (!DXF_file.CreatePolyline())
	{
		AfxMessageBox(_LS(IDS_CMD_IMPORTDXF_READERROR));
		return FALSE;
	}

	int nCreateIndex = 1;
	CString strName;
	int nPolySize;
	int nPointSize;
	int nSeq = m_pDoc->m_pAttrCtrl->GetCountLdln() + 1;

	T_LDLN_D LdlnD;
	T_LDLN_BASE LdlnBase;
	int nLayer = DXF_file.m_arDxfLayer.GetSize();
	for (int i=0; i<nLayer; i++)
	{
		if (!IsSelectedLayer(DXF_file.m_arDxfLayer[i].sLayerName)) continue;
		
		//Load Line 생성
		nPolySize = DXF_file.m_arDxfLayer[i].arPolyline.GetSize();
		for (int j=0; j<nPolySize; j++)
		{
			LdlnD.Initialize();
			strName.Format(_T("%s%d"), strLoadLineName, nCreateIndex);
			while(m_pDoc->m_pAttrCtrl->ExistLdln(strName))
			{
				nCreateIndex++;
				strName.Format(_T("%s%d"), strLoadLineName, nCreateIndex);
			}
			LdlnD.Name = strName;
			LdlnD.nSeq = nSeq;

			nPointSize = DXF_file.m_arDxfLayer[i].arPolyline[j].arNode.GetSize();
			for (int k=0; k<nPointSize; k++)
			{
				//Scale
				LdlnBase.x = DXF_file.m_arDxfLayer[i].arPolyline[j].arNode[k].x * m_dScaleFactor;
				LdlnBase.y = DXF_file.m_arDxfLayer[i].arPolyline[j].arNode[k].y * m_dScaleFactor;
				LdlnBase.z = DXF_file.m_arDxfLayer[i].arPolyline[j].arNode[k].z * m_dScaleFactor;

				//Rotate
				CMathFunc::mathRotateX(m_aRotateAngle[0], LdlnBase.x, LdlnBase.y, LdlnBase.z);
				CMathFunc::mathRotateY(m_aRotateAngle[1], LdlnBase.x, LdlnBase.y, LdlnBase.z);
				CMathFunc::mathRotateZ(m_aRotateAngle[2], LdlnBase.x, LdlnBase.y, LdlnBase.z);

				//Origin 이동
				LdlnBase.x+=m_dOrgX;
				LdlnBase.y+=m_dOrgY;
				LdlnBase.z+=m_dOrgZ;

				LdlnD.aData.Add(LdlnBase);
			}
			if (m_pDoc->m_pDataCtrl->AddLdln(LdlnD))
				nSeq++;
		}
	}
	return TRUE;
}

BOOL CCMGridImportDxfDlg::IsSelectedLayer(CString &csLayer)
{
	int nNum = m_aSelLayers.GetSize();
	for (int i=0; i<nNum; i++)
		if (m_aSelLayers[i].Compare(csLayer) == 0) return TRUE;
	return FALSE;
}
