#ifndef __DATFMT_H__
#define __DATFMT_H__

typedef enum _dat_fmt
{
	dat_fhtime, //符合数据
	dat_sd, //散点数据
	dat_energy,   //能量数据
	dat_time	  //时间数据
}DAT_FMT;

typedef union _fh_time
{
	DWORD dwDat;
	struct {
		DWORD time : 7;
		DWORD time_flag : 1;
		DWORD a_strip : 6;
		DWORD a_block : 3;
		DWORD a_bucket : 4;
		DWORD b_strip : 6;
		DWORD b_block : 3;
		DWORD coin_flag : 2;
	};
}FH_TIME;

//符合数据格式
typedef union _fh_rowdat
{
	DWORD dwDat;
	struct{
		DWORD time : 7;
		DWORD strip : 6;
		DWORD energy : 1;
		DWORD valid : 1;
		DWORD block : 3;
		DWORD bucket : 5;
	};
}FH_ROWDAT;

//散点数据格式
typedef union _sd_rowdat
{
	DWORD dwDat;
	struct{
		DWORD x : 6;
		DWORD nouse1 : 1;
		DWORD y : 6;
		DWORD nouse2 : 1;
		DWORD valid : 1;
		DWORD block : 3;
		DWORD bucket : 5;  //no use
	};
}SD_ROWDAT;

//能量直方图数据格式
typedef union _energy_rowdat
{
	DWORD dwDat;
	struct{
		DWORD energy : 6;
		DWORD nouse1 : 1;
		DWORD strip : 6;  //No.5~No.0
		DWORD nouse3 : 1;
		DWORD valid : 1;
		DWORD block : 3;
		DWORD bucket : 5;  //no use
	};
}ENERGY_ROWDAT;

//时间直方图数据格式
typedef union _time_rowdat
{
	DWORD dwDat;
	struct{
		DWORD time : 7;
		DWORD strip : 6;  //No.5~No.0
		DWORD nouse2 : 1;
		DWORD valid : 1;
		DWORD block : 3;
		DWORD bucket : 5;
	};
}TIME_ROWDAT;

#endif //__DATFMT_H__