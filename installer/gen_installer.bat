
set DATA_DIR=.\packages\com.vendor.root\data
mkdir %DATA_DIR%\tmp\

copy  .\..\build\Desktop_Qt_6_7_3_MinGW_64_bit-Release\micro_brico_app.exe %DATA_DIR%\tmp\micro_brico_app.exe

call windeployqt %DATA_DIR%\tmp\micro_brico_app.exe
copy C:\Qt\6.7.3\mingw_64\bin\libmysql.dll %DATA_DIR%\tmp\libmysql.dll

del %DATA_DIR%\microBricoArch.zip
call archivegen %DATA_DIR%\microBricoArch.zip %DATA_DIR%\tmp\* -f zip

rmdir /s /Q %DATA_DIR%\tmp\

call binarycreator.exe --offline-only -p ./packages -c config/config.xml micro_brico_installer

