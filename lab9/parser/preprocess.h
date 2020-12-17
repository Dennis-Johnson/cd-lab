#ifndef PREPROCESS_H_
#define PREPROCESS_H_

int removeWhitespace(const char inputPath[], const char outputPath[]);
int stripPreprocessorDirectives(const char inputPath[], const char outputPath[]);
int removeComments(const char inputPath[], const char outputPath[]);
const char *preprocess(char *path);

#endif //PREPROCESS_H_ 
