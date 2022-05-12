//
// file:            main_jsonpacker_test01.cpp
// path:			src/tests/main_jsonpacker_test01.cpp
// created on:		2022 May 11
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#include <jsonpacker/packer.hpp>
#include <iostream>
#include <sstream>

// ../../../conffiles/test_json_input01.txt ../../../.tests/test_json_out01.txt

static const char*const s_testJsonString =
        "{ \"key1\":\"value\", \"key2\":42}\n"
        "{ \"key1\":\"value\", \"key2\":42, \"key3\":true}\n"
        "{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}\n";


int main(int a_argc, char* a_argv[])
{

    bool bPackReturn;

    std::cout << "Number of args: "<<a_argc<<std::endl;

    switch(a_argc){
    case 1:{
        ::std::stringstream aStrStream(std::string(s_testJsonString),std::ios_base::in);
        bPackReturn = jsonpacker::packer::Pack( aStrStream,"test_json_out01.txt");
    }break;
    case 2:
        bPackReturn = jsonpacker::packer::Pack( ::std::cin,a_argv[1]);
        break;
    default:
        bPackReturn = jsonpacker::packer::Pack(a_argv[2],a_argv[1]);
        break;
    }

    std::cout << "pack function returned: "<<(bPackReturn?"true":"false")<<std::endl;

    if(!bPackReturn){
        ::std::cerr<<"Unable to pack data\n";
        return 2;
    }


    return 0;
}
