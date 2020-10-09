
//#include <stdio.h>
//#include <iostream>
//#include <Windows.h>
//#include "dhnetsdk.h"
//#include <dhconfigsdk.h>
//#include <IvsDrawer.h>
//#include <avglobal.h>
//#include <cstring>
//#include <winCon.h>
//#include "opencv2/opencv.hpp"
//#include <list>
//using namespace std;
//#define SWITCH 1
//#define PLAYPORT 1
//typedef struct VideoData
//{
//	char* data;
//	int width;
//	int height;
//}TVideoData; //��Ƶ���ݽṹ��
//   list<VideoData> videolist;//list�洢��Ƶ����
//
////���뺯�� ��YUV420����ΪIplImage
//IplImage* YUV420_To_IplImage_Opencv(char* pYUV420, int width, int height)
//{
//	if (!pYUV420)
//	{
//		return NULL;
//	}
//	IplImage *yuvimage, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
//	int nWidth = width;
//	int nHeight = height;
//	rgbimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
//	yuvimage = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
//	yimg = cvCreateImageHeader(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
//	uimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
//	vimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
//	uuimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
//	vvimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
//	cvSetData(yimg, pYUV420, nWidth);
//	cvSetData(uimg, pYUV420 + nWidth * nHeight, nWidth / 2);
//	cvSetData(vimg, pYUV420 + long(nWidth*nHeight*1.25), nWidth / 2);
//	cvResize(uimg, uuimg, CV_INTER_LINEAR);
//	cvResize(vimg, vvimg, CV_INTER_LINEAR);
//	cvMerge(yimg, uuimg, vvimg, NULL, yuvimage);
//	cvCvtColor(yuvimage, rgbimg, CV_YCrCb2RGB);
//	cvReleaseImage(&uuimg);
//	cvReleaseImage(&vvimg);
//	cvReleaseImageHeader(&yimg);
//	cvReleaseImageHeader(&uimg);
//	cvReleaseImageHeader(&vimg);
//	cvReleaseImage(&yuvimage);
//	if (!rgbimg)
//	{
//		return NULL;
//	}
//	CvSize sz;
//	IplImage *desc;
//	sz.width = rgbimg->width*0.5;
//	sz.height = rgbimg->height*0.5;
//	desc = cvCreateImage(sz, rgbimg->depth, rgbimg->nChannels);
//	cvResize(rgbimg, desc, CV_INTER_CUBIC);
//	cvShowImage("test", desc);
//	cvWaitKey(1);
//	cvReleaseImage(&desc);
//	return rgbimg;
//}
////�豸���߻ص�����
//void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
//{
//	printf("�豸����.\n");
//	return;
//}
////�Զ������ص�����
//void CALLBACK AutoReConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
//{
//	printf("�Զ������ɹ�.\n");
//	return;
//}
////ʵʱ�������ص����� ����ʹ�õ�NVR����ʹ����չ�ص�����
//void CALLBACK RealDataCallBackEx(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG lParam, DWORD dwUser)
//{
//	if (dwDataType == 0)  //ԭʼ��Ƶ���Ͳ��ſ�    
//	{
//		PLAY_InputData(PLAYPORT, pBuffer, dwBufSize);
//	}
//}
////����ص�����
//void CALLBACK DecCBFun(LONG nPort, char * pBuf, LONG nSize, FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved2)
//{
//	// pbuf���������YUV I420��ʽ������   
//	if (pFrameInfo->nType == 3) //��Ƶ����   
//	{
//		//���ص���ȡ��YUV420���ݷ���list���ݽṹ��
//		//���ַ�ʽ���Ա�֤�������ݲ��ᶪʧ
//		//����ʵʱ��ʾ,���Խ��ж�֡���������Ϳ��١�
//		TVideoData data;
//		data.data = (char*)malloc(sizeof(char)*nSize);
//		memcpy(data.data, pBuf, nSize);
//		data.height = pFrameInfo->nHeight;
//		data.width = pFrameInfo->nWidth;
//		videolist.push_back(data);
//	}
//	return;
//}
////�ļ��ط����ؽ��Ȼص�����
//void CALLBACK cbDownLoadPos(LLONG lPlayHandle, DWORD dwTotalSize, DWORD dwDownLoadSize, LDWORD dwUser)
//{
//	//printf("cbDownLoadPos\n");
//}
////���ݴ�����
//DWORD WINAPI DataDeal(LPVOID lpParameter)
//{
//	while (1)
//	{
//		while (videolist.size() == 0);
//		TVideoData data = videolist.front();
//		double Time = (double)cvGetTickCount();
//		YUV420_To_IplImage_Opencv(data.data, data.width, data.height);
//		Time = (double)cvGetTickCount() - Time;
//		printf("run time = %gms\n", Time / (cvGetTickFrequency() * 1000));
//		free(data.data);
//		videolist.pop_front();
//	}
//	return 0;
//}
//int main(void)
//{
//	//����һ��ʵʱ��ʾԤ��
//	//���ܶ����ļ����ػط�
//	PLAY_OpenStream(PLAYPORT, 0, 0, 1024 * 900);
//	PLAY_SetDecCallBackEx(PLAYPORT, DecCBFun, NULL);
//	PLAY_Play(PLAYPORT, NULL);
//	//���ϴ���Ϊ���ý���
//	CLIENT_LogClose();
//	NET_DEVICEINFO_Ex info_ex = { 0 };
//	int err = 0;
//	unsigned long lLogin = 0;
//	LLONG lSearch = 0;
//	LLONG lRealPlay = 0;
//	CLIENT_Init(NULL, 0);
//	CLIENT_SetAutoReconnect(AutoReConnectFunc, 0);
//	lLogin = CLIENT_LoginEx2("192.168.0.101", 37777, "admin", "kz123456", EM_LOGIN_SPEC_CAP_TCP, NULL, &info_ex, &err);
//	if (lLogin == 0)
//	{
//		printf("login error!\r\n");
//	}
//	else
//	{
//		printf("login success!\r\n");
//#if SWITCH //SWITCH �궨�壬��ͨ���޸ĸÿ����л�ʵʱ������ʾ���ļ��ص���ʾ
//		//1.ʵʱȡ����
//		lRealPlay = CLIENT_RealPlayEx(lLogin, 2, NULL, DH_RType_Realplay);
//		//CLIENT_RealPlayEx �ڶ�������ΪNVR ����ͨ�� �˴�Ϊ��ͨ����ʾ�ɸ�Ϊ��ͨ��Ԥ��
//		if (lRealPlay != 0)
//		{
//			
//			CLIENT_SetRealDataCallBackEx2(lRealPlay, RealDataCallBackEx, 0, 0x0000001f);
//		}
//		HANDLE hThread1 = CreateThread(NULL, 0, DataDeal, NULL, 0, NULL);
//#endif
//#if !SWITCH
//		//2.�ļ�ȡ�� �ط�
//		NET_RECORDFILE_INFO info = { 0 };
//		LPNET_TIME time_start = (LPNET_TIME)malloc(sizeof(LPNET_TIME));
//		LPNET_TIME time_end = (LPNET_TIME)malloc(sizeof(LPNET_TIME));
//		time_start->dwYear = 2017;
//		time_start->dwMonth = 11;
//		time_start->dwDay = 10;
//		time_start->dwHour = 9;
//		time_start->dwMinute = 10;
//		time_start->dwSecond = 0;
//		time_end->dwYear = 2017;
//		time_end->dwMonth = 11;
//		time_end->dwDay = 10;
//		time_end->dwHour = 9;
//		time_end->dwMinute = 15;
//		time_end->dwSecond = 0;
//		//���ûط�ʱ���
//		lSearch = CLIENT_FindFile(lLogin, 0, 0, NULL, time_start, time_end, FALSE, 1000);
//		//CLIENT_FindFile �ڶ�������Ϊͨ���ţ��ļ��ط�ֻ������һ��ͨ�������ļ��ط� ����������Ϊ�ļ�����
//		int result = CLIENT_FindNextFile(lSearch, &info);
//		//��ѯ�����ϲ������ļ��� �洢��info�ṹ����
//		LONG state = CLIENT_PlayBackByRecordFileEx(lLogin, &info, NULL, cbDownLoadPos, NULL, fDownLoadDataCallBack, NULL);
//		HANDLE hThread1 = CreateThread(NULL, 0, DataDeal, NULL, 0, NULL);
//#endif
//	}
//	getchar();
//	//�ͷ������  
//	CLIENT_StopRealPlay(lRealPlay);
//	CLIENT_Logout(lLogin);
//	CLIENT_Cleanup();
//	//�رղ���ͨ�����ͷ���Դ  
//	PLAY_Stop(PLAYPORT);
//	PLAY_CloseStream(PLAYPORT);
//	return 0;
//}
