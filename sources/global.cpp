#include "global.h"

void CheckGLError()
{
    GLenum errLast = GL_NO_ERROR;

    for (;; )
    {
        GLenum err = glGetError();
        if (err == GL_NO_ERROR)
            return;

        // normally the error is reset by the call to glGetError() but if
        // glGetError() itself returns an error, we risk looping forever here
        // so check that we get a different error than the last time
        if (err == errLast)
        {
            wxLogError("OpenGL error state couldn't be reset.");
            return;
        }

        errLast = err;

        wxLogError("OpenGL error %d", err);
    }
}