#pragma once
#define DELAYTIME 0x0500				//延时时间0.05秒

//gif文件头
typedef struct gifheader	//6
{
	BYTE m_Signature[3];	//GIF标记域 “GIF”
	BYTE m_Version[3];		//GIF版本号 “87a”或“89a”
} GIFHEADER;

//逻辑屏幕描述块
typedef struct gifscrdesc	//7
{
	WORD m_wWidth;	//逻辑屏幕宽度
	WORD m_wDepth;	//逻辑屏幕高度
	struct globalflag	//包装域
	{				
		BYTE PalBits : 3;		//全局颜色表大小
		BYTE SortFlag : 1;		//全局颜色表分类标志，如果置位1表示全局颜色列表分类排列
		BYTE ColorRes : 3;		//颜色深度，cr+1确定颜色深度
		BYTE GlobalPal : 1;		//全局颜色列表标志，置位表示有全局颜色列表，pixel值有意义
	} m_GloabalFlag;
	BYTE m_byBackground;		//背景颜色（在全局颜色列表中的索引，如果没有全局颜色列表，该值没有意义）
	BYTE m_byAspect;			//像素宽高比
} GIFSCRDESC;

//图像描述块
typedef struct gifimage	//10
{
	BYTE m_byBegin; //图像标识符开始，固定值为‘，’
	WORD m_wLeft;	//X轴方向偏移量
	WORD m_wTop;	//Y轴方向偏移量
	WORD m_wWidth;	//图像宽度
	WORD m_wDepth;	//图像高度
	struct localflag
	{
		BYTE PalBits : 3;	//局部颜色列表大小，pixel+1为颜色列表的位数
		BYTE Reserved : 2;	//保留，必须初始化为0
		BYTE SortFlag : 1;	//分类标志，如果置位表示紧跟着的局部颜色列表分类排列
		BYTE Interlace : 1;	//交织标志，置位时图像数据使用交织方式排列，否则使用顺序排列
		BYTE LocalPal : 1;	//局部颜色列表标志，置位时表示紧接着在图像标识符后面有一个局部颜色列表
	} m_LocalFlag;
} GIFIMAGE;
//图形控制扩展块
typedef struct gifcontrol
{
	BYTE m_byExtensionIntroducer;	//标识这是一个扩展块，固定值为0x21
	BYTE m_byGraphicControlLabel;	//图形控制扩展标签，标识这是一个图形控制扩展块，固定值0xF9
	BYTE m_byBlaockSize;			//块大小，不包括终结器，固定值4
	struct flag
	{
		//BYTE Transparency : 1;		//透明颜色标志，置位时表示使用透明颜色
		BYTE UserInput : 1;			//用户输入标志，指是否期待用户输入之后继续进行下去置位表示期待用户输入，置否表示不期待用户输入
		BYTE DisposalMethod : 3;
			//0:不使用处置方式
			//1:不处置图形，把图形从当前位置移去
			//2:回复到背景色
			//3:回复到先前状态
			//4-7:自定义
		BYTE Reserved : 3;			//保留
	} m_Flag;
	WORD m_wDelayTime;				//延迟时间，单位0.01秒
	BYTE m_byTransparencyIndex;		//透明颜色索引
	BYTE m_byTeminator;				//块终结器
} GIFCONTROL;


typedef struct pen
{
	int m_style;		//画笔样式
	int m_width;		//画笔宽度
	COLORREF m_color;	//画笔颜色
} PEN;


class CGif
{
public:
	CGif();
	~CGif();
public:
	// 得到gif编码
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	// bmp图片转成Gif图片
	static bool GetGifPic(CString strBmpPathName, CString strGifPathName);
	// 合并多张gif生成动态图
	static void CombineGif(CFile* pFile, CString& strgifName, WORD beginSize);
};

