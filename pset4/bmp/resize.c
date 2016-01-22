/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    int a = atoi(argv[1]);
    if( a <= 0 || a > 100)
    {
        printf("You size multiplier is not according to the range specified");
        return 5;
    }
    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    bi.biWidth = bi.biWidth * a;
    bi.biHeight = bi.biHeight * a;
    int padding =  (4 - ((bi.biWidth/a) * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_output =  (4 - (((bi.biWidth) * sizeof(RGBTRIPLE)) % 4)) % 4;
    bi.biSizeImage = ((bi.biWidth * 3) + padding_output) * abs(bi.biHeight);
    bf.bfSize = 54 + bi.biSizeImage ;
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    int q = 54; 
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight/a); i < biHeight; i++)
    {
             fseek(inptr, q , SEEK_SET);
           for(int r = 0; r < a; r++)
           {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth/a; j++)
            {
                // temporary storage
                    RGBTRIPLE triple;
                // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE),1, inptr);    
                for(int k = 0; k < a; k++)
                {  
                           
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }      
            }

            // skip over padding, if any
            fseek(inptr, padding_output, SEEK_CUR);

            // then add it back (to demonstrate how)
             
                for (int k = 0; k < padding_output; k++)
                {
                    fputc(0x00, outptr);
                }
                fseek(inptr, q , SEEK_SET);
             
             }
             q = q + ((bi.biWidth/a) * 3 + padding);
        }     
     
     
     
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
