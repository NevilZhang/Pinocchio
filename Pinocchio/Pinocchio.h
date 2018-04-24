
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PINOCCHIO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PINOCCHIO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#ifdef _WIN32
#ifdef PINOCCHIO_EXPORTS
#define PINOCCHIO_API __declspec(dllexport)
#else //PINOCCHIO_EXPORTS
#define PINOCCHIO_API __declspec(dllimport)
#endif //PINOCCHIO_EXPORTS
#else //_WIN32
#define PINOCCHIO_API
#endif //_WIN32
