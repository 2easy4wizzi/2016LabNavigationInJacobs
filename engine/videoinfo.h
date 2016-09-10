#ifndef VIDEOINFO_H
#define VIDEOINFO_H
#include "includes.h"


//inline struct to help keep the info of the video
struct videoInfo{
    videoInfo(){}
    videoInfo(int fromId, int toId, int startIndex, int endIndex, string pathToVideo){
        _fromId = fromId;
        _toId = toId;
        _startIndex = startIndex;
        _endIndex = endIndex;
        _pathToVideo = pathToVideo;
    }
    string ToString() const
    {
        string toString =
                "fromId:" + std::to_string(_fromId) + "; " +
                "_toId:" + std::to_string(_toId) + "; " +
                "_startIndex:" + std::to_string(_startIndex) + "; " +
                "_endIndex:" + std::to_string(_endIndex) + "; " +
                "_pathToVideo:" + _pathToVideo + "; "
                ;
        return toString;
    }

    int _fromId;
    int _toId;
    int _startIndex;
    int _endIndex;
    string _pathToVideo;
    inline bool operator==(const videoInfo& rhs){
        return (_fromId == rhs._fromId) &&
               (_toId == rhs._toId) &&
               (_startIndex == rhs._startIndex) &&
               (_endIndex == rhs._endIndex) &&
               (_pathToVideo == rhs._pathToVideo) ;

    }
};

#endif // VIDEOINFO_H
