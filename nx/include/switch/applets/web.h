/**
 * @file web.h
 * @brief Wrapper for using the web LibraryApplets.
 * @author p-sam, yellows8
 * @copyright libnx Authors
 */
#pragma once
#include "../types.h"
#include "../services/applet.h"
#include "../services/caps.h"

/// This indicates the type of web-applet.
typedef enum {
    WebShimKind_Login   = 2,
    WebShimKind_Offline = 3,
    WebShimKind_Share   = 4,
    WebShimKind_Web     = 5,
    WebShimKind_Wifi    = 6,
    WebShimKind_Lobby   = 7,
} WebShimKind;

typedef struct {
    u32 unk_x0;                 ///< Official sw sets this to 0 with appletStorageWrite, separately from the rest of the config struct.
    char conntest_url[0x100];
    char initial_url[0x400];
    u128 userID;
    u32 unk_x514;
} PACKED WebWifiPageArg;

typedef struct {
    u32 unk_x0;
    Result res;
} PACKED WebWifiReturnValue;

typedef struct {
    WebWifiPageArg arg;
} WebWifiConfig;

/// TLV storage, starts with \ref WebArgHeader followed by \ref WebArgTLV entries.
typedef struct {
    u8 data[0x2000];
} WebCommonTLVStorage;

typedef struct {
    u32 exitReason;
    u32 pad;
    char lastUrl[0x1000];
    u64 lastUrlSize;
} PACKED WebCommonReturnValue;

/// Header struct at offset 0 in the web Arg storage (non-webWifi).
typedef struct {
    u16 total_entries;  ///< Total \ref WebArgTLV entries following this struct.
    u16 pad;
    WebShimKind shimKind;
} PACKED WebArgHeader;

/// Web TLV used in the web Arg storage.
typedef struct {
    u16 type;       ///< Type of this arg.
    u16 size;       ///< Size of the arg data following this struct.
    u8 pad[4];
} PACKED WebArgTLV;

typedef struct {
    WebCommonTLVStorage arg;
    AppletId appletid;
    u32 version;
} WebCommonConfig;

typedef struct {
    bool type;                     ///< false = ret, true = storage.
    WebCommonReturnValue ret;
    WebCommonTLVStorage storage;
} WebCommonReply;

/// Types for \ref WebArgTLV, input storage.
typedef enum {
    WebArgType_Url                                      = 0x1,    ///< [1.0.0+] String, size 0xC00. Initial URL.
    WebArgType_CallbackUrl                              = 0x3,    ///< [1.0.0+] String, size 0x400.
    WebArgType_CallbackableUrl                          = 0x4,    ///< [1.0.0+] String, size 0x400.
    WebArgType_ShareStartPage                           = 0x9,    ///< [1.0.0+] u32 enum WebShareStartPage
    WebArgType_Whitelist                                = 0xA,    ///< [1.0.0+] String, size 0x1000.
    WebArgType_NewsFlag                                 = 0xB,    ///< [1.0.0+] u8 bool
    WebArgType_UnknownD                                 = 0xD,    ///< [1.0.0+] u8
    WebArgType_UserID                                   = 0xE,    ///< [1.0.0+] u128 userID, controls which user-specific savedata to mount.
    WebArgType_AlbumEntry                               = 0xF,    ///< [1.0.0+] Share-applet caps AlbumEntry
    WebArgType_ScreenShot                               = 0x10,   ///< [1.0.0+] u8 bool
    WebArgType_EcClientCert                             = 0x11,   ///< [1.0.0+] u8 bool
    WebArgType_Unknown12                                = 0x12,   ///< [1.0.0+] u8
    WebArgType_PlayReport                               = 0x13,   ///< [1.0.0+] u8 bool
    WebArgType_Unknown14                                = 0x14,   ///< [1.0.0+] u8
    WebArgType_Unknown15                                = 0x15,   ///< [1.0.0+] u8
    WebArgType_BootDisplayKind                          = 0x17,   ///< [1.0.0+] u32 enum *BootDisplayKind
    WebArgType_BackgroundKind                           = 0x18,   ///< [1.0.0+] u32 enum *BackgroundKind
    WebArgType_Footer                                   = 0x19,   ///< [1.0.0+] u8 bool
    WebArgType_Pointer                                  = 0x1A,   ///< [1.0.0+] u8 bool
    WebArgType_LeftStickMode                            = 0x1B,   ///< [1.0.0+] u32 enum *LeftStickMode
    WebArgType_KeyRepeatFrame0                          = 0x1C,   ///< [1.0.0+] s32 KeyRepeatFrame, first param
    WebArgType_KeyRepeatFrame1                          = 0x1D,   ///< [1.0.0+] s32 KeyRepeatFrame, second param
    WebArgType_BootAsMediaPlayerInverted                = 0x1E,   ///< [1.0.0+] u8 bool. With News on [3.0.0+] this is set after BootAsMediaPlayer with the value inverted.
    WebArgType_DisplayUrlKind                           = 0x1F,   ///< [1.0.0+] u8 bool, DisplayUrlKind (value = (input_enumval==0x1)).
    WebArgType_BootAsMediaPlayer                        = 0x21,   ///< [2.0.0+] u8 bool
    WebArgType_ShopJump                                 = 0x22,   ///< [2.0.0+] u8 bool
    WebArgType_MediaPlayerUserGestureRestriction        = 0x23,   ///< [2.0.0+] u8 bool
    WebArgType_LobbyParameter                           = 0x24,   ///< [2.0.0+] String, size 0x100.
    WebArgType_ApplicationAlbumEntry                    = 0x26,   ///< [3.0.0+] Share-applet caps ApplicationAlbumEntry
    WebArgType_JsExtension                              = 0x27,   ///< [3.0.0+] u8 bool
    WebArgType_AdditionalCommentText                    = 0x28,   ///< [4.0.0+] String, size 0x100. Share-applet AdditionalCommentText.
    WebArgType_TouchEnabledOnContents                   = 0x29,   ///< [4.0.0+] u8 bool
    WebArgType_UserAgentAdditionalString                = 0x2A,   ///< [4.0.0+] String, size 0x80.
    WebArgType_AdditionalMediaData                      = 0x2B,   ///< [4.0.0+] Share-applet 0x10-byte u8 array, AdditionalMediaData. If the user-input size is less than 0x10, the remaining data used for the TLV is cleared.
    WebArgType_MediaPlayerAutoClose                     = 0x2C,   ///< [4.0.0+] u8 bool
    WebArgType_PageCache                                = 0x2D,   ///< [4.0.0+] u8 bool
    WebArgType_WebAudio                                 = 0x2E,   ///< [4.0.0+] u8 bool
    WebArgType_2F                                       = 0x2F,   ///< [5.0.0+] u8
    WebArgType_YouTubeVideoFlag                         = 0x31,   ///< [5.0.0+] u8 bool Indicates that the built-in whitelist for YouTubeVideo should be used.
    WebArgType_FooterFixedKind                          = 0x32,   ///< [5.0.0+] u32 enum *FooterFixedKind
    WebArgType_PageFade                                 = 0x33,   ///< [5.0.0+] u8 bool
    WebArgType_MediaCreatorApplicationRatingAge         = 0x34,   ///< [5.0.0+] Share-applet 0x20-byte s8 array, MediaCreatorApplicationRatingAge.
    WebArgType_BootLoadingIcon                          = 0x35,   ///< [5.0.0+] u8 bool
    WebArgType_PageScrollIndicator                      = 0x36,   ///< [5.0.0+] u8 bool
} WebArgType;

/// Types for \ref WebArgTLV, output storage.
/// Official user-processes doesn't check the TLV size for any of these.
typedef enum {
    WebReplyType_ExitReason          = 0x1,  ///< [3.0.0+] u32 ShareExitReason
    WebReplyType_LastUrl             = 0x2,  ///< [3.0.0+] string
    WebReplyType_LastUrlSize         = 0x3,  ///< [3.0.0+] u64
    WebReplyType_SharePostResult     = 0x4,  ///< [3.0.0+] u32 SharePostResult
    WebReplyType_PostServiceName     = 0x5,  ///< [3.0.0+] string
    WebReplyType_PostServiceNameSize = 0x6,  ///< [3.0.0+] u64
    WebReplyType_PostId              = 0x7,  ///< [3.0.0+] string
    WebReplyType_PostIdSize          = 0x8,  ///< [3.0.0+] u64
} WebReplyType;

/// This controls the initial page for ShareApplet, used by \ref webShareCreate.
typedef enum {
    WebShareStartPage_Default  = 0,  ///< The default "/" page.
    WebShareStartPage_Settings = 1,  ///< The "/settings/" page.
} WebShareStartPage;

/// Kind values for \ref webConfigSetBootDisplayKind with Web applet. Controls the background color while displaying the loading screen during applet boot.
typedef enum {
    WebBootDisplayKind_White    = 0,  ///< Default white background.
    WebBootDisplayKind_Unknown1 = 1,
    WebBootDisplayKind_Black    = 2,  ///< Black background.
    WebBootDisplayKind_Unknown3 = 3,
    WebBootDisplayKind_Unknown4 = 4,
} WebBootDisplayKind;

/**
 * @brief Creates the config for WifiWebAuthApplet.
 * @param config WebWifiConfig object.
 * @param conntest_url URL used for the connection-test requests, if NULL initial_url is used for this.
 * @param initial_url Initial URL navigated to by the applet.
 * @param userID Account userID, 0 for common.
 * @param unk Value written to \ref WebWifiPageArg unk_x514, this can be 0.
 */
void webWifiCreate(WebWifiConfig* config, const char* conntest_url, const char* initial_url, u128 userID, u32 unk);

/**
 * @brief Launches WifiWebAuthApplet with the specified config and waits for it to exit.
 * @param config WebWifiConfig object.
 * @param out Optional output applet reply data, can be NULL.
 */
Result webWifiShow(WebWifiConfig* config, WebWifiReturnValue *out);

/**
 * @brief Creates the config for WebApplet. This applet uses an URL whitelist loaded from the user-process host title, when running under an Application. Content mounting *must* be successful otherwise the applet will throw a fatalerr.
 * @note Sets WebArgType_UnknownD, and WebArgType_Unknown12 on pre-3.0.0, to value 1.
 * @param config WebCommonConfig object.
 * @param url Initial URL navigated to by the applet.
 */
Result webPageCreate(WebCommonConfig* config, const char* url);

/**
 * @brief Creates the config for WebApplet. This is based on \ref webPageCreate, for News. Hence other functions referencing \ref webPageCreate also apply to this.
 * @note Sets WebArgType_UnknownD to value 1, and sets WebArgType_NewsFlag to true. Also uses \ref webConfigSetEcClientCert and \ref webConfigSetShopJump with flag=true.
 * @note The domain from the input URL is automatically whitelisted, in addition to any already loaded whitelist.
 * @param config WebCommonConfig object.
 * @param url Initial URL navigated to by the applet.
 */
Result webNewsCreate(WebCommonConfig* config, const char* url);

/**
 * @brief Creates the config for WebApplet. This is based on \ref webPageCreate, for YouTubeVideo. Hence other functions referencing \ref webPageCreate also apply to this. This uses a whitelist which essentially only allows youtube embed/ URLs (without mounting content from the host title).
 * @note This is only available on [5.0.0+].
 * @note Sets WebArgType_UnknownD to value 1, and sets WebArgType_YouTubeVideoFlag to true. Also uses \ref webConfigSetBootAsMediaPlayer with flag=true.
 * @param config WebCommonConfig object.
 * @param url Initial URL navigated to by the applet.
 */
Result webYouTubeVideoCreate(WebCommonConfig* config, const char* url);

/**
 * @brief Creates the config for ShareApplet.
 * @note If a non-zero userID isn't set with \ref webConfigSetUserID prior to using \ref webConfigShow, the applet will launch the profile-selector applet to select an account.
 * @note Uses \ref webConfigSetLeftStickMode with mode=1, \ref webConfigSetUserID with userID=0, \ref webConfigSetDisplayUrlKind with kind=true, and sets WebArgType_Unknown14/WebArgType_Unknown15 to value 1. Uses \ref webConfigSetBootDisplayKind with WebBootDisplayKind_Unknown3.
 * @param config WebCommonConfig object.
 * @param page \ref WebShareStartPage
 */
Result webShareCreate(WebCommonConfig* config, WebShareStartPage page);

/**
 * @brief Creates the config for LobbyApplet.
 * @note Only available on [2.0.0+].
 * @note If a non-zero userID isn't set with \ref webConfigSetUserID prior to using \ref webConfigShow, the applet will launch the profile-selector applet to select an account.
 * @note Uses \ref webConfigSetLeftStickMode with mode=1, \ref webConfigSetPointer with flag=false on [3.0.0+], \ref webConfigSetUserID with userID=0, and sets WebArgType_Unknown14/WebArgType_Unknown15 to value 1. Uses \ref webConfigSetBootDisplayKind with WebBootDisplayKind_Unknown4, \ref webConfigSetBackgroundKind with kind=2, and sets WebArgType_BootAsMediaPlayerInverted to false.
 * @param config WebCommonConfig object.
 */
Result webLobbyCreate(WebCommonConfig* config);

/**
 * @brief Sets the CallbackUrl.
 * @note Only available with config created by \ref webPageCreate or with Share-applet.
 * @param config WebCommonConfig object.
 * @param url URL
 */
Result webConfigSetCallbackUrl(WebCommonConfig* config, const char* url);

/**
 * @brief Sets the CallbackableUrl.
 * @note Only available with config created by \ref webPageCreate.
 * @param config WebCommonConfig object.
 * @param url URL
 */
Result webConfigSetCallbackableUrl(WebCommonConfig* config, const char* url);

/**
 * @brief Sets the whitelist.
 * @note Only available with config created by \ref webPageCreate.
 * @note If the whitelist isn't formatted properly, the applet will exit briefly after the applet is launched.
 * @param config WebCommonConfig object.
 * @param whitelist Whitelist string, each line is a regex for each whitelisted URL.
 */
Result webConfigSetWhitelist(WebCommonConfig* config, const char* whitelist);

/**
 * @brief Sets the account UserID. Controls which user-specific savedata to mount.
 * @note Only available with config created by \ref webPageCreate, \ref webLobbyCreate, or with Share-applet.
 * @note Used automatically by \ref webShareCreate and \ref webLobbyCreate with userID=0.
 * @param config WebCommonConfig object.
 * @param userID Account userID
 */
Result webConfigSetUserID(WebCommonConfig* config, u128 userID);

/**
 * @brief Sets the Share CapsAlbumEntry.
 * @note Only available with config created by \ref webShareCreate.
 * @param config WebCommonConfig object.
 * @param entry \ref CapsAlbumEntry
 */
Result webConfigSetAlbumEntry(WebCommonConfig* config, CapsAlbumEntry *entry);

/**
 * @brief Sets the ScreenShot flag, which controls whether screen-shot capture is allowed.
 * @note Only available with config created by \ref webPageCreate.
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetScreenShot(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the EcClientCert flag.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet.
 * @note Used automatically by \ref webNewsCreate with flag=true.
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetEcClientCert(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the BootDisplayKind.
 * @note Only available with config created by \ref webShareCreate, \ref webPageCreate, \ref webLobbyCreate, or with Offline-applet.
 * @note Used automatically by \ref webShareCreate with kind=WebBootDisplayKind_Unknown3.
 * @note Used automatically by \ref webLobbyCreate with kind=WebBootDisplayKind_Unknown4.
 * @param config WebCommonConfig object.
 * @param kind Kind, different enums for Web (\ref WebBootDisplayKind) and Offline.
 */
Result webConfigSetBootDisplayKind(WebCommonConfig* config, u32 kind);

/**
 * @brief Sets the BackgroundKind.
 * @note Only available with config created by \ref webPageCreate, \ref webLobbyCreate, or with Offline-applet.
 * @note Used automatically by \ref webLobbyCreate with kind=2.
 * @param config WebCommonConfig object.
 * @param kind Kind, different enums for Web and Offline.
 */
Result webConfigSetBackgroundKind(WebCommonConfig* config, u32 kind);

/**
 * @brief Sets the whether the UI footer is enabled.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet.
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetFooter(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the whether the Pointer is enabled.
 * @note Only available with config created by \ref webPageCreate, \ref webLobbyCreate, or with Offline-applet.
 * @note Used automatically by \ref webLobbyCreate with flag=false on [3.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetPointer(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the LeftStickMode.
 * @note Only available with config created by \ref webShareCreate, \ref webPageCreate, \ref webLobbyCreate, or with Offline-applet.
 * @note Used automatically by \ref webShareCreate and \ref webLobbyCreate with mode=1.
 * @param config WebCommonConfig object.
 * @param mode Mode, different enums for Web and Offline.
 */
Result webConfigSetLeftStickMode(WebCommonConfig* config, u32 mode);

/**
 * @brief Sets the KeyRepeatFrame.
 * @note Only available with Offline-applet.
 * @param config WebCommonConfig object.
 * @param inval0 First input param.
 * @param inval1 Second input param.
 */
Result webConfigSetKeyRepeatFrame(WebCommonConfig* config, s32 inval0, s32 inval1);

/**
 * @brief Sets the DisplayUrlKind.
 * @note Only available with config created by \ref webShareCreate or \ref webPageCreate.
 * @param config WebCommonConfig object.
 * @note Used automatically by \ref webShareCreate with kind=true.
 * @param kind Kind
 */
Result webConfigSetDisplayUrlKind(WebCommonConfig* config, bool kind);

/**
 * @brief Sets the BootAsMediaPlayer flag.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [2.0.0+].
 * @note With config created by \ref webNewsCreate on [3.0.0+], this also sets WebArgType_BootAsMediaPlayerInverted to !flag.
 * @param config WebCommonConfig object.
 * @param flag Flag. true = BootAsMediaPlayer, false = BootAsWebPage.
 */
Result webConfigSetBootAsMediaPlayer(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the ShopJump flag.
 * @note Only available with config created by \ref webPageCreate on [2.0.0+].
 * @note Used automatically by \ref webNewsCreate with flag=true.
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetShopJump(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the MediaPlayerUserGestureRestriction flag.
 * @note Only available with config created by \ref webPageCreate on [2.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetMediaPlayerUserGestureRestriction(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the LobbyParameter.
 * @note Only available with config created by \ref webLobbyCreate.
 * @param config WebCommonConfig object.
 * @param str String
 */
Result webConfigSetLobbyParameter(WebCommonConfig* config, const char* str);

/**
 * @brief Sets the Share CapsApplicationAlbumEntry.
 * @note Only available with config created by \ref webShareCreate on [3.0.0+].
 * @param config WebCommonConfig object.
 * @param entry \ref CapsApplicationAlbumEntry, see also capssu.h.
 */
Result webConfigSetApplicationAlbumEntry(WebCommonConfig* config, CapsApplicationAlbumEntry *entry);

/**
 * @brief Sets whether JsExtension is enabled.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [3.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetJsExtension(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the Share AdditionalCommentText.
 * @note Only available with config created by \ref webShareCreate on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param str String
 */
Result webConfigSetAdditionalCommentText(WebCommonConfig* config, const char* str);

/**
 * @brief Sets the TouchEnabledOnContents flag.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetTouchEnabledOnContents(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the UserAgentAdditionalString. " " followed by this string are appended to the normal User-Agent string.
 * @note Only available with config created by \ref webPageCreate on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param str String
 */
Result webConfigSetUserAgentAdditionalString(WebCommonConfig* config, const char* str);

/**
 * @brief Sets the Share AdditionalMediaData.
 * @note Only available with config created by \ref webShareCreate on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param data Input data
 * @param size Size of the input data, max size is 0x10.
 */
Result webConfigSetAdditionalMediaData(WebCommonConfig* config, const u8* data, size_t size);

/**
 * @brief Sets the MediaPlayerAutoClose flag.
 * @note Only available with config created by \ref webPageCreate on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetMediaPlayerAutoClose(WebCommonConfig* config, bool flag);

/**
 * @brief Sets whether PageCache is enabled.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetPageCache(WebCommonConfig* config, bool flag);

/**
 * @brief Sets whether WebAudio is enabled.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [4.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetWebAudio(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the FooterFixedKind.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [5.0.0+].
 * @param config WebCommonConfig object.
 * @param kind Kind, different enums for Web and Offline.
 */
Result webConfigSetFooterFixedKind(WebCommonConfig* config, u32 kind);

/**
 * @brief Sets the PageFade flag.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [5.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetPageFade(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the Share MediaCreatorApplicationRatingAge.
 * @note Only available with config created by \ref webShareCreate on [5.0.0+].
 * @param config WebCommonConfig object.
 * @param data 0x20-byte input data
 */
Result webConfigSetMediaCreatorApplicationRatingAge(WebCommonConfig* config, const s8 *data);

/**
 * @brief Sets the BootLoadingIcon flag.
 * @note Only available with Offline-applet on [5.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetBootLoadingIcon(WebCommonConfig* config, bool flag);

/**
 * @brief Sets the PageScrollIndicator flag.
 * @note Only available with config created by \ref webPageCreate or with Offline-applet, on [5.0.0+].
 * @param config WebCommonConfig object.
 * @param flag Flag
 */
Result webConfigSetPageScrollIndicator(WebCommonConfig* config, bool flag);

/**
 * @brief Launches the {web applet} with the specified config and waits for it to exit.
 * @param config WebCommonConfig object.
 * @param out Optional output applet reply data, can be NULL.
 */
Result webConfigShow(WebCommonConfig* config, WebCommonReply *out);

/**
 * @brief Gets the ExitReason from the specified reply.
 * @param reply WebCommonReply object.
 * @param exitReason Output exitReason
 */
Result webReplyGetExitReason(WebCommonReply *reply, u32 *exitReason);

/**
 * @brief Gets the LastUrl from the specified reply.
 * @note If you want to allocate a string buffer on heap, you can call this with outstr=NULL/outstr_maxsize=0 to get the out_size, then call it again with the allocated buffer.
 * @param outstr Output string buffer. If NULL, the string is not loaded.
 * @param outstr_maxsize Size of the buffer, including NUL-terminator. If outstr is set, this size must be >1. The size used for the actual string-copy is this size-1, to make sure the output is NUL-terminated (the entire buffer is cleared first).
 * @param out_size Output string length including NUL-terminator, for the original input string in the reply loaded from a separate size field.
 */
Result webReplyGetLastUrl(WebCommonReply *reply, char *outstr, size_t outstr_maxsize, size_t *out_size);

/**
 * @brief Gets the SharePostResult from the specified reply.
 * @note Only available with reply data from ShareApplet on [3.0.0+].
 * @param reply WebCommonReply object.
 * @param sharePostResult Output sharePostResult
 */
Result webReplyGetSharePostResult(WebCommonReply *reply, u32 *sharePostResult);

/**
 * @brief Gets the PostServiceName from the specified reply.
 * @note Only available with reply data from ShareApplet on [3.0.0+].
 * @note If you want to allocate a string buffer on heap, you can call this with outstr=NULL/outstr_maxsize=0 to get the out_size, then call it again with the allocated buffer.
 * @param outstr Output string buffer. If NULL, the string is not loaded.
 * @param outstr_maxsize Size of the buffer, including NUL-terminator. If outstr is set, this size must be >1. The size used for the actual string-copy is this size-1, to make sure the output is NUL-terminated (the entire buffer is cleared first).
 * @param out_size Output string length including NUL-terminator, for the original input string in the reply loaded from a separate size field.
 */
Result webReplyGetPostServiceName(WebCommonReply *reply, char *outstr, size_t outstr_maxsize, size_t *out_size);

/**
 * @brief Gets the PostId from the specified reply.
 * @note Only available with reply data from ShareApplet on [3.0.0+].
 * @note If you want to allocate a string buffer on heap, you can call this with outstr=NULL/outstr_maxsize=0 to get the out_size, then call it again with the allocated buffer.
 * @param outstr Output string buffer. If NULL, the string is not loaded.
 * @param outstr_maxsize Size of the buffer, including NUL-terminator. If outstr is set, this size must be >1. The size used for the actual string-copy is this size-1, to make sure the output is NUL-terminated (the entire buffer is cleared first).
 * @param out_size Output string length including NUL-terminator, for the original input string in the reply loaded from a separate size field.
 */
Result webReplyGetPostId(WebCommonReply *reply, char *outstr, size_t outstr_maxsize, size_t *out_size);

