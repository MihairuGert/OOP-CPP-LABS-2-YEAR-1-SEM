#include "FileHandler.h"

FileHandler::FileHandler(std::string& inputFilename, std::string& outputFilename) : inputFilename(inputFilename), outputFilename(outputFilename) {
    WAVHeaderParser inWavHeaderParser;
    try {
        inWavHeaderParser.parseWAV(inputFilename);
    }
    catch (const char* error_message) {
        std::cerr << error_message << '\n';
    }
    inHeaderEnd = inWavHeaderParser.getHeaderSize();

    areFilesSame = inputFilename == outputFilename;
    if (!areFilesSame) {
        in.open(inputFilename, std::ios::binary);
    }
    out.open(outputFilename, std::ios::binary | std::ios::in | std::ios::out);

    bool wasOutOpen = out.is_open() && out.peek() != -1;
    WAVHeaderParser outWavHeaderParser;
    if (wasOutOpen) {
        try {
            outWavHeaderParser.parseWAV(outputFilename);
        }
        catch (const char *error_message) {
            std::cerr << error_message << '\n';
        }
        outHeaderEnd = outWavHeaderParser.getHeaderSize();
    }
    else {
        out.open(outputFilename, std::ios::binary | std::ios::out);
        canBeMixed = false;
    }

    if (!wasOutOpen) {
        WAVHeaderWriter wavHeaderWriter = WAVHeaderWriter(inWavHeaderParser);
        wavHeaderWriter.writeWavHeader(out);
    }
    // attention
    out.seekg(inHeaderEnd, std::ios::beg);
    inSubchunk2Size = inWavHeaderParser.getSubchunk2Size();
    if (wasOutOpen)
        outSubchunk2Size = outWavHeaderParser.getSubchunk2Size();
    sampleRate = inWavHeaderParser.getSampleRate();
}

FileHandler::~FileHandler() {
    if (!areFilesSame)
        in.close();
    out.close();
}

char *FileHandler::getSecondFromIn() {
    char* buffer = new char[second];
    in.read(buffer, second);
    return buffer;
}

char *FileHandler::getSecondFromOut() {
    char* buffer = new char[second];
    out.read(buffer, second);
    return buffer;
}

void FileHandler::writeString(char *buffer, int offset) {
    out.seekp(offset, std::ios::cur);
    out.write(buffer, second);
}

int FileHandler::getSampleRate() const {
    return sampleRate;
}

bool FileHandler::isAreFilesSame() const {
    return areFilesSame;
}

bool FileHandler::isCanBeMixed() const {
    return canBeMixed;
}

void FileHandler::initialize() {
    in.seekg(inHeaderEnd, std::ios::beg);
    if (outHeaderEnd > 0)
        out.seekg(outHeaderEnd, std::ios::beg);
}

int FileHandler::getOutSize() const {
    return outSubchunk2Size;
}

int FileHandler::getInSize() const {
    return inSubchunk2Size;
}

int FileHandler::getOutStartOffset() const {
    return outHeaderEnd;
}

int FileHandler::getInStartOffset() const {
    return inHeaderEnd;
}

void FileHandler::moveWriterPointer(int offset) {
    out.seekp(offset, std::ios::cur);
}

void FileHandler::writeBinaryInFile(int value, int bytes) {
    out.write(reinterpret_cast<const char *>(&value), bytes);
}

void FileHandler::moveReaderPointer(int offset) {
    in.seekg(offset, std::ios::cur);
}
