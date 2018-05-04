#ifndef IOUTIL_H
#define IOUTIL_H


#include <QString>


class IOUtil
{

public:

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    /**
     * @brief getForegroundProcNameWin32
     * Gets the path and name of the foreground or focused process.
     * @param useCursorPosition (OPTIONAL) Default is true for foreground process to be determined by being under current cursor position.
     *                          Set false to simply get foreground window instead to determine foreground process.
     * @return The foreground process path and name string.
     */
    static QString getForegroundProcNameWin32(bool useCursorPosition=true);
    #endif

private:

    // Private Constructor for static utility class.
    IOUtil();
    IOUtil& operator=(const IOUtil &rhs);
};

#endif // IOUTIL_H
