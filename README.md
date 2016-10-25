# web-translate-agent
Resident monitoring double Ctrl-C / Ctrl-Ins hits and translating the clipboard via Web interface.

Default Web Service for translation: Lingvo online (http://www.lingvo.ua/ru/Search/en-ru/_clipboard_content_).<br>
That cant be changed using a command line startup argument, say:<br>
* WebDict.exe "http://www.multitran.com/m.exe?CL=1&s=%s&l1=1&l2=2"<br>
 or<br>
* WebDict.exe "http://translate.google.com/#auto/en/%s"<br>
 where %s is what will be replaced with the clipboard content.<br>
<br>
Tested with Windows 8.1 (x64).
