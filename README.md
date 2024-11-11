# PotPlayerM3U8
一个PotPlayer的脚本，播放本地的m3u8视频

把`MediaPlayParse - M3U8.as`和`libm3u8.dll`放在PotPlayer安装目录的`Extension\Media\PlayParse`文件夹下

下载`ffmpeg.exe`，将其放在`C:\windows\system32`路径下或将其所在路径加入环境变量

实现原理：

通过`MediaPlayParse - M3U8.as`调用`libm3u8.dll`的parse函数

parse函数调用system执行以下命令：调用ffmpeg对m3u8文件解析

```sh
ffmpeg -allowed_extensions ALL -i "index.m3u8" -c copy -f mpegts -y output.ts
```

- index.m3u8是potplayer打开的路径
- output.ts设置在临时目录下：`C:\Users\用户名\AppData\Local\Temp`

MediaPlayParse - M3U8.as会将output.ts的路径返回给PotPlayer播放

PotPlayer关闭，output.ts文件不会自动删除
