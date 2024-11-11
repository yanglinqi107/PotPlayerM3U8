/*
    本地M3U8文件解析器
    功能：判断文件是否为本地m3u8文件
*/

// 基础信息函数
string GetTitle()
{
    return "Local M3U8 Parser";
}

string GetVersion()
{
    return "1.0";
}

string GetDesc()
{
    return "Parse local M3U8 files";
}

// 检查文件是否为本地m3u8
bool PlayitemCheck(const string &in path)
{
    string ext = HostGetExtension(path);  // 获取文件扩展名
    ext = ext.MakeLower();  // 转换为小写
    // 检查扩展名是否为m3u8或m3u
    if (ext != ".m3u8" && ext != ".m3u") return false; 
    // 检查是否为本地文件（不包含://，排除网络路径）
    if (path.find("://") >= 0) return false;
    return true;
}

uintptr m3u8Dll = 0;
uintptr parseFunc = 0;

// 主解析函数
string PlayitemParse(const string &in path, dictionary &MetaData, array<dictionary> &QualityList)
{
    //HostOpenConsole();  // 打开调试控制台
    // HostPrintUTF8("PlayitemParse called with path: " + path + "\n");
    // 防止多次解析同一个文件
    string savePath = HostLoadString("m3u8");
    if (savePath == path) return HostLoadString("video");

    // 1. 首先加载dll
    if (m3u8Dll == 0) 
    {
        m3u8Dll = HostLoadLibrary("libm3u8.dll");
    }
    if (m3u8Dll == 0)
    {
        HostPrintUTF8("无法加载m3u8.dll");
        return "";
    }

    // 2. 获取parse函数地址
    if (parseFunc == 0)
    {
        parseFunc = HostGetProcAddress(m3u8Dll, "parse");
    }
    if (parseFunc == 0)
    {
        HostPrintUTF8("无法找到parse函数");
        HostFreeLibrary(m3u8Dll);
        return "";
    }

    // 3. 准备输入参数
    uintptr pathPtr = HostString2UIntPtr(path);

    // 4. 调用函数
    // "P" 表示参数类型是指针(Pointer)
    // 函数返回值会以指针形式返回
    uintptr result = HostCallProcUIntPtr(parseFunc, "P", pathPtr);
    if (result == 0)
    {
        HostPrintUTF8("libm3u8.dll文件解析失败");
        return "";
    }
    // 5. 转换返回的指针为字符串
    string videoUrl = HostUIntPtr2String(result);
    // HostPrintUTF8("解析结果: " + videoUrl + "\n");
    // 保存该文件名到临时缓存，防止多次解析
    HostSaveString("m3u8", path);
    HostSaveString("video", videoUrl);
    return videoUrl;
}