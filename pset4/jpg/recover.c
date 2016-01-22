/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    FILE* file = fopen("card.raw", "r");
    uint8_t buffer[513];
    if(file == NULL)
    {
        return 1;
    }
    else if(file != NULL)
    {
        int i = 0;
        int j = 0;
        char title[20]; 
        FILE* image;
        while(!feof(file))
        {
            
            if(fread(buffer, 512, 1, file) >= 1)
            {
                if( (buffer[0] ==0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 ||  buffer[3] == 0xe1 )) )
                  {
                    if(j > 0)
                    {
                        fclose(image);
                        
                    }
                    sprintf(title, "%03d.jpg", i);
                    image = fopen(title, "a");
                    fwrite(buffer, 512, 1, image);
                    //buffer = 0;
                    i++;
                    j++;
                  }
                 else if(j > 0)
                 {
                   fwrite(buffer, 1, 512, image);
                   
                 }
            
            }
            
        }
            fclose(image);
           
            fclose(file);
    
    }
    
}    
