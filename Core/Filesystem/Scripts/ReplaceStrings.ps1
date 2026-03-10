$content = [System.IO.File]::ReadAllText($args[0])
for ($i=2; $i -le $args.Count; $i += 2)
{
	$content = $content.Replace($args[$i], $args[$i + 1])
}
[System.IO.File]::WriteAllText($args[1], $content)
