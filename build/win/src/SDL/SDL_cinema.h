#ifndef SDL_CINEMA_H
#define SDL_CINEMA_H



#ifdef __cplusplus
extern "C" {
#endif

#define SDL_CINEMAEVENT 0xE000              ///< Event that comes from SDL Cinema
#define SDL_CINEMA_EOS  0                   ///< Event signaling the end of the video stream
#define SDL_CINEMA_ERR  1                   ///< Event signaling an error occured during playback of the video stream

// 0 is failure, 1 is success
extern int  CIN_Init();                     ///< Initializes SDL Cinema
extern void CIN_DeInit();                   ///< Deinitializes SDL Cinema

// 0 is failure, 1 is success
extern int  CIN_LoadCIN(const char *Url);   ///< Loads a cinema from a url (ex: file:///media/internal/Video.mp4)
extern void CIN_Play(void);                 ///< Start cinema playback (A user-event "SDL_CINEMA_EOS" event is sent once the cinema has completed playback)
extern void CIN_Pause(void);                ///< Pauses cinema playback
extern void CIN_Stop(void);                 ///< Stops cinema playback

#ifdef __cplusplus
}
#endif

#endif
