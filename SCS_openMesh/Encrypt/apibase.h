//#define BUILDING_DLL
#ifdef BUILD_DLL
#define DLLEXPORT __declspec(dllexport)
#define DLLEXPORT_C extern "C" __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#define DLLEXPORT_C extern "C" __declspec(dllimport)
#endif

#ifdef BUILD_COMPUTAION_DLL
#define COMP_DLLEXPORT __declspec(dllexport)
#define COMP_DLLEXPORT_C extern "C" __declspec(dllexport)
#else
#define COMP_DLLEXPORT __declspec(dllimport)
#define COMP_DLLEXPORT_C extern "C" __declspec(dllimport)
#endif

#pragma warning( disable: 4251 )
