#include "pi.h"

//////////////////////////////////////////////////////////////////////////////////	 

/* *******************************************************************
 PI控制器

 说明：计算 PI 校正。
 */
 
tPIParm Parm;
void CalcPI( tPIParm *pParm)
{
	float qErr = pParm->qInRef - pParm->qInMeas;
	float U = pParm->qdSum + pParm->qKp * qErr;
	if( U > pParm->qOutMax )
	pParm->qOut =  pParm->qOutMax;
	else if( U < pParm->qOutMin )
	pParm->qOut = pParm->qOutMin;
	else
	pParm->qOut = U;
	float Exc = U - pParm->qOut;
	pParm->qdSum = pParm->qdSum + pParm->qKi * qErr - pParm->qKc * Exc;
}
#define QKP 			0.06F
#define QKI				0.003F
#define QKC				0.9999999F
#define DOUTMAX   1900
#define DOUTMIN   1140
void InitPI( tPIParm *pParm)
{
	pParm->qdSum = 0;
	pParm->qOut = 0;  
	pParm->qKp = QKP;
	pParm->qKi = QKI;
	pParm->qKc = QKC;
	pParm->qOutMax =DOUTMAX;
	pParm->qOutMin =DOUTMIN;
}
 
