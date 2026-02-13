#include "stdafx.h"
#include "SeismicDesignSird_H24.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

CSeismicDesignSird_H24::CSeismicDesignSird_H24() : CSeismicDesignSird_H14()
{
}

CSeismicDesignSird_H24::~CSeismicDesignSird_H24()
{
}

void CSeismicDesignSird_H24::AddProgressStep(CDgnProgressDlg* pProgress)
{
	CSeismicDesignSird_H14::AddProgressStep(pProgress);
}

BOOL CSeismicDesignSird_H24::Calculate(CDgnProgressDlg* pProgress)
{
	return CSeismicDesignSird_H14::Calculate(pProgress);
}

BOOL CSeismicDesignSird_H24::Get_Cr(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_Cr)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == nullptr) return FALSE;

	T_SIGR_LOWER_D dSigrLower;
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dSigrLower) == FALSE) return FALSE;

	switch (dSigrLower.nMaterialType)
	{
	case T_SIGR_LOWER_D::eMaterialType::kRC:
		d_Cr = 0.6;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSteel:
		d_Cr = 0.45;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSRC:
		d_Cr = 0.45;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

BOOL CSeismicDesignSird_H24::Get_r(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_r)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == nullptr) return FALSE;

	T_SIGR_LOWER_D dSigrLower;
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dSigrLower) == FALSE) return FALSE;

	switch (dSigrLower.nMaterialType)
	{
	case T_SIGR_LOWER_D::eMaterialType::kRC:
		d_r = 0.0;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSteel:
		d_r = 0.2;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSRC:
		d_r = 0.1;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}