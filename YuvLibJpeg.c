
#include "YuvLibJpeg.h"
#include "YuvLibLog.h"

int YUV_Jpeg_InitInfo(YuvJpegInfo* pJpegInfo,
                      int nq,
                      const int nWidth,
                      const int nHeight,
                      const char* pFileName)
{
    struct jpeg_compress_struct* pCInfo;

    if ((pJpegInfo->pJpegFile = fopen(pFileName, "wb")) == NULL)
    {
        LOG_FORMAT("fopen(%s) ERROR!", pFileName);
        return -1;
    }

    pJpegInfo->pYuvLine = (char*)malloc(nWidth*3);
    if (pJpegInfo->pYuvLine == NULL)
    {
        LOG_FORMAT("malloc(%d) ERROR!", (nWidth*3));
        return -1;
    }

    memset(pJpegInfo->pYuvLine, 0, nWidth*3);

    pCInfo = &(pJpegInfo->oCInfo);
    pCInfo->err = jpeg_std_error(&(pJpegInfo->jerr));
    jpeg_create_compress(pCInfo);
    jpeg_stdio_dest(pCInfo, pJpegInfo->pJpegFile);

    // image width and height, in pixels
    pCInfo->image_width      = nWidth;
    pCInfo->image_height     = nHeight;
    pCInfo->input_components = 3;          // # of color components per pixel
    pCInfo->in_color_space   = JCS_YCbCr;  // colorspace of input image
    jpeg_set_defaults(pCInfo);
    jpeg_set_quality(pCInfo, nq, TRUE);

    pCInfo->jpeg_color_space           = JCS_YCbCr;
    pCInfo->comp_info[0].h_samp_factor = 2;
    pCInfo->comp_info[0].v_samp_factor = 2;
    jpeg_start_compress(pCInfo, TRUE);

    return 0;
}

int YUV_Jpeg_NeedMoreData(YuvJpegInfo* pJpegInfo)
{
    return (pJpegInfo->oCInfo.next_scanline < pJpegInfo->oCInfo.image_height);
}

int YUV_Jpeg_SaveLine(YuvJpegInfo* pJpegInfo)
{
    pJpegInfo->row_pointer[0] = pJpegInfo->pYuvLine;
    jpeg_write_scanlines(&(pJpegInfo->oCInfo), pJpegInfo->row_pointer, 1);

    return 0;
}

int YUV_Jpeg_ReleaseInfo(YuvJpegInfo* pJpegInfo)
{
    struct jpeg_compress_struct* pCInfo;
    
    pCInfo = &(pJpegInfo->oCInfo);
    
    free(pJpegInfo->pYuvLine);
    
    jpeg_finish_compress( pCInfo);
    jpeg_destroy_compress(pCInfo);
    
    fclose(pJpegInfo->pJpegFile);
    
    return 0;
}

int YUV_UyvyToNv12(char* pUyvy,
                   const int width,
                   const int height,
                   char* pNv12)
{
    int i;
    int j;
    char* pNv12UV;
    
    pNv12UV = pNv12 + width*height;
    for (j=0; j<height; j++)
    {
        for (i=0; i<width; i++)
        {
            pNv12[i] = pUyvy[i*2+1];
            pNv12UV[i] = pUyvy[i*2];
        }
        pUyvy   += width*2;
        pNv12   += width;
        pNv12UV += width;
    }
}

