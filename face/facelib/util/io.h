#ifndef FLIB_IO_H
#define FLIB_IO_H

#include <vector>
#include <string>

#include "facelib_global.h"

namespace flib {

using namespace std;

/**
 * @brief getCVCaptureFile
 * @param path
 * @return
 */
string FACELIBSHARED_EXPORT getCVCaptureFile(const string &path);

/**
 * @brief findFilesByRegex
 * @param[in] path - папка
 * @param[in] regexp - регулярное выражение
 * @param[out] files - файлы
 * @param onlyTarget - требование, чтобы в папке были только целевые файлы
 * Найти все файлы в исходной папке, удовлетворяющие регулярному выражению
 */

void FACELIBSHARED_EXPORT findFilesByRegex(const string &path,
                      const string &regexp,
                      std::vector<string> &files,bool onlyTarget = false);


}// flib


#endif // FLIB_IO_H
