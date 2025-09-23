cd /d "%~dp0"
for %%F in ("uirs\*.uir") do (
CviConverter.exe "%%F"
)
pause