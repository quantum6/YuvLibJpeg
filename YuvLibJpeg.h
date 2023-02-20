#ifndef __YUV_LIB_JPEG_H__
#define __YUV_LIB_JPEG_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jpeglib.h>

typedef struct 
{
    struct jpeg_compress_struct oCInfo;
    struct jpeg_error_mgr       jerr;

    JSAMPROW row_pointer[1];
    
    FILE*  pJpegFile;
    char* pYuvLine;
} YuvJpegInfo;

int YUV_Jpeg_InitInfo(YuvJpegInfo* pJpegInfo,
                      int nq,
                      const int nWidth,
                      const int nHeight,
                      const char* pFileName);
    
int YUV_Jpeg_NeedMoreData(YuvJpegInfo* pJpegInfo);

int YUV_Jpeg_SaveLine(YuvJpegInfo* pJpegInfo);

int YUV_Jpeg_ReleaseInfo(YuvJpegInfo* pJpegInfo);

int YUV_UyvyToNv12(char* pUyvy,
                   const int width,
                   const int height,
                   char* pNv12);
                   
#endif

