Write-Output "BACKOFFICE=$Env:AMPLIFY_BACKOFFICE_APP_ID"
Write-Output  "USER_BRANCH=$Env:USER_BRANCH"
Write-Output  "AWS_PROFILE=$Env:AWS_PROFILE"

$sourceConfig = @"
{
    "SourceDir": "aws-exports",
    "DistributionDir": "release",
    "BuildCommand": "echo Please build from the generator",
    "StartCommand": "echo No startup command supported"
}
"@

$awsCloudFormationConfig = @"
{
    "configLevel": "project",
    "useProfile": true,
    "profileName": "$Env:AWS_PROFILE"
}
"@

$amplify = @"
{
    "projectName": "tapgamebackoffice",
    "appId": "$Env:AMPLIFY_BACKOFFICE_APP_ID",
    "envName": "$Env:USER_BRANCH",
    "defaultEditor": "code"
}
"@

$frontend = @"
{
    "frontend": "javascript",
    "framework": "none",
    "config": $sourceConfig
}
"@

$providers = @"
{
    "awscloudformation": $awsCloudFormationConfig
}
"@

& amplify pull --amplify $amplify --frontend $frontend --providers $providers --yes
Rename-Item -Path "aws-exports\aws-exports.js" -NewName "aws-exports.mjs"
$aws_exports = node .\ci\scripts\extract-export.mjs | ConvertFrom-Json
$endpoint = Write-Output $aws_exports.aws_cloud_logic_custom[0].endpoint

$header_file = @"
#pragma once
#define BACKOFFICE_ENDPOINT "$endpoint"
"@

$header_file | Out-File -FilePath "src/backoffice-endpoint.generated.h" -Encoding ascii
