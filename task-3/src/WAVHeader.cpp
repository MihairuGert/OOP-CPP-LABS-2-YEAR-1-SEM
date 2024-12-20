#include "WAVHeader.h"

int WAVHeader::binaryStringToInt(char* string, int bytes, bool isBigEndian) const noexcept {
    int res = 0;
    for (int i = 0; i < bytes - 1; i++) {
        res |= (unsigned char) string[isBigEndian ? bytes - 1 - i : i];
        res <<= 8;
    }
    res |= (unsigned char) string[(bytes - 1) * !isBigEndian];
    return res;
}

int WAVHeader::getHeaderSize() const noexcept {
    return headerSize;
}

WAVHeaderWriter::WAVHeaderWriter(const WAVHeaderParser& wavHeaderParser) {
    chunkId = wavHeaderParser.chunkId;
    chunkSize = wavHeaderParser.chunkSize;
    format = wavHeaderParser.format;
    subchunk1Id = wavHeaderParser.subchunk1Id;
    subchunk1Size = wavHeaderParser.subchunk1Size;
    audioFormat = wavHeaderParser.audioFormat;
    numChannels = wavHeaderParser.numChannels;
    sampleRate = wavHeaderParser.sampleRate;
    byteRate = wavHeaderParser.byteRate;
    blockAlign = wavHeaderParser.blockAlign;
    bitsPerSample = wavHeaderParser.bitsPerSample;
    subchunk2Id = wavHeaderParser.subchunk2Id;
    subchunk2Size = wavHeaderParser.subchunk2Size;
}

WAVHeader::WAVHeader() {}

int WAVHeader::getSampleRate() const noexcept {
    return sampleRate;
}

int WAVHeader::getSubchunk2Size() const noexcept {
    return subchunk2Size;
}

void WAVHeaderParser::parseWAV(std::string filename) {
    file.open(filename, std::ios::binary);
    char buffer[BUFFER_SIZE + 1];
    buffer[BUFFER_SIZE] = '\0';

    file.read(buffer, 4);
    chunkId = buffer;
    if (strcmp(buffer, "RIFF") != 0) {
        std::cerr << "Corrupted wave file";
        throw ExceptionMSG("incorrect_chunk_id");
    }

    file.read(buffer, 4);
    chunkSize = buffer;

    file.read(buffer, 4);
    format = buffer;
    if (strcmp(buffer, "WAVE") != 0) {
        std::cerr << "Corrupted wave file";
        throw ExceptionMSG("incorrect_format");
    }

    file.read(buffer, 4);
    subchunk1Id = buffer;

    file.read(buffer, 4);
    subchunk1Size = binaryStringToInt(buffer, 4, true);
    if (subchunk1Size != 16) {
        std::cerr << "Corrupted wave file";
        throw ExceptionMSG("not_pcm_format");
    }

    file.read(buffer, 2);
    audioFormat = binaryStringToInt(buffer, 2, true);
    if (audioFormat != 1) {
        std::cerr << "Only files without compression are supported";
        throw ExceptionMSG("must_be_no_compress");
    }

    file.read(buffer, 2);
    numChannels = binaryStringToInt(buffer, 2, true);
    if (numChannels != 1) {
        std::cerr << "Only mono-channel files are supported";
        throw ExceptionMSG("must_be_mono_channel");
    }

    file.read(buffer, 4);
    sampleRate = binaryStringToInt(buffer, 4, true);
    if (sampleRate != 44100) {
        std::cerr << "Only files with 44100 hz sample rate are supported";
        throw ExceptionMSG("must_be_44100_hz");
    }

    file.read(buffer, 4);
    byteRate = binaryStringToInt(buffer, 4, true);

    file.read(buffer, 2);
    blockAlign = binaryStringToInt(buffer, 2, true);

    file.read(buffer, 2);
    bitsPerSample = binaryStringToInt(buffer, 2, true);
    if (bitsPerSample != 16) {
        std::cerr << "Only files with 16 bits per sample are supported";
        throw ExceptionMSG("must_be_16_bits_per_sample");
    }

    file.read(buffer, 4);
    subchunk2Id = buffer;
    if (!strcmp(buffer, "LIST")) {
        file.read(buffer, 4);
        subchunk2Size = binaryStringToInt(buffer, 4, true);
        headerSize += subchunk2Size;
        file.seekg(subchunk2Size, std::ios::cur);
        file.read(buffer, 4);
    }
    if (strcmp(buffer, "data") != 0) {
        std::cerr << "Corrupted wave file";
        throw ExceptionMSG("corrupted_file");
    }
    subchunk2Id = buffer;
    file.read(buffer, 4);
    subchunk2Size = binaryStringToInt(buffer, 4, true);
    headerSize = static_cast<int>(file.tellg());
    file.close();
}

void WAVHeaderWriter::writeBinaryInFile(std::ostream &out, int value, int bytes) {
    out.write(reinterpret_cast<const char*>(&value), bytes);
}

void WAVHeaderWriter::writeWavHeader(std::ostream &out) {
    out << chunkId;
    out << chunkSize;
    out << format;
    //
    out << subchunk1Id;
    writeBinaryInFile(out, subchunk1Size, 4);
    writeBinaryInFile(out, audioFormat, 2);
    writeBinaryInFile(out, numChannels, 2);
    writeBinaryInFile(out, sampleRate, 4);
    writeBinaryInFile(out, byteRate, 4);
    writeBinaryInFile(out, blockAlign, 2);
    writeBinaryInFile(out, bitsPerSample, 2);
    out << subchunk2Id;
    writeBinaryInFile(out, subchunk2Size, 4);
}
