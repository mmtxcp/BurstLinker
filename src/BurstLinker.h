//
// Created by succlz123 on 2017/11/30.
//

#ifndef BURSTLINKER_BURSTLINKER_H
#define BURSTLINKER_BURSTLINKER_H

#include <cstdint>
#include "GifEncoder.h"

namespace blk {

    class GIF_EXPORT BurstLinker {

    public:

        bool init(const char *path, uint16_t width, uint16_t height, uint32_t loopCount=0,
                  uint32_t threadNum=1);
		//生成到内存，release 返回内存字符串
		bool init(uint16_t width, uint16_t height, uint32_t loopCount=0,
			uint32_t threadNum=1);

        bool connect(std::vector<uint32_t> &image, uint32_t delay,
                     QuantizerType quantizerType= QuantizerType::Octree, DitherType ditherType= DitherType::No, int32_t transparencyOption=0,
                     uint16_t left=0, uint16_t top=0);

        bool connect(std::vector<std::vector<uint32_t >> &images, uint32_t delay,
                     QuantizerType quantizerType = QuantizerType::Octree, DitherType ditherType = DitherType::No, int32_t transparencyOption=0,
                     uint16_t left=0, uint16_t top=0);
		//如果是内存字符串流，返回内存块值
		//如果是文件，返回空字符串，并将文件保存。
		std::string release();

        void analyzerGifInfo(const char *path);

    private:

        std::unique_ptr<GifEncoder> gifEncoder;
		std::ofstream outfile;
		std::ostringstream ss;
		bool m_bFile = false;
    };

}

#endif //BURSTLINKER_BURSTLINKER_H
