param([switch]$Elevated, [string]$ExclusionPath)

function Test-Admin {
    $currentUser = New-Object Security.Principal.WindowsPrincipal $([Security.Principal.WindowsIdentity]::GetCurrent())
    $currentUser.IsInRole([Security.Principal.WindowsBuiltinRole]::Administrator)
}

if ((Test-Admin) -eq $false)  {
    if ($elevated) {
        # tried to elevate, did not work, aborting
    } else {
        Start-Process powershell.exe -Verb RunAs -ArgumentList ('-noprofile -noexit -file "{0}" -elevated -ExclusionPath "{1}"' -f ($myinvocation.MyCommand.Definition), $ExclusionPath)
    }
    exit
}

# Store the output of Get-MpPreference | Select-Object -ExpandProperty ExclusionPath in a variable
$exclusionPaths = Get-MpPreference | Select-Object -ExpandProperty ExclusionPath

# Display the contents of the variable
$ExclusionFound = $false
foreach ($path in $exclusionPaths) {
    if ($path -eq $ExclusionPath) {
        $ExclusionFound = $true
    }
}

if (-not $ExclusionFound) {
    #Write-Host "Exclsion not found. Creating exclusion for $ExclusionPath"
    Add-MpPreference -ExclusionPath $ExclusionPath
}