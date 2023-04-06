#ifndef PD_BASE64_ENCODEDECODE
#define PD_BASE64_ENCODEDECODE


/*
*	PUBLIC DOMAIN LICENSE
*	No Rights Reserved
*	This code is released into the public domain. You are free to use, distribute and modify it as you please, including for commercial use.
*	No license is needed, no credit needs to be given, and this license statement can be removed if you so desire. Do as you please.
*	The author offers no warranty and accepts no responsibility or liability for use and/or misuse of this code.
*/

class std::string;


namespace base64{

	std::string encode(const std::string& in);

	std::string decode(const std::string& in);

}
#endif
