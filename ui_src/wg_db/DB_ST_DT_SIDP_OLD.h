
struct T_SIDP_ADPR_D_CH_RW_955
{
	int nAllowDispPlasFormula;
	double dAlpha;
};

struct T_SIDP_D_CH_RW_955
{
	T_SIDP_K key;
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirDisp_del;
	double dAngle_del;
	T_SIDP_ADPR_D_CH_RW_955 ADPR_D;
	T_SIDP_LSLP_D_CH_RW LSLP_D;
};
