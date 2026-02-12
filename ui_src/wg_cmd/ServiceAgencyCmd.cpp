// ServiceAgencyCmd.cpp: implementation of the CServiceAgencyCmd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "ServiceAgencyCmd.h"
#include "CMD_ServiceDef.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "CMSectItemDlg.h"
#include "SpfcMakeSpectrumUtil.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServiceAgencyCmd::CServiceAgencyCmd()
{

}

CServiceAgencyCmd::~CServiceAgencyCmd()
{

}


int CServiceAgencyCmd::DoService(int nServiceNo, void* pData)
{
	switch(nServiceNo)
	{
		case D_CMD_DO_MODAL: 
			return Dlg_DoModal(pData);
		case D_CMD_MODELESS: 
			return Dlg_Modeless(pData);
		case D_CMD_SPECTRUM_DATA:
			return MakeSpectrumData(pData);
		default: 
			ASSERT(0); break;
	}
	return D_SP_ERR_IRG_SERVICE_NO;
}

int CServiceAgencyCmd::Dlg_DoModal(void* pData)
{
	T_AGEN_D agen_data = *((T_AGEN_D*)pData);
	switch(agen_data.nDlgID)
	{
		case D_CMD_DO_MODAL_CMSECTITEMDLG: 
			{
				T_PAIR_DD<T_SECT_K, T_SECT_D> pair_data;
				pair_data = *((T_PAIR_DD<T_SECT_K, T_SECT_D>*)agen_data.pData);
				
				T_SECT_K sect_key  = pair_data.first;
				T_SECT_D sect_data = pair_data.second;
				BOOL bDesignSect = FALSE;
				CCMSectItemDlg dlg;
				dlg.SetInitPos(D_INIT_POS_RT);
				dlg.SetMode(bDesignSect);
				dlg.SetModifyData(sect_key, sect_data);
				dlg.SetRetunData(pData);
				if(dlg.DoModal()==IDOK)
				{
					T_SECT_D rSect_data = *((T_SECT_D*)dlg.m_pVoidData);
					pair_data.first = sect_key;
					pair_data.second = rSect_data; //return data
					T_AGEN_D *pAgen = (T_AGEN_D*)pData;
					*((T_PAIR_DD<T_SECT_K, T_SECT_D>*)pAgen->pData) = pair_data;
					delete dlg.m_pVoidData;
					return 1; //TRUE
				}
	     }
			break;
		default: ASSERT(0); break;
	}

	return 0; //FALSE
}

int CServiceAgencyCmd::Dlg_Modeless(void* pData)
{
	T_AGEN_D agen_data = *((T_AGEN_D*)pData);
	switch(agen_data.nDlgID)
	{
		case D_CMD_DO_MODAL_CMSECTITEMDLG:
			break;
		default: ASSERT(0); break;
	}

	return 1;
}

int CServiceAgencyCmd::MakeSpectrumData(void* pData)
{
	auto* spectrumData = static_cast<SpectrumData*>(pData);
	if (CSpfcMakeSpectrumUtil().MakeSpectrumData(spectrumData->codeParam, spectrumData->nSpecCodeType, spectrumData->parPeriod, spectrumData->parAccel) == false)
		return -1;

	return 1;
}

class CServiceAgencyCmdInit
{
public:
	CServiceAgencyCmdInit() 
	{ 
		CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
		if (pHeadOffice)
		{
			pHeadOffice->RegAgency(_ULS(cmd), new CServiceAgencyCmd);
		}
	}
};

CServiceAgencyCmdInit init;