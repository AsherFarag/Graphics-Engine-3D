@echo off
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=2& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mt.dll C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/bin64/assimp-vc143-mt.dll || (set FAIL_LINE=3& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=4& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mt.exp C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mt.exp || (set FAIL_LINE=5& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=6& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mt.lib C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mt.lib || (set FAIL_LINE=7& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=8& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mtd.dll C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/bin64/assimp-vc143-mtd.dll || (set FAIL_LINE=9& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=10& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mtd.exp C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mtd.exp || (set FAIL_LINE=11& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=12& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mtd.ilk C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mtd.ilk || (set FAIL_LINE=13& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=14& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mtd.lib C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mtd.lib || (set FAIL_LINE=15& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=16& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mtd.pdb C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mtd.pdb || (set FAIL_LINE=17& goto :ABORT)
cd /D C:\Users\s231237\Documents\GitHub\AIEBootstrap-2024\dependencies\assimp\out\build\x64-Debug || (set FAIL_LINE=18& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/out/build/x64-Debug/code/assimp-vc143-mtd.pdb C:/Users/s231237/Documents/GitHub/AIEBootstrap-2024/dependencies/assimp/lib64/assimp-vc143-mtd.pdb || (set FAIL_LINE=19& goto :ABORT)
goto :EOF

:ABORT
set ERROR_CODE=%ERRORLEVEL%
echo Batch file failed at line %FAIL_LINE% with errorcode %ERRORLEVEL%
exit /b %ERROR_CODE%