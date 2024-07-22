#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

const int		FIELD_SIZE_S = 2;
const int		FIELD_SIZE_L = 4;
const int		BUFFER_SIZE = 4;

typedef struct s_wav
{
	std::ifstream						file;
	char								chunkID[BUFFER_SIZE + 1];
	uint32_t							chunkSize;
	char								format[BUFFER_SIZE + 1];
	char								subChunk1ID[BUFFER_SIZE + 1];
	uint32_t							subChunk1Size;
	uint16_t							audioFormat;
	uint16_t							numChannels;
	uint32_t							sampleRate;
	uint32_t							byteRate;
	uint16_t							blockAlign;
	uint16_t							bitsPerSample;
	char								subChunk2ID[BUFFER_SIZE + 1];
	uint32_t							subChunk2Size;
	std::vector<std::vector<int32_t>> 	data;
	
} t_wav;

t_wav*	parseWaveFile(std::string filePath)
{
	t_wav*			Data = new t_wav;
	std::fstream	sample_data_txt;

	std::memset(Data->chunkID, 0, sizeof(Data->chunkID));
	std::memset(Data->format, 0, sizeof(Data->format));
	std::memset(Data->subChunk1ID, 0, sizeof(Data->subChunk1ID));
	std::memset(Data->subChunk2ID, 0, sizeof(Data->subChunk2ID));
	Data->file.open(filePath, std::ios::binary);
	sample_data_txt.open("./data/sample_data.txt", std::ios::out | std::ios::trunc);
	if (Data->file.is_open())
	{
		auto &file = Data->file;

		// RIFF chunk
		file.read(Data->chunkID, FIELD_SIZE_L);
		file.read(reinterpret_cast<char*>(&Data->chunkSize), FIELD_SIZE_L);
		file.read(Data->format, FIELD_SIZE_L);
		// fmt sub-chunk
		file.read(Data->subChunk1ID, FIELD_SIZE_L);
		file.read(reinterpret_cast<char*>(&Data->subChunk1Size), FIELD_SIZE_L);
		file.read(reinterpret_cast<char*>(&Data->audioFormat), FIELD_SIZE_S);
		file.read(reinterpret_cast<char*>(&Data->numChannels), FIELD_SIZE_S);
		file.read(reinterpret_cast<char*>(&Data->sampleRate), FIELD_SIZE_L);
		file.read(reinterpret_cast<char*>(&Data->byteRate), FIELD_SIZE_L);
		file.read(reinterpret_cast<char*>(&Data->blockAlign), FIELD_SIZE_S);
		file.read(reinterpret_cast<char*>(&Data->bitsPerSample), FIELD_SIZE_S);
		// data sub-chunk
		file.read(Data->subChunk2ID, FIELD_SIZE_L);
		file.read(reinterpret_cast<char*>(&Data->subChunk2Size), FIELD_SIZE_L);

		uint32_t	samples = Data->subChunk2Size / Data->numChannels / (Data->bitsPerSample / 8);
		Data->data.resize(samples, std::vector<int32_t>(Data->numChannels));
		const int SIZE = Data->bitsPerSample / 8;
		for (size_t i = 0; i < samples; i++)
		{
			for (size_t ch = 0; ch < Data->numChannels; ch++)
			{
				switch (Data->bitsPerSample)
				{
					case 8:
						{
							int8_t sampleVal = 0;
							file.read(reinterpret_cast<char*>(&sampleVal), SIZE);
							Data->data[i][ch] = sampleVal;
							break;
						}
					case 16:
						{
							int16_t sampleVal = 0;
							file.read(reinterpret_cast<char*>(&sampleVal), SIZE);
							Data->data[i][ch] = sampleVal;
							break;
						}
					case 24:
						{
							int32_t sampleVal = 0;
							file.read(reinterpret_cast<char*>(&sampleVal), SIZE);
							if (sampleVal & 0x800000) {
								sampleVal |= 0xFF000000;
							}
							Data->data[i][ch] = sampleVal;
							break;
						}
					case 32:
						{
							int32_t sampleVal = 0;
							file.read(reinterpret_cast<char*>(&sampleVal), SIZE);
							Data->data[i][ch] = sampleVal;
							break;
						}
					default:
						throw std::runtime_error("Unsupported bits per sample");
				}
				sample_data_txt << std::to_string(Data->data[i][ch]) << "\n";
			}
		}
	}
	Data->file.close();
	sample_data_txt.close();
	return (Data);
}

void	printWaveData(t_wav *Data)
{
		std::cout << "||.WAV header data ||\n" << std::endl;
		std::cout << "ChunkID: " << Data->chunkID << std::endl;
		std::cout << "ChunkSize: " << Data->chunkSize << std::endl;
		std::cout << "Format: " << Data->format << std::endl << std::endl;
		std::cout << "Subchunk1ID: " << Data->subChunk1ID << std::endl;
		std::cout << "Subchunk1Size: " << Data->subChunk1Size << std::endl;
		if (Data->audioFormat == 1)
			std::cout << "AudioFormat: " << Data->audioFormat << ", (PCM) linear quantization" << std::endl;
		else
			std::cout << "AudioFormat: " << Data->audioFormat << ", probably some other form of compression" << std::endl;
		std::cout << "NumChannels: " << Data->numChannels << std::endl;
		std::cout << "SampleRate: " << Data->sampleRate << std::endl;
		std::cout << "ByteRate: " << Data->byteRate << std::endl;
		std::cout << "BlockAlign: " << Data->blockAlign << std::endl;
		std::cout << "BitsPerSample: " << Data->bitsPerSample << std::endl << std::endl;
		std::cout << "Subchunk2ID: " << Data->subChunk2ID << std::endl;
		std::cout << "Subchunk2Size: " << Data->subChunk2Size << std::endl;
		uint32_t samples = (Data->subChunk2Size / Data->numChannels / (Data->bitsPerSample / 8));
		uint32_t seconds = samples / Data->sampleRate;
		std::cout << "Time: " << seconds / 60 << " minutes and " << seconds % 60 << " seconds" << std::endl;
}

int main(int argc, char *argv[])
{
	std::string 		filePath;
	t_wav*				Data;

	if (argc == 1) {
		std::cout << "Welcome to the WAV decoder. Please provide path to a .WAV file: " << std::endl;
		std::cin >> filePath;
		std::cout << "Given path: " << filePath << std::endl;
	} else if (argc == 2) {
		std::cout << "Welcome to the WAV decoder. " << argv[1] << " will be decoded.\n" << std::endl;
		filePath = argv[1];
	}
	try {
		Data = parseWaveFile(filePath);
		printWaveData(Data);
	} catch (const std::runtime_error &e) {

		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}
	delete Data;
	return (0);
}
