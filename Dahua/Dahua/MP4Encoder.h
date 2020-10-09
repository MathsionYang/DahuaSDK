#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include "mp4v2\mp4v2.h"
#include "dhnetsdk.h"
using namespace std;

#define BUFFER_SIZE  (1024*1024)
#define VIDEO_TIME_SCALE 90000		// ��Ƶÿ���ticks��
#define VIDEO_WIDTH 1920				// ��Ƶ���
#define VIDEO_HEIGHT 1080		    // ��Ƶ�߶�

// NALU��Ԫ
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
	bool getConvertMP4File(BYTE *pBuffer, DWORD dwBufSize, int dwDataType);							    // ����H264����ΪMP4�ļ��Ľӿ�
	void getMP4FileClose();																				// �ر�MP4�ļ��Ľӿ�

private:
	string GetSystemTime();																				// ��ȡϵͳ��ǰʱ��
	MP4FileHandle CreateMP4File();																		// ����mp4�ļ�
	int WriteH264Data(MP4FileHandle MP4File, const unsigned char* pData, int size,int dwDataType);     // ��H264����д��MP4�ļ�
	static int ReadOneNaluFromBuf(const unsigned char *pBuffer, unsigned int nBufferSize, unsigned int offSet, MP4ENC_NaluUnit &nalu); // ��H264���ݻ�������ȡһ��nalu
	
private:
	MP4FileHandle mFile;			// MP4�ļ����
	MP4TrackId mVideoId;			// viodeo��trackID
		
};


