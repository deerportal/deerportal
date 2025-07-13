## Issue: LICENSE missing from Windows installer/search
- Cause: Not included in qmake/CMake packaging steps.
- Fix: Added LICENSE to OTHER_FILES, DISTFILES, and CMake install. To be verified in 0.9.2-pre.1. 

## Issue: PowerShell execution policy error when running .exe files
- Symptom: Error message "The term '.\DeerPortal-0.9.2-pre.2-Windows.exe' is not recognized as the name of a cmdlet, function, script file, or operable program"
- Cause: Windows PowerShell execution policy treats .exe files as scripts in some configurations
- Status: Application still runs correctly despite the error message
- Solutions:
  1. Use Command Prompt instead: `cmd /c "DeerPortal-0.9.2-pre.2-Windows.exe"`
  2. Use full path: `& ".\DeerPortal-0.9.2-pre.2-Windows.exe"`
  3. Change execution policy: `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`
  4. Run via Start-Process: `Start-Process ".\DeerPortal-0.9.2-pre.2-Windows.exe"` 