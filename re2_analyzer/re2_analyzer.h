// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RE2_ANALYZER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RE2_ANALYZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RE2_ANALYZER_EXPORTS
#define RE2_ANALYZER_API __declspec(dllexport)
#else
#define RE2_ANALYZER_API __declspec(dllimport)
#endif

RE2_ANALYZER_API Ire_analyzer* get_analyzer_object(void);

class re2_analyzer : public Ire_analyzer
{
public:
	re2_analyzer(){}
	~re2_analyzer(){}

	virtual std::string get_library_name(void);
	virtual UINT get_regex_size(const std::string& pattern, std::string& out_error);
};