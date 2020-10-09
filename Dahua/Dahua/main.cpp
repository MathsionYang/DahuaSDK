#include <iostream>
#include "dhnetsdk.h"
#include "inlcude\dhconfigsdk.h"
#include "inlcude\IvsDrawer.h"
#include "inlcude\avglobal.h"
#include "MP4Encoder.h"
#include "opencv2/opencv.hpp"
#include <opencv\cv.h>
#include <opencv\highgui.h>
using namespace std;
LLONG g_lRealHandle;
MP4Encoder* pMP4;
IplImage* YUV420_To_IplImage_Opencv(char* pYUV420, int width, int height)
{
	if (!pYUV420)
	{
		return NULL;
	}
	IplImage *yuvimage, *rgbimg, *yimg, *uimg, *vimg, *uuimg, *vvimg;
	int nWidth = width;
	int nHeight = height;
	rgbimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
	yuvimage = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 3);
	yimg = cvCreateImageHeader(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	uimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
	vimg = cvCreateImageHeader(cvSize(nWidth / 2, nHeight / 2), IPL_DEPTH_8U, 1);
	uuimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	vvimg = cvCreateImage(cvSize(nWidth, nHeight), IPL_DEPTH_8U, 1);
	cvSetData(yimg, pYUV420, nWidth);
	cvSetData(uimg, pYUV420 + nWidth * nHeight, nWidth / 2);
	cvSetData(vimg, pYUV420 + long(nWidth*nHeight*1.25), nWidth / 2);
	cvResize(uimg, uuimg, CV_INTER_LINEAR);
	cvResize(vimg, vvimg, CV_INTER_LINEAR);
	cvMerge(yimg, uuimg, vvimg, NULL, yuvimage);
	cvCvtColor(yuvimage, rgbimg, CV_YCrCb2RGB);
	cvReleaseImage(&uuimg);
	cvReleaseImage(&vvimg);
	cvReleaseImageHeader(&yimg);
	cvReleaseImageHeader(&uimg);
	cvReleaseImageHeader(&vimg);
	cvReleaseImage(&yuvimage);
	if (!rgbimg)
	{
		return NULL;
	}
	CvSize sz;
	IplImage *desc;
	sz.width = rgbimg->width*0.5;
	sz.height = rgbimg->height*0.5;
	desc = cvCreateImage(sz, rgbimg->depth, rgbimg->nChannels);
	cvResize(rgbimg, desc, CV_INTER_CUBIC);
	cvShowImage("test", desc);
	cvWaitKey(1);
	cvReleaseImage(&desc);
	return rgbimg;
}

void CALLBACK RealDataCallBackEx2(LLONG lRealHandle, DWORD dwDataType, BYTE*pBuffer, DWORD dwBufSize, tagVideoFrameParam param, LDWORD dwUser)
{
	if (lRealHandle == g_lRealHandle)
	{
		int type = param.frametype;
		switch (type)
		{
		case 0: // I 帧 
			pMP4->getConvertMP4File(pBuffer, dwBufSize, type);
			break;
		case 1: // P 帧
			pMP4->getConvertMP4File(pBuffer, dwBufSize, type);
			break;
		default:
			break;
		}
	}
	cv::Mat image = cv::Mat(500, 500, CV_8UC3, pBuffer).clone();
	cvWaitKey(0);

}

int main()
{
	BOOL g_bNetSDKInitFlag = FALSE;
	// 初始化 SDK
	g_bNetSDKInitFlag = CLIENT_Init(NULL, 0);
	if (FALSE == g_bNetSDKInitFlag)
	{
		cout << "Initialize client SDK fail;" << endl;
		return 0;
	}
	else
		cout << "Initialize client SDK done; " << endl;
	
	NET_DEVICEINFO_Ex deviceInfo = { 0 };
	int nError = 0;
	LLONG login_handle = CLIENT_LoginEx2("ip", 8686, "admin", "password", EM_LOGIN_SPEC_CAP_TCP, nullptr, &deviceInfo, &nError);
	/*登录失败*/
	if (login_handle != 0)
	{
		cout << "登录成功" << endl;
		//开启实时监视
		int nChannelID = 0; // 预览通道号
		DH_RealPlayType emRealPlayType = DH_RType_Realplay; // 实时监视
		HWND hWnd = GetConsoleWindow();
		g_lRealHandle = CLIENT_RealPlayEx(login_handle, nChannelID, hWnd, emRealPlayType);
		pMP4 = new MP4Encoder;
		if (0 == g_lRealHandle)
		{
			cout << "CLIENT_RealPlayEx: failed! Error code:" << CLIENT_GetLastError() << endl;
			system("pause");
			return 0;
		}
		else
		{
			//DWORD dwFlag = 0x00000001;
			if (FALSE == CLIENT_SetRealDataCallBackEx2(g_lRealHandle, &RealDataCallBackEx2, NULL, REALDATA_FLAG_DATA_WITH_FRAME_INFO))
			{
				cout << "CLIENT_SetRealDataCallBackEx: failed! Error code: " << CLIENT_GetLastError() << endl;
				return 0;
			}
			Sleep(5000);		
		}	
	}




	pMP4->getMP4FileClose();
	delete pMP4;
	//关闭预览
	if (CLIENT_StopRealPlayEx(g_lRealHandle))
	{
		g_lRealHandle = 0;
	};

	// 退出设备
	if (0 != login_handle)
	{
		if (FALSE == CLIENT_Logout(login_handle))
		{
			cout << "CLIENT_Logout Failed!Last Error \n" << CLIENT_GetLastError() << endl;
		}
		else
		{
			login_handle = 0;
		}
	}
	// 清理初始化资源
	if (TRUE == g_bNetSDKInitFlag)
	{
		CLIENT_Cleanup();
		g_bNetSDKInitFlag = FALSE;
	}
	return 0;
}


