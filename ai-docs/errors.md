## Issue: LICENSE missing from Windows installer/search
- Cause: Not included in qmake/CMake packaging steps.
- Fix: Added LICENSE to OTHER_FILES, DISTFILES, and CMake install. To be verified in 0.9.2-pre.1. 