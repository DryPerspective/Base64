
#include <string>

#include "base64.h"

namespace base64{


	std::string encode(const std::string& in){

		static const char lookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		static const char padding = '=';

		std::string result;
		result.reserve(((in.size()/3) + (in.size() % 3 > 0)) * 4);

		//Encoding is done in threes. We take our first character (8 bits in size), and shift it 16 bits to the left
		//The second is shifted 8 bits to the left, and the third is not shifted at all.
		//This results in a 24-bit block of data which is then stored across four six-bit "segments"
		unsigned long temp = 0;
		std::string::const_iterator cursor = in.begin();
		for(std::size_t i = 0; i < in.size()/3; ++i){

			temp =  (*cursor++) << 16;
			temp += (*cursor++) << 8;
			temp += (*cursor++);

			//Note: 0x3F = 63 = 0011'1111, i.e. 6 bits in a row.
			result.append(1, lookup[ (temp & (0x3F << 18)) >> 18  ]); 	//Bits 23-18
			result.append(1, lookup[ (temp & (0x3F << 12)) >> 12  ]); 	//Bits 17-12
			result.append(1, lookup[ (temp & (0x3F << 6))  >> 6   ]);   //Bits 11-6
			result.append(1, lookup[ (temp & 0x3F) 				  ]);   //Bits 5-1
		}
		//Then deal with any remaining bits if the size of the input isn't exactly divisble by 3
		switch(in.size() % 3){
		case 1:
		   temp = (*cursor++) << 16;
		   result.append(1, lookup[ (temp & (0x3F << 18)) >> 18 ]); 	//Bits 23-18
		   result.append(1, lookup[ (temp & (0x3F << 12)) >> 12 ]); 	//Bits 17-12
		   //Then add two padding chars
		   result.append(2, padding);
		   break;
		case 2:
			temp = (*cursor++) << 16;
			temp += (*cursor++) << 8;
			result.append(1, lookup[ (temp & (0x3F << 18)) >> 18 ]); 	//Bits 23-18
			result.append(1, lookup[ (temp & (0x3F << 12)) >> 12 ]); 	//Bits 17-12
			result.append(1, lookup[ (temp & (0x3F << 6)) >> 6   ]);  	//Bits 11-6
			//Add one padding char
			result.append(1,padding);
			break;
		}
		return result;
	}

	std::string decode(const std::string& in){

		short padding = 0;
		if (in.length())
		{
			if (in[in.length()-1] == '=')
				++padding;
			if (in[in.length()-2] == '=')
				++padding;
		}



		//Big table of our six-bit representation of each digit we wish to return
		static const char decode[] =
		{
		0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
		0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
		0x64, 0x64, 0x64, 0x64, 0x64, 0x3E, 0x64, 0x64, 0x64, 0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,
		0x3D, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
		0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x64, 0x64, 0x64, 0x64,
		0x64, 0x64, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
		0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x64, 0x64, 0x64, 0x64, 0x64  };

		std::string result;
		result.reserve(((in.length()/4)*3) - padding);
		std::string::const_iterator cursor = in.begin();
		unsigned long temp;
		//In principle, we want to run length()/4 times to cover a full string. However, if there is padding then running
		//length()/4 times would process one too many times and give us UB and corrupt data.
		for(std::size_t i = 0, upperBound = (in.length() / 4); i < ((padding) ? upperBound -1 : upperBound); ++i){
			//Fill our temporary with the four-part "block" of encoded data
			//Static cast as some compilers issue warnings when accessing an array with a char as an index.
			temp =  decode[static_cast<unsigned char>(*cursor++)] << 18;
			temp += decode[static_cast<unsigned char>(*cursor++)] << 12;
			temp += decode[static_cast<unsigned char>(*cursor++)] << 6;
			temp += decode[static_cast<unsigned char>(*cursor++)];

			//NB: 0xFF = 255 = 1111'1111
			result.append(1, (temp >> 16) & 0xFF);
			result.append(1, (temp >> 8)  & 0xFF);
			result.append(1, temp & 0xFF);
		}
		switch(padding){
		case 1:
			//Grab two full characters and part of the data for the padding
			temp =  decode[static_cast<unsigned char>(*cursor++)] << 18;
			temp += decode[static_cast<unsigned char>(*cursor++)] << 12;
			temp += decode[static_cast<unsigned char>(*cursor++)] << 6;
			//Ignore the padding and just add the two characters.
			result.append(1, (temp >> 16) & 0xFF);
			result.append(1, (temp >> 8)  & 0xFF);
            break;

        case 2:
            //Grab one full character and part of the data for the padding
			temp =  decode[static_cast<unsigned char>(*cursor++)] << 18;
			temp += decode[static_cast<unsigned char>(*cursor++)] << 12;

			//Ignore the padding and just add the character.
			result.append(1, (temp >> 16) & 0xFF);
		    break;
        }

		return result;

	}

}

