#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

__declspec(dllexport) char* parse(const char* m3u8Path) {
    static char tempPath[MAX_PATH] = "\0";
    char excute[1024] = "\0";
    char outpath[MAX_PATH] = "\0";
    char m3u8Dir[MAX_PATH] = "\0";
    char originalDir[MAX_PATH] = "\0";
    char driver[4] = "\0";
    char dir[MAX_PATH] = "\0";

    // 获取系统的临时目录
    DWORD pathLength = GetTempPath(MAX_PATH, tempPath);
    if (pathLength == 0 || pathLength > MAX_PATH) {
        printf("Failed to get temp directory\n");
        return NULL;
    }
    //printf("Temp directory: %s\n", tempPath);
    sprintf_s(outpath, sizeof(outpath), "%s\output.ts", tempPath);
    printf("Output path: %s\n", outpath);

    // 拼接 ffmpeg 命令
    sprintf_s(excute, sizeof(excute), "ffmpeg -allowed_extensions ALL -i \"%s\" -c copy -f mpegts -y \"%s\"", m3u8Path, outpath);
    //sprintf_s(excute, sizeof(excute), "ffmpeg -allowed_extensions ALL -i \"%s\" -c copy -f mpegts -y \"%s\" > nul 2>&1", m3u8Path, outpath);
    printf("%s\n", excute);
    // 获取当前工作目录
    if (GetCurrentDirectory(MAX_PATH, originalDir) == 0)
    {
        printf("Failed to get current directory\n");
        return NULL;
    }
    printf("Original directory: %s\n", originalDir);
    // 获取m3u8文件所在目录
    _splitpath(m3u8Path, driver, dir, NULL, NULL);
    sprintf_s(m3u8Dir, sizeof(m3u8Dir), "%s%s", driver, dir);
    printf("M3U8 directory: %s\n", m3u8Dir);
    // 切换到m3u8文件所在目录
    if (SetCurrentDirectory(m3u8Dir) == 0)
    {
        printf("Failed to change directory\n");
        return NULL;
    }
    printf("change to m3u8 directory: %s\n", m3u8Dir);
    // 构建 ffmpeg 命令
    int result = system(excute);
    if (result != 0) {
        printf("Failed to execute ffmpeg command\n");
        return NULL;
    }
    // 切换回原来的工作目录
    if (SetCurrentDirectory(originalDir) == 0)
    {
        printf("Failed to change directory\n");
        return NULL;
    }
    printf("back to original directory: %s\n", originalDir);
    // 返回临时文件路径
    return outpath;
}