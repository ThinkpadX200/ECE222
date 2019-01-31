/* lab5.c
 * Richard Coffey
 * rrcoffe
 * ECE 2220, Fall 2016
 *
 * Purpose: Program to read in a bmp file and perform various transformations, such as edge detecton or rotation
 *
 * Assumptions: 
 *       The file must be 24-bit color, without compression, and without
 *       a color map.
 *
 *       Some bmp files do not set the ImageSize field.  This code
 *       prints a warning but does not consider this an error since the
 *       size of the image can be calculated from other values.
 *
 * Command line argument
 *   lab5 argument filein fileout
 *
 * Bugs:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* WARNING: the header is 14 bytes, however on most systems
 * you will find that sizeof(struct Header) is 16 due to alignment
 * Thus trying to read the header with one fread may fail.  So,
 * read each member separately
 */
struct Header
{  
    unsigned short int Type;                 /* Magic identifier            */
    unsigned int Size;                       /* File size in bytes          */
    unsigned short int Reserved1, Reserved2;
    unsigned int Offset;                     /* Offset to image data, bytes */
};

struct InfoHeader
{  
    unsigned int Size;               /* header size in bytes      */
    int Width, Height;               /* Width and height of image */
    unsigned short int Planes;       /* Number of colour planes   */
    unsigned short int Bits;         /* Bits per pixel            */
    unsigned int Compression;        /* Compression type          */
    unsigned int ImageSize;          /* Image size in bytes       */
    int xResolution,yResolution;     /* Pixels per meter          */
    unsigned int Colors;             /* Number of colors         */
    unsigned int ImportantColors;    /* Important colors         */
};

const char Matrix[3][3] = 
{ 
    {  0, -1,  0 },
    { -1,  4, -1 },
    {  0, -1,  0 }
};

#define LINE 256

struct Pixel
{ 
    unsigned char Red, Green, Blue;
}; 


/*----------------------------------------------------------*/

int main(int argc, char *argv[])
{ 
    char filein[LINE];
    char fileout[LINE];
    FILE *fpout;
    FILE *fpin;
    struct InfoHeader infoheader;
    struct Header header;
    int expected_bytes;
    int error_code = 0;
    int row, column;
    int pixel_cols, pixel_rows, pixel_count;
    int pixel_colsR, pixel_rowsR; //for the rotation
    int items_found;
    struct Pixel one_pixel;
    int result;
    unsigned char new_pixel_color;
    int i,j;
    struct Pixel **pixelmatrix;
    struct Pixel **pixelmatrixout;
    struct Pixel **pixelmatrixrot;
    //int t1,t3,t7,t9; unused temp storage for matrix mult
    int t2,t4,t5,t6,t8; //temps for matrix sum
    int a; //argument assignment
    //    double **m1;


    if (argc != 4)
    {
        printf("Usage: lab5 argument filein fileout\n");
        exit(1);
    }
    strcpy(filein, argv[2]);

    if ((fpin = fopen(filein, "rb")) == NULL)
    { 
        printf("Cannot Open File. %s\n", filein);
        exit (1);
    }

    /* Read header */
    fread(&header.Type, sizeof(short int), 1, fpin);
    fread(&header.Size, sizeof(int), 1, fpin);
    fread(&header.Reserved1, sizeof(short int), 1, fpin);
    fread(&header.Reserved2, sizeof(short int), 1, fpin);
    fread(&header.Offset, sizeof(int), 1, fpin);

    printf("header.Type = %x\n", header.Type);
    printf("header.Size = %d\n", header.Size);
    printf("header.Offset = %d\n", header.Offset);

    if (header.Type != 0x4D42)
    {
        printf("This does not appear to be a bmp file: %s\n", filein);
        exit(1);
    }
    fread(&infoheader.Size, sizeof(int), 1, fpin);
    fread(&infoheader.Width, sizeof(int), 1, fpin);
    fread(&infoheader.Height, sizeof(int), 1, fpin);
    fread(&infoheader.Planes, sizeof(short int), 1, fpin);
    fread(&infoheader.Bits, sizeof(short int), 1, fpin);
    fread(&infoheader.Compression, sizeof(int), 1, fpin);
    fread(&infoheader.ImageSize, sizeof(int), 1, fpin);
    fread(&infoheader.xResolution, sizeof(int), 1, fpin);
    fread(&infoheader.yResolution, sizeof(int), 1, fpin);
    fread(&infoheader.Colors, sizeof(int), 1, fpin);
    fread(&infoheader.ImportantColors, sizeof(int), 1, fpin);

    printf("infoheader.Size = %d\n", infoheader.Size);
    printf("infoheader.Width = %d\n", infoheader.Width);
    printf("infoheader.Height = %d\n", infoheader.Height);
    printf("infoheader.Planes = %d\n", infoheader.Planes);
    printf("infoheader.Bits = %d\n", infoheader.Bits);
    printf("infoheader.Compression = %d\n", infoheader.Compression);
    printf("infoheader.ImageSize = %d\n", infoheader.ImageSize);
    printf("infoheader.xResolution = %d\n", infoheader.xResolution);
    printf("infoheader.yResolution = %d\n", infoheader.yResolution);
    printf("infoheader.Colors = %d\n", infoheader.Colors);
    printf("infoheader.ImportantColors = %d\n", infoheader.ImportantColors);

    if (header.Offset != 54)
    {
        printf("problem with offset.  Cannot handle color table\n");
        error_code +=1;
    }
    if (infoheader.Size != 40)
    {
        printf("Size is not 40, perhaps a bmp format not handled\n");
        error_code +=2;
    }
    if (infoheader.Planes != 1 || infoheader.Compression != 0)
    {
        printf("Planes or Compression format not handled\n");
        error_code +=4;
    }
    if (infoheader.Bits != 24)
    {
        printf("Only 24 bit color handled\n");
        error_code +=8;
    }
    expected_bytes = (infoheader.Width * infoheader.Height * infoheader.Bits)/8;
    if (expected_bytes != infoheader.ImageSize)
    {
        printf("Problem with image size.  Sometimes this field is not set so we will ignore the error.\n");
        error_code +=16;
    }
    if (expected_bytes + 14 + 40 != header.Size)
    {
        printf("Problem with size in header\n");
        error_code +=32;
    }
    if (infoheader.Colors != 0 || infoheader.ImportantColors != 0)
    {
        printf("Cannot handle color map\n");
        error_code +=64;
    }
    if (error_code != 0 && error_code != 16)
    {
        printf("exit with code %x\n", error_code);
        exit(EXIT_FAILURE);
    }

    printf("Reading pixels\n");

    pixel_rows = infoheader.Height;
    pixel_cols = infoheader.Width;
    pixel_count = 0;

    //malloc
    pixelmatrix = (struct Pixel **)malloc(pixel_rows * sizeof(struct Pixel *));
    for (i = 0; i < pixel_rows; i++) {
        pixelmatrix[i] = (struct Pixel *)malloc(pixel_cols*sizeof(struct Pixel));
    }

    for (row = 0; row < pixel_rows; row++)
    { 
        for (column = 0; column < pixel_cols; column++)
        { 
            items_found = fread(&one_pixel, 3, 1, fpin);
            if (items_found != 1)
            {
                printf("failed to read pixel %d at [%d][%d]\n", 
                        pixel_count, row, column);
                exit(1);
            }
            // comment out the following print after you 
            // confirm the file is being read correctly.
            //    printf("%d: R=%u, G=%u, B=%u\n", pixel_count,
            //          one_pixel.Red, one_pixel.Green, one_pixel.Blue);
            //  pixel_count++;

            pixelmatrix[row][column] = one_pixel;
        }
    }
    fclose(fpin);


    //malloc the pixel output



    pixelmatrixout = (struct Pixel **)malloc(pixel_rows * sizeof(struct Pixel *));
    for (i = 0; i < pixel_rows; i++) {
        pixelmatrixout[i] = (struct Pixel *)malloc(pixel_cols*sizeof(struct Pixel));
    }    

    //argument filtering
    if (strcmp(argv[1], "edtrunc") == 0) {
        a = 0;
    }
    else if (strcmp(argv[1], "edmag") == 0) {
        a = 1;
    }
    else if (strcmp(argv[1], "rotr") == 0) {
        a = 2;
    }
    else if (strcmp(argv[1], "rotl") == 0) {
        a = 3;
    }

    if (a == 0 || a == 1) {

        //edge cases  
        for (i = 0; i < pixel_rows; i++) {
            for (j = 0; j < pixel_cols; j++) {

                //top edge
                pixelmatrixout[i][0].Red = 0x00;
                pixelmatrixout[i][0].Green = 0x00;
                pixelmatrixout[i][0].Blue = 0x00;

                //bottom edge            
                pixelmatrixout[i][pixel_cols - 1].Red = 0x00;
                pixelmatrixout[i][pixel_cols - 1].Green = 0x00;
                pixelmatrixout[i][pixel_cols - 1].Blue = 0x00;

                //right edge
                pixelmatrixout[pixel_rows - 1][j].Red = 0x00;
                pixelmatrixout[pixel_rows - 1][j].Green = 0x00;
                pixelmatrixout[pixel_rows - 1][j].Blue = 0x00;

                //left edge
                pixelmatrixout[0][j].Red = 0x00;
                pixelmatrixout[0][j].Green = 0x00;
                pixelmatrixout[0][j].Blue = 0x00;
            }
        }



        for (i = 1; i < pixel_rows-1; i++) {
            for (j = 1; j < pixel_cols-1; j++) {

                // I decided her to elimate the 4 products that should all equl zero, howver I left them in for expansion of the project.
               // t1 = pixelmatrix[i-1][j-1].Red * Matrix[0][0];
                t2 = pixelmatrix[i-1][j].Red * Matrix[0][1];
               // t3 = pixelmatrix[i-1][j+1].Red * Matrix[0][2];
                t4 = pixelmatrix[i][j-1].Red * Matrix[1][0];
                t5 = pixelmatrix[i][j].Red * Matrix[1][1];
                t6 = pixelmatrix[i][j+1].Red * Matrix[1][2];
               // t7 = pixelmatrix[i+1][j-1].Red * Matrix[2][0];
                t8 = pixelmatrix[i+1][j].Red * Matrix[2][1];
               // t9 = pixelmatrix[i+1][j+1].Red * Matrix[2][2];


                result = (t2+t4+t5+t6+t8);

                if (a == 0) {    
                    new_pixel_color = (result & 0xFF); //0xFF doesn't do anything since the compiler only takes the least signifigant 8 bits anyways.
                }
                else if (a == 1) {
                    result = abs(result);
                    if (result > 255) {
                        result = 255;
                    }
                    new_pixel_color = result;
                }

                pixelmatrixout[i][j].Red = new_pixel_color;
            }
        }

        for (i = 1; i < pixel_rows-1; i++) {
            for (j = 1; j < pixel_cols-1; j++) {

               // t1 = pixelmatrix[i-1][j-1].Green * Matrix[0][0];
                t2 = pixelmatrix[i-1][j].Green * Matrix[0][1];
               // t3 = pixelmatrix[i-1][j+1].Green * Matrix[0][2];
                t4 = pixelmatrix[i][j-1].Green * Matrix[1][0];
                t5 = pixelmatrix[i][j].Green * Matrix[1][1];
                t6 = pixelmatrix[i][j+1].Green * Matrix[1][2];
               // t7 = pixelmatrix[i+1][j-1].Green * Matrix[2][0];
                t8 = pixelmatrix[i+1][j].Green * Matrix[2][1];
               // t9 = pixelmatrix[i+1][j+1].Green * Matrix[2][2];


                result = (t2+t4+t5+t6+t8);

                if (a == 0) {    
                    new_pixel_color = (result & 0xFF);
                }
                else if (a == 1) {
                    result = abs(result);
                    if (result > 255) { 
                        result = 255;
                    }
                    new_pixel_color = result;
                }

                pixelmatrixout[i][j].Green = new_pixel_color;
            }
        }


        for(i = 1; i < pixel_rows-1; i++) {
            for (j = 1; j < pixel_cols-1; j++) {

               // t1 = pixelmatrix[i-1][j-1].Blue * Matrix[0][0];
                t2 = pixelmatrix[i-1][j].Blue * Matrix[0][1];
               // t3 = pixelmatrix[i-1][j+1].Blue * Matrix[0][2];
                t4 = pixelmatrix[i][j-1].Blue * Matrix[1][0];
                t5 = pixelmatrix[i][j].Blue * Matrix[1][1];
                t6 = pixelmatrix[i][j+1].Blue * Matrix[1][2];
               // t7 = pixelmatrix[i+1][j-1].Blue * Matrix[2][0];
                t8 = pixelmatrix[i+1][j].Blue * Matrix[2][1];
               // t9 = pixelmatrix[i+1][j+1].Blue * Matrix[2][2];


                result = (t2+t4+t5+t6+t8);

                if (a == 0) {    
                    new_pixel_color = (result & 0xFF);
                }
                else if (a == 1) {
                    result = abs(result);
                    if (result > 255) {
                        result = 255;
                    }
                    new_pixel_color = result;
                }

                pixelmatrixout[i][j].Blue = new_pixel_color;
            }
        }
    }

    //We want each pixel to be rotated
    if (a == 2) {

        //swapping heights and widths
        pixel_rowsR = infoheader.Width;
        pixel_colsR = infoheader.Height;

        pixelmatrixrot = (struct Pixel **)malloc(pixel_rowsR * sizeof(struct Pixel *));
        for (i = 0; i < pixel_rowsR; i++) {
            pixelmatrixrot[i] = (struct Pixel *)malloc(pixel_colsR*sizeof(struct Pixel));
        } 

        //reading in a transposed matrix backwards from the last row. The tranpose rotates the matrix, and then it is mirrored horizontally to achieve "rotation" to the right.
        for (i = 0; i < pixel_rowsR; i++) {
            for (j = 0; j < pixel_colsR; j++) {
                pixelmatrixrot[i][j] = pixelmatrix[j][pixel_rowsR - i];

            }
        }
    }



    if (a == 3) {


        pixel_rowsR = infoheader.Width;
        pixel_colsR = infoheader.Height;

        pixelmatrixrot = (struct Pixel **)malloc(pixel_rowsR * sizeof(struct Pixel *));
        for (i = 0; i < pixel_rowsR; i++) {
            pixelmatrixrot[i] = (struct Pixel *)malloc(pixel_colsR*sizeof(struct Pixel));
        } 
        
        //Same deal as right rotation, except the tranposed matrix is mirrored the opposite way to account for the left "rotation"
        for (i = 0; i < pixel_rowsR; i++) {
            for (j = 0; j < pixel_colsR; j++) {
                pixelmatrixrot[i][j] = pixelmatrix[pixel_colsR-1 - j][i];

            }
        }
    }




    strcpy(fileout, argv[3]);

    fpout = fopen(fileout, "w+");

    fwrite(&header.Type, sizeof(short int), 1, fpout);
    fwrite(&header.Size, sizeof(int), 1, fpout);
    fwrite(&header.Reserved1, sizeof(short int), 1, fpout);
    fwrite(&header.Reserved2, sizeof(short int), 1, fpout);
    fwrite(&header.Offset, sizeof(int), 1, fpout);


    if (header.Type != 0x4D42)
    {
        printf("This does not appear to be a bmp file: %s\n", fileout);
        exit(1);
    }

    
    fwrite(&infoheader.Size, sizeof(int), 1, fpout);
    
    //if the arg is for a rotation, fwrite the swapped width/height into the info header
    if ( a == 2 || a == 3) {

        infoheader.Width = pixel_rows;
        infoheader.Height = pixel_cols;

        fwrite(&infoheader.Width, sizeof(int), 1, fpout);
        fwrite(&infoheader.Height, sizeof(int), 1, fpout);
    }
    else {

        fwrite(&infoheader.Width, sizeof(int), 1, fpout);
        fwrite(&infoheader.Height, sizeof(int), 1, fpout);
    }

    fwrite(&infoheader.Planes, sizeof(short int), 1, fpout);
    fwrite(&infoheader.Bits, sizeof(short int), 1, fpout);
    fwrite(&infoheader.Compression, sizeof(int), 1, fpout);
    fwrite(&infoheader.ImageSize, sizeof(int), 1, fpout);
    fwrite(&infoheader.xResolution, sizeof(int), 1, fpout);
    fwrite(&infoheader.yResolution, sizeof(int), 1, fpout);
    fwrite(&infoheader.Colors, sizeof(int), 1, fpout);
    fwrite(&infoheader.ImportantColors, sizeof(int), 1, fpout);


    //writing the bitmap image
    if (a == 0 || a == 1) {

        pixel_count = 0;
        for (row = 0; row < pixel_rows; row++) {

            for (column = 0; column < pixel_cols; column++)
            { 

                // comment out the following print after you 
                // confirm the file is being read correctly.
                //  printf("%d: R=%u, G=%u, B=%u\n", pixel_count,
                //        one_pixel.Red, one_pixel.Green, one_pixel.Blue);
                //  pixel_count++;

                one_pixel = pixelmatrixout[row][column]; 
                fwrite(&one_pixel, 3, 1, fpout);

                pixel_count++;

            }
        }
    }

    if (a == 2 || a == 3) {

        pixel_count = 0;
        for (row = 0; row < pixel_rowsR; row++) { 
            for (column = 0; column < pixel_colsR; column++) { 

                // comment out the following print after you 
                // confirm the file is being read correctly.
                //  printf("%d: R=%u, G=%u, B=%u\n", pixel_count,
                //        one_pixel.Red, one_pixel.Green, one_pixel.Blue);
                //  pixel_count++;

                one_pixel = pixelmatrixrot[row][column]; 
                fwrite(&one_pixel, 3, 1, fpout);

                pixel_count++;

            }
        }
    }

    fclose(fpout);

    //free

    for (i = 0; i < pixel_rows; i++) {
        free(pixelmatrix[i]);
    }
    free(pixelmatrix);

    for (i = 0; i < pixel_rows; i++) {
        free(pixelmatrixout[i]);
    }
    free(pixelmatrixout);

    //don't need to free this matrix unless it's created
    if (a == 2 || a == 3) {
        for (i = 0; i < pixel_rowsR; i++) {
            free(pixelmatrixrot[i]);
        }
        free(pixelmatrixrot);
    }

    return 0;
}

