#include <stdio.h>
#include <stdint.h>
#include "sio_char.h"


#ifdef __ICCARM__
  #include <yfuns.h>
#if __VER__ < 8000000
  _STD_BEGIN
#endif
  #pragma module_name = "?__write"
#endif

/*******************************************************************************
Typedef definitions
*******************************************************************************/


/*******************************************************************************
Macro definitions
*******************************************************************************/
/* File descriptor */
#ifndef __ICCARM__
  #define STDIN           (0)
  #define STDOUT          (1)
  #define STDERR          (2)
#endif

#define SIORW_SUCCESS   (0)
#define SIORW_ERROR     (-1)
#define SIORW_FLAG_OFF  (0)
#define SIORW_FLAG_ON   (1)

int _write(int file,char *ptr,int len);
int _read (int file, char *ptr, int len);

/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/


/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/


/*******************************************************************************
Private global variables and functions
*******************************************************************************/


/*******************************************************************************
* Function Name: SioWrite
* Description  : The character strings specified with buffer is output for n
*              : bytes from serial port. The output is determined by file number fileno.
*              : The effective outputs in this version are STDOUT and STDERR, and
*              : it is output to the same serial port.
*              : The linefeed code '\n'(LF) is converted in '\r'(CR)+'\n'(LF) to output.
* Arguments    : int32_t  file_no  ; I : File number to be the target of writing
*              : int_t  * buffer   ; O : Pointer to the area in which writing data is stored
*              : uint32_t writing_b; I : Writing bytes
* Return Value : >=0 : Number of transmitting characters
*              : -1  : File number error
*******************************************************************************/
#ifndef __ICCARM__
int _write(int file,char *ptr, int len)
{
    return (SioWrite(file,ptr,(uint32_t)len));
}

  int32_t SioWrite(int32_t file_no, const char_t * buffer, uint32_t writing_b)
#else
  size_t __write(int handle, const unsigned char * buffer, size_t size)
#endif
{
    uint32_t offset;

#ifndef __ICCARM__
    if ((STDOUT == file_no) || (STDERR == file_no))
    {
        for (offset = 0; offset < writing_b; offset++)
#else
    if ((_LLIO_STDOUT == handle) && (_LLIO_STDERR != handle))
    {
        for (offset = 0; offset < size; offset++)
#endif
        {
            /* Writing in buffer converting linefeed code */
            if ('\n' == *(buffer + offset))
            {
                if (0 == offset)
                {
                    IoPutchar('\r');
                }
                else
                {
                    if (*(buffer + offset - 1) != '\r')
                    {
                        IoPutchar('\r');
                    }
                }
                IoPutchar('\n');
            }
            else
            {
                IoPutchar(*(buffer + offset));
            }
        }
        return (int32_t)offset;
    }

#ifndef __ICCARM__
    return SIORW_ERROR;     /* File number error */
#else
    return _LLIO_ERROR;     /* File number error */
#endif
}

/*******************************************************************************
* Function Name: SioRead
* Description  : The character strings specified with buffer is input for
*              : n bytes from serial port.The input is determined by file number fileno.
*              : The effective input in this version is STDIN.
* Arguments    : int32_t  file_no  ; I : File number to be the target of reading
*              : int_t  * buffer   ; O : Pointer to the area in which reading data is stored
*              : uint32_t reading_b; I : Reading bytes
* Return Value : >0 : Number of receiving characters
*              : -1 : File number, receiving data error
*******************************************************************************/
#ifndef __ICCARM__
int _read (int file, char *ptr, int len)
{
    return (SioRead(file,ptr,(uint32_t)len));
}


  int32_t SioRead(int32_t file_no, char_t * buffer, uint32_t reading_b)
#else
  size_t __read(int handle, unsigned char * buffer, size_t size)
#endif
{
    int32_t        char_mem;
    int32_t        sp_char;
    uint32_t       offset;
    static int32_t sjis_flg = SIORW_FLAG_OFF;

#ifndef __ICCARM__
    if (STDIN == file_no)
    {
        for (offset = 0; offset < reading_b; )
#else
    if (_LLIO_STDIN == handle)
    {
        for (offset = 0; offset < size; )
#endif
        {
            /* Reading receiving data */
            char_mem = IoGetchar();

            if (-1 == char_mem)   /* -1 is returned when it is receiving data error */
            {
#ifndef __ICCARM__
                return SIORW_ERROR;
#else
			    return _LLIO_ERROR;
#endif
            }

            if (SIORW_FLAG_ON == sjis_flg)
            {
                sjis_flg = SIORW_FLAG_OFF;
#ifndef __ICCARM__
                SioWrite(STDOUT, (char_t *)&char_mem, 1);
                *(buffer + offset) = (char_t)char_mem;
#else
                __write(_LLIO_STDOUT, (const unsigned char *)&char_mem, 1);
                *(buffer + offset) = (unsigned char)char_mem;
#endif
                offset++;
            }
            else if ((0x20 <= char_mem) && (char_mem <= 0x7E))
            {
                /* Data possible to display */
#ifndef __ICCARM__
                SioWrite(STDOUT, (char_t *)&char_mem, 1);
                *(buffer + offset) = (char_t)char_mem;
#else
                __write(_LLIO_STDOUT, (const unsigned char *)&char_mem, 1);
                *(buffer + offset) = (unsigned char)char_mem;
#endif
                offset++;
            }
            else if ('\b' == char_mem)      /* BS process */
            {
                if (offset > 0)
                {
                    sp_char = 0x20;
#ifndef __ICCARM__
                    SioWrite(STDOUT, (char_t *)&char_mem, 1);
                    SioWrite(STDOUT, (char_t *)&sp_char, 1);
                    SioWrite(STDOUT, (char_t *)&char_mem, 1);
#else
                    __write(_LLIO_STDOUT, (const unsigned char *)&char_mem, 1);
                    __write(_LLIO_STDOUT, (const unsigned char *)&sp_char, 1);
                    __write(_LLIO_STDOUT, (const unsigned char *)&char_mem, 1);
#endif
                    offset--;
                }
            }
            else if ('\r' == char_mem)      /* CR process */
            {
                *(buffer + offset) = '\n';
#ifndef __ICCARM__
                SioWrite(STDOUT, buffer + offset, 1);
#else
                __write(_LLIO_STDOUT, (const unsigned char*)(buffer + offset), 1);
#endif
                offset++;
                break;
            }
            /* Japanese SJIS ? */
            else if (((char_mem >= 0x80) && (char_mem < 0xA0)) || ((char_mem >= 0xE0) && (char_mem < 0xFE)))
            {
                /* Data possible to display */
#ifndef __ICCARM__
                SioWrite(STDOUT, (char_t *)&char_mem, 1);
                *(buffer + offset) = (char_t)char_mem;
#else
                __write(_LLIO_STDOUT, (const unsigned char*)&char_mem, 1);
                *(buffer + offset) = (unsigned char)char_mem;
#endif
                offset++;
                sjis_flg = SIORW_FLAG_ON;
            }
            /* Character cord out of the support */
            else
            {
                /* Do Nothing */
            }
        }
        return (int32_t)offset;
    }

#ifndef __ICCARM__
    return SIORW_ERROR;     /* File number error */
#else
    return _LLIO_ERROR;     /* File number error */
#endif
}


#ifdef __ICCARM__
#if __VER__ < 8000000
  _STD_END
#endif
#endif

/* End of File */

