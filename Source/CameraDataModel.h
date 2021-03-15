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

// ������������Ľ����ṹ
typedef struct __WS_INFO
{
	int				_version;		// �ṹ�汾��
	int				_size;			// ���������ݳߴ磨�����ŵ�ǰ�ṹ����һ����
	int				_type;			// �������������͡�0���ߴ��Լ��ߴ�ͼ��1��覴��Լ�覴�ͼ��2����������, 3������������,4:����ʵ�ʱ�׼ֵ,5:�������ʵ��ֵ 6Ĥ���(�������·�)
	                                //7��ͼ��������� 8������� 9�������(�������·�)
										//10һ������(_size=999)/�ر�(_size=1000),100�����׼�ĳߴ�ֵ
										//11----�ٶ���Ϣ  12----���۱ջ�����ֵ 13----������Ϣ  14---�ջ��趨 99---�汾��Ϣ
	int				_workStation;	// ���λ�ñ�š�-1���������λ
							// 0��16k�����1���ϴ�·����8k��2���ϴ�·����8k��
							// 3���´�·����8k��4���´�·����8k��5���ϴ�·����8k��6���´�·����8k
							// 7���ϴ�·������ƫ8k��8���´�·������ƫ8k
							// 9���ϴ�·�����4k��10���ϴ�·�ҷ���4k
							// 11���´�·�����4k��12���´�·�ҷ���4k	// ʱ��
							
	juce::int64			_time;
} WS_INFO;

// �ߴ缰�ߴ�ͼ��
typedef struct __SIZE_INFO
{
	double					_y;				// ������ȫ��λ�ã�mm��

	// �����߼����ĳߴ�
	double					_width;			// ��Ƭ��ȣ���AT9��
	double					_widthLeftEar;	// �󼫶���
	double					_widthRightEar;	// �Ҽ�����
	double					_heightLeftEar;	// �󼫶���
	double					_heightRightEar;// �Ҽ�����
	double					_leftInterval;	// �󼫶����
	double					_rightInterval;	// �Ҽ������
	double					_h1Left;		// H1
	double					_h1Right;		// H1'
	double					_h3Left;		// H3 ����AT9��
	double					_h3Right;		// H3'����AT9��
	int							_earIndex;		// ������һ��EA���������������0
	double					_divideCorrectUp;//������ƫ��
	double					_divideCorrectDown;//������ƫ��
	double					_startGap;//ͿĤ���
	double					_scale;//��ͼ�����ű���
	double					_mm_perpixel;//���������ת����λ

	//2020.10.27 ����ͬ����������ͻ���
	double					_EA_SumLen;//���ʱһ��EA�Ľ���,�����ܳ�������-1.
	int							 _EA_order; //EA���
	bool						_resultSize;    //true:����OK;false:����NG
	double					_encoderSpeed; //�ӱ����������ȡ���ٶ�
	double					 _intervalDiff;     //������׼�Ĳ�ֵ
	double					 _dCenterOffset;     //ͿĤ����

	int							_imgWidth;		// ͼ����
	int							_imgHeight;		// ͼ��߶�
	unsigned char* _data;			// ͼ�����ݡ�����Ϊ�գ���ͼ���Ⱥ͸߶ȴ�0ʱ

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

// 覴ü�覴�ͼ��
typedef struct __DEFECT_INFO
{
	double			_x;				// ȱ�ݵ�X���꣨mm��
	double			_y;				// ȱ�ݵ�Y���꣨mm�������������ͳһ��0�������
	int             _EAorder;       //EA���
	double          _dividePos;     //����λ��
	int				_flag;			// 0��ȱ������ࣻ1��ȱ�����Ҳࣻ2��ȱ��λ�������ߣ�Ӱ����������
	
	int            _direction;     //0:�ڲࣻ1�����
	bool            _resultSize;    //true:����OK;false:����NG
	bool            _resultDefect;        // true:覴�OK; false:覴�NG

	int				_type;			// ȱ�����ͣ���Defect_Type��ʾ
	double			_defLength;		// ȱ�ݳ��ȣ�mm��
	double			_defWidth;		// ȱ�ݿ�ȣ�mm��
	double			_defArea;		// ȱ�������mm^2��

	int				_imgWidth;		// ͼ����
	int				_imgHeight;		// ͼ��߶�
	unsigned char* _data;			// ͼ�����ݡ�����Ϊ�գ���ͼ���Ⱥ͸߶ȴ�0ʱ
	char _imgPath[256];        //ͼƬ·��

public:
	__DEFECT_INFO()
	{
		_imgWidth = 0;
		_imgHeight = 0;
		_data = 0;
	}
} DEFECT_INFO;

// ���ۼ�����ͼ��
typedef struct __EYE_INFO
{
	double			_left;			// �������Ե���꣨mm��
	double			_right;			// �����ұ�Ե���꣨mm��
	double			_offset;		// ����offset��mm��
	double			_motor;			// ���۵��λ�ã�mm��
	int				_status;		// ����״̬��0���ֶ���1�����У�2���Զ�

	int				_imgWidth;		// ͼ����
	int				_imgHeight;		// ͼ��߶�
	unsigned char* _data;			// ͼ�����ݡ�����Ϊ�գ���ͼ���Ⱥ͸߶ȴ�0ʱ
public:
	__EYE_INFO()
	{
		_imgWidth = 0;
		_imgHeight = 0;
		_data = 0;
	}
} EYE_INFO;

//������
typedef struct __MAIN__INFO
{
	double _data[5];//1.ģ�о�ƫ��0����ǰ��ƫֵ��1�����ܿ�2�����ܿ�3����ǰ��ƫλ�ã�
	                //2.�ϴ�·������ƫ��0������ǰ��ƫ��1���ܿ�2����ǰ��ƫλ�ã�
	                //3.�´�·������ƫ��0������ǰ��ƫ��1���ܿ�2����ǰ��ƫλ�ã�
	                //4.�ϴ�·��0�����������ȣ�1���Ҳ�������ȣ�2��������Ȳ3���ܿ�
	                //5.�´�·��0�����������ȣ�1���Ҳ�������ȣ�2��������Ȳ3���ܿ�
	                //6.��λ1��0��������ȣ�1��������Ȳ
	                //7.��λ2��0��������ȣ�
					//8.��λ3��0��������ȣ�1��������Ȳ
					//9.��λ4��0��������ȣ�
}MAIN_INFO;


//�������·���ƫ����
typedef struct __GUIDER__CMD
{
	int _mode; //1----�Զ� 0-----�ֶ� 2����
	int _action; //-1----�޶��� 1----���� 0----����
public:
	__GUIDER__CMD()
	{
		_mode = 1;
		_action = -1;
	}
}GUIDER__CMD;

//�ֶ��ջ�ֵ
typedef struct __GUIDER__OFFSET
{
	double _offset;

}GUIDER__OFFSET;

//��������׼ֵ����
typedef struct __STD_EAR
{

	double  _stdEarWidth; //������ȱ�׼
	double  _stdEarHeight; //�����߶ȱ�׼

	double _stdEarInterval[50]; //����׼ֵ
	int			_nInteral;						//������
	double _height[50];					// ��������߶�����������=_numOfPoles

	bool      _bRealVal; //true---ʵ��ֵ, false----��׼ֵ

	double		_tol;			// ������
	double		_tolD1;			// �����о֮��ļ�����๫��
	double		_tolD;			// һ���о�ڲ��ļ�����๫��
	double		_tolDeltaAT9;	// �������AT9��Ȳ�Ĺ���
	double		_tolLength;		// һ���о�ܳ��Ĺ���

} STD_EAR;

//�������ʵ��ֵ
typedef struct __MEASURE_INTERVAL_EAR
{
	double _stdEarInterval[50]; //���ʵ��ֵ
	int			_nInteral;						//������

} MEASURE_INTERVAL_EAR;

typedef struct __ROLL_INFO
{
	int					_version;		// �ṹ�汾��
	juce::int64	 _time;
	char				_rollName[100]; //Ĥ���
	double _dWid;   //ģ�о�ƫģ��
	double _dWidUp;  //�ϴ�·������ƫģ��
	double _dWidDown;  //�´�·������ƫģ��

} ROLL_INFO;


#ifdef SEVER_IS
// ȱ���ͺ�
enum Defect_Type
{
	// 覴�
	Defect_Type_1,   // ¶��
	Defect_Type_2,   // ����
	Defect_Type_3,   // ����--���/���ۣ����ڵ�
	Defect_Type_4,   // ���۰���--��ɫ�Ƚ���
	Defect_Type_5,   // ����
	Defect_Type_6,   // ��β
	Defect_Type_7,   // ���˱�

	// ����
	Defect_Type_8,   // ����
	Defect_Type_9,   // ���ȴ�λ
	Defect_Type_10,   // ���
	Defect_Type_11,   // ��ȴ�λ

	Defect_Type_12,   // ���ۣ�͹��)
	Defect_Type_13,   // ͿĤ��������©--��ڣ������ȸ�
	Defect_Type_14,   // ��̼��Ĥ��--��ڣ������Ƚϵ�
	Defect_Type_15,   // ����
	Defect_Type_16,   // ��Ƭ�������׶�(ѡ�书��)
	Defect_Type_17,   // ��Ƭ�ڵ�
	Defect_Type_18,   // ��Ƭ����,����
	Defect_Type_19,   // ����
	Defect_Type_20,   // ͹��

	Defect_Type_21,   // ͣ��λ

	Defect_Type_22,   // 覴�
	Defect_Type_23,   // 覴�(AT9)
	Defect_Type_24,   // 覴�(ͿĤ)
	Defect_Type_25,   // 覴�(�ղ���)

	Defect_Type_26,      // ����

	// ��Ŀ���Ŀ
	Defect_Type_27,   // ©�~
	Defect_Type_28,   // ����
	Defect_Type_29,   // �Ɠp
	Defect_Type_30,   // �O��δ��
	Defect_Type_31,   // �ӎ�
	Defect_Type_32,   // ����
	Defect_Type_33,   // �ڰ�
	Defect_Type_34,   // �ۺ�/����
	Defect_Type_35,   // �װ�
	Defect_Type_36,   // ��͹��
	Defect_Type_37,   // ��̼

	Defect_Type_38,   // ����

	Defect_Type_39,   // ���ϵĹ�񣬲���ȱ�ݺͼ���ֻ��Ϊ���ò����趨�Ի���
	Defect_Type_40,   // AT9©����
	Defect_Type_41,   // ��Ե©����
	Defect_Type_42,   // ��������
	Defect_Type_43,   // ��������
	Defect_Type_44,   // δͿĤ
	Defect_Type_45,   // δ��ѹ
	Defect_Type_46,   // ֱ������

	Defect_Type_Count  // None
};
#endif // SEVER_IS

// �������
struct StickMarkInfo
{
	long	EA;			// EA ����
	char	ip[20];		// IP��ַ
	juce::int64	sendTime;	// UI���ʹ��ʱ��
	juce::int64	stickTime;	// ʵ�ʴ��ʱ��

	int		codeReason;	// ���ԭ��
	double	lenPos;		// ���λ��
	int		roadUser;	// �������� ��·1��2��3��4  �������ʵ�ʴ�������(1,1)  (0,1), (0,0) (1,0)
	int		road;		// ������·
	int		iSe;		// ����
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

//����ź�����������ͳ�ƽṹ
typedef struct __NumCount_
{
	int id;    //������ı�ʶ
	int num[3];//�������ź�������������������
}NumCount;


typedef struct __OUTPUT_
{
	char	_name[20];	   // ����������
	int		_id;		   // ��Ŀ���
	double	_vStandard[10];// ������
	int		_vCount[10];   // ����
	bool    _vCheck[10];   // �Ƿ�ѡ
	bool	_alarm;		  // �Ƿ񱨾�
	bool	_stop;		// �Ƿ�ͣ��
	bool	_mark;		// �Ƿ���

	__OUTPUT_()
	{
		_id = -1;
		_alarm = false;
		_stop = false;
		_mark = false;
	}
} OUTPUT;


typedef struct _WARNING_MSG {
	char	ip[20];					// β��ip
	int		type;					// ���ͣ�1����or2ͣ��
	char	mes[512];				// ������Ϣ
	bool	isSend2Plc;				// �Ƿ�ɹ����͵�plc
}WARNING_MSG;

// �ͻ��˰汾��Ϣ
typedef struct __WS_VERSION
{
	char				_version[20];	

	juce::int64			_time;
} WS_VERSION;

// �ջ�����
typedef struct __WS_CLOSING_SETTING
{
	//���⹤λ����
	//��ƫ��λѡ��
	bool _bGuiderUpRoad; //�ϴ�·��ƫ
	bool _bGuiderDownRoad; //�´�·��ƫ
	//����ջ���λ��false:���Ǿ�ƫ����ƫ)
	bool _bLaserCloseUpRoad;
	bool _bLaserCloseDownRoad;
	//�����ƫ��λ
	bool bUseAT9RemainDiff_Up;//AT9������Ȳ�ֵ
	//bool bUseAT9Width_Up;

	//�´�·
	bool bUseAT9RemainDiff_Down;//AT9������Ȳ�ֵ
	//bool bUseAT9Width_Down;

	//������λ
	int		   iCloseMode2; //0,1,2,3 ���������ջ���ʽ
	//int    bIncludedAT9; //bIncludedAT9 ����AT9���,	bool bOnlyCoatWidth; ͿĤ�����
	//bool bOnlyCoatWidth; //ͿĤ�����
	//bool bHalfSumWidth; //�ܿ�����ֵ
	//bool bSetWidth; //(��������λ�ջ�)


	__WS_CLOSING_SETTING()
	{
		_bGuiderUpRoad = true; //�ϴ�·��ƫ
		_bGuiderDownRoad = true; //�´�·��ƫ
		//����ջ���λ��false:���Ǿ�ƫ����ƫ)
		_bLaserCloseUpRoad = false;
		_bLaserCloseDownRoad = true;
		//�����ƫ��λ
		bUseAT9RemainDiff_Up = true;//AT9������Ȳ�ֵ
		//bool bUseAT9Width_Up; //

		//�´�·
		bUseAT9RemainDiff_Down = true;//AT9������Ȳ�ֵ
		//bool bUseAT9Width_Down;

		//������λ
		iCloseMode2 = 0; //0,1,2,3 ���������ջ���ʽ

	}
} WS_CLOSING_SETTING;


// ��׼���ߴ�
typedef struct __STD_MODEL_SIZE
{
	int			_version;		// �ṹ�İ汾��

	//���۲���
	double	_eye_width;
	double _eye_tol;
	//������
	double _last_width;
	double _last_tol;
	//�����������������
	double _EA_length1; //��
	double _EA_tol1;
	double _EA_length2; //��?
	double _EA_tol2;
	double _AT9_width;
	double _AT9_tol;
	int			_Ear_num;
	int			_Ear_num_tol;
	double _Ear_width_C; //�������Ͽ��
	double _Ear_width_CTol; //�������Ͽ�ȹ���
	double _Ear_height_tol; //�����߹���
	double _Ear_width_tol; //������ȹ���
	double _Ear_Inter_tol; //������๫��


	int					_numOfSize;		// ���������ݸ���
	double*		_EarHeights;			// ������
	double*		_EarWidths;			// ������
	double*		_EarInters;			// �������
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

		//���۲���
		_eye_width = src._eye_width;
		_eye_tol = src._eye_tol;
		//������
		_last_width = src._last_width;
		_last_tol = src. _last_tol;
		//�����������������
		_EA_length1 = src._EA_length1; //��
		_EA_tol1 = src._EA_tol1;
		_EA_length2 = src._EA_length2; //��?
		_EA_tol2 = src._EA_tol2;
		_AT9_width = src._AT9_width;
		_AT9_tol = src._AT9_tol;
		_Ear_num = src._Ear_num;
		_Ear_num_tol = src._Ear_num_tol;
		_Ear_width_C = src._Ear_width_C; //�������Ͽ��
		_Ear_width_CTol = src._Ear_width_CTol; //�������Ͽ�ȹ���
		_Ear_height_tol = src._Ear_height_tol; //�����߹���
		_Ear_width_tol = src._Ear_width_tol; //������ȹ���
		_Ear_Inter_tol = src._Ear_Inter_tol; //������๫��

		return *this;
	}
}_STD_MODEL_SIZE;

#endif // __CAMERA_DATA_MODEL_H__





