# Q7zip

Qt's 7z program, currently only supports windows
Qt 的7z程序，目前仅支持windows





 * uesr windows execute 7z.exe in qt qrc file.
 * Only Windows is currently supported.
 *
 * build taget in the Pro file director,
 * case changed target build path in Qt Pro File:
 *
 *_______________Q7zip.pro___________________
 *
 * DESTDIR += $$PWD
 *___________________________________________
 *
 *
 *
 * user method in main function:
 *____________________________________________
 * Q7zip zip;
 * zip.setOutToConselo();
 * zip.inCompress("D:\\outtest.log","D:\\log.7z");
 * zip.unCompress("D:\\log.7z","D:\\log");
 *____________________________________________
 
 
 使用它非常简单 目前只支持了压缩和解压
