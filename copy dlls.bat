@echo off

copy /Y "x64\Debug\Direct3D11.dll"     "CSTestProject\bin\Debug\Direct3D11.dll"
copy /Y "x64\Release\Direct3D11.dll"   "CSTestProject\bin\Release\Direct3D11.dll"
copy /Y "x64\Debug\RenderingAPI.dll"   "CSTestProject\bin\Debug\RenderingAPI.dll"
copy /Y "x64\Release\RenderingAPI.dll" "CSTestProject\bin\Release\RenderingAPI.dll"
pause