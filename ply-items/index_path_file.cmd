@echo off
dir /B /S /A:-D-S /O:N %UserProfile%\Desktop\*.*
dir /B /S /A:-D-S /O:N "%AppData%\Microsoft\Windows\Start Menu\Programs\*.*"
dir /B /S /A:-D-S /O:N "%ProgramData%\Microsoft\Windows\Start Menu\Programs\*.*"
