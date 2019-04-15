#include "io.h"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace flib {




void findFilesByRegex(const string &path, const string &regexp, vector<string> &files, bool onlyTarget)
{
    files.clear();
    const boost::regex filter(regexp);
    boost::filesystem::directory_iterator end_itr;
    for( boost::filesystem::directory_iterator i( path ); i != end_itr; ++i )
    {
        // не файл
        if( !boost::filesystem::is_regular_file( i->status() ) ){
            if(onlyTarget == true) return;
            else continue;
        }


        boost::smatch what;

        // не удовлетворяет регулярному выраженнию
        string filename = i->path().filename().string();
        if( !boost::regex_match(filename, what, filter ) ){
            if(onlyTarget == true) return;
            else continue;
        }

        files.push_back( filename );
    }
}

// всевозможные префиксы
static const vector<string> _img_prefixes ={
    "cam_",
    "IMG",
    "IMG_",
    "RLCAFTCONF-C0_1",
    "RLCAFTCONF-C1_1",
    "RLCAFTCONF-C2_1",

};

// всевозможные расширения для файлов
static const vector<string> _img_extensions ={
    "jpg",
    "jpeg",
    "png",
    "bmp",

};

string getCVCaptureFile(const string &path)
{
    for(int i = 0; i < _img_prefixes.size(); i++){
        const string &pref = _img_prefixes[i];
        for(int j = 0; j < _img_extensions.size(); j++){
            const string &ext = _img_extensions[j];
            for(int k = 1; k < 10; k++){
                //собираем регулярное выражение: префикс + k цифр + формат картинки
                string regexp = pref + "[0-9]{" + std::to_string(k) + "}\." + ext;
                vector<string> files;
                findFilesByRegex(path,regexp,files,true);
                if(files.size() > 1){
                    // есть файлы, удовлетворяющие шаблону -> верни его для cv capture
                    return path + pref + "%0" + std::to_string(k) + "d." + ext;
                }
            }

        }
    }
    //ничего не нашли -> пустая строка
    return string();
}

}// flib
