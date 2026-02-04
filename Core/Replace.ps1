$content = [System.IO.File]::ReadAllText($args[0]).Replace($args[2],$args[3])
[System.IO.File]::WriteAllText($args[1], $content)
