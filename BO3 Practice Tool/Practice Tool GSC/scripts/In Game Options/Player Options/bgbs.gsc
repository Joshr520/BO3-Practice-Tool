GetBGBName(bgb)
{
    str = StrTok(bgb, "_");
    name = "";
    for (i = 2; i < str.size; i++) {
        name += ToUpper(GetSubStr(str[i], 0, 1)) + GetSubStr(str[i], 1) + " ";
    }
    return name;
}