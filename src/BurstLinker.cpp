//
// Created by succlz123 on 2018/1/30.
//

#include "BurstLinker.h"
#include "GifAnalyzer.h"

using namespace blk;

bool BurstLinker::init(const char *path, uint16_t width, uint16_t height, uint32_t loopCount, uint32_t threadNum) {
    
	outfile.open(path, std::ios::out | std::ios::binary);
	if (!outfile.is_open()) {
		return false;
	}
	m_bFile = true;
	gifEncoder = std::make_unique<GifEncoder>(outfile);
    return gifEncoder->init(width, height, 0, threadNum);
}

bool blk::BurstLinker::init(uint16_t width, uint16_t height, uint32_t loopCount/*=0*/, uint32_t threadNum/*=1*/)
{
	m_bFile = false;
	//ss.str("");
	gifEncoder = std::make_unique<GifEncoder>(ss);
	return gifEncoder->init( width, height, 0, threadNum);
}

bool BurstLinker::connect(std::vector<uint32_t> &image, uint32_t delay,
                          QuantizerType quantizerType, DitherType ditherType, int32_t transparencyOption,
                          uint16_t left, uint16_t top) {
    if (gifEncoder == nullptr) {
        return false;
    }
    std::vector<uint8_t> content;
    gifEncoder->addImage(image, delay, quantizerType, ditherType, transparencyOption, left, top, content);
	
	gifEncoder->flush(content);
    
    return true;
}

bool BurstLinker::connect(std::vector<std::vector<uint32_t>> &images, uint32_t delay,
                          QuantizerType quantizerType, DitherType ditherType, int32_t transparencyOption,
                          uint16_t left, uint16_t top) {
    if (gifEncoder == nullptr) {
        return false;
    }
    size_t size = images.size();
    std::vector<std::future<std::vector<uint8_t>>> tasks;
    for (int k = 0; k < size; ++k) {
        auto result = gifEncoder->threadPool->enqueue([=, &images]() {
            std::vector<uint8_t> content;
            auto image = images[k];
            gifEncoder->addImage(image, delay, quantizerType, ditherType, transparencyOption, left, top, content);
            return content;
        });
        tasks.emplace_back(std::move(result));
    }
	
	for (auto &task : tasks) {
		std::vector<uint8_t> result = task.get();
		gifEncoder->flush(result);
	}
   
    return true;
}

std::string BurstLinker::release() {
	std::string imgbuff;
	if (gifEncoder == nullptr)
	{
		return imgbuff;
	}
	gifEncoder->finishEncoding();
	if (m_bFile)
	{
		outfile.close();
	}
	else
	{
		//ss.flush();
	   imgbuff = ss.str();
	   //清空字符串流
	   ss.str("");
   }
   return imgbuff;
}

void BurstLinker::analyzerGifInfo(const char *path) {
    GifAnalyzer gifAnalyzer;
    gifAnalyzer.showGifInfo(path);
}
