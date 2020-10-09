#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "mp4v2\mp4v2.h"
#include "dhnetsdk.h"
using namespace std;

#define BUFFER_SIZE  (1024*1024)
#define VIDEO_TIME_SCALE 90000		// 视频每秒的ticks数
#define VIDEO_WIDTH 1920				// 视频宽度
#define VIDEO_HEIGHT 1080		    // 视频高度

// NALU单元
typedef struct _MP4ENC_NaluUnit
{
	int type;
	int size;
	unsigned char *data;
}MP4ENC_NaluUnit;

typedef struct _MP4ENC_Metadata
{
	// video, must be h264 type
	unsigned int	    nSpsLen;
	unsigned char	Sps[1024];
	unsigned int	    nPpsLen;
	unsigned char	Pps[1024];

} MP4ENC_Metadata, *LPMP4ENC_Metadata;

class MP4Encoder
{
public:
	MP4Encoder();
	~MP4Encoder();
public:
	bool getConvertMP4File(BYTE *pBuffer, DWORD dwBufSize, int dwDataType);							    // 保存H264裸码为MP4文件的接口
	void getMP4FileClose();																				// 关闭MP4文件的接口

private:
	string GetSystemTime();																				// 获取系统当前时间
	MP4FileHandle CreateMP4File();																		// 创建mp4文件
	int WriteH264Data(MP4FileHandle MP4File, const unsigned char* pData, int size,int dwDataType);     // 把H264裸码写入MP4文件
	static int ReadOneNaluFromBuf(const unsigned char *pBuffer, unsigned int nBufferSize, unsigned int offSet, MP4ENC_NaluUnit &nalu); // 从H264数据缓冲区读取一个nalu
	
private:
	MP4FileHandle mFile;			// MP4文件句柄
	MP4TrackId mVideoId;			// viodeo的trackID
		
};


