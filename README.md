# json_packer_for_nxlog
  
Sources to pack JSON data to binary stream.  
  
## Introductio  
  
### Geting sources  
  
``` bash  
git clone ${urlToThisRepo}
cd json_packer_for_nxlog
git submodule update --init --recursive
```  
  
  
### To compile  
  
```bash  
make QT_ROOT_DIR=~/Qt/5.15.2/gcc_64 JSONPACKER_DEBUG=1  # to compile debug version (with debugging information) 
make QT_ROOT_DIR=~/Qt/5.15.2/gcc_64   # to compile release version  
```  
  
Two variables (`QT_ROOT_DIR` and `JSONPACKER_DEBUG`) can be declared as environment variables (instead of providing as arguments)  
  
  
### About content  
  
Developed application to prepare JSON stream binary packer. See [description](docs/NXLog_C++_test1.pdf). 
For TLV implementation [Jhuster's TLV](https://github.com/Jhuster/TLV) is used.  
For parsing JSON (getting keys and values) [QT](https://www.qt.io/) is used.  
The APIs provided in the freamwork of this task are [following](include/jsonpacker/packer.hpp):  
  
```cpp  
namespace jsonpacker{ namespace packer{


JSONPACKER_EXPORT bool Pack(const char* a_inpFileName, const char* a_outFileName);

template <typename CharIn, typename CharOut>
JSONPACKER_EXPORT bool Pack(::std::basic_istream<CharIn>& a_inp, ::std::basic_ostream<CharOut>& a_out);

template <typename CharIn>
JSONPACKER_EXPORT bool Pack(::std::basic_istream<CharIn>& a_inp, const char* a_outFileName);

template <typename CharOut>
JSONPACKER_EXPORT bool Pack(const char* a_inpFileName, ::std::basic_ostream<CharOut>& a_out);


}}  // namespace jsonpacker{ namespace packer{
```  
  
There is a simple test program to demonstrate the usage of the APIs. The main of this program is something like
[this](src/tests/main_jsonpacker_test01.cpp).  
  
```cpp  
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
```  
  
Maybe in the future, this test can be replaced by unit tests (for example using google tests)
