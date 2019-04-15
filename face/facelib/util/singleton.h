#ifndef FLIB_SINGLETON_H
#define FLIB_SINGLETON_H

#include "facelib_global.h"

namespace flib {

/**
 * @brief The Singleton class
 * Одиночка
 */
template <class T>
class FACELIBSHARED_EXPORT Singleton {
public:
    static T& instance() {
        static T instance;
        return instance;
    }

private:
    Singleton();
    ~Singleton();
    Singleton(const Singleton &);
    Singleton& operator=(const Singleton &);
};

}


#endif // FLIB_SINGLETON_H
