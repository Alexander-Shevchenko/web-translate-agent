# web-translate-agent
Resident monitoring double Ctrl-C / Ctrl-Ins hits and translating the clipboard via Web interface.

Default Web Service for translation: Lingvo online (http://www.lingvo.ua/ru/Search/en-ru/<clipboard content>)
That cant be changed using a command line startup argument, say:
      WebDict.exe "http://www.multitran.com/m.exe?CL=1&s=%s&l1=1&l2=2"
    or
      WebDict.exe "http://translate.google.com/#auto/en/%s"
    where %s is what will be replaced with the clipboard content.

Tested with Windows 8.1 (x64).
