#ifndef GLOBAL_H_
#define GLOBAL_H_

#ifdef __ANDROID__
////// DEBUG | RELEASE

// Debug
#ifndef DEBUG
#define DEBUG
#endif
#undef NDEBUG

// Relase
//#ifndef NDEBUG
//#define NDEBUG
//#endif
//#undef DEBUG

#endif // __ANDROID__

#define DISPLAY_DELAY           50

// Log levels (< 5 to log)
#define LOG_LEVEL_FINGERG       4
#define LOG_LEVEL_LEVELMENU     4
#define LOG_LEVEL_LEVELMEGA     4
#define LOG_LEVEL_PAD           4
#define LOG_LEVEL_MEGAMAN       4
#define LOG_LEVEL_LEVELNATIVE   4
#define LOG_LEVEL_LEVELCLAY     4
#define LOG_LEVEL_CLAYMAN       4
#define LOG_LEVEL_LEVELWALK     4
#define LOG_LEVEL_WALKMAN       4
#define LOG_LEVEL_LEVELCAM      4
#define LOG_LEVEL_LEVELMIC      4

#endif // GLOBAL_H_
