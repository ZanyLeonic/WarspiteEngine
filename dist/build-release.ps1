function Pause ($Message="Press any key to continue...")
{
	Write-Host -NoNewLine $Message
	$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
	Write-Host ""
}

Write-Output "Generating project files..."
../build-deps.ps1 Release rel-build x64
Write-Output "Generated project files, attempting to build."
cmake --build ../rel-build/ --config Release --clean-first -j 4
Write-Output "Finished building! Check above for any errors."
Pause