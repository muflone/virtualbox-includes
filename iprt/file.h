 * Copyright (C) 2006-2012 Oracle Corporation
#include <iprt/fs.h>
#include <iprt/sg.h>
/** Platform specific native standard out "handle". */
/** Don't allow symbolic links as part of the path.
 * @remarks this flag is currently not implemented and will be ignored. */
#define RTFILE_O_NO_SYMLINKS            UINT32_C(0x20000000)

                                      /* UINT32_C(0x40000000)
                                     and UINT32_C(0x80000000) are unused atm. */
#define RTFILE_O_VALID_MASK             UINT32_C(0x3ffffff7)
#ifdef IN_RING3
#endif /* IN_RING3 */
RTDECL(int)  RTFileOpen(PRTFILE pFile, const char *pszFilename, uint64_t fOpen);
RTDECL(int)  RTFileOpenF(PRTFILE pFile, uint64_t fOpen, const char *pszFilenameFmt, ...);
RTDECL(int)  RTFileOpenV(PRTFILE pFile, uint64_t fOpen, const char *pszFilenameFmt, va_list va);
RTDECL(int)  RTFileOpenBitBucket(PRTFILE phFile, uint64_t fAccess);
RTDECL(int)  RTFileClose(RTFILE File);
RTDECL(int) RTFileFromNative(PRTFILE pFile, RTHCINTPTR uNative);
RTDECL(RTHCINTPTR) RTFileToNative(RTFILE File);
RTDECL(int)  RTFileDelete(const char *pszFilename);
RTDECL(int)  RTFileSeek(RTFILE File, int64_t offSeek, unsigned uMethod, uint64_t *poffActual);
RTDECL(int)  RTFileRead(RTFILE File, void *pvBuf, size_t cbToRead, size_t *pcbRead);
RTDECL(int)  RTFileReadAt(RTFILE File, RTFOFF off, void *pvBuf, size_t cbToRead, size_t *pcbRead);

/**
 * Read bytes from a file at a given offset into a S/G buffer.
 * This function may modify the file position.
 *
 * @returns iprt status code.
 * @param   hFile       Handle to the file.
 * @param   off         Where to read.
 * @param   pSgBuf      Pointer to the S/G buffer to read into.
 * @param   cbToRead    How much to read.
 * @param   *pcbRead    How much we actually read .
 *                      If NULL an error will be returned for a partial read.
 */
RTDECL(int)  RTFileSgReadAt(RTFILE hFile, RTFOFF off, PRTSGBUF pSgBuf, size_t cbToRead, size_t *pcbRead);
RTDECL(int)  RTFileWrite(RTFILE File, const void *pvBuf, size_t cbToWrite, size_t *pcbWritten);
RTDECL(int)  RTFileWriteAt(RTFILE File, RTFOFF off, const void *pvBuf, size_t cbToWrite, size_t *pcbWritten);

/**
 * Write bytes from a S/G buffer to a file at a given offset.
 * This function may modify the file position.
 *
 * @returns iprt status code.
 * @param   hFile       Handle to the file.
 * @param   off         Where to write.
 * @param   pSgBuf      What to write.
 * @param   cbToWrite   How much to write.
 * @param   *pcbWritten How much we actually wrote.
 *                      If NULL an error will be returned for a partial write.
 */
RTDECL(int)  RTFileSgWriteAt(RTFILE hFile, RTFOFF off, PRTSGBUF pSgBuf, size_t cbToWrite, size_t *pcbWritten);
RTDECL(int)  RTFileFlush(RTFILE File);
RTDECL(int)  RTFileSetSize(RTFILE File, uint64_t cbSize);
RTDECL(int)  RTFileGetSize(RTFILE File, uint64_t *pcbSize);
RTDECL(RTFOFF) RTFileGetMaxSize(RTFILE File);
RTDECL(int) RTFileGetMaxSizeEx(RTFILE File, PRTFOFF pcbMax);
RTDECL(RTFOFF) RTFileGetMaxSize(RTFILE File);
RTDECL(bool) RTFileIsValid(RTFILE File);
#define RTFILEMOVE_FLAGS_REPLACE      0x1
/** Don't allow symbolic links as part of the path.
 * @remarks this flag is currently not implemented and will be ignored. */
#define RTFILEMOVE_FLAGS_NO_SYMLINKS  0x2
/**
 * Creates a new file with a unique name using the given template.
 *
 * One or more trailing X'es in the template will be replaced by random alpha
 * numeric characters until a RTFileOpen with RTFILE_O_CREATE succeeds or we
 * run out of patience.
 * For instance:
 *          "/tmp/myprog-XXXXXX"
 *
 * As an alternative to trailing X'es, it is possible to put 3 or more X'es
 * somewhere inside the file name. In the following string only the last
 * bunch of X'es will be modified:
 *          "/tmp/myprog-XXX-XXX.tmp"
 *
 * @returns iprt status code.
 * @param   pszTemplate     The file name template on input. The actual file
 *                          name on success. Empty string on failure.
 * @param   fMode           The mode to create the file with.  Use 0600 unless
 *                          you have reason not to.
 */
RTDECL(int) RTFileCreateTemp(char *pszTemplate, RTFMODE fMode);

/**
 * Secure version of @a RTFileCreateTemp with a fixed mode of 0600.
 *
 * This function behaves in the same way as @a RTFileCreateTemp with two
 * additional points.  Firstly the mode is fixed to 0600.  Secondly it will
 * fail if it is not possible to perform the operation securely.  Possible
 * reasons include that the file could be removed by another unprivileged
 * user before it is used (e.g. if is created in a non-sticky /tmp directory)
 * or that the path contains symbolic links which another unprivileged user
 * could manipulate; however the exact criteria will be specified on a
 * platform-by-platform basis as platform support is added.
 * @see RTPathIsSecure for the current list of criteria.
 * @returns iprt status code.
 * @returns VERR_NOT_SUPPORTED if the interface can not be supported on the
 *                             current platform at this time.
 * @returns VERR_INSECURE      if the file could not be created securely.
 * @param   pszTemplate        The file name template on input. The actual
 *                             file name on success. Empty string on failure.
 */
RTDECL(int) RTFileCreateTempSecure(char *pszTemplate);


RTDECL(int)  RTFileLock(RTFILE File, unsigned fLock, int64_t offLock, uint64_t cbLock);
RTDECL(int)  RTFileChangeLock(RTFILE File, unsigned fLock, int64_t offLock, uint64_t cbLock);
RTDECL(int)  RTFileUnlock(RTFILE File, int64_t offLock, uint64_t cbLock);
RTDECL(int) RTFileQueryInfo(RTFILE File, PRTFSOBJINFO pObjInfo, RTFSOBJATTRADD enmAdditionalAttribs);
RTDECL(int) RTFileSetTimes(RTFILE File, PCRTTIMESPEC pAccessTime, PCRTTIMESPEC pModificationTime,
                           PCRTTIMESPEC pChangeTime, PCRTTIMESPEC pBirthTime);
RTDECL(int) RTFileGetTimes(RTFILE File, PRTTIMESPEC pAccessTime, PRTTIMESPEC pModificationTime,
                           PRTTIMESPEC pChangeTime, PRTTIMESPEC pBirthTime);
RTDECL(int) RTFileSetMode(RTFILE File, RTFMODE fMode);
RTDECL(int) RTFileGetMode(RTFILE File, uint32_t *pfMode);
RTDECL(int) RTFileSetOwner(RTFILE File, uint32_t uid, uint32_t gid);
RTDECL(int) RTFileGetOwner(RTFILE File, uint32_t *pUid, uint32_t *pGid);
RTDECL(int) RTFileIoCtl(RTFILE File, unsigned long ulRequest, void *pvData, unsigned cbData, int *piRet);
RTDECL(int) RTFileQueryFsSizes(RTFILE hFile, PRTFOFF pcbTotal, RTFOFF *pcbFree,
                               uint32_t *pcbBlock, uint32_t *pcbSector);
#ifdef IN_RING3

 * @param   fFlags          Combination of RTFILEAIOCTX_FLAGS_*.
RTDECL(int) RTFileAioCtxCreate(PRTFILEAIOCTX phAioCtx, uint32_t cAioReqsMax,
                               uint32_t fFlags);
/** When set RTFileAioCtxWait() will always wait for completing requests,
 * even when there is none waiting currently, instead of returning 
 * VERR_FILE_AIO_NO_REQUEST. */
#define RTFILEAIOCTX_FLAGS_WAIT_WITHOUT_PENDING_REQUESTS RT_BIT_32(0)
/** mask of valid flags. */
#define RTFILEAIOCTX_FLAGS_VALID_MASK (RTFILEAIOCTX_FLAGS_WAIT_WITHOUT_PENDING_REQUESTS)

 * Associates a file with an async I/O context.