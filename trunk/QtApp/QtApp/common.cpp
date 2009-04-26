#include "common.h"


//将map转换为命令行
QString convert_to_cmdline(CommandMap props)
{
	int id=0;
	QString strCmdLine;
	while(1)
	{
		QString strCmdID=QString::number(id++);
		if(props.find(strCmdID)==props.end())
			break;
		strCmdLine+=props[strCmdID];
		strCmdLine+=" ";
	}
	if(id<2) return "";

	QList<QString> keys=props.keys();
	bool ok;
	for(int i=0;i<keys.size();++i)
	{
		keys[i].toInt(&ok);
		if(ok) continue;
		if(keys[i].length()==0 || props[keys[i]].length()==0) continue;
		strCmdLine+=keys[i];
		strCmdLine+="=";
		strCmdLine+=props[keys[i]];
		strCmdLine+=" ";
	}
	return strCmdLine;
}
//从命令行转换为map
CommandMap convert_from_cmdline(QString strCmdLine)
{
	CommandMap props;
	QStringList strList=strCmdLine.split(' ');
	int id=0;
	for(int i=0;i<strList.size();++i)
	{
		QStringList kvList=strList[i].split('=');
		switch(kvList.size())
		{
		case 1:
			if(kvList[0].length()>0)
			{
				props[QString::number(id++)]=kvList[0];
			}
			break;
		case 2:
			if(kvList[0].length()>0 && kvList[1].length()>0)
			{
				props[kvList[0]]=kvList[1];
			}
			break;
		default://其他情况丢弃
			break;
		}
	}
	return props;
}



QByteArray raw_url_encode(QByteArray &s)
{
	QByteArray str;
	int x, y;
	int len=s.length();
	for(x=0,y=0;len--;++x,++y)
	{
		str[y]=s[x];
		if ((str[y] < '0' && str[y] != '-' && str[y] != '.') ||
			(str[y] < 'A' && str[y] > '9') ||
			(str[y] > 'Z' && str[y] < 'a' && str[y] != '_') ||
			(str[y] > 'z')) {
				str[y++] = '%';
				str[y++] = hexchars[(unsigned char) s[x] >> 4];
				str[y] = hexchars[(unsigned char) s[x] & 15];
		}
	}
	return str;
}
QByteArray raw_url_decode(QByteArray &str)
{
	int len=0;
	int maxlen=str.length();
	int upperlen=maxlen-2;
	QByteArray ret;
	while(len<maxlen)
	{
		if(str[len]=='%' && len <upperlen && isxdigit(str[len+1])&& isxdigit(str[len+2]))
		{
			int c=str[len+1];
			if(isupper(c)) c=tolower(c);
			int val=(c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;
			c=str[len+2];
			if(isupper(c)) c=tolower(c);
			val+= c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;
			ret+=(char) val;
			len+=2;
		}
		else
		{
			ret+=str[len];
		}
		++len;
	}
	return ret;
}
//
//char *php_raw_url_encode(char const *s, int len, int *new_length)
//{
//	register int x, y;
//	unsigned char *str;
//
//	str = (unsigned char *) safe_emalloc(3, len, 1);
//	for (x = 0, y = 0; len--; x++, y++) {
//		str[y] = (unsigned char) s[x];
//#ifndef CHARSET_EBCDIC
//		if ((str[y] < '0' && str[y] != '-' && str[y] != '.') ||
//			(str[y] < 'A' && str[y] > '9') ||
//			(str[y] > 'Z' && str[y] < 'a' && str[y] != '_') ||
//			(str[y] > 'z')) {
//				str[y++] = '%';
//				str[y++] = hexchars[(unsigned char) s[x] >> 4];
//				str[y] = hexchars[(unsigned char) s[x] & 15];
//#else /*CHARSET_EBCDIC*/
//		if (!isalnum(str[y]) && strchr("_-.", str[y]) != NULL) {
//			str[y++] = '%';
//			str[y++] = hexchars[os_toascii[(unsigned char) s[x]] >> 4];
//			str[y] = hexchars[os_toascii[(unsigned char) s[x]] & 15];
//#endif /*CHARSET_EBCDIC*/
//		}
//	}
//	str[y] = '\0';
//	if (new_length) {
//		*new_length = y;
//	}
//	return ((char *) str);
//}
//
//PHPAPI int php_raw_url_decode(char *str, int len)
//{
//	char *dest = str;
//	char *data = str;
//
//	while (len--) {
//		if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) 
//			&& isxdigit((int) *(data + 2))) {
//#ifndef CHARSET_EBCDIC
//				*dest = (char) php_htoi(data + 1);
//#else
//				*dest = os_toebcdic[(char) php_htoi(data + 1)];
//#endif
//				data += 2;
//				len -= 2;
//		} else {
//			*dest = *data;
//		}
//		data++;
//		dest++;
//	}
//	*dest = '\0';
//	return dest - str;
//}

