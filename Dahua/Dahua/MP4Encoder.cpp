#include "MP4Encoder.h"

void MP4Encoder::getMP4FileClose()
{
	MP4Close(mFile);
}

bool MP4Encoder::getConvertMP4File(BYTE *pBuffer, DWORD dwBufSize, int dwDataType)
{
	
	CreateMP4File();
	WriteH264Data(mFile, pBuffer, dwBufSize, dwDataType);
	return true;
}

MP4Encoder::MP4Encoder()
{	
	mFile = MP4_INVALID_FILE_HANDLE;
	mVideoId = MP4_INVALID_TRACK_ID;
}


MP4Encoder::~MP4Encoder()
{
	
}


string MP4Encoder::GetSystemTime()
{
	SYSTEMTIME m_time;
	GetLocalTime(&m_time);
	char szDateTime[100] = { 0 };
	sprintf_s(szDateTime, "%02d%02d%02d%02d%02d%02d", m_time.wYear, m_time.wMonth,
		m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond);
	string time(szDateTime);
	return time;
}

MP4FileHandle MP4Encoder::CreateMP4File()
{
	if (mFile == NULL)
	{
		string s = GetSystemTime() + ".mp4";
		const char* FileName = s.c_str();
		mFile = MP4Create(FileName);
		if (mFile == MP4_INVALID_FILE_HANDLE)
		{
			cout << "Open file failed!\n";
			return 0;
		}

		if (mFile == NULL)
		{
			printf("ERROR:Create file failed!");
			return false;
		}	
		// 设置时间片
		MP4SetTimeScale(mFile, VIDEO_TIME_SCALE);
	}	
	return mFile;
}


int MP4Encoder::ReadOneNaluFromBuf(const unsigned char *pBuffer, unsigned int nBufferSize, unsigned int offSet, MP4ENC_NaluUnit &nalu)
{
	unsigned int i = offSet;
	int j = 0;
	while (i<nBufferSize)
	{//寻找第一个00 00 00 01		
		if (pBuffer[i] == 0x00 &&pBuffer[i+1] == 0x00 &&pBuffer[i+2] == 0x00 &&pBuffer[i+3] == 0x01)
		{			
			i = i + 4;
			unsigned int pos = i;
			while (pos<nBufferSize)
			{//寻找最后一个00 00 00 01
				if (pBuffer[pos] == 0x00 &&pBuffer[pos+1] == 0x00 &&pBuffer[pos+2] == 0x00 &&pBuffer[pos+3] == 0x01)
					break;
				pos++;
			}
			pos = pos + 4;
			if (pos == nBufferSize)
				nalu.size = pos - i;
			else
				nalu.size = (pos - 4) - i;
			nalu.type = pBuffer[i] & 0x1f;
			nalu.data = (unsigned char*)&pBuffer[i];
			return (nalu.size + i - offSet);
		}
		i++;
	}
	return 0;
}

int MP4Encoder::WriteH264Data(MP4FileHandle MP4File, const unsigned char* pData, int size,int dwDataType)
{
	if (MP4File == NULL)
	{
		return -1;
	}
	if (pData == NULL)
	{
		return -1;
	}
	MP4ENC_NaluUnit nalu;
	int pos = 0;
	int	len = 0;

	while (len = ReadOneNaluFromBuf(pData, size, pos, nalu))
	{
		if (nalu.type == 0x07) // sps
		{
			if (mVideoId == MP4_INVALID_TRACK_ID)
			{	// 添加h264 track 
				mVideoId = MP4AddH264VideoTrack(
					MP4File,
					VIDEO_TIME_SCALE,       // 视频每秒的ticks数（如90000）
					VIDEO_TIME_SCALE / 13,  // 视频的固定的视频帧的显示时间,公式为timeScale（90000）/fps(码率例如20f)
					VIDEO_WIDTH,			    // 视频的宽度
					VIDEO_HEIGHT,			// 视频的高度
					nalu.data[1],			// sps[1] AVCProfileIndication
					nalu.data[2],			// sps[2] profile_compat
					nalu.data[3],			// sps[3] AVCLevelIndication
					3						// 4 bytes length before each NAL unit
					);

				if (mVideoId == MP4_INVALID_TRACK_ID)
				{
					printf("add video track failed.\n");
					system("pause");
					return 0;
				}
				MP4SetVideoProfileLevel(MP4File, 0x01); //  Simple Profile @ Level 3
				MP4AddH264SequenceParameterSet(MP4File, mVideoId, nalu.data, nalu.size);
			}
		}
		else if (nalu.type == 0x08) // pps
		{
			MP4AddH264PictureParameterSet(MP4File, mVideoId, nalu.data, nalu.size);
		}
		else
		{
			int datalen = nalu.size + 4;
			unsigned char *data = new unsigned char[datalen];

			// MP4 Nalu前四个字节表示Nalu长度
			data[0] = nalu.size >> 24;
			data[1] = nalu.size >> 16;
			data[2] = nalu.size >> 8;
			data[3] = nalu.size & 0xff;
			memcpy(data + 4, nalu.data, nalu.size);
			if (dwDataType == 0)
			{
				MP4WriteSample(MP4File, mVideoId, data, datalen, MP4_INVALID_DURATION, 0, 1);
			}
			if (dwDataType == 1)
			{
				MP4WriteSample(MP4File, mVideoId, data, datalen, MP4_INVALID_DURATION, 0, 0);
			}
			delete[] data;
		}

		pos += len;
	}
	return pos;
}




