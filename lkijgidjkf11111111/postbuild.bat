@echo off
REM echo %1
echo Running post build script...
mkdir %1resources
copy "resources\*" %1resources\* /Y /B
echo Done!