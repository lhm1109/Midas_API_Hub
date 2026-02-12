#ifndef  __CMD_SERVICEDEF_H__
#define  __CMD_SERVICEDEF_H__

//=== ServiceDef.h ===
//SERVICE NUMBER DEFINE
#define D_CMD_DO_MODAL            	  1
#define D_CMD_MODELESS            	  2
#define D_CMD_SPECTRUM_DATA           3

// DLG NUMBER DEFINE
#define D_CMD_DO_MODAL_CMSECTITEMDLG  1

struct SpectrumData
{
	T_SPFC_CODE codeParam;
	int nSpecCodeType;
	CArray<double, double> parPeriod;
	CArray<double, double> parAccel;

	void Initialize(int nSpecCodeType)
	{
		codeParam.Initialize(nSpecCodeType);
		this->nSpecCodeType = nSpecCodeType;
		parPeriod.RemoveAll();
		parAccel.RemoveAll();
	}
};

#endif