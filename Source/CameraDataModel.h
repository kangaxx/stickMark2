#ifndef __CAMERA_DATA_MODEL_H__
#define __CAMERA_DATA_MODEL_H__
//#define SEVER_IS
#include "JuceHeader.h"

enum Com_Send_Type
{
	CST_Length = 0,
	CST_Defect = 1,
	CST_Eye = 2,
	CST_Main = 3,
	CST_Ear_Std = 4,
	CST_Ear_Chk = 5,
	CST_Roll = 6,
	CST_EyeImage = 7,
	CST_Stick = 8
};

// 定义与服务器的交互结构
typedef struct __WS_INFO
{
	int				_version;		// 结构版本号
	int				_size;			// 后续的数据尺寸（紧接着当前结构的下一个）
	int				_type;			// 后续的数据类型。0：尺寸以及尺寸图像，1：瑕疵以及瑕疵图像，2：电眼数据, 3：主界面数据,4:极耳实际标准值,5:极耳间距实际值 6膜卷号(服务器下发)
	                                //7带图像电眼数据 8打标数据 9输出参数(服务器下发)
										//10一键开启(_size=999)/关闭(_size=1000),100代表标准的尺寸值
										//11----速度信息  12----电眼闭环补偿值 13----报警信息  14---闭环设定 99---版本信息
	int				_workStation;	// 相机位置编号。-1：非相机工位
							// 0：16k相机，1：上带路正面8k，2：上带路反面8k，
							// 3：下带路正面8k，4：下带路反面8k，5：上带路背光8k，6：下带路背光8k
							// 7：上带路二级纠偏8k，8：下带路二级纠偏8k
							// 9：上带路左分条4k，10：上带路右分条4k
							// 11：下带路左分条4k，12：下带路右分条4k	// 时间
							
	juce::int64			_time;
} WS_INFO;

// 尺寸及尺寸图像
typedef struct __SIZE_INFO
{
	double					_y;				// 极耳的全局位置（mm）

	// 各条边计算后的尺寸
	double					_width;			// 极片宽度（含AT9）
	double					_widthLeftEar;	// 左极耳宽
	double					_widthRightEar;	// 右极耳宽
	double					_heightLeftEar;	// 左极耳高
	double					_heightRightEar;// 右极耳高
	double					_leftInterval;	// 左极耳间距
	double					_rightInterval;	// 右极耳间距
	double					_h1Left;		// H1
	double					_h1Right;		// H1'
	double					_h3Left;		// H3 （左AT9宽）
	double					_h3Right;		// H3'（右AT9宽）
	int							_earIndex;		// 极耳在一个EA里面的索引，基于0
	double					_divideCorrectUp;//分条纠偏上
	double					_divideCorrectDown;//分条纠偏下
	double					_startGap;//涂膜左边
	double					_scale;//传图的缩放比例
	double					_mm_perpixel;//像素与毫米转化单位

	//2020.10.27 必须同步主画面与客户端
	double					_EA_SumLen;//如果时一组EA的结束,则有总长，否则-1.
	int							 _EA_order; //EA序号
	bool						_resultSize;    //true:测量OK;false:测量NG
	double					_encoderSpeed; //从编码器计算获取的速度
	double					 _intervalDiff;     //间距跟标准的差值
	double					 _dCenterOffset;     //涂膜中心

	int							_imgWidth;		// 图像宽度
	int							_imgHeight;		// 图像高度
	unsigned char* _data;			// 图像数据。可以为空，当图像宽度和高度传0时

public:
	__SIZE_INFO()
	{
		_imgWidth = 0;
		_imgHeight = 0;
		_data = 0;
		_EA_SumLen = -1.;
		_EA_order = -1;
		_resultSize = true;
		_encoderSpeed = -1.;
		_intervalDiff = -99999.;
		_dCenterOffset = -99999.;
	}

} SIZE_INFO;

// 瑕疵及瑕疵图像
typedef struct __DEFECT_INFO
{
	double			_x;				// 缺陷的X坐标（mm）
	double			_y;				// 缺陷的Y坐标（mm，所有相机坐标统一到0号相机）
	int             _EAorder;       //EA序号
	double          _dividePos;     //分条位置
	int				_flag;			// 0：缺陷在左侧；1：缺陷在右侧；2：缺陷位于中心线，影响左右两侧
	
	int            _direction;     //0:内侧；1：外侧
	bool            _resultSize;    //true:测量OK;false:测量NG
	bool            _resultDefect;        // true:瑕疵OK; false:瑕疵NG

	int				_type;			// 缺陷类型，由Defect_Type表示
	double			_defLength;		// 缺陷长度（mm）
	double			_defWidth;		// 缺陷宽度（mm）
	double			_defArea;		// 缺陷面积（mm^2）

	int				_imgWidth;		// 图像宽度
	int				_imgHeight;		// 图像高度
	unsigned char* _data;			// 图像数据。可以为空，当图像宽度和高度传0时
	char _imgPath[256];        //图片路径

public:
	__DEFECT_INFO()
	{
		_imgWidth = 0;
		_imgHeight = 0;
		_data = 0;
	}
} DEFECT_INFO;

// 电眼及电眼图像
typedef struct __EYE_INFO
{
	double			_left;			// 电眼左边缘坐标（mm）
	double			_right;			// 电眼右边缘坐标（mm）
	double			_offset;		// 电眼offset（mm）
	double			_motor;			// 电眼电机位置（mm）
	int				_status;		// 电眼状态。0：手动，1：对中，2：自动

	int				_imgWidth;		// 图像宽度
	int				_imgHeight;		// 图像高度
	unsigned char* _data;			// 图像数据。可以为空，当图像宽度和高度传0时
public:
	__EYE_INFO()
	{
		_imgWidth = 0;
		_imgHeight = 0;
		_data = 0;
	}
} EYE_INFO;

//主界面
typedef struct __MAIN__INFO
{
	double _data[5];//1.模切纠偏（0：当前纠偏值；1：左总宽；2：右总宽；3：当前纠偏位置）
	                //2.上带路分条纠偏（0：分条前纠偏；1：总宽；2：当前纠偏位置）
	                //3.下带路分条纠偏（0：分条前纠偏；1：总宽；2：当前纠偏位置）
	                //4.上带路（0：左侧切入深度；1：右侧切入深度；2：切入深度差；3：总宽）
	                //5.下带路（0：左侧切入深度；1：右侧切入深度；2：切入深度差；3：总宽）
	                //6.工位1（0：分条宽度；1：分条宽度差）
	                //7.工位2（0：分条宽度）
					//8.工位3（0：分条宽度；1：分条宽度差）
					//9.工位4（0：分条宽度）
}MAIN_INFO;


//主界面下发纠偏动作
typedef struct __GUIDER__CMD
{
	int _mode; //1----自动 0-----手动 2居中
	int _action; //-1----无动作 1----上移 0----下移
public:
	__GUIDER__CMD()
	{
		_mode = 1;
		_action = -1;
	}
}GUIDER__CMD;

//手动闭环值
typedef struct __GUIDER__OFFSET
{
	double _offset;

}GUIDER__OFFSET;

//极耳间距标准值传输
typedef struct __STD_EAR
{

	double  _stdEarWidth; //极耳宽度标准
	double  _stdEarHeight; //极耳高度标准

	double _stdEarInterval[50]; //间距标准值
	int			_nInteral;						//间距个数
	double _height[50];					// 极耳渐变高度增量，数量=_numOfPoles

	bool      _bRealVal; //true---实际值, false----标准值

	double		_tol;			// 允许公差
	double		_tolD1;			// 两组电芯之间的极耳间距公差
	double		_tolD;			// 一组电芯内部的极耳间距公差
	double		_tolDeltaAT9;	// 两侧残留AT9宽度差的公差
	double		_tolLength;		// 一组电芯总长的公差

} STD_EAR;

//极耳间距实际值
typedef struct __MEASURE_INTERVAL_EAR
{
	double _stdEarInterval[50]; //间距实际值
	int			_nInteral;						//间距个数

} MEASURE_INTERVAL_EAR;

typedef struct __ROLL_INFO
{
	int					_version;		// 结构版本号
	juce::int64	 _time;
	char				_rollName[100]; //膜卷号
	double _dWid;   //模切纠偏模宽
	double _dWidUp;  //上带路分条纠偏模宽
	double _dWidDown;  //下带路分条纠偏模宽

} ROLL_INFO;


#ifdef SEVER_IS
// 缺陷型号
enum Defect_Type
{
	// 瑕疵
	Defect_Type_1,   // 露箔
	Defect_Type_2,   // 颗粒
	Defect_Type_3,   // 针眼--针孔/针眼，纯黑点
	Defect_Type_4,   // 划痕暗痕--颜色比较深
	Defect_Type_5,   // 气泡
	Defect_Type_6,   // 拖尾
	Defect_Type_7,   // 波浪边

	// 测量
	Defect_Type_8,   // 长度
	Defect_Type_9,   // 长度错位
	Defect_Type_10,   // 宽度
	Defect_Type_11,   // 宽度错位

	Defect_Type_12,   // 条痕（凸起)
	Defect_Type_13,   // 涂膜区金属外漏--外黑，内亮度高
	Defect_Type_14,   // 脱碳（膜）--外黑，内亮度较低
	Defect_Type_15,   // 亮点
	Defect_Type_16,   // 极片金属区孔洞(选配功能)
	Defect_Type_17,   // 极片黑点
	Defect_Type_18,   // 极片鼓泡,气泡
	Defect_Type_19,   // 凹点
	Defect_Type_20,   // 凸点

	Defect_Type_21,   // 停机位

	Defect_Type_22,   // 瑕疵
	Defect_Type_23,   // 瑕疵(AT9)
	Defect_Type_24,   // 瑕疵(涂膜)
	Defect_Type_25,   // 瑕疵(空箔区)

	Defect_Type_26,      // 其它

	// 海目星項目
	Defect_Type_27,   // 漏銅
	Defect_Type_28,   // 劃痕
	Defect_Type_29,   // 破損
	Defect_Type_30,   // 極耳未切
	Defect_Type_31,   // 接帶
	Defect_Type_32,   // 氣泡
	Defect_Type_33,   // 黑斑
	Defect_Type_34,   // 折痕/暗痕
	Defect_Type_35,   // 白斑
	Defect_Type_36,   // 凹凸点
	Defect_Type_37,   // 脱碳

	Defect_Type_38,   // 忽略

	Defect_Type_39,   // 卷料的规格，并非缺陷和检测项，只是为重用参数设定对话框
	Defect_Type_40,   // AT9漏金属
	Defect_Type_41,   // 边缘漏金属
	Defect_Type_42,   // 极耳破损
	Defect_Type_43,   // 极耳余料
	Defect_Type_44,   // 未涂膜
	Defect_Type_45,   // 未辊压
	Defect_Type_46,   // 直边余料

	Defect_Type_Count  // None
};
#endif // SEVER_IS

// 打标数据
struct StickMarkInfo
{
	long	EA;			// EA 索引
	char	ip[20];		// IP地址
	juce::int64	sendTime;	// UI发送打标时间
	juce::int64	stickTime;	// 实际打标时间

	int		codeReason;	// 打标原因
	double	lenPos;		// 打标位置
	int		roadUser;	// 从里往外 带路1，2，3，4  相对我们实际打标机机：(1,1)  (0,1), (0,0) (1,0)
	int		road;		// 打标机带路
	int		iSe;		// 打标机
	bool	bSticked;	// 

	StickMarkInfo()
	{
		EA = 0;
		ip[0] = '\0';
		sendTime = 0;
		stickTime = 0;

		codeReason = 0;
		lenPos = 0;
		roadUser = 0;
		road = 0;
		iSe = 0;
		bSticked = false;
	}
};

//打标信号数与贴标数统计结构
typedef struct __NumCount_
{
	int id;    //贴标机的标识
	int num[3];//发出的信号数，贴标数，反馈数
}NumCount;


typedef struct __OUTPUT_
{
	char	_name[20];	   // 输出项的名称
	int		_id;		   // 项目编号
	double	_vStandard[10];// 规格参数
	int		_vCount[10];   // 数量
	bool    _vCheck[10];   // 是否勾选
	bool	_alarm;		  // 是否报警
	bool	_stop;		// 是否停机
	bool	_mark;		// 是否打标

	__OUTPUT_()
	{
		_id = -1;
		_alarm = false;
		_stop = false;
		_mark = false;
	}
} OUTPUT;


typedef struct _WARNING_MSG {
	char	ip[20];					// 尾号ip
	int		type;					// 类型，1报警or2停机
	char	mes[512];				// 具体信息
	bool	isSend2Plc;				// 是否成功发送到plc
}WARNING_MSG;

// 客户端版本信息
typedef struct __WS_VERSION
{
	char				_version[20];	

	juce::int64			_time;
} WS_VERSION;

// 闭环设置
typedef struct __WS_CLOSING_SETTING
{
	//激光工位设置
	//纠偏工位选择
	bool _bGuiderUpRoad; //上带路纠偏
	bool _bGuiderDownRoad; //下带路纠偏
	//激光闭环工位（false:则是纠偏器纠偏)
	bool _bLaserCloseUpRoad;
	bool _bLaserCloseDownRoad;
	//激光纠偏工位
	bool bUseAT9RemainDiff_Up;//AT9残留宽度差值
	//bool bUseAT9Width_Up;

	//下带路
	bool bUseAT9RemainDiff_Down;//AT9残留宽度差值
	//bool bUseAT9Width_Down;

	//分条工位
	int		   iCloseMode2; //0,1,2,3 二级分条闭环方式
	//int    bIncludedAT9; //bIncludedAT9 包含AT9宽度,	bool bOnlyCoatWidth; 涂膜区宽度
	//bool bOnlyCoatWidth; //涂膜区宽度
	//bool bHalfSumWidth; //总宽中心值
	//bool bSetWidth; //(单分条工位闭环)


	__WS_CLOSING_SETTING()
	{
		_bGuiderUpRoad = true; //上带路纠偏
		_bGuiderDownRoad = true; //下带路纠偏
		//激光闭环工位（false:则是纠偏器纠偏)
		_bLaserCloseUpRoad = false;
		_bLaserCloseDownRoad = true;
		//激光纠偏工位
		bUseAT9RemainDiff_Up = true;//AT9残留宽度差值
		//bool bUseAT9Width_Up; //

		//下带路
		bUseAT9RemainDiff_Down = true;//AT9残留宽度差值
		//bool bUseAT9Width_Down;

		//分条工位
		iCloseMode2 = 0; //0,1,2,3 二级分条闭环方式

	}
} WS_CLOSING_SETTING;


// 标准规格尺寸
typedef struct __STD_MODEL_SIZE
{
	int			_version;		// 结构的版本号

	//电眼参数
	double	_eye_width;
	double _eye_tol;
	//测宽参数
	double _last_width;
	double _last_tol;
	//极耳检测程序参数设置
	double _EA_length1; //左
	double _EA_tol1;
	double _EA_length2; //右?
	double _EA_tol2;
	double _AT9_width;
	double _AT9_tol;
	int			_Ear_num;
	int			_Ear_num_tol;
	double _Ear_width_C; //极耳带料宽度
	double _Ear_width_CTol; //极耳带料宽度公差
	double _Ear_height_tol; //极耳高公差
	double _Ear_width_tol; //极耳宽度公差
	double _Ear_Inter_tol; //极耳间距公差


	int					_numOfSize;		// 极耳组数据个数
	double*		_EarHeights;			// 极耳高
	double*		_EarWidths;			// 极耳宽
	double*		_EarInters;			// 极耳间距
	__STD_MODEL_SIZE()
	{
		_version = 1;
		_numOfSize = 0;
		_EarHeights = 0;
		_EarWidths = 0;
		_EarInters = 0;
	}
	~__STD_MODEL_SIZE()
	{
		delete[] _EarHeights;
		delete[] _EarWidths;
		delete[] _EarInters;
	}

	__STD_MODEL_SIZE(const __STD_MODEL_SIZE& src)
	{
		*this = src;
	}
	__STD_MODEL_SIZE& operator = (const __STD_MODEL_SIZE& src)
	{
		_version = src._version;
		_numOfSize = src._numOfSize;
		if (_EarHeights)
			delete[] _EarHeights;
		if (_EarWidths)
			delete[] _EarWidths;
		if (_EarInters)
			delete[] _EarInters;
		_EarHeights = 0;
		_EarWidths = 0;
		_EarInters = 0;


		if (_numOfSize > 0)		
		{
			_EarHeights = new double[_numOfSize];
			_EarWidths = new double[_numOfSize];
			_EarInters = new double[_numOfSize];
			memcpy(_EarHeights, src._EarHeights, sizeof(double)* _numOfSize);
			memcpy(_EarWidths, src._EarWidths, sizeof(double)* _numOfSize);
			memcpy(_EarInters, src._EarInters, sizeof(double)* _numOfSize);
		}

		//电眼参数
		_eye_width = src._eye_width;
		_eye_tol = src._eye_tol;
		//测宽参数
		_last_width = src._last_width;
		_last_tol = src. _last_tol;
		//极耳检测程序参数设置
		_EA_length1 = src._EA_length1; //左
		_EA_tol1 = src._EA_tol1;
		_EA_length2 = src._EA_length2; //右?
		_EA_tol2 = src._EA_tol2;
		_AT9_width = src._AT9_width;
		_AT9_tol = src._AT9_tol;
		_Ear_num = src._Ear_num;
		_Ear_num_tol = src._Ear_num_tol;
		_Ear_width_C = src._Ear_width_C; //极耳带料宽度
		_Ear_width_CTol = src._Ear_width_CTol; //极耳带料宽度公差
		_Ear_height_tol = src._Ear_height_tol; //极耳高公差
		_Ear_width_tol = src._Ear_width_tol; //极耳宽度公差
		_Ear_Inter_tol = src._Ear_Inter_tol; //极耳间距公差

		return *this;
	}
}_STD_MODEL_SIZE;

#endif // __CAMERA_DATA_MODEL_H__





