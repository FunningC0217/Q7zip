# Q7zip

Qt's 7z program, currently only supports windows



 user 7z.exe(windows execute) in qt's qrc file.
 u can usr toolchain from MSVC && mingw
  
 build taget in the Pro file (Qt's project) director,
 case changed target build path in Qt Pro File:
 
 --------------Q7zip.pro-------------------
 
 * DESTDIR += $$PWD
 ------------------------------------------
 
 user method in main function:
 ---------------------------------------------
 * Q7zip zip;
 * zip.setOutToConselo();
 * zip.inCompress("D:\\outtest.log","D:\\log.7z");
 * zip.unCompress("D:\\log.7z","D:\\log");
 --------------------------------------------




 Qt 的7z程序，目前仅支持windows
 使用它非常简单 目前只支持了压缩和解压
 
 在 Qt 的 qrc（资源文件）中使用 7z.exe
 你可以使用 MSVC 和 mingw 工具链
 构建目标在Qt 工程的文件夹
 因为我更改了目标在Pro文件中 
 
 _______________Q7zip.pro__________________
 
 * DESTDIR += $$PWD
 __________________________________________
 
 
 main 函数的使用方法：
 ____________________________________________
 * Q7zip zip;
 * zip.setOutToConselo();
 * zip.inCompress("D:\\outtest.log","D:\\log.7z");
 * zip.unCompress("D:\\log.7z","D:\\log");
 ____________________________________________
 
 
