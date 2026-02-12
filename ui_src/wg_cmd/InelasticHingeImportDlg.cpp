// InelasticHingeImportDlg.cpp : implementation file
//

// Eqmf -> Inmf로 변경 05.02.14 by jbseon

#include "stdafx.h"
#include "wg_cmd.h"
#include "InelasticHingeImportDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TypeEx.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ReadMGB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeImportDlg dialog


CInelasticHingeImportDlg::CInelasticHingeImportDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CInelasticHingeImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInelasticHingeImportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_arRadio.Add(IDC_CMD_ADD_RDO1);
	m_arRadio.Add(IDC_CMD_ADD_RDO2);
}


void CInelasticHingeImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInelasticHingeImportDlg)
	DDX_Control(pDX, IDC_CMD_INITIAL_FORCE_CHK, m_InitialForceChk);
	DDX_Control(pDX, IDC_CMD_ASSIGN_CHK, m_AssignIehpChk);
	DDX_Control(pDX, IDC_CMD_IEHP_CHK, m_IehpChk);
	DDX_Control(pDX, IDC_CMD_FILE_PATH_EDT, m_FilePathEdt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInelasticHingeImportDlg, CDialogMove)
	//{{AFX_MSG_MAP(CInelasticHingeImportDlg)
	ON_BN_CLICKED(IDC_CMD_IEHP_CHK, OnCmdIehpChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeImportDlg message handlers

BOOL CInelasticHingeImportDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	m_pAttrCtrl = m_pDoc->m_pAttrCtrl;
	m_pEditData = m_pDoc->m_pEditData;

	CString str;
	str = CString(_LS(IDS_CMD_DATA_FILE_NAME)) + _T(" (") + D_MODEL_FILE_EXT + _T(")");

	GetDlgItem(IDC_CMD_STATIC)->SetWindowText(str);

	CDlgUtil::CtrlRadioSetCheck(this, m_arRadio, 1);
	m_IehpChk.SetCheck(TRUE);
	m_AssignIehpChk.SetCheck(TRUE);
	m_InitialForceChk.SetCheck(TRUE);
	
	m_FilePathEdt.EnableFileBrowseButton(NULL, D_MODEL_FILE_FILTER, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInelasticHingeImportDlg::OnCmdIehpChk() 
{
	BOOL bChk = m_IehpChk.GetCheck();

	if (!bChk)
		m_AssignIehpChk.SetCheck(FALSE);
	CDlgUtil::CtrlEnableDisable(this, m_arRadio, bChk);
	m_AssignIehpChk.EnableWindow(bChk);
}

void CInelasticHingeImportDlg::OnOK() 
{
	if (!Execute())
		return;
	
	CDialogMove::OnOK();
}

BOOL CInelasticHingeImportDlg::Execute()
{
	CString strFileName;
	m_FilePathEdt.GetWindowText(strFileName);

	if (strFileName == _T(""))
		return FALSE;

	int nImportType;
	BOOL bHingeProperty = m_IehpChk.GetCheck();
	BOOL bAssignHinge = m_AssignIehpChk.GetCheck();
	BOOL bInitialForce = m_InitialForceChk.GetCheck();
	CDlgUtil::CtrlRadioGetCheck(this, m_arRadio, nImportType);

	//File 생성
	m_pFile = new CFile();

	m_pFile->Open(strFileName, CFile::modeRead);

	int nITPF = 1;
	float FileVersion=1.0, RetFileVersion;
	char HeadName[4];
	memcpy(HeadName,D_MODEL_FILE_HEAD,4);

	BOOL bException = FALSE;
	TRY
	{
	  m_BnFile.INIT_F(m_pFile, HeadName, nITPF, FileVersion);

	  int nError = m_BnFile.CHKHED_F(RetFileVersion);
	  if(nError != BN_ER_GOOD && nError != BN_ER_VRLO)
	  {
		  m_pDoc->m_pPostCtrl->DisplayErrorMessage(_LS(IDS_DB_MGB_ER_READ), nError);
		  //goto BAD;
			bException=TRUE;
	  }
	}
	CATCH(CException, e)
	{
	  bException=TRUE;
	  e->ReportError();
	}
	END_CATCH
	if(bException) //goto BAD;
	//BAD:
	{
		delete m_pFile;
		return FALSE;
	}

	m_arIehp.RemoveAll();
	m_arIehg.RemoveAll();
	m_arInmf.RemoveAll();

	//Import 시작
	if (bHingeProperty)
		RB_IEHP();
	if (bAssignHinge)
		RB_IEHG();
	if (bInitialForce)
	{
		RB_INMF();
		MakeInmfMap();
	}
	
	if (bHingeProperty && m_arIehp.GetSize() == 0)
		GSaveHistoryFormatNF(_LS(IDS_CMD_NOT_EXIST_IEHP));

	if (bAssignHinge && m_arIehg.GetSize() == 0)
		GSaveHistoryFormatNF(_LS(IDS_CMD_NOT_EXIST_IEHG));

	if (bInitialForce && m_arInmf.GetSize() == 0)
		GSaveHistoryFormatNF(_LS(IDS_CMD_NOT_EXIST_INMF));
	
	BOOL bReturn = ImportData(nImportType);
	if (bReturn)
	{
		CString str = _T("");
		if (bHingeProperty)
		{
			str = _LS(IDS_CMD_INELASTIC_HINGE);
			GSaveHistoryFormatNF(_LS(IDS_CMD_INELASTIC_HINGE_IMPORT_SUCCESS), str);
		}
		if (bAssignHinge)
		{
			str = _LS(IDS_CMD_ASSIGN_INELASTIC_HINGE);
			GSaveHistoryFormatNF(_LS(IDS_CMD_INELASTIC_HINGE_IMPORT_SUCCESS), str);
		}
		if (bInitialForce)
		{
			str = _LS(IDS_CMD_INITIAL_FORCES);
			GSaveHistoryFormatNF(_LS(IDS_CMD_INELASTIC_HINGE_IMPORT_SUCCESS), str);
		}
	}
	delete m_pFile;
	return bReturn;
}

BOOL CInelasticHingeImportDlg::RB_IEHP()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_IEHP Iehp;
	CArray<T_IEHP_K, T_IEHP_K> aKey;
	CArray<T_IEHP_D, T_IEHP_D&> aData;
	if (!CReadMGB::RB_IEHP(m_BnFile, aKey, aData)) return FALSE;

	int nCount = aKey.GetSize();
	for (int i=0; i<nCount; i++)
	{
	  pDoc->m_pUnitCtrl->ConvertUnitIehpOut(aData[i], aData[i].AllProp.PROP, TRUE);
		pDoc->m_pUnitCtrl->ConvertUnitIehpOut(aData[i], aData[i].AllSubProp.PROP, FALSE);
	}

	// 정렬
	CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&> aPosIdPair;
	aPosIdPair.SetSize(nCount);

	for (int i = 0; i < nCount; i++)
	{
		aPosIdPair[i].nPos = i;
		aPosIdPair[i].nVal = aData[i].nSeq;
	}

	// sort and copy
	int ix;
	qsort((void*)aPosIdPair.GetData(), nCount, sizeof(T_POS_INT_PAIR), CCompFunc::ComparePosIntPairA);
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		ix = aPosIdPair[i].nPos;
		Iehp.Key = aKey[ix];
		Iehp.Data = aData[ix];
		m_arIehp.Add(Iehp);
	}
	return TRUE;
}

BOOL CInelasticHingeImportDlg::RB_IEHG()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_IEHG Iehg;
	CArray<T_IEHG_K, T_IEHG_K> aKey;
	CArray<T_IEHG_D, T_IEHG_D&> aData;
	if (!CReadMGB::RB_IEHG(m_BnFile, aKey, aData)) return FALSE;
	for (int i=0; i<aKey.GetSize(); i++)
	{
	  pDoc->m_pUnitCtrl->ConvertUnitIehgOut(aData[i]);
		Iehg.Key = aKey[i];
		Iehg.Data = aData[i];
		m_arIehg.Add(Iehg);
	}
	return TRUE;
}

BOOL CInelasticHingeImportDlg::RB_INMF()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_INMF Inmf;
	CArray<T_INMF_K, T_INMF_K> aKey;
	CArray<T_INMF_D, T_INMF_D&> aData;
	if (!CReadMGB::RB_INMF(m_BnFile, aKey, aData)) return FALSE;
	for (int i=0; i<aKey.GetSize(); i++)
	{
	  pDoc->m_pUnitCtrl->ConvertUnitInmfOut(aData[i]);
		Inmf.Key = aKey[i];
		Inmf.Data = aData[i];
		m_arInmf.Add(Inmf);
	}
	return TRUE;
}

void CInelasticHingeImportDlg::MakeInmfMap()
{
	m_mapElem.RemoveAll();
	m_mapELink.RemoveAll();
	m_mapGLink.RemoveAll();

	CArray<T_INMF_K, T_INMF_K> rKeyList;
	m_pAttrCtrl->GetInmfKeyList(rKeyList);

	T_INMF_D InmfData;
	for (int i=0; i<rKeyList.GetSize(); i++)
	{
		if (!m_pAttrCtrl->GetInmf(rKeyList[i], InmfData)) continue;

		if (InmfData.nType == 1 || InmfData.nType == 2) //Truss, Beam
			m_mapElem.SetAt(InmfData.ElemKey, rKeyList[i]);
		else if (InmfData.nType == 3) //E-link
			m_mapELink.SetAt(InmfData.ElemKey, rKeyList[i]);
		else if (InmfData.nType == 4) //G-link
			m_mapGLink.SetAt(InmfData.ElemKey, rKeyList[i]);
	}
}

BOOL CInelasticHingeImportDlg::ImportData(int nImportType)
{
	//nImportType - 0:Add, 1:Add/Replace
	CString strCmdName(_LS(IDS_CMD_INELASTIC_HINGE_DATA_IMPORT));
	if(!m_pDoc->m_pUndoCtrl->StartEditDB(strCmdName, CMDTYPE_REMOVE_ANALYSIS)) return FALSE;
	
	CMap<UINT,UINT,UINT,UINT> arKeyMapper;
	CString strIehpName;
	T_IEHP_K IehpKey;
	//Inelastic Hinge Property
	for (int i=0; i<m_arIehp.GetSize(); i++)
	{
		if (m_pAttrCtrl->ExistIehp(m_arIehp[i].Data.strName))
		{
			if (nImportType == 0)
			{
				//Add
				m_arIehp[i].Data.strName = MakeNewIehpName(m_arIehp[i].Data.strName);
				strIehpName = m_arIehp[i].Data.strName;
				if (!m_pEditData->AddIehp(m_arIehp[i].Data)) return EndEdit(FALSE);
			}
			else
			{
				//Modify
				strIehpName = m_arIehp[i].Data.strName;
				IehpKey = m_pAttrCtrl->GetIehpKey(strIehpName);
				if(!m_pEditData->ModifyIehp(IehpKey, m_arIehp[i].Data))return EndEdit(FALSE);
			}
		}
		else
		{
			strIehpName = m_arIehp[i].Data.strName;
			if (!m_pEditData->AddIehp(m_arIehp[i].Data)) return EndEdit(FALSE);
		}
		IehpKey = m_pAttrCtrl->GetIehpKey(strIehpName);
		arKeyMapper.SetAt(m_arIehp[i].Key, IehpKey);
	}

	//Inelastic Hinge Assign
	for (int i=0; i<m_arIehg.GetSize(); i++)
	{
		if (!arKeyMapper.Lookup(m_arIehg[i].Data.IehpKey, IehpKey)) return EndEdit(FALSE);
		m_arIehg[i].Data.IehpKey = IehpKey;
		if(!m_pEditData->AddIehg(m_arIehg[i].Key, m_arIehg[i].Data, TRUE))return EndEdit(FALSE);
	}

	//Initial Forces
	T_INMF_K InmfKey;
	for (int i=0; i<m_arInmf.GetSize(); i++)
	{
		if (m_arInmf[i].Data.nType == 1 || m_arInmf[i].Data.nType == 2)
		{
			//Truss, Beam
			if (m_mapElem.Lookup(m_arInmf[i].Data.ElemKey, InmfKey))
			{
				if(!m_pEditData->ModifyInmf(InmfKey, m_arInmf[i].Data)) return EndEdit(FALSE);
			}
			else
			{
				if(!m_pEditData->AddInmf(m_arInmf[i].Data)) return EndEdit(FALSE);
			}
		}
		else if (m_arInmf[i].Data.nType == 3)
		{
			//E-Link
			if (m_mapELink.Lookup(m_arInmf[i].Data.ElemKey, InmfKey))
			{
				if(!m_pEditData->ModifyInmf(InmfKey, m_arInmf[i].Data)) return EndEdit(FALSE);
			}
			else
			{
				if(!m_pEditData->AddInmf(m_arInmf[i].Data)) return EndEdit(FALSE);
			}
		}
		else if (m_arInmf[i].Data.nType == 4)
		{
			//G-Link
			if (m_mapGLink.Lookup(m_arInmf[i].Data.ElemKey, InmfKey))
			{
				if(!m_pEditData->ModifyInmf(InmfKey, m_arInmf[i].Data)) return EndEdit(FALSE);
			}
			else
			{
				if(!m_pEditData->AddInmf(m_arInmf[i].Data)) return EndEdit(FALSE);
			}
		}
	}
	return EndEdit(TRUE);
}

CString CInelasticHingeImportDlg::MakeNewIehpName(CString strName)
{
	strName.TrimRight();

	int nIndex;
	CString tmp;
	CString tmp2;
	while (TRUE)
	{
		nIndex = strName.ReverseFind('_');
		if (nIndex == -1)
		{
			strName += _T("_1");
		}
		else
		{
			tmp = strName.Right(strName.GetLength() - nIndex - 1);
			if (tmp.GetLength() == 0)
				strName += _T("1");
			else
			{
				BOOL bDigit = TRUE;
				//모두 숫자로 되어 있는지 판단
				for (int i=0; i<tmp.GetLength(); i++)
				{
					if (tmp.GetAt(i) < '0' || tmp.GetAt(i) > '9')
					{
						bDigit = FALSE;
						break;
					}
				}

				if (bDigit)
				{
					int nNum = _ttoi(tmp);
					nNum++;
					tmp2.Format(_T("%d"), nNum);
					strName = strName.Left(nIndex) + _T("_") + tmp2;
				}
				else
				{
					strName += _T("_1");
				}
			}
		}

		if (!m_pAttrCtrl->ExistIehp(strName))
			break;
	}
	return strName;
}

BOOL CInelasticHingeImportDlg::EndEdit(BOOL bEnd, BOOL bDirectDrawLabel, unsigned int nCommand)
{
	if(bEnd && m_pDoc->m_pDataCtrl->CheckModifiedModel())
	{
		BOOL bUpdateDefault=TRUE;
		m_pDoc->m_pUndoCtrl->CloseEditDB(bUpdateDefault, bDirectDrawLabel, nCommand);
		return TRUE;
	}
	else
	{
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
}
