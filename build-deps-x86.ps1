# Based on a script from CommitteeOfZero/impacto
param(
    [string]$VcpkgDir = $env:VCPKG_INSTALLATION_ROOT,
    [string]$CI = ""
)

function SetEnv() {
    $vswhere = "${env:ProgramFiles(x86)}/Microsoft Visual Studio/Installer/vswhere.exe"
    $ip = & $vswhere -prerelease -latest -property InstallationPath
    if ($ip -and (test-path "$ip\Common7\Tools\vsdevcmd.bat")) {
        & "${env:COMSPEC}" /s /c "`"$ip\Common7\Tools\vsdevcmd.bat`" -arch=x86 -no_logo && set" | foreach-object {
            $name, $value = $_ -split '=', 2
            set-content env:\"$name" $value
        }
    }
}

function GenerateProjectFiles() {
	Write-Output "Generating project files..."
    if (!(Get-Command cmake -ErrorAction SilentlyContinue)) {
        SetEnv
    }
	
	$bargs = @("")
	
	if ($CI -eq "--CI")
	{
		$bargs = @( 
		"-AWin32",
		"-DCMAKE_TOOLCHAIN_FILE=$VcpkgDir\scripts\buildsystems\vcpkg.cmake",
		'-DCMAKE_BUILD_TYPE="Debug"', 
		"-DVCPKG_TARGET_TRIPLET=x86-windows",
		"-DPYTHON_EXECUTABLE:FILEPATH=C:\hostedtoolcache\windows\Python\3.8.5\x86\python.exe",
		"-Bbuild-win32",
		".")
	}
	else
	{
		$bargs = @( 
		"-AWin32",
		"-DCMAKE_TOOLCHAIN_FILE=$VcpkgDir\scripts\buildsystems\vcpkg.cmake",
		'-DCMAKE_BUILD_TYPE="Debug"', 
		"-DVCPKG_TARGET_TRIPLET=x86-windows",
		"-DPYTHON_EXECUTABLE:FILEPATH=C:\Program Files (x86)\Python38-32\python.exe",
		"-Bbuild-win32",
		".")
	}
    & cmake $bargs
	
	Write-Output "Finished generating project files."
}

function InstallPackages() {
	Write-Output "Installing dependencies..."
    $vcpkg = "$VcpkgDir/vcpkg.exe"
    if (!(Get-Command $vcpkg -ErrorAction SilentlyContinue)) {
        $vcpkg = "vcpkg"
    }
    if (!(Get-Command $vcpkg  -ErrorAction SilentlyContinue)) {
        if (!(Test-Path build/vcpkg)) {
            mkdir build -Force | Out-Null
            Push-Location build
            & git clone https://github.com/Microsoft/vcpkg.git --depth 1
            Push-Location vcpkg
            ./bootstrap-vcpkg -disableMetrics
            Pop-Location
            Pop-Location
        }
        $vcpkg = "build/vcpkg/vcpkg.exe"
        $local_vcpkg = $true
    }

    & $vcpkg install sdl2 sdl2-image[libjpeg-turbo,libwebp,tiff] sdl2-ttf libvorbis rapidjson zlib openal-soft python3 pybind11 spdlog fmt --triplet x86-windows
    if ($local_vcpkg) {
        & $vcpkg integrate install
        Write-Output "Cleaning up..."
        Remove-Item build/vcpkg/downloads -Recurse
        Remove-Item build/vcpkg/buildtrees -Recurse
    }
	Write-Output "Dependencies installed."
}

# I don't care about errors or warnings cringe
InstallPackages
GenerateProjectFiles
# SIG # Begin signature block
# MIIV/gYJKoZIhvcNAQcCoIIV7zCCFesCAQExCzAJBgUrDgMCGgUAMGkGCisGAQQB
# gjcCAQSgWzBZMDQGCisGAQQBgjcCAR4wJgIDAQAABBAfzDtgWUsITrck0sYpfvNR
# AgEAAgEAAgEAAgEAAgEAMCEwCQYFKw4DAhoFAAQUjG3oGbW+9s9jf6+qT+sl2954
# /b2gghB6MIIC+jCCAeKgAwIBAgIQLb49IosXD65IOgUeZpaMXzANBgkqhkiG9w0B
# AQsFADAVMRMwEQYDVQQDDApaYW55TGVvbmljMB4XDTIyMDUwNjE1MDUyOVoXDTIz
# MDUwNjE1MjUyOVowFTETMBEGA1UEAwwKWmFueUxlb25pYzCCASIwDQYJKoZIhvcN
# AQEBBQADggEPADCCAQoCggEBAOP8F63ZVo0JneQAeJGaOLyYZASiIzoE4KYE0EPe
# dAvm7/LUYWcBMcv1mkaMAuRhuIUck7Hd/NtsQvkUrhCAm+Ph9SFBemoPjq3FgcyE
# wCb8XYBq1jE+CTxkg/imxjI3S+D2WoCNjLtkKWRawDXHRbknlcy21/8FEcZrTJye
# GasoBzr7N7aiC+/f2wLIRkcHZxdiD72OpoWJ2zcvYOg+eRYW0aAtLv9NJR2PHsF5
# J7g5bVl5ioFuBn3v43EVE7TDxNSYwCo89cHDWFwEtauZTADaUVE6+BHKGKsLMSTi
# lUNHO7mrZwXiJa9jiPyIqPtWrlumabfQtZFfMFlRGR8WlCECAwEAAaNGMEQwDgYD
# VR0PAQH/BAQDAgeAMBMGA1UdJQQMMAoGCCsGAQUFBwMDMB0GA1UdDgQWBBQTQg1B
# Q/e+waazuSacoVi5f6oopjANBgkqhkiG9w0BAQsFAAOCAQEAcYtRS16HwHL/kOq5
# +qedLuguBNLyj5E3wi1Vp9rKO8QZVsI1/JhWGOH0dXSyqD+qlFkfSZTjvcAwcvbR
# MIi/s3dMig2qcMBXOqj/sVMXGhGgLLFNUakU/Wc3U+zM8Qe417JX/FX9NEgbHHlj
# nzqXGj40I8aiKHyG6S/5gAZcP18hTjTq1vIn2XilBPYDT8CC/NAeNKY36apM7kUS
# ncsKxoAavckVjs5ZsOj5GcgwEXMqJgjG1QyA83eIoPQWmsJTnkU6BF8YInXUs872
# iO6oQPcLE4Yb/J7lUcpavS5J8B8QzO13+flkubpIvm6EbYDWO5Io9D60luTmv9ip
# Sey77jCCBq4wggSWoAMCAQICEAc2N7ckVHzYR6z9KGYqXlswDQYJKoZIhvcNAQEL
# BQAwYjELMAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UE
# CxMQd3d3LmRpZ2ljZXJ0LmNvbTEhMB8GA1UEAxMYRGlnaUNlcnQgVHJ1c3RlZCBS
# b290IEc0MB4XDTIyMDMyMzAwMDAwMFoXDTM3MDMyMjIzNTk1OVowYzELMAkGA1UE
# BhMCVVMxFzAVBgNVBAoTDkRpZ2lDZXJ0LCBJbmMuMTswOQYDVQQDEzJEaWdpQ2Vy
# dCBUcnVzdGVkIEc0IFJTQTQwOTYgU0hBMjU2IFRpbWVTdGFtcGluZyBDQTCCAiIw
# DQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAMaGNQZJs8E9cklRVcclA8TykTep
# l1Gh1tKD0Z5Mom2gsMyD+Vr2EaFEFUJfpIjzaPp985yJC3+dH54PMx9QEwsmc5Zt
# +FeoAn39Q7SE2hHxc7Gz7iuAhIoiGN/r2j3EF3+rGSs+QtxnjupRPfDWVtTnKC3r
# 07G1decfBmWNlCnT2exp39mQh0YAe9tEQYncfGpXevA3eZ9drMvohGS0UvJ2R/dh
# gxndX7RUCyFobjchu0CsX7LeSn3O9TkSZ+8OpWNs5KbFHc02DVzV5huowWR0QKfA
# csW6Th+xtVhNef7Xj3OTrCw54qVI1vCwMROpVymWJy71h6aPTnYVVSZwmCZ/oBpH
# IEPjQ2OAe3VuJyWQmDo4EbP29p7mO1vsgd4iFNmCKseSv6De4z6ic/rnH1pslPJS
# lRErWHRAKKtzQ87fSqEcazjFKfPKqpZzQmiftkaznTqj1QPgv/CiPMpC3BhIfxQ0
# z9JMq++bPf4OuGQq+nUoJEHtQr8FnGZJUlD0UfM2SU2LINIsVzV5K6jzRWC8I41Y
# 99xh3pP+OcD5sjClTNfpmEpYPtMDiP6zj9NeS3YSUZPJjAw7W4oiqMEmCPkUEBID
# fV8ju2TjY+Cm4T72wnSyPx4JduyrXUZ14mCjWAkBKAAOhFTuzuldyF4wEr1GnrXT
# drnSDmuZDNIztM2xAgMBAAGjggFdMIIBWTASBgNVHRMBAf8ECDAGAQH/AgEAMB0G
# A1UdDgQWBBS6FtltTYUvcyl2mi91jGogj57IbzAfBgNVHSMEGDAWgBTs1+OC0nFd
# ZEzfLmc/57qYrhwPTzAOBgNVHQ8BAf8EBAMCAYYwEwYDVR0lBAwwCgYIKwYBBQUH
# AwgwdwYIKwYBBQUHAQEEazBpMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdp
# Y2VydC5jb20wQQYIKwYBBQUHMAKGNWh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNv
# bS9EaWdpQ2VydFRydXN0ZWRSb290RzQuY3J0MEMGA1UdHwQ8MDowOKA2oDSGMmh0
# dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFRydXN0ZWRSb290RzQuY3Js
# MCAGA1UdIAQZMBcwCAYGZ4EMAQQCMAsGCWCGSAGG/WwHATANBgkqhkiG9w0BAQsF
# AAOCAgEAfVmOwJO2b5ipRCIBfmbW2CFC4bAYLhBNE88wU86/GPvHUF3iSyn7cIoN
# qilp/GnBzx0H6T5gyNgL5Vxb122H+oQgJTQxZ822EpZvxFBMYh0MCIKoFr2pVs8V
# c40BIiXOlWk/R3f7cnQU1/+rT4osequFzUNf7WC2qk+RZp4snuCKrOX9jLxkJods
# kr2dfNBwCnzvqLx1T7pa96kQsl3p/yhUifDVinF2ZdrM8HKjI/rAJ4JErpknG6sk
# HibBt94q6/aesXmZgaNWhqsKRcnfxI2g55j7+6adcq/Ex8HBanHZxhOACcS2n82H
# hyS7T6NJuXdmkfFynOlLAlKnN36TU6w7HQhJD5TNOXrd/yVjmScsPT9rp/Fmw0HN
# T7ZAmyEhQNC3EyTN3B14OuSereU0cZLXJmvkOHOrpgFPvT87eK1MrfvElXvtCl8z
# OYdBeHo46Zzh3SP9HSjTx/no8Zhf+yvYfvJGnXUsHicsJttvFXseGYs2uJPU5vIX
# mVnKcPA3v5gA3yAWTyf7YGcWoWa63VXAOimGsJigK+2VQbc61RWYMbRiCQ8KvYHZ
# E/6/pNHzV9m8BPqC3jLfBInwAM1dwvnQI38AC+R2AibZ8GV2QqYphwlHK+Z/GqSF
# D/yYlvZVVCsfgPrA8g4r5db7qS9EFUrnEw4d2zc4GqEr9u3WfPwwggbGMIIErqAD
# AgECAhAKekqInsmZQpAGYzhNhpedMA0GCSqGSIb3DQEBCwUAMGMxCzAJBgNVBAYT
# AlVTMRcwFQYDVQQKEw5EaWdpQ2VydCwgSW5jLjE7MDkGA1UEAxMyRGlnaUNlcnQg
# VHJ1c3RlZCBHNCBSU0E0MDk2IFNIQTI1NiBUaW1lU3RhbXBpbmcgQ0EwHhcNMjIw
# MzI5MDAwMDAwWhcNMzMwMzE0MjM1OTU5WjBMMQswCQYDVQQGEwJVUzEXMBUGA1UE
# ChMORGlnaUNlcnQsIEluYy4xJDAiBgNVBAMTG0RpZ2lDZXJ0IFRpbWVzdGFtcCAy
# MDIyIC0gMjCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBALkqliOmXLxf
# 1knwFYIY9DPuzFxs4+AlLtIx5DxArvurxON4XX5cNur1JY1Do4HrOGP5PIhp3jzS
# MFENMQe6Rm7po0tI6IlBfw2y1vmE8Zg+C78KhBJxbKFiJgHTzsNs/aw7ftwqHKm9
# MMYW2Nq867Lxg9GfzQnFuUFqRUIjQVr4YNNlLD5+Xr2Wp/D8sfT0KM9CeR87x5MH
# aGjlRDRSXw9Q3tRZLER0wDJHGVvimC6P0Mo//8ZnzzyTlU6E6XYYmJkRFMUrDKAz
# 200kheiClOEvA+5/hQLJhuHVGBS3BEXz4Di9or16cZjsFef9LuzSmwCKrB2NO4Bo
# /tBZmCbO4O2ufyguwp7gC0vICNEyu4P6IzzZ/9KMu/dDI9/nw1oFYn5wLOUrsj1j
# 6siugSBrQ4nIfl+wGt0ZvZ90QQqvuY4J03ShL7BUdsGQT5TshmH/2xEvkgMwzjC3
# iw9dRLNDHSNQzZHXL537/M2xwafEDsTvQD4ZOgLUMalpoEn5deGb6GjkagyP6+Sx
# IXuGZ1h+fx/oK+QUshbWgaHK2jCQa+5vdcCwNiayCDv/vb5/bBMY38ZtpHlJrYt/
# YYcFaPfUcONCleieu5tLsuK2QT3nr6caKMmtYbCgQRgZTu1Hm2GV7T4LYVrqPnqY
# klHNP8lE54CLKUJy93my3YTqJ+7+fXprAgMBAAGjggGLMIIBhzAOBgNVHQ8BAf8E
# BAMCB4AwDAYDVR0TAQH/BAIwADAWBgNVHSUBAf8EDDAKBggrBgEFBQcDCDAgBgNV
# HSAEGTAXMAgGBmeBDAEEAjALBglghkgBhv1sBwEwHwYDVR0jBBgwFoAUuhbZbU2F
# L3MpdpovdYxqII+eyG8wHQYDVR0OBBYEFI1kt4kh/lZYRIRhp+pvHDaP3a8NMFoG
# A1UdHwRTMFEwT6BNoEuGSWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2Vy
# dFRydXN0ZWRHNFJTQTQwOTZTSEEyNTZUaW1lU3RhbXBpbmdDQS5jcmwwgZAGCCsG
# AQUFBwEBBIGDMIGAMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5j
# b20wWAYIKwYBBQUHMAKGTGh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdp
# Q2VydFRydXN0ZWRHNFJTQTQwOTZTSEEyNTZUaW1lU3RhbXBpbmdDQS5jcnQwDQYJ
# KoZIhvcNAQELBQADggIBAA0tI3Sm0fX46kuZPwHk9gzkrxad2bOMl4IpnENvAS2r
# OLVwEb+EGYs/XeWGT76TOt4qOVo5TtiEWaW8G5iq6Gzv0UhpGThbz4k5HXBw2U7f
# IyJs1d/2WcuhwupMdsqh3KErlribVakaa33R9QIJT4LWpXOIxJiA3+5JlbezzMWn
# 7g7h7x44ip/vEckxSli23zh8y/pc9+RTv24KfH7X3pjVKWWJD6KcwGX0ASJlx+pe
# dKZbNZJQfPQXpodkTz5GiRZjIGvL8nvQNeNKcEiptucdYL0EIhUlcAZyqUQ7aUcR
# 0+7px6A+TxC5MDbk86ppCaiLfmSiZZQR+24y8fW7OK3NwJMR1TJ4Sks3KkzzXNy2
# hcC7cDBVeNaY/lRtf3GpSBp43UZ3Lht6wDOK+EoojBKoc88t+dMj8p4Z4A2UKKDr
# 2xpRoJWCjihrpM6ddt6pc6pIallDrl/q+A8GQp3fBmiW/iqgdFtjZt5rLLh4qk1w
# bfAs8QcVfjW05rUMopml1xVrNQ6F1uAszOAMJLh8UgsemXzvyMjFjFhpr6s94c/M
# fRWuFL+Kcd/Kl7HYR+ocheBFThIcFClYzG/Tf8u+wQ5KbyCcrtlzMlkI5y2SoRoR
# /jKYpl0rl+CL05zMbbUNrkdjOEcXW28T2moQbh9Jt0RbtAgKh1pZBHYRoad3AhMc
# MYIE7jCCBOoCAQEwKTAVMRMwEQYDVQQDDApaYW55TGVvbmljAhAtvj0iixcPrkg6
# BR5mloxfMAkGBSsOAwIaBQCgeDAYBgorBgEEAYI3AgEMMQowCKACgAChAoAAMBkG
# CSqGSIb3DQEJAzEMBgorBgEEAYI3AgEEMBwGCisGAQQBgjcCAQsxDjAMBgorBgEE
# AYI3AgEVMCMGCSqGSIb3DQEJBDEWBBQo39z4YQlvHah5XBjmfakxZhqKvDANBgkq
# hkiG9w0BAQEFAASCAQDZNvbRbW9k6Ar2h4wKi+IfOY4xH37Y1bjbmC9T4vQkMsDT
# OfaMMDRIB9ILNzEovZ4yAk/KqrOYCucZf2iFKrUl0ld/a3t1iTKch6ltZG0PFJys
# GdGc5XoEo7aHWZQ0iCI8BEi74arSwpC7kRnOmbp1OISrx4jY4GH57yrNaNeQbY91
# qkALejCviB8QGXqXfYixA5PSAlIWRldsDILVVS8AbGWwt4tHIU9trE8uVo/WeO7l
# MRVCY9Dcwi0GccsvNayMK4O/lQAJgRfWdllh9lWPuger4rjq7a7Djfmz9HO1xzjt
# 9fLH4x2Q49grl0cn5hGxpAcyu8DMGuc6mxWFQNjnoYIDIDCCAxwGCSqGSIb3DQEJ
# BjGCAw0wggMJAgEBMHcwYzELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDkRpZ2lDZXJ0
# LCBJbmMuMTswOQYDVQQDEzJEaWdpQ2VydCBUcnVzdGVkIEc0IFJTQTQwOTYgU0hB
# MjU2IFRpbWVTdGFtcGluZyBDQQIQCnpKiJ7JmUKQBmM4TYaXnTANBglghkgBZQME
# AgEFAKBpMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8X
# DTIyMDUwNjE1MTcwN1owLwYJKoZIhvcNAQkEMSIEIPjRD8D6kkYW6eBvCy5EBJIf
# AMVTPNHspOASHkvFO0EgMA0GCSqGSIb3DQEBAQUABIICAGXL8oUJ0O9yxhx8mO16
# 7RIePbayh5hSJi925+K8Z9VWGDMp3M7mQcuetqvl6FsjOWxiaZP/ai3VHnvqBeLg
# KiIb1c2dEgTGGidAEjTKEQu31yFesl1QiiQ0ju+aZoHbXh9+rcwLqdMody/KwAOt
# mgNDZT+6yfgX/OLXRWYaM36eSgAyoE7fvNdHUP8yaQhE0/RcIVJ3cRps4k7c2PSO
# x0/T4Fg/BM2p/MSHetzYNIVOXpLxlwaihCZR3xpVMd7PP3eo56C/H3QQc3CnVxWJ
# 7mhVi3aSTriYxnD9rhvGLbEmnDIxN0RWZyFTibwZcHf5hV/SzB6VolOky1JGZLUM
# 7wa240fBWGEULO1Str6FfgvpeFttZY6w+C+wovfLynEG2lhMNhPnevsp+ZnbPsUE
# EqmdqzqzFcO1yhNNbY3wZ3ptFz/XpiUPJHuJ8mvkAXk5ykuKT5jVOMfjglGIZ3GI
# l7qDzhbsKvMQJbSl6hSSBUfp+uJN6EEgw+euQzr3pSYlGMLMDP08Es66yTM2fUsK
# HEvApJWMRkZ73GM77ipq5FhzNNA8GBpgzdVzPboGZPEUabiOZLzHKM6JRo8y4pqg
# I+wtSukf/UnXH/YieBalzZEiyRT5IeX0eXWxmWZ5qWz7qfcHg5VM6QA1eFVM6a0d
# H+pt4IMtGFPnBI4VhZNkjlh2
# SIG # End signature block
