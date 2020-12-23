#include <stdio.h>
#include <stdlib.h>
#include "preprocess.h"

int removeWhitespace(const char inputPath[], const char outputPath[])
{
    int wspace_count = 0, ch;
    FILE *fin, *fout;

    fin = fopen(inputPath, "r");
    fout = fopen(outputPath, "w");

    if (fin == NULL)
    {
        fprintf(stderr, "Couldn't open input file %s\n", inputPath);
        exit(EXIT_FAILURE);
    }
    if (fout == NULL)
    {
        fprintf(stderr, "Couldn't open output file %s\n", outputPath);
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(fin)) != EOF)
    {
        if (ch == '\t' || ch == ' ')
        {
            wspace_count++;
        }
        else
        {
            if (wspace_count > 0 && ch != '\n')
                fputc(' ', fout);

            fputc(ch, fout);
            wspace_count = 0;
        }
    }

    fclose(fin);
    fclose(fout);

    return 0;
}

int stripPreprocessorDirectives(const char inputPath[], const char outputPath[])
{

    int ch, temp;
    FILE *fin, *fout;
    fin = fopen(inputPath, "r");
    fout = fopen(outputPath, "w");

    if (fin == NULL)
    {
        fprintf(stderr, "Couldn't open input file %s\n", inputPath);
        exit(EXIT_FAILURE);
    }
    if (fout == NULL)
    {
        fprintf(stderr, "Couldn't open output file %s\n", outputPath);
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(fin)) != EOF)
    {
        if (ch == '#')
        {
            //Read and ignore chars till the end of the line.
            while ((temp = fgetc(fin)) != '\n')
                ;
        }
        else
        {
            fputc(ch, fout);
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int removeComments(const char inputPath[], const char outputPath[])
{
    FILE *fin, *fout;
    int ca, cb;

    fin = fopen(inputPath, "r");
    if (fin == NULL)
    {
        fprintf(stderr, "Cannot open input file%s\n", inputPath);
        exit(EXIT_FAILURE);
    }

    fout = fopen(outputPath, "w");
    if (fout == NULL)
    {
        fprintf(stderr, "Error opening output file%s\n", outputPath);
        exit(EXIT_FAILURE);
    }

    while ((ca = fgetc(fin)) != EOF)
    {
        if (ca == ' ')
        {
            fputc(ca, fout);
            while (ca == ' ')
                ca = fgetc(fin);
        }

        if (ca == '/')
        {
            cb = fgetc(fin);

            if (cb == '/')
            {
                while (ca != '\n')
                    ca = fgetc(fin);
                ca = fgetc(fin);
            }

            else if (cb == '*')
            {
                do
                {
                    while (ca != '*')
                        ca = fgetc(fin);
                    ca = fgetc(fin);
                } while (ca != '/');
            }

            else
            {
                fputc(ca, fout);
                fputc(cb, fout);
            }
        }

        else
            fputc(ca, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

const char *preprocess(char *path)
{
    // 1. Remove whitespace
    const char *removedWhitespacePath = "./temp/removedWS.i";
    removeWhitespace(path, removedWhitespacePath);

    // 2. Strip preprocessor directives
    const char *strippedPPDirectives = "./temp/removedPPD.i";
    stripPreprocessorDirectives(removedWhitespacePath, strippedPPDirectives);

    // 3. Remove Comments
    const char *removedCommentsPath = "./temp/removedComments.i";
    removeComments(strippedPPDirectives, removedCommentsPath);

    return removedCommentsPath;
}
