//
//  ImageLoader.cpp
//  D2DEngine
//
//  Created by dai on 15/8/3.
//  Copyright (c) 2015年 dai. All rights reserved.
//

#include "ImageLoader.h"
#include "png.h"
#include "pngconf.h"
#include "pnglibconf.h"
#include <cstdlib>
#include "Texture2D.h"
#include "FileUtil.h"
#include "Data.h"
#include <assert.h>

namespace D2D {
#define PNG_READ_HEADER 4
    
    int read_chunk_callback(png_structp ptr,png_unknown_chunkp chunk)
    {
        return 1;
    }
    
    void read_row_callback(png_structp ptr,png_uint_32 row,int pass)
    {
        
    }
    
    int png_read(const char* filename,png_datap data)
    {
        FILE *fp = fopen(filename,"rb");
        if(fp == NULL) return PNG_ERROR_NO_FILE;
        
        fseek(fp,0,SEEK_END);
  //      int fSize = (int)ftell(fp);
        rewind(fp);
        unsigned char* header = (unsigned char*)malloc(PNG_READ_HEADER);
        fread(header,1,PNG_READ_HEADER,fp);
        int is_png = !png_sig_cmp(header,0,PNG_READ_HEADER);
        free(header);
        if(!is_png) return PNG_ERROR_NOT_PNG;
        
        png_structp png_ptr;
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
        if(!png_ptr) return PNG_ERROR_CREATE_PNG_STRUCT;
        
        png_infop info_ptr;
        info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr)
        {
            printf("create info struct failed!\n");
            png_destroy_read_struct(&png_ptr,NULL,NULL);
            return PNG_ERROR_CREATE_INFO_STRUCT;
        }
        png_infop end_info;
        end_info = png_create_info_struct(png_ptr);
        if(!end_info)
        {
            printf("create end info failed!\n");
            png_destroy_read_struct(&png_ptr,NULL,NULL);
            return PNG_ERROR_CREATE_INFO_STRUCT;
        }
        if(setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);
            return PNG_ERROR_JMP;
        }
        png_init_io(png_ptr,fp);
        png_set_sig_bytes(png_ptr,PNG_READ_HEADER);
        // png_voidp user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
        // png_set_read_user_chunk_fn(png_ptr,user_chunk_ptr,read_chunk_callback);
        // png_set_keep_unknown_chunks(png_ptr,PNG_HANDLE_CHUNK_NEVER,NULL,0);
        
        //png_set_read_status_fn(png_ptr,read_row_callback);
        
        //low lever read
        png_read_info(png_ptr,info_ptr);
        png_uint_32 width;
        png_uint_32 height;
        int bit_depth;
        int color_type;
        png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,NULL,NULL,NULL);
        // printf("width:%d,height:%d,bit_depth:%d,color_type:%d",width,height,bit_depth,color_type);
        
        // for default  format = RGBA8888
        png_bytep raw_data = (png_bytep)malloc(width*height*4*sizeof(png_byte));
        // printf("row bytes:%d\n",png_get_rowbytes(png_ptr,info_ptr));
        
        png_bytep *row_pointers = (png_bytep*)malloc(height*sizeof(png_bytep));
        for(int i=0;i<height;i++)
        {
            row_pointers[i] = raw_data+ i* (width * 4);
        }
        png_read_image(png_ptr,row_pointers);
        
        data->width = width;
        data->height = height;  
        data->data = (char*)raw_data;
        data->format = Texture2D::PixelFormat::RGBA8888;
        // printf("png_read_success\n");  
        
        //clean  
        fclose(fp);  
        png_destroy_read_struct(&png_ptr,&info_ptr,&end_info);  
        free(row_pointers);  
        
        return PNG_READ_SUCCESS;  
    }
    
    //---------------------------------------------------------
    /*
     * convert 24bit RGB888 to 16bit RGB565 color format
     */
    unsigned short RGB888toRGB565(unsigned char red, unsigned char green, unsigned char blue){
        unsigned short  B = (blue >> 3) & 0x001F;
        unsigned short  G = ((green >> 2) << 5) & 0x07E0;
        unsigned short  R = ((red >> 3) << 11) & 0xF800;
        return (unsigned short) (R | G | B);
    }
    
    //--------------------------------------------------------
    struct MyErrorMgr
    {
        struct jpeg_error_mgr pub;	/* "public" fields */
        jmp_buf setjmp_buffer;	/* for return to caller */
    };
    typedef struct MyErrorMgr * MyErrorPtr;
    
    /*
     * Here's the routine that will replace the standard error_exit method:
     */
    
    METHODDEF(void)
    myErrorExit(j_common_ptr cinfo)
    {
        /* cinfo->err really points to a MyErrorMgr struct, so coerce pointer */
        MyErrorPtr myerr = (MyErrorPtr) cinfo->err;
        
        /* Always display the message. */
        /* We could postpone this until after returning, if we chose. */
        /* internal message function cann't show error message in some platforms, so we rewrite it here.
         * edit it if has version confilict.
         */
        //(*cinfo->err->output_message) (cinfo);
        char buffer[JMSG_LENGTH_MAX];
        (*cinfo->err->format_message) (cinfo, buffer);
        printf("jpeg error: %s\n", buffer);
        
        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }
    
    int jpeg_read(const char* filename,jpeg_datap result){
  
        auto jpegData = D2D::FileUtil::getInstacne()->getDataFromFile(filename,true);
        assert(!jpegData.isNull());
        
        /* these are standard libjpeg structures for reading(decompression) */
        struct jpeg_decompress_struct cinfo;
        /* We use our private extension JPEG error handler.
         * Note that this struct must live as long as the main JPEG parameter
         * struct, to avoid dangling-pointer problems.
         */
        struct MyErrorMgr jerr;
        /* libjpeg data structure for storing one row, that is, scanline of an image */
        JSAMPROW row_pointer[1] = {0};
        
        unsigned long location = 0;
        unsigned int i = 0;
        
        /* We set up the normal JPEG error routines, then override error_exit. */
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = myErrorExit;
        /* Establish the setjmp return context for MyErrorExit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            printf("decompress jpeg : %s error.\n",filename);
            abort();
            return -1;
        }
        
        /* setup decompression process and source, then read JPEG header */
        jpeg_create_decompress( &cinfo );
        
        jpeg_mem_src( &cinfo, const_cast<unsigned char*>(jpegData.getBytes()),jpegData.getSize());
        
        /* reading the image header which contains image information */
#if (JPEG_LIB_VERSION >= 90)
        // libjpeg 0.9 adds stricter types.
        jpeg_read_header( &cinfo, TRUE );
#else
        jpeg_read_header( &cinfo, true );
#endif
        
        // we only support RGB or grayscale
        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
        {
            result->format = Texture2D::PixelFormat::I8;
        }else
        {
            cinfo.out_color_space = JCS_RGB;
            result->format = Texture2D::PixelFormat::RGB888;
        }
        
        /* Start decompression jpeg here */
        jpeg_start_decompress( &cinfo );
        
        /* init image info */
        result->width  = cinfo.output_width;
        result->height = cinfo.output_height;
        
        row_pointer[0] = static_cast<unsigned char*>(malloc(cinfo.output_width*cinfo.output_components * sizeof(unsigned char)));
        
        ssize_t dataLen = cinfo.output_width * cinfo.output_height*cinfo.output_components;
        result->data = static_cast<unsigned char*>(malloc(dataLen * sizeof(unsigned char)));
        
        /* now actually read the jpeg into the raw buffer */
        /* read one scan line at a time */
        while( cinfo.output_scanline < cinfo.output_height ){
            
            jpeg_read_scanlines( &cinfo, row_pointer, 1 );
            for( i=0; i<cinfo.output_width*cinfo.output_components;i++)
            {
                result->data[location++] = row_pointer[0][i];
            }
        }
        
        /* When read image file with broken data, jpeg_finish_decompress() may cause error.
         * Besides, jpeg_destroy_decompress() shall deallocate and release all memory associated
         * with the decompression object.
         * So it doesn't need to call jpeg_finish_decompress().
         */
        //jpeg_finish_decompress( &cinfo );
        jpeg_destroy_decompress( &cinfo );
        
        if (row_pointer[0] != nullptr)
        {
            free(row_pointer[0]);
        };
        
        return 0;
    }
}
