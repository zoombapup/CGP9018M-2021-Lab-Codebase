#ifndef GLERROR_H
#define GLERROR_H

// PAC Note: this code game from Morten Nobel's blog:
// https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
// but could be improved.. for instance, why not add a std::string reference for when the user includes the check
// so they can see from the output what it was checking? i.e. gl_check_error("Just tried loading model file");
// the file and line stuff are standard C? macro's 
// either way, you probably don't want this check going on in your draw loop!

void _check_gl_error(const char *file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#endif // GLERROR_H