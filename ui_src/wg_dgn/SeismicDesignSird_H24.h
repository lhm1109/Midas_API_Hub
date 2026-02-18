#pragma once

#include "SeismicDesignSird_H14.h"

class CSeismicDesignSird_H24 : public CSeismicDesignSird_H14
{
public:
	CSeismicDesignSird_H24(void);
	virtual ~CSeismicDesignSird_H24(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

	static BOOL Get_Cr(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_Cr);
	static BOOL Get_r(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_r);
};