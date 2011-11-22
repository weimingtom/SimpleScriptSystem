function readdoc(filename)
	local fin = assert(io.open(filename, "r"))
	local tab = {}, name, prototype, preview
	while true do
		local line = fin:read("*l")
		if not line then break end
		local head = string.sub(line, 1, 1)
		if head == '[' then
			local len = string.len(line)
			if string.sub(line, len, len) == ']' then
				name = string.sub(line, 2, len - 1)
				tab[name] = {}
				--print(name)
			end
		elseif head == '-' then
			prototype = string.sub(line, 2)
			tab[name]["prototype"] = prototype
			--print(prototype)
		elseif head == '+' then
			preview = string.sub(line, 2)
			tab[name]["preview"] = preview
			--print(preview)
		else
			if not tab[name]["comment"] then
				tab[name]["comment"] = ""
			end
			tab[name]["comment"] = tab[name]["comment"]..line.."<br />"
		end
	end
	fin:close()
	return tab
end

function writedoc(filename, temp, name, prototype, comment)
	local fout = assert(io.open(filename, "w+"))
	fout:write(string.format(temp, name, prototype, comment))
	fout:close()
end

function writecont(filename, temp, temp2, t, tsort)
	local fout = assert(io.open(filename, "w+"))
	local str = ""
	for _, name in ipairs(tsort) do
		local info = t[name]
		str = str..string.format(temp2, name, name, info["preview"]).."\n"
	end
	fout:write(string.format(temp, str))
	fout:close()
end

function writenavi(filename, temp, temp2, t, tsort)
	local fout = assert(io.open(filename, "w+"))
	local str = ""
	for _, name in ipairs(tsort) do
		local info = t[name]
		str = str..string.format(temp2, name, name, info["preview"]).."\n"
	end
	fout:write(string.format(temp, str))
	fout:close()
end

function writecss(filename)
	local fout = assert(io.open(filename, "w+"))
	fout:write([[
BODY
{
	width: 600px;
	color: #000000;
	background-color: #FFFFFF;
	font-size: 10pt;
	text-align: left;

	margin-top: 10px;
	margin-left: 10px;
	margin-bottom: 10px;
	margin-right: 10px;
}

P
{
	width: 600px;
	line-height: 1.1em;
	font-size: 100%;

	padding-top: 0px;
	padding-left: 20px;
	padding-bottom: 0px;
	padding-right: 20px;

	margin-top: 0.4em;
	margin-left: 10px;
	margin-bottom: 0.8em;
	margin-right: 10px;
}

TD
{
	font-size: 80%;

	padding-left: 15px;
}

TABLE
{
	width: 500px;

	padding-top: 3px;
	padding-left: 3px;
	padding-bottom: 3px;
	padding-right:3px;

	margin-top: 10px;
	margin-left: 15px;
	margin-bottom: 10px;
	margin-right: 15px;
}

TABLE.line
{
	border-top: #666666 1px solid;
}
TD.line
{
	border-bottom: #666666 1px solid;
}

A
{
	font-size: 10pt;
	text-decoration: none;

	padding-top: 0px;
	padding-left: 0px;
	padding-bottom: 0px;
	padding-right: 0px;
}

A:link
{
	color: #6666FF;
}

A:visited
{
	color: #7560CC;
}

A:hover
{
	color: #5809FF;
	text-decoration: underline;
}

HR
{
	width: 600px;
	height: 1px;
	max-height: 1px;
	border-color: #6666FF;
	border-style: solid;
	background-color: #6666FF;

	margin-top: 0em;
	margin-left: 0px;
	margin-bottom: 0em;
	margin-right: 0px;
}

IMG
{
	border-style: none;
}


ADDRESS
{
	font-style: normal;
	font-size: 100%;

	margin-top: 0.5em;
	margin-left: 20px;
	margin-bottom: 0.5em;
	margin-right: 0px;
}

H1
{
	width: 600px;
	height: 40px;
	font-size: 18pt;
	font-weight: bold;
	color: #FFFFFF;

	text-align: right;

	background-repeat: no-repeat;
	background-image:url(title.gif);
	background-color: #ccccff;

	padding-top: 8px;
	padding-left: 40px;
	padding-bottom: 5px;
	padding-right: 10px;

	margin-top: 0px;
	margin-left: 0px;
	margin-bottom: 0px;
	margin-right: 0px;

	border: #000000 1px solid;
}

H2
{
	width: 600px;
	font-size: 180%;
	font-weight: bold;
	color: #6666ff;

	background-repeat: no-repeat;
	background-color: #DDDDFF;

	padding-top: 6px;
	padding-left: 5px;
	padding-bottom: 4px;
	padding-right: 0px;

	margin-top: 15px;
	margin-left: 3px;
	margin-bottom: 12px;
	margin-right: 3px;

	border-left: #6666FF 10px solid;
	border-bottom: #9999CC 2px solid;
	border-right: #9999CC 2px solid;
}

H3
{
	background-color: #ffffff;

	width: 570px;
	font-size: 100%;
	color: #ff3333;
	font-weight: bold;

	padding-top: 3px;
	padding-left: 3px;
	padding-bottom: 3px;
	padding-right:3px;

	margin-top: 10px;
	margin-left: 15px;
	margin-bottom: 10px;
	margin-right: 15px;

	border: #6666FF 1px solid;
}

H4
{
	background-color: #6666cc;

	width: 65px;
	font-size: 120%;
	color: #FFFFFF;
	font-weight: bold;

	text-align: center;
	vertical-align: middle;

	padding-top: 8px;
	padding-left: 3px;
	padding-bottom: 6px;
	padding-right:3px;

	margin-top: 15px;
	margin-left: 10px;
	margin-bottom: 5px;
	margin-right: 10px;

	border-bottom: #333399 2px solid;
	border-right: #333399 2px solid;

}

STRONG
{
	background-repeat: no-repeat;
	background-image:url(arrow.gif);
	background-position: center left;

	font-size: 10pt;

	padding-top: 2px;
	padding-left: 0px;
	padding-bottom: 0px;
	padding-right: 0px;

	margin-top: 0px;
	margin-left: 0px;
	margin-bottom:10px;
	margin-right: 0px;
}
]])
	fout:close()
end


function writeindex(filename)
	fout = assert(io.open(filename, "w+"))
	fout:write([[
<html>

<head>
<meta http-equiv="Content-Type"
content="text/html; charset=utf-8">
<meta http-equiv="Content-Style-Type" content="text/css">
<title></title>
</head>
<frameset cols="250,*">
<frame src="navi.htm" name="navi">
<frame src="cont.htm" name="cont">

<noframes>
<body>
<p>A frames-capable browser is required to view this site.</p>
</body>
</noframes>
</frameset>
</html>
]])
		fout:close()
	end
------------------------------

function main()
	local template= [[
<html>
<head>
<meta http-equiv="Content-Type"
content="text/html; charset=utf-8">
<meta http-equiv="Content-Style-Type" content="text/css">
<title></title>
<link rel="stylesheet" href="default.css" type="text/css">
</head>
<body bgcolor="#FFFFFF" text="#000000" link="#9933FF"
vlink="#FF66FF" alink="#CC99FF" topmargin="20" leftmargin="20"
class="page" marginheight="0" marginwidth="0">
<h2>%s</h2>
<h3>%s</h3>
<p>
%s
</p>
</body>
</html>
]]
	local conttemp = [[
<html>
<head>
<meta http-equiv="Content-Type"
content="text/html; charset=utf-8">
<meta http-equiv="Content-Style-Type" content="text/css">
<title></title>
<link rel="stylesheet" href="./default.css" type="text/css">
</head>
<body bgcolor="#FFFFFF" text="#000000" link="#9933FF"
vlink="#FF66FF" alink="#CC99FF" topmargin="20" leftmargin="20"
class="page" marginheight="0" marginwidth="0">
<h2>Simple Script System 0.0.1 Function Table</h2>
<table border="0">
%s
</table>
</body>
</html>
]]
	local conttemp2 = [[
    <tr>
        <td width="100" bgcolor="#FFCCCC"><font size="2"><a href="%s.htm"><strong>%s</strong></a></font></td>
        <td><font size="2">%s</font></td>
    </tr>
]]

	local navitemp = [[
<html>
<head>
<meta http-equiv="Content-Type"
content="text/html; charset=utf-8">
<meta http-equiv="Content-Style-Type" content="text/css">
<title></title>
<link rel="stylesheet" href="./default.css" type="text/css">
</head>
<body bgcolor="#FFFFFF" text="#000000" link="#9933FF"
vlink="#FF66FF" alink="#CC99FF" topmargin="20" leftmargin="20"
class="page" marginheight="0" marginwidth="0">
<a href="cont.htm" target="cont"><strong>Go to top</strong></a><br />
<br />
%s
</body>
</html>
]]
	local navitemp2 = [[
	<a href="%s.htm" target="cont"><strong>%s</strong></a><br />
]]
	local tab = readdoc("sssdoc.txt")
	local tab2 = {}
	for name, info in pairs(tab) do
		tab2[#tab2 + 1] = name
		if true then
			writedoc(name..".htm",
				template,
				name,
				info["prototype"],
				info["comment"])
		else
			print(name)
			print(info["prototype"])
			print(info["preview"])
			print(info["comment"])
		end
	end
	table.sort(tab2)
	writecont("cont.htm", conttemp, conttemp2, tab, tab2)
	writenavi("navi.htm", navitemp, navitemp2, tab, tab2)
	writecss("default.css")
	writeindex("index.htm")
end

main()
